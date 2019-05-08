/*************************************************** Library ***************************************************/

#include "sysInit.h"

/************************************************ Use functions ************************************************/
void RCCInit(void){
	RCC->CR |= ((uint32_t)RCC_CR_HSEON); 												// Enable HSE
	while (!(RCC->CR & RCC_CR_HSERDY));													// Ready start HSE		
	
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;					// Clock Flash memory
	
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;														// AHB = SYSCLK/1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;														// APB1 = HCLK/2
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;														// APB2 = HCLK/1
	
	RCC->CFGR &= ~RCC_CFGR_PLLMULL;               							// clear PLLMULL bits
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;															// clearn PLLSRC bits
	RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;														// clearn PLLXTPRE bits
	
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE;														// source HSE
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE ; 												// source HSE/1 = 8 MHz
	RCC->CFGR |= RCC_CFGR_PLLMULL9; 														// PLL x9: clock = 8 MHz * 9 = 72 MHz
	
	RCC->CR |= RCC_CR_PLLON;                      							// enable PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}      							// wait till PLL is ready
	
	RCC->CFGR &= ~RCC_CFGR_SW;                   							 	// clear SW bits
  RCC->CFGR |= RCC_CFGR_SW_PLL;                 							// select source SYSCLK = PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) {} 			// wait till PLL is used
	
}

/****************************************************************************************************************/

void GPIOInit(void){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                       	// enable clock for port A
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;                       	// enable clock for port B
	
	/* To use the serial wire DP to release some GPIOs
	the user software must set SWJ_CFG=010 just after reset. 
	This releases PA15, PB3 and PB4 which now becomeavailable as GPIOs.*/
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR = AFIO_MAPR_SWJ_CFG_1;

	GPIOB->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
	GPIOB->CRL |= (GPIO_CRL_MODE3_0 | GPIO_CRL_MODE4_0 | GPIO_CRL_MODE5_0 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0);
	
	GPIOB->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12);
	GPIOB->CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE10_0 | GPIO_CRH_MODE11_0 | GPIO_CRH_MODE12_0);
		
	GPIOB->BSRR |= (GPIO_BSRR_BR3 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7 
								| GPIO_BSRR_BR8 |  GPIO_BSRR_BR9 | GPIO_BSRR_BR10 | GPIO_BSRR_BR11 | GPIO_BSRR_BR12);

	
	//Set pin PA1 as input
	GPIOA->CRL |= GPIO_CRL_CNF1_0; //Input floating
	GPIOA->CRL &= ~GPIO_CRL_MODE1;
	
}

/****************************************************************************************************************/

void GenMCO (void){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                       	// enable clock for port A
	
	GPIOA->CRH &= ~GPIO_CRH_CNF8_0;															// setting out alternative push-pull for PA8
	GPIOA->CRH |= GPIO_CRH_CNF8_1;
	GPIOA->CRH |= (GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1);				// gpio speed 50 MHz
	
	RCC->CFGR |= RCC_CFGR_MCO_HSE;															// select source clokc HSE
	
}

/************************************************* Flash memory *************************************************/

void flashUnlock(void){

	FLASH->KEYR = INT_FLASH_KEY1;
	FLASH->KEYR = INT_FLASH_KEY2;
	
}

void flashlock(void){

	FLASH->CR |= FLASH_CR_LOCK;
	
}

void flashEraseAll (void){
	
	FLASH->CR |= FLASH_CR_MER;
	FLASH->CR |= FLASH_CR_STRT;
	
}

uint32_t flashReadData (uint32_t address){

	return (*(__IO uint32_t*) address);
	
}

void flashWriteData(uint32_t address, uint32_t data){

	FLASH->CR |= FLASH_CR_PG;
	while((FLASH->SR & FLASH_SR_BSY)!=0);
	
	*(__IO uint16_t*)address = (uint16_t)data;
	while((FLASH->SR & FLASH_SR_BSY)!=0);
	
	address+=2;
	data>>=16;
	
	*(__IO uint16_t*)address = (uint16_t)data;
	while((FLASH->SR & FLASH_SR_BSY)!=0);

	FLASH->CR &= ~(FLASH_CR_PG);
}
