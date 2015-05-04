#ifndef ETHERNET_H
#define ETHERNET_H

#include "../etherbootdefs.h"

void SendPacket(EthbootPacket* Packet);
EthbootPacket* ReceivePacket();
void SetupSocket();
void DisposeSocket();

#endif
