/*
	Sample main for STM32L4 target.
	The code will toggle a GPIO (PB3) when a EXTI interrupt occurs on PA0
	PA0 is connected to a button connected to grounb. PA0 needs to be pulled up internally.
*/

#include "stm32l4xx.h"
#include "main.h"
#include "macros.h"

void EXTI0_IRQHandler(void){
	// check if line interrupt occured on correct EXTI
	if (EXTI->PR1 & EXTI_PR1_PIF0)
	{
		TOG_BIT(GPIOB->ODR, GPIO_ODR_OD3);
		// reset interrupt flag by setting '1' in SWIER_SWI0 bit
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
}

int main(void)
{
	// Enable GPIO clock (A and B port)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Enable SYSCFG for EXTI interrupts
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// GPIO B3 output configuration
	GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
	GPIOB->MODER |= GPIO_MODER_MODE3_0;

	// GPIO A0 input configuration
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
	// Set pull up pin (button connected to ground)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0; // Set bit 0 (pull up)

	// Map PA0 to EXTI0
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_Msk;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;

	// Configure EXTI
	// unmask interrupt on line 0
	EXTI->IMR1 |= EXTI_IMR1_IM0;
	// Reset rising trigger, enable falling trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT0_Msk;
	EXTI->FTSR1 |= EXTI_FTSR1_FT0;

	// Enable the interrupt
	NVIC_SetPriority(EXTI0_IRQn, 0x02);
	NVIC_EnableIRQ(EXTI0_IRQn);

	while (1)
	{
	}
	return 0;
}
