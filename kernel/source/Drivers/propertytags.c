#include <Etherboot/propertytags.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/memutil.h>
#include <Etherboot/bcm2835.h>

boolean PropertyTagsGetTag(u32 TagId, void* Tag, unsigned TagSize, unsigned  RequestParmSize)
{
	unsigned BufferSize = sizeof(PropertyBuffer) + TagSize + sizeof(u32);

	u8 Buffer[BufferSize + 15];
	PropertyBuffer *PropBuffer = (PropertyBuffer*) (((u32) Buffer + 15) & ~15);
	
	PropBuffer->BufferSize = BufferSize;
	PropBuffer->Code = CODE_REQUEST;
	
	MemoryCopy (Tag, PropBuffer->Tags, TagSize);
	
	PropertyTag* Header = (PropertyTag*) PropBuffer->Tags;
	Header->TagId = TagId;
	Header->ValueBufSize = TagSize - sizeof(PropertyTag);
	Header->ValueLength = RequestParmSize & ~VALUE_LENGTH_RESPONSE;

	u32*EndTag = (u32*) (PropBuffer->Tags + TagSize);
	*EndTag = PROPTAG_END;

	CleanDataCache ();
	DataSyncBarrier ();

	u32 BufferAddress = GPU_MEM_BASE + (u32) PropBuffer;
	if (MailBoxWriteRead(8, BufferAddress) != BufferAddress)
	{
		return FALSE;
	}
	
	InvalidateDataCache ();
	DataSyncBarrier ();

	if (PropBuffer->Code != CODE_RESPONSE_SUCCESS)
	{
		return FALSE;
	}
	
	if (!(Header->ValueLength & VALUE_LENGTH_RESPONSE))
	{
		return FALSE;
	}
	
	Header->ValueLength &= ~VALUE_LENGTH_RESPONSE;
	if (Header->ValueLength == 0)
	{
		return FALSE;
	}

	MemoryCopy(PropBuffer->Tags, Tag, TagSize);

	return TRUE;
}
