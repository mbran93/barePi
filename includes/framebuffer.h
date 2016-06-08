#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

typedef struct {
	volatile unsigned int pWidth;
	volatile unsigned int pHeight;
	volatile unsigned int vWidth;
	volatile unsigned int vHeight;
	volatile unsigned int pitch;
	volatile unsigned int bitDepth;
	volatile unsigned int x;
	volatile unsigned int y;
	void *ptr;
	volatile unsigned int size;
} __attribute__((aligned(4))) FrameBufferInfo;

volatile FrameBufferInfo *framebuffer;
unsigned int _fbHeight, _fbWidth;
unsigned int framebuffer_init;

unsigned int InitialiseFrameBuffer(unsigned int, unsigned int, unsigned int);
volatile FrameBufferInfo *getFrameBuffer();


#endif