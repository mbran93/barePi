#ifndef __SYSTEMTIMER_H__
#define __SYSTEMTIMER_H__

#define RPI_ARMTIMER_CTRL_23BIT         ( 1 << 1 )

#define RPI_ARMTIMER_CTRL_PRESCALE_1    ( 0 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_16   ( 1 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_256  ( 2 << 2 )

/** @brief 0 : Timer interrupt disabled - 1 : Timer interrupt enabled */
#define RPI_ARMTIMER_CTRL_INT_ENABLE    ( 1 << 5 )
#define RPI_ARMTIMER_CTRL_INT_DISABLE   ( 0 << 5 )

/** @brief 0 : Timer disabled - 1 : Timer enabled */
#define RPI_ARMTIMER_CTRL_ENABLE        ( 1 << 7 )
#define RPI_ARMTIMER_CTRL_DISABLE       ( 0 << 7 )

volatile unsigned int *timer;

typedef struct {
	volatile unsigned int Load;
	volatile unsigned int Value;
	volatile unsigned int Control;
	volatile unsigned int IRQ_CLR_ACK;
	volatile unsigned int RAW_IRQ;
	volatile unsigned int MASKED_IRQ;
	volatile unsigned int Reload;
	volatile unsigned int Pre_divider;
	volatile unsigned int FREE_RUN_CNT;
} system_timer_t;

system_timer_t *sysTime;

void initTimer();
void initSystemTimer();

unsigned int getTimer();

void delayus(unsigned int);
void delayms(unsigned int);

void enableTimeout(unsigned int, unsigned int);

#endif 