#include "includes/systemTimer.h"
#include "includes/bases.h"
#include "includes/uspios.h"
#include "includes/interrupts.h"

void initTimer() {
    timer = (unsigned int *)TIMER_BASE;
}

void initSystemTimer() {
    sysTime = (system_timer_t *)SYSTEM_TIMER_BASE;
}

unsigned int getTimer() {
    return *(timer + 1);
}

void delayus(unsigned int delay) {
    volatile unsigned int current = getTimer();

	while( ( getTimer() - current ) < delay );
}

void delayms(unsigned int delay) {
    volatile unsigned int current = getTimer();

	while( ( getTimer() - current ) < ( delay * 1000 ) );
}

void enableTimeout(unsigned int timeout, unsigned int prescaler) {
    sysTime->Load = timeout;

	sysTime->Control = prescaler;
}

/////////////////////////////////////////////////////////////////////////////////
//                              USPIOS                                         //
/////////////////////////////////////////////////////////////////////////////////

void MsDelay (unsigned nMilliSeconds) {
    delayms(nMilliSeconds);
}

void usDelay (unsigned nMicroSeconds) {
    delayus(nMicroSeconds);
}

// returns the timer handle (hTimer)
unsigned StartKernelTimer (unsigned nHzDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext) {
    if(interrupt->ENABLE_BASIC_IRQ != 1) {initInterrupts(); interruptBasicIRQ();}
    
    enableTimeout(40, RPI_ARMTIMER_CTRL_23BIT | RPI_ARMTIMER_CTRL_ENABLE | RPI_ARMTIMER_CTRL_INT_ENABLE | RPI_ARMTIMER_CTRL_PRESCALE_256);

    pHandler(1, pParam, pContext);
    return 1;
}

void CancelKernelTimer (unsigned hTimer) {
    sysTime->Control |= RPI_ARMTIMER_CTRL_DISABLE;
}