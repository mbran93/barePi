#include "includes/types.h"
#include "includes/memory.h"
#include "includes/helper.h"
#include "includes/gpio.h"
#include "includes/mailbox.h"
#include "includes/framebuffer.h"
#include "includes/drawing.h"
#include "includes/console.h"
#include "includes/debug.h"
#include "includes/interrupts.h"
#include "includes/systemTimer.h"
#include "includes/characters.h"

extern void _enable_interrupts();

int main() {
    
    gpioInit();
    initInterrupts();
    mailboxInit();
    mailBufferInit();
    initTimer();
    initSystemTimer();
    _enable_interrupts();
    
    gpioSetFunc(16, 1);
    gpioSet(16, 0);
    
    if(!InitialiseFrameBuffer(480, 360, 32)) led_invert();
    setForeColor(0xFFFFFFFF);
    
    consoleInit();
    
    delayms(1000);
    
    // interruptBasicIRQ();
    enableTimeout(0x400, RPI_ARMTIMER_CTRL_23BIT | RPI_ARMTIMER_CTRL_ENABLE | RPI_ARMTIMER_CTRL_INT_ENABLE | RPI_ARMTIMER_CTRL_PRESCALE_256);
    
    
    //This is the section that is giving me trouble
    interrupt->ENABLE_BASIC_IRQ |= ( 1 << 4 );
    gpioSetFunc(19, 0);
    *(gpio + 15) = ( 1 << 19 );
    *(gpio +  18) = ( 1 << 19 );
    interrupt->ENABLE_IRQ_2 = (1 << 17) | ( 1 << 18 ) | ( 1 << 19 ) | ( 1 << 20 );
    
    while(1);
    
    return 0;

}