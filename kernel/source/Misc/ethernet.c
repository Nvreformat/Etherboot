#include <USPI/uspi.h>
#include <Etherboot/ethernet.h>
#include <Etherboot/string.h>
#include <Etherboot/memutil.h>
#include <Etherboot/timer.h>
#include <../../etherbootdefs.h>

void ProcessPacket(EthbootPacket* RecvPacket);
void SendPacketRaw(EthbootPacket* Packet);
EthbootPacket* ReceivePacketRaw();

uint8_t RaspiMAC[6] = RASPBERRY_MAC;
uint8_t ComputerMAC[6] = COMPUTER_MAC;
uint8_t SendBuffer[1600];
uint32_t SendCurrentPacketID = 0;
uint32_t LastRecvPacketID = 0;
EthbootPacket* SndPacket = (EthbootPacket*) SendBuffer;

void LogRemote(const char* String)
{
	SndPacket->Code = CODE_SEND_MESSAGE;
	SndPacket->DataLength = StringGetLength(String) + 1;
	MemoryCopy((void*) String, SndPacket->Data, StringGetLength(String) + 1);
	
	SendPacket(SndPacket);
}

void SendPacketRaw(EthbootPacket* Packet)
{
	MemoryCopy(ComputerMAC, Packet->Header.MACReceiver, 6);
	MemoryCopy(RaspiMAC, Packet->Header.MACSender, 6);
	Packet->Header.ProtocolType = ETHBOOT_PROTOCOL;
	Packet->ID = ++SendCurrentPacketID;

	USPiSendFrame(Packet, sizeof(EthbootPacket) + Packet->DataLength);
}

void SendPacket(EthbootPacket* Packet)
{
	SendPacketRaw(Packet);
	
	EthbootPacket* PendingPacket = 0;
	uint32_t StartTime = TimerGetTime() + 1;
	
	// Wait for ACK
	while (1)
	{
		// If the peer didn't reply an ACK in 1 second, send the packet again
		if (TimerGetTime() > StartTime)
		{
			SendPacketRaw(Packet);
			StartTime = TimerGetTime() + 1;
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
	unsigned int FrameLength = 0;
	
	if (USPiReceiveFrame(ReceiveBuffer, &FrameLength))
	{	
		EthbootPacket* Packet = (EthbootPacket*)ReceiveBuffer;
		
		if (MemoryCompare(RaspiMAC, Packet->Header.MACReceiver, 6) && MemoryCompare(ComputerMAC, Packet->Header.MACSender, 6) && Packet->Header.ProtocolType == ETHBOOT_PROTOCOL)
		{		
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
