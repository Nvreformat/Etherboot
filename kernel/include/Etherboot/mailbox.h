#ifndef MAILBOX_H
#define MAILBOX_H

#include <Etherboot/bcm2835.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned MailBoxWriteRead(unsigned Mailbox, unsigned Data);
unsigned MailBoxRead(unsigned Mailbox);
void MailBoxWrite(unsigned Mailbox, unsigned Data);
void MailBoxFlush(unsigned Mailbox);

#ifdef __cplusplus
}
#endif

#endif
