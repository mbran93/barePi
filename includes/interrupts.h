#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

typedef struct {
	volatile unsigned int IRQ_BASIC_PENDING;
	volatile unsigned int IRQ_PENDING_1;
	volatile unsigned int IRQ_PENDING_2;
	volatile unsigned int FIQ_CONTROL;
	volatile unsigned int ENABLE_IRQ_1;
	volatile unsigned int ENABLE_IRQ_2;
	volatile unsigned int ENABLE_BASIC_IRQ;
	volatile unsigned int DISABLE_IRQ_1;
	volatile unsigned int DISABLE_IRQ_2;
	volatile unsigned int DISABLE_BASIC_IRQ;
} interrupt_t;

volatile interrupt_t *interrupt;

void initInterrupts();
void interruptBasicIRQ();
void disableBasicIRQ();

#endif