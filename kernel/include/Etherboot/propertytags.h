#ifndef PROPERTYTAGS_H
#define PROPERTYTAGS_H

#include <Etherboot/mailbox.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PROPTAG_END			0x00000000

#define PROPTAG_GET_FIRMWARE_REVISION	0x00000001
#define PROPTAG_GET_BOARD_MODEL		0x00010001
#define PROPTAG_GET_BOARD_REVISION	0x00010002
#define PROPTAG_GET_MAC_ADDRESS		0x00010003
#define PROPTAG_GET_BOARD_SERIAL	0x00010004
#define PROPTAG_GET_ARM_MEMORY		0x00010005
#define PROPTAG_GET_VC_MEMORY		0x00010006
#define PROPTAG_SET_POWER_STATE		0x00028001
#define PROPTAG_GET_CLOCK_RATE		0x00030002
#define PROPTAG_GET_TEMPERATURE		0x00030006
#define PROPTAG_GET_EDID_BLOCK		0x00030020
#define PROPTAG_GET_DISPLAY_DIMENSIONS	0x00040003
#define PROPTAG_GET_COMMAND_LINE	0x00050001

#define CODE_REQUEST		0x00000000
#define CODE_RESPONSE_SUCCESS	0x80000000
#define CODE_RESPONSE_FAILURE	0x80000001

#define VALUE_LENGTH_RESPONSE	(1 << 31)

#define DEVICE_ID_SD_CARD	0
#define DEVICE_ID_USB_HCD	3

#define POWER_STATE_OFF		(0 << 0)
#define POWER_STATE_ON		(1 << 0)
#define POWER_STATE_WAIT	(1 << 1)
#define POWER_STATE_NO_DEVICE	(1 << 1)

#define CLOCK_ID_EMMC		1
#define CLOCK_ID_UART		2

#define EDID_FIRST_BLOCK	0
#define EDID_STATUS_SUCCESS	0

typedef struct
{
	u32	BufferSize;
	u32	Code;
	u8	Tags[0];
} PropertyBuffer;

typedef struct
{
	u32	TagId;
	u32	ValueBufSize;
	u32	ValueLength;
} PropertyTag;

typedef struct
{
	PropertyTag Tag;
	u32	Value;
} PropertyTagSimple;

typedef struct
{
	PropertyTag Tag;
	u8 Address[6];
	u8 Padding[2];
} PropertyTagMACAddress;

typedef struct
{
	PropertyTag	Tag;
	u32	Serial[2];
} PropertyTagSerial;

typedef struct
{
	PropertyTag	Tag;
	u32	BaseAddress;
	u32	Size;
} PropertyTagMemory;

typedef struct
{
	PropertyTag	Tag;
	u32	DeviceId;
	u32	State;
} PropertyTagPowerState;

typedef struct
{
	PropertyTag	Tag;
	u32	ClockId;
	u32	Rate;
} PropertyTagClockRate;

typedef struct
{
	PropertyTag	Tag;
	u32	TemperatureId;
	u32	Value;
} PropertyTemperature;

typedef struct
{
	PropertyTag	Tag;
	u32	BlockNumber;
	u32	Status;
	u8	Block[128];
} PropertyTagEDIDBlock;

typedef struct
{
	PropertyTag	Tag;
	u32	Width;
	u32	Height;
} PropertyTagDisplayDimensions;

typedef struct
{
	PropertyTag	Tag;
	u8 String[1024];
} PropertyTagCommandLine;

boolean PropertyTagsGetTag(u32 TagId, void* Tag, unsigned TagSize, unsigned RequestParmSize);

#ifdef __cplusplus
}
#endif

#endif
