/*************************************************** Library ***************************************************/

#include "libPWM.h"

/************************************************ Use functions ************************************************/

void PWMInit (void){
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//SET PORTA PIN0 output alternative function for PWM TIM2 CH1
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |= GPIO_CRL_CNF0_1;
	GPIOA->CRL &= ~GPIO_CRL_MODE0;
	GPIOA->CRL |= GPIO_CRL_MODE0;
	
	uint32_t TIM2prescaler = 72;
	TIM2->PSC = TIM2prescaler - 1;											//frequency formula: APB2speed/(TIM2prescaler*TIM2ARR)=72MHz/(72*1000)=1KHz
	TIM2->ARR = 1000; 
	TIM2->CCR1 = 500;																		//fill factor PWM
	
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //110: PWM mode 1 - In upcounting
	TIM2->CCER |= TIM_CCER_CC1E;												//ON PWM generation CH1
	TIM2->CCER &= ~TIM_CCER_CC1P;												//PWM inversion OFF CH1
	
	TIM2->CR1 &= ~TIM_CR1_DIR;													//upcount counter
	TIM2->CR1 |= TIM_CR1_CEN;														//TIM count enable
	
}

