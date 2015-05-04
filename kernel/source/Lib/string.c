#include <Etherboot/string.h>
#include <Etherboot/memutil.h>
#include <stdarg.h>

#define MAX_STRING_SIZE 256

char StringBuffer[MAX_STRING_SIZE];

size_t StringGetLength(const char* String)
{
	size_t Count = 0;
	
	while (String[++Count] != '\0');
	
	return Count;
}

void StringCopy(const char* SourceString, void* Destination)
{
	MemoryCopy((void*)SourceString, Destination, StringGetLength(SourceString) + 1); 
}

int StringEqual(const char* String1, const char* String2)
{
	size_t Length1 = StringGetLength(String1);
	size_t Length2 = StringGetLength(String2);
	
	if (Length1 == Length2)
	{
		int i;
		
		for (i = 0; i < Length1; ++i)
		{
			if (String1[i] != String2[i])
				return 0;
		}
		
		return 1;
	}
	
	return 0;
}

const char* StringConcat(const char* String1, const char* String2)
{
	size_t Length1 = StringGetLength(String1);
	size_t Length2 = StringGetLength(String2);
	
	if (Length1 + Length2 < MAX_STRING_SIZE)
	{
		StringCopy(String1, StringBuffer);
		StringCopy(String2, StringBuffer + Length1);
	
		StringBuffer[Length1 + Length2] = '\0';
	
		return StringBuffer;
	}
	
	return 0;
}
