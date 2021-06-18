/*
	Sample main for STM32L4 target.
	The code will toggle a GPIO output (PB3) a 1Hz frequency
*/

#include "stm32l4xx.h"
#include "main.h"
#include "macros.h"

void TIM2_IRQHandler(void){
	// Check if The update interrupt flag is enabled 
	if(TIM2->SR & TIM_SR_UIF){
		// Clear the interrupt
		TIM2->SR &= ~TIM_SR_UIF;

		// Toggle the GPIO
		TOG_BIT(GPIOB->ODR, GPIO_ODR_OD3);
	}
}

int main(void)
{
	// Enable GPIO clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// Enable Timer 2
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// Reset GPIO mode
	GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
	// Set bit 0 of mode -> 0x01 (Output)
	GPIOB->MODER |= GPIO_MODER_MODE3_0;

	// Enable and set Timer IRQ priority
	NVIC_SetPriority(TIM2_IRQn, 0x02);
	NVIC_EnableIRQ(TIM2_IRQn);

	// Set Timer prescaler to the ms
	TIM2->PSC= SystemCoreClock/1000;
	// Set the timer auto-reload register to 1 sec
	TIM2->ARR = 1000;
	// Re-init the counter and update the registers
	TIM2->EGR |= TIM_EGR_UG;

	// Enable Timer interrupt
	TIM2->DIER |= TIM_DIER_UIE;

	// Enable the Timer
	TIM2->CR1 |= TIM_CR1_CEN;


	while (1)
	{
	}
	return 0;
}