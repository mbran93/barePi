#ifndef __GPIO_H__
#define __GPIO_H__

volatile unsigned int *gpio;
unsigned int led_status;

void gpioInit();
void gpioSetFunc(unsigned int, unsigned int);
void gpioSet(unsigned int, unsigned int);
void led_invert();

#endif