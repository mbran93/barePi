#include "includes/framebuffer.h"
#include "includes/mailbox.h"

unsigned int InitialiseFrameBuffer(unsigned int width, unsigned int height, unsigned int bitDepth) {
	if( width > 4096 || height > 4096 || bitDepth > 32) return 0;

    framebuffer = (FrameBufferInfo *)0x40040000;
	framebuffer->pWidth = width;
	framebuffer->vWidth = width;

	framebuffer->pHeight = height;
	framebuffer->vHeight = height;

	_fbHeight = height;
	_fbWidth = width;

	framebuffer->bitDepth = bitDepth;

	framebuffer->pitch = 0;
	framebuffer->x = 0;
	framebuffer->y = 0;
	framebuffer->ptr = 0;
	framebuffer->size = 0;

	MailboxWrite( 0x40040000, 1 );
	
	framebuffer_init = 1;
	return MailboxRead( 1 );
}

volatile FrameBufferInfo *getFrameBuffer() {
	return framebuffer;
}

