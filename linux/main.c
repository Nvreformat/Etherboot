#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../etherbootdefs.h"
#include "ethernet.h"

int Running = 1;
int KernelSize;
int BytesSent = 0;
unsigned char* Kernel;
extern EthbootPacket* SndPacket;

void ReadKernel(const char* Path)
{
	FILE *File = fopen(Path, "rb");
	fseek(File, 0, SEEK_END);
	KernelSize = ftell(File);
	fseek(File, 0, SEEK_SET);

	Kernel = malloc(KernelSize + 1);
	fread(Kernel, KernelSize, 1, File);
	fclose(File);
}

void ProcessPacket(EthbootPacket* Packet)
{
	if (Packet->Code == CODE_SEND_MESSAGE)
	{
		printf("RASPI: %s%s", Packet->Data, "\n");
	}
	else if (Packet->Code == CODE_CONNECTION_ESTABLISHED)
	{
		SndPacket->Code = CODE_SEND_KERNEL_SIZE;
		SndPacket->DataLength = 4;
		memcpy(SndPacket->Data, &KernelSize, 4);
		SendPacket(SndPacket);
	}
	else if (Packet->Code == CODE_GET_KERNEL)
	{
		SndPacket->Code = CODE_SEND_KERNEL_PART;
		SndPacket->DataLength = (KernelSize - BytesSent - 1024) >= 0 ? 1024 : KernelSize % 1024;
		memcpy(SndPacket->Data, Kernel + BytesSent, SndPacket->DataLength);
		BytesSent += SndPacket->DataLength;
		SendPacket(SndPacket);
		
		printf("Uploading kernel... %u%% (%u/%u)%s", (int)(((float)BytesSent / KernelSize) * 100), BytesSent, KernelSize, "\r");
		
		if (BytesSent == KernelSize)
			printf("\n");
	}
	else if (Packet->Code == CODE_BOOTING)
	{
		Running = 0;
	}
}

int main(int argc, char **argv)
{
	if (geteuid() != 0)
	{
		printf("You must run this program as root\n");
		
		return;
	}
	
	if (argc <= 1)
	{
		printf("No kernel specified\n");
		
		return;
	}
	else
	{
		if (access(argv[1], 0) == -1)
		{
			printf("File does not exist\n");
		
			return;
		}
	}
	
	system("tput civis");
	
	ReadKernel(argv[1]);
	SetupSocket();
	
	printf("Attempting to communicate with the RPi...\n");

	SndPacket->Code = CODE_ESTABLISH_CONNECTION;
	SndPacket->DataLength = 0;
	SendPacket(SndPacket);
	
	while(Running)
	{
		EthbootPacket* RecvPacket;
		
		if (RecvPacket = ReceivePacket())
		{
			ProcessPacket(RecvPacket);
		}
	}

	DisposeSocket();
	free(Kernel);
	system("tput cnorm");

	return(EXIT_SUCCESS);
}
