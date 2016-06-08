#include "includes/gpio.h"
#include "includes/bases.h"

void gpioInit() {
    gpio = (unsigned int *)GPIO_BASE;
}

void gpioSetFunc(unsigned int pin, unsigned int func) {
    *( gpio + ( pin / 10 ) ) |= ( func << ( pin - ( pin / 10 ) * 10 ) * 3 );
    
    if(pin == 16 && func == 1) led_status = 0;
}

void gpioSet(unsigned int pin, unsigned int value) {
    if(value == 0) {
        *(gpio + 10) = ( 1 << pin );
    } else if(value == 1) {
        *(gpio + 7) = ( 1 << pin );
    }
}

void led_invert() {
    gpioSet(16, !led_status);
    led_status = !led_status;
}