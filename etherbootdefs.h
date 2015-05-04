#ifndef ETHBOOTDEFS_H
#define ETHBOOTDEFS_H

#include <stddef.h>
#include <stdint.h>

// Things you have to change
#define RASPBERRY_MAC {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define COMPUTER_MAC {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define KERNEL_ADDRESS 0x8000
#define INTERFACE "eth0"

// RPI Ethernet Codes
#define CODE_ESTABLISH_CONNECTION 0x0001
#define CODE_SEND_KERNEL_PART 0x0002
#define CODE_SEND_KERNEL_SIZE 0x0003

// Computer Ethernet Codes
#define CODE_SEND_MESSAGE 0x0004
#define CODE_GET_KERNEL 0x0005
#define CODE_CONNECTION_ESTABLISHED 0x0006
#define CODE_BOOTING 0x0007

#define CODE_ACK 0xAC

#define ETHBOOT_PROTOCOL 1337

typedef struct EthernetHeader
{
	uint8_t MACReceiver[6];
	uint8_t MACSender[6];
	uint16_t ProtocolType;
}
__attribute__ ((packed)) EthernetHeader;

typedef struct EthbootPacket
{
	EthernetHeader Header;
	uint32_t ID;
	uint32_t Code;
	uint32_t DataLength;
	uint8_t Reserved[2];
	uint8_t Data[0];
}
__attribute__ ((packed)) EthbootPacket;


#endif
