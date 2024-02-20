#ifndef HTIMER_H
#define HTIMER_H
#include "stm32l4xx.h"

enum hgpio_timer {
	HGPIO_TIM1 = 0,
	HGPIO_TIM2 = 1,
	HGPIO_TIM6 = 3,
	HGPIO_TIM15 = 4,
	HGPIO_TIM16 = 5,
};

void htimer_stop(enum hgpio_timer timer);
void htimer_start(enum hgpio_timer timer);
void htimer_configure(enum hgpio_timer timer, uint32_t psc, uint32_t arr);

#endif /* HGPIO_H */
