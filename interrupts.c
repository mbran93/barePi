#include "includes/interrupts.h"
#include "includes/gpio.h"
#include "includes/bases.h"
#include "includes/systemTimer.h"
#include "includes/debug.h"

void initInterrupts() {
    interrupt = (interrupt_t *)INTERRUPT_BASE;
}

void interruptBasicIRQ() {
    interrupt->ENABLE_BASIC_IRQ = 1;
}

void disableBasicIRQ() {
    interrupt->DISABLE_BASIC_IRQ = 1;
}

void __attribute__((interrupt("ABORT"))) reset_vector(void)
{

}

void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void)
{
    while( 1 )
    {
        /* Do Nothing! */
    }
}

void __attribute__((interrupt("SWI"))) software_interrupt_vector(void)
{

}

void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void)
{

}

void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{

}

void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{

    /* Clear the ARM Timer interrupt - it's the only interrupt we have
       enabled, so we want don't have to work out which interrupt source
       caused us to interrupt */
       
    unsigned int pending = *(unsigned int *)INTERRUPT_BASE;
    // if(pending != 1) 
    
    if(pending & 0x1) sysTime->IRQ_CLR_ACK = 1;
    
    if(pending != 1 && pending != 0) LOGF("pending basic %d\n", pending);
    
    pending = interrupt->IRQ_PENDING_2;
    
    if(pending != 0) LOGF("pending 2 %d\n", pending);
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{

}