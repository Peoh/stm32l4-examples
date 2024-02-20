/*
	Sample main for STM32L4 target.
	The code will toggle a GPIO output (PB3) a 1Hz frequency
*/

#include "main.h"

#include <stdbool.h>

#include "macros.h"
#include "stm32l4xx.h"

enum test_states {
	TEST_STATE_8MHZ_RUN_MR1,
	TEST_STATE_8MHZ_RUN_MR2,
	TEST_STATE_8MHZ_LPRUN,
	TEST_STATE_8MHZ_SLEEP_MR1,
	TEST_STATE_8MHZ_SLEEP_MR2,
	TEST_STATE_8MHZ_LPSLEEP,
	TEST_STATE_8MHZ_STOP0_MR1,
	TEST_STATE_8MHZ_STOP0_MR2,
	TEST_STATE_8MHZ_STOP1,
	TEST_STATE_8MHZ_STOP2,
	TEST_STATE_8MHZ_STANDBY_LPR,
	TEST_STATE_8MHZ_STANDBY_OFF,
	TEST_STATE_8MHZ_SHUTDOWN,
	TEST_STATE_MAX,
};

volatile bool change_state;


void EXTI0_IRQHandler(void){
	// check if line interrupt occured on correct EXTI
	if (EXTI->PR1 & EXTI_PR1_PIF0)
	{
		change_state = true;
		//TOG_BIT(GPIOB->ODR, GPIO_ODR_OD3);
		// reset interrupt flag by setting '1' in SWIER_SWI0 bit
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
}

void init_pa0_button(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;

	// GPIO A0 input configuration
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
	// Set pull up pin (button connected to ground)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0; // Set bit 0 (pull up)
}

void init_pa0_exti(){
	// Enable SYSCFG for EXTI interrupts
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
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
}

void init_pb3_led(){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// GPIO B3 output configuration
	GPIOB->MODER &= ~GPIO_MODER_MODE3_Msk;
	GPIOB->MODER |= GPIO_MODER_MODE3_0;
}

int main(void) {
	// Configure GPIO PA0 for

	init_pa0_button();
	init_pa0_exti();
	init_pb3_led();

	enum test_states state = TEST_STATE_8MHZ_RUN_MR1;

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
	
	// // Range 2
	// uint32_t pwr_cr1 = PWR->CR1;
	// pwr_cr1 = (pwr_cr1 & ~PWR_CR1_VOS_Msk) | PWR_CR1_VOS_0;
	// PWR->CR1 = pwr_cr1;

	// 1 wait state
	uint32_t acr = FLASH->ACR;
	acr = (acr & ~FLASH_ACR_LATENCY_Msk) | FLASH_ACR_LATENCY_1WS;
	FLASH->ACR = acr;
	while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != FLASH_ACR_LATENCY_1WS) {
	}

	RCC->CR |= RCC_CR_MSIRGSEL;
	uint32_t cr = RCC->CR;
	cr = (cr & ~RCC_CR_MSIRANGE_Msk) | RCC_CR_MSIRANGE_7; // 8 MHz
	RCC->CR = cr;

	SystemCoreClockUpdate();

	while (1) {

		switch (state)
		{
			case TEST_STATE_8MHZ_RUN_MR1: 
			{
				uint32_t pwr_cr1 = PWR->CR1;
				pwr_cr1 = (pwr_cr1 & ~PWR_CR1_VOS_Msk) | PWR_CR1_VOS_0;
				PWR->CR1 = pwr_cr1;

				while (change_state == false)
					;
			} 
			break;

			case TEST_STATE_8MHZ_RUN_MR2: 
			{
				uint32_t pwr_cr1 = PWR->CR1;
				pwr_cr1 = (pwr_cr1 & ~PWR_CR1_VOS_Msk) | PWR_CR1_VOS_1;
				PWR->CR1 = pwr_cr1;

				while (change_state == false)
					;
			} 

			case TEST_STATE_8MHZ_LPRUN :
			{
				uint32_t pwr_cr1 = PWR->CR1;
				pwr_cr1 = (pwr_cr1 & ~PWR_CR1_LPR_Msk) | PWR_CR1_LPR;
				PWR->CR1 = pwr_cr1;

				while (change_state == false)
					;
					
				uint32_t pwr_cr1 = PWR->CR1;
				pwr_cr1 = (pwr_cr1 & ~PWR_CR1_LPR_Msk) | PWR_CR1_LPR;
				PWR->CR1 = pwr_cr1;
			} 
			break;
		default:
			break;
		}

		state ++;
		if(state == TEST_STATE_MAX){
			state = TEST_STATE_8MHZ_RUN_MR1;
		}

		change_state = false;
		//__WFI();
	}
	return 0;
}