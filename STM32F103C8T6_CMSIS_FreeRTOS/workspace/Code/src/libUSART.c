/*************************************************** Library ***************************************************/

#include "libUSART.h"

/************************************************ Use functions ************************************************/

void InitUART (void){
	InitUSART1();
	InitUSART2();
}

/*************************************************** USART1 ****************************************************/
void InitUSART1(void){

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//Set PA9 as TX for USART1(output, push-pull)
	GPIOA->CRH &= ~GPIO_CRH_CNF9;
	GPIOA->CRH |= GPIO_CRH_CNF9_1;
	GPIOA->CRH |= GPIO_CRH_MODE9;
	//Set PA10 as RX for USART1(input, input floating)
	GPIOA->CRH &= ~GPIO_CRH_CNF10;
	GPIOA->CRH |= GPIO_CRH_CNF10_0;
	GPIOA->CRH &= ~GPIO_CRH_MODE10;
	
	USART1->BRR |= 0x271; //USART BaudRate=(fck(72MHz)+baudrate/2)/baudrate 115200
	USART1->CR1 |= USART_CR1_TE; //transmit enable
	USART1->CR1 |= USART_CR1_RE; //recieve enable
	USART1->CR1 |= USART_CR1_UE; //usart enable
	
	USART1->CR1 |= USART_CR1_RXNEIE; //enable interrupts while recieve
	NVIC_EnableIRQ(USART1_IRQn);
	
}

/***************************************************************************************************************/

void SendUSART1(char chr){

	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = chr;
	
}

/***************************************************************************************************************/

void SendDataUSART1(uint8_t data){

	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = data;
	
}

/***************************************************************************************************************/

void SendStringUSART1(char* str){

	uint8_t i = 0;
	while(str[i]){
		SendUSART1(str[i++]);
	}
	
}

/*************************************************** USART2 ****************************************************/

void InitUSART2(void){

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//Set PA2 as TX for USART2(output, push-pull)
	GPIOA->CRL &= ~GPIO_CRL_CNF2;
	GPIOA->CRL |= GPIO_CRL_CNF2_1;
	GPIOA->CRL |= GPIO_CRL_MODE2;
	//Set PA3 as RX for USART2(input, input floating)
	GPIOA->CRL &= ~GPIO_CRL_CNF3;
	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	GPIOA->CRL &= ~GPIO_CRL_MODE3;
	
	USART2->BRR |= 0xEA6; //USART BaudRate=(fck(36Mhz)+baudrate/2)/baudrate 9600
	USART2->CR1 |= USART_CR1_TE; //transmit enable
	USART2->CR1 |= USART_CR1_RE; //recieve enable
	USART2->CR1 |= USART_CR1_UE; //usart enable
	
	USART2->CR1 |= USART_CR1_RXNEIE; //enable interrupts while recieve
	NVIC_EnableIRQ(USART2_IRQn);
	
}

/***************************************************************************************************************/

void SendUSART2(char chr){

	while(!(USART2->SR & USART_SR_TC));
	USART2->DR = chr;
	
}

/***************************************************************************************************************/

void SendDataUSART2(uint8_t data){

	while(!(USART2->SR & USART_SR_TC));
	USART2->DR = data;
	
}

/***************************************************************************************************************/

void SendStringUSART2(char* str){

	uint8_t i = 0;
	while(str[i]){
		SendUSART2(str[i++]);
	}
	
}
