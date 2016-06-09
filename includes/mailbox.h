#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#define RR_REQUEST 0x00000000
#define RR_RESPONSE_OK 0x80000000
#define RR_RESPONSE_ERROR 0x80000001

#define SLOT_OVERALL_LENGTH 0
#define SLOT_RR 1
#define SLOT_TAGSTART 2

#define SLOT_TAG_ID 0
#define SLOT_TAG_BUFLEN 1
#define SLOT_TAG_DATALEN 2
#define SLOT_TAG_DATA 3

#define MBOX_HEADER_LENGTH 2
#define TAG_HEADER_LENGTH 3

#define POWER_ON_TAG 0x00020001

volatile unsigned int *mailbox;
volatile unsigned int *mailbuffer;

void mailboxInit();
void mailBufferInit();
void MailboxWrite(unsigned int, unsigned int);
unsigned int MailboxRead(unsigned int);
void add_mailbox_tag(volatile unsigned int* buffer, unsigned int tag, unsigned int buflen, unsigned int len, unsigned int* data);
void build_mailbox_request(volatile unsigned int* buffer);

#endif