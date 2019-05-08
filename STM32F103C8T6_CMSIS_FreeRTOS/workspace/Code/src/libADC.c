/*************************************************** Library ***************************************************/

#include "libADC.h"

/************************************************ Use functions ************************************************/

void InitADC (void){
	
	RCC->APB2ENR 		|= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR 		|= RCC_APB2ENR_AFIOEN;
	GPIOA->CRL 			&= ~GPIO_CRL_CNF4;
	GPIOA->CRL 			&= ~GPIO_CRL_MODE4;
	
	RCC->APB2ENR    |= RCC_APB2ENR_ADC1EN;                //Включили тактирование ADC
  RCC->APB2RSTR   |= RCC_APB2RSTR_ADC1RST;              //ADC 1 reset
	RCC->APB2RSTR   &= ~RCC_APB2RSTR_ADC1RST;  
	RCC->CFGR       &= ~RCC_CFGR_ADCPRE;
	RCC->CFGR       |=  RCC_CFGR_ADCPRE_DIV6;             //choose 6 cause RCC 72MHz/6 = 12MHz
	
	ADC1->CR1       = 0;                                  //Предочистка
  ADC1->CR2       = 0;                                  //Предочистка
	ADC1->SQR3      = 0;                                  //Предочистка
	
	ADC1->CR1       |= ADC_CR1_SCAN;                      //Scan mode enabled
		
	ADC1->CR2 			&= ~ADC_CR2_CONT;											// 0 - single conversion, 1 - continuous conversion	
	ADC1->CR2 			|= ADC_CR2_EXTSEL;      							// event start conversion SWSTART
	ADC1->CR2 			|= ADC_CR2_EXTTRIG;                   // enable start conversion external signal
	ADC1->SMPR2 		|= ADC_SMPR2_SMP4; 										// sempling 239.5 cycle
	
	ADC1->SQR1      &= ~ADC_SQR1_L;                      	//Regular channel sequence length 1 conversion
	ADC1->SQR3 			|= ADC_SQR3_SQ1_2;   									// selection channel (4channel - PA4) (bin->0100 = dec->4) 
	
	ADC1->CR2 			|= ADC_CR2_ADON;    									// enable ADC
	
	ADC1->CR2  			|= ADC_CR2_RSTCAL;										// reset before start calibration
  while((ADC1->CR2 & ADC_CR2_RSTCAL) != 0);							// waiting reset before start calibration
  ADC1->CR2  			|= ADC_CR2_CAL;												// start calibration
  while((ADC1->CR2 & ADC_CR2_CAL) != 0);								// waiting end calibration
}

uint16_t StartConvADC (void){

		ADC1->CR2 		|= ADC_CR2_SWSTART;
		while (!(ADC1->SR & ADC_SR_EOC));
		return (ADC1->DR);

}

