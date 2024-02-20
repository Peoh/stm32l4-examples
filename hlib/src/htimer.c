#include "htimer.h"
struct htimer_map {
	uint8_t port;
	TIM_TypeDef *ptimer;
};

struct htimer_map timer_map[] = {
	{HGPIO_TIM1, TIM1},	  {HGPIO_TIM2, TIM2},	{HGPIO_TIM6, TIM6},
	{HGPIO_TIM15, TIM15}, {HGPIO_TIM16, TIM16},
};

void htimer_stop(enum hgpio_timer timer) {
	TIM_TypeDef *t = timer_map[timer].ptimer;

	t->CR1 &= ~TIM_CR1_CEN;
}
void htimer_start(enum hgpio_timer timer) {
	TIM_TypeDef *t = timer_map[timer].ptimer;

	// Re-init the counter and update the registers
	t->EGR |= TIM_EGR_UG;
	// Enable the Timer
	t->CR1 |= TIM_CR1_CEN;
}

void htimer_configure(enum hgpio_timer timer, uint32_t psc, uint32_t arr) {
	TIM_TypeDef *t = timer_map[timer].ptimer;

    t->PSC = psc;
	t->ARR = arr;
}