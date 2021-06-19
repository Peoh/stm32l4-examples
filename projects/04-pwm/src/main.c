/*
	Sample main for STM32L4 target.
	The code will toggle a GPIO output (PB3) a 1Hz frequency
*/

#include "stm32l4xx.h"
#include "main.h"
#include "macros.h"

int main(void)
{
	// Enable GPIO clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// Enable Timer 2
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// Reset GPIO mode
	GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
	// Set bit 1 of mode -> 0b10 (AF)
	GPIOB->MODER |= GPIO_MODER_MODE3_1;
	// Set AF1 (TIM2_CH2 for PB3)
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3_Msk;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0;

	// Set Timer prescaler to the us (1Mhz)
	// ftim = fck/(PSC+1)
	TIM2->PSC = SystemCoreClock / 1000000 - 1;
	// Set the timer auto-reload register to 1m sec (1khz timer)
	TIM2->ARR = 1000;

	// Output compare configuration
	// Reset channel 2 fields
	TIM2->CCMR1 &= ~(TIM_CCMR1_CC2S_Msk | TIM_CCMR1_OC2FE_Msk | TIM_CCMR1_OC2PE_Msk | TIM_CCMR1_OC2M_Msk | TIM_CCMR1_OC2CE_Msk);
	// CCMR1 CC2S is in output by reset (0b00)
	// Set CCMR1 OC2M as PWM mode 1 (0b0110)
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	// Preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	// Capture/compare polarity set to active high (bit cleared)
	TIM2->CCER &= ~TIM_CCER_CC2P_Msk;
	// Enable capture/compare on channel 2
	TIM2->CCER |= TIM_CCER_CC2E;
	// Set capture/compare register value
	TIM2->CCR2 = 100;

	// Re-init the counter and update the registers
	TIM2->EGR |= TIM_EGR_UG;
	// Enable the Timer
	TIM2->CR1 |= TIM_CR1_CEN;

	while (1)
	{
	}
	return 0;
}