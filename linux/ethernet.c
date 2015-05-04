#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include "../etherbootdefs.h"
#include "ethernet.h"

const uint8_t RaspiMAC[6] = RASPBERRY_MAC;
const uint8_t ComputerMAC[6] = COMPUTER_MAC;

uint8_t EthernetBuffer[1600];
EthbootPacket* SndPacket = (EthbootPacket*)EthernetBuffer;

int Socket;
struct sockaddr_ll Device;
extern int BytesSent;
extern int Running;

uint32_t SendCurrentPacketID = 0;
uint32_t LastRecvPacketID = 0;

EthbootPacket* ReceivePacketRaw();
void SendPacketRaw(EthbootPacket* Packet);
void ProcessPacket(EthbootPacket* Packet);

void SendPacketRaw(EthbootPacket* Packet)
{
	memcpy(Packet->Header.MACReceiver, RaspiMAC, 6);
	memcpy(Packet->Header.MACSender, ComputerMAC, 6);
	Packet->Header.ProtocolType = ETHBOOT_PROTOCOL;
	Packet->ID = ++SendCurrentPacketID;
	
	//printf("SEND %X\n", Packet->Code);
	
	sendto(Socket, Packet, sizeof(EthbootPacket) + Packet->DataLength, 0, (struct sockaddr*)&Device, sizeof(Device));
}

void SendPacket(EthbootPacket* Packet)
{
	SendPacketRaw(Packet);
	
	EthbootPacket* PendingPacket = 0;
	uint32_t StartTime = time(NULL) + 1;
	
	// Wait for ACK
	while (1)
	{
		// If the peer didn't reply an ACK in 1 second, send the packet again
		if (time(NULL) > StartTime)
		{
			SendPacketRaw(Packet);
			StartTime = time(NULL) + 1;
		}
	
		EthbootPacket* RecvPacket = ReceivePacketRaw();
		
		if (RecvPacket)
		{
			if (RecvPacket->Code == CODE_ACK)
			{
				// The packet is an ACK, exit the loop
				break;
			}
			else
			{
				// The packet is not an ACK, but it contains data we want so we store it
				PendingPacket = RecvPacket;
			}
		}
	}
	
	// Process the stored packet (if there was one) and send the corresponding ACK
	if (PendingPacket)
	{
		SndPacket->Code = CODE_ACK;
		SndPacket->DataLength = 0;
		SendPacketRaw(SndPacket);
	
		// Check if this is the first time we get this packet
		if (PendingPacket->ID > LastRecvPacketID)
		{
			LastRecvPacketID = PendingPacket->ID;
			ProcessPacket(PendingPacket);
		}
	}
}

EthbootPacket* ReceivePacketRaw()
{
	static uint8_t ReceiveBuffer[1600];
	uint32_t FrameLength;
	
	if (recvfrom(Socket, ReceiveBuffer, sizeof(ReceiveBuffer), 0, NULL, NULL))
	{
		EthbootPacket* Packet = (EthbootPacket*)ReceiveBuffer;
		
		if (!memcmp(ComputerMAC, Packet->Header.MACReceiver, 6) && !memcmp(RaspiMAC, Packet->Header.MACSender, 6) && Packet->Header.ProtocolType == ETHBOOT_PROTOCOL)
		{
			//printf("RECV %X\n", Packet->Code);
		
			return Packet;
		}
	}
	
	return 0;
}

EthbootPacket* ReceivePacket()
{
	EthbootPacket* Packet = ReceivePacketRaw();
	
	// Check if this is the first time we get this packet
	if (Packet && Packet->ID > LastRecvPacketID)
	{	
		LastRecvPacketID = Packet->ID;
		SndPacket->Code = CODE_ACK;
		SndPacket->DataLength = 0;
		
		SendPacketRaw(SndPacket);
	}
	else
	{
		Packet = 0;
	}

	return Packet;
}

void SetupSocket()
{
	memset(&Device, 0, sizeof(Device));
	  
	if ((Device.sll_ifindex = if_nametoindex(INTERFACE)))
	{
		Device.sll_family = AF_PACKET;
		memcpy(Device.sll_addr, ComputerMAC, 6);
		Device.sll_halen = 6;
		  
		if ((Socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
		{
			printf("socket() failed ");
		}
		
		struct timeval tv;

		tv.tv_sec = 3;
		tv.tv_usec = 0;

		setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
	}
	else
	{
		printf("if_nametoindex() failed to obtain interface index ");
	}	
}

void DisposeSocket()
{
	close(Socket);
}
