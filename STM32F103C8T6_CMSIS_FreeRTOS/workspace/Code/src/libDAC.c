/*************************************************** Library ***************************************************/

#include "libDAC.h"

//STM32F103C8T6 HAVE NO DAC

/************************************************ Use functions ************************************************/
/*
void InitDAC (void){
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~GPIO_CRL_CNF4;
	GPIOA->CRL &= ~GPIO_CRL_MODE4;

	DAC->CR |= DAC_CR_EN1;
}

void writeDAC (uint16_t data){
	if (data < 4096){
		DAC->DHR12R1 = data;
	}
}
*/
