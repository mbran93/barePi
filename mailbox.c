#include "includes/mailbox.h"
#include "includes/bases.h"
#include "includes/uspios.h"

void mailboxInit() {
    mailbox = (unsigned int *)MAIL_BASE;
}

void mailBufferInit() {
	mailbuffer = (unsigned int *)BUFFER_ADDRESS;
}

void MailboxWrite(unsigned int fbinfo_addr, unsigned int channel) {
	if ( ( fbinfo_addr & 0xF ) != 0  || channel > 15 ) return;

	while ( ( *( mailbox + 6 ) & 0x80000000 ) != 0 );

	fbinfo_addr &= ~( 0xF );
	fbinfo_addr |= channel;

	*( mailbox + 8 ) = fbinfo_addr;
}

unsigned int MailboxRead(unsigned int channel) {
	if ( channel > 15) return 0;

	while ( ( *(mailbox + 6) & 0x40000000 ) != 0 );

	unsigned int msg = *mailbox;

	return ( msg >> 4 );
}

void add_mailbox_tag(volatile unsigned int* buffer, unsigned int tag, unsigned int buflen, unsigned int len, unsigned int* data) {
  volatile unsigned int* start = buffer + SLOT_TAGSTART;
  start[SLOT_TAG_ID] = tag;
  start[SLOT_TAG_BUFLEN] = buflen;
  start[SLOT_TAG_DATALEN] = len & 0x7FFFFFFF;

  unsigned int bufwords = buflen >> 2;

  int i;
  if (0 == data) {
    for (i = 0; i < bufwords; ++i) {
      start[SLOT_TAG_DATA + i] = 0;
    }
  } else {
    for (i = 0; i < bufwords; ++i) {
      start[SLOT_TAG_DATA + i] = data[i];
    }
  }

  start[SLOT_TAG_DATA+bufwords] = 0; // end of tags, unless overwritten later
}

void build_mailbox_request(volatile unsigned int* buffer) {
  unsigned int tag_length = buffer[MBOX_HEADER_LENGTH + SLOT_TAG_BUFLEN];
  unsigned int end = (MBOX_HEADER_LENGTH*4) + (TAG_HEADER_LENGTH*4) + tag_length;
  unsigned int overall_length = end + 4;
  buffer[SLOT_OVERALL_LENGTH] = overall_length;
  buffer[SLOT_RR] = RR_REQUEST;
}

int setPowerOnState(unsigned int nDeviceId) {
	unsigned int data = nDeviceId;
	add_mailbox_tag(mailbuffer, POWER_ON_TAG, 8, 4, &data);
	build_mailbox_request(mailbuffer);
	
	MailboxWrite(BUFFER_ADDRESS, 8);
	MailboxWrite(8);
	
	int powerState = mailbuffer[6];
	if(powerState != 0) return powerState;
	else return 0;
}
