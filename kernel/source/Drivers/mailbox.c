#include <Etherboot/mailbox.h>
#include <Etherboot/sysutils.h>
#include <Etherboot/timer.h>

unsigned MailBoxWriteRead(unsigned Mailbox, unsigned Data)
{
	DataMemBarrier();

	MailBoxFlush(Mailbox);
	MailBoxWrite(Mailbox, Data);

	unsigned Result = MailBoxRead(Mailbox);

	DataMemBarrier();

	return Result;
}

void MailBoxFlush(unsigned Mailbox)
{
	while(!(Read32(MAILBOX_STATUS) & MAILBOX_STATUS_EMPTY))
	{
		Read32(MAILBOX_READ);

		TimerSimpleMsDelay(20);
	}
}

unsigned MailBoxRead(unsigned Mailbox)
{
	unsigned Result;
	
	do
	{
		while(Read32(MAILBOX_STATUS) & MAILBOX_STATUS_EMPTY);
		
		Result = Read32(MAILBOX_READ);
	}
	while((Result & 0xF) != Mailbox);

	return Result & ~0xF;
}

void MailBoxWrite(unsigned Mailbox, unsigned Data)
{
	while(Read32(MAILBOX_STATUS) & MAILBOX_STATUS_FULL);

	Write32(MAILBOX_WRITE, Mailbox | Data);
}
