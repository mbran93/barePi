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
    gpioSetFunc(16, 1);
    gpioSet(16, 0);
    
    _enable_interrupts();
    initInterrupts();
    mailboxInit();
    mailBufferInit();
    initTimer();
    
    if(!InitialiseFrameBuffer(480, 360, 32)) led_invert();
    setForeColor(0xFFFFFFFF);
    
    consoleInit();
    // LOGF("This is a test %d", 15);
    
    return 0;

}