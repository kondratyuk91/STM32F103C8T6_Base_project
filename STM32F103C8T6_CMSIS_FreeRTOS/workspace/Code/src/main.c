/*************************************************** Library ***************************************************/

#include "main.h"

/**************************************************** Main *****************************************************/
int main (void){

	RCCInit();
	GenMCO ();
	GPIOInit();
	InitUART();
	PWMInit();
	InitADC();
	// InitDAC();
	
	xTaskCreate(vTaskLed1, "LED PORTB 3-8", 32, NULL, 1, NULL);
	xTaskCreate(vTaskButtonPA1, "Button PA1", 32, NULL, 1, NULL);
	xTaskCreate(vTaskPWM, "PWM PA2", 32, NULL, 1, NULL);
	xTaskCreate(vTaskConvADC, "ADC Queue", 182, NULL, 1, NULL);
	xTaskCreate(vTaskNextionHMI, "HMI", 128, NULL, 1, NULL);
	
	SendDataADC = xQueueCreate (5, sizeof (uint16_t));
	
	vTaskStartScheduler();
	
	while(1)
	{
		
	}
}
/********************************************** Use Tasks FreeRTOS *********************************************/
void vTaskLed1 (void *argument){
	while(1) {
		GPIOB->BSRR |= GPIO_BSRR_BR7;
		GPIOB->BSRR |= GPIO_BSRR_BS3;
		vTaskDelay(50);
		GPIOB->BSRR |= GPIO_BSRR_BR3;
		GPIOB->BSRR |= GPIO_BSRR_BS4;
		vTaskDelay(50);
		GPIOB->BSRR |= GPIO_BSRR_BR4;
		GPIOB->BSRR |= GPIO_BSRR_BS5;
		vTaskDelay(50);
		GPIOB->BSRR |= GPIO_BSRR_BR5;
		GPIOB->BSRR |= GPIO_BSRR_BS6;
		vTaskDelay(50);
		GPIOB->BSRR |= GPIO_BSRR_BR6;
		GPIOB->BSRR |= GPIO_BSRR_BS7;
		vTaskDelay(50);
	}	
}
/****************************************************/
void vTaskButtonPA1 (void *argument){
	uint8_t buttonState = 0;
	
	while(1) {

		if((GPIOB->ODR & GPIO_ODR_ODR8) != 0){
			buttonState=1;
			}
			else {
			buttonState=0;
			}
	
		if ((GPIOA->IDR & GPIO_IDR_IDR1) != 0){
			buttonState+=1;
			if (buttonState==1){
				vTaskDelay(300);
				SendStringUSART1("bt0.val=1");
				SendDataUSART1(0xFF);
				SendDataUSART1(0xFF);
				SendDataUSART1(0xFF);
				
				GPIOB->BSRR |= GPIO_BSRR_BS8;
				GPIOB->BSRR |= GPIO_BSRR_BS9;
				
				vTaskDelay(300);
			}
			
			if (buttonState==2){
				vTaskDelay(300);
				SendStringUSART1("bt0.val=0");
				SendDataUSART1(0xFF);
				SendDataUSART1(0xFF);
				SendDataUSART1(0xFF);
				
				GPIOB->BSRR |= GPIO_BSRR_BR8;
				GPIOB->BSRR |= GPIO_BSRR_BR9;
				vTaskDelay(300);
				buttonState=0;
			}
			
		}
		vTaskDelay(100);
	}	
}
/****************************************************/
void vTaskPWM(void *argument){
	uint16_t i = 0;
	while (1) {	
		while(i<1000) {
			TIM2->CCR1 = i;
			i++;
			vTaskDelay(10);
			}
		if (i==1000){
			i=0;
			}
		vTaskDelay(100);
	}
}
/****************************************************/
void vTaskConvADC(void *argument){
	uint16_t adcResult;
	
	while(1){
		adcResult = StartConvADC();
		xQueueSend(SendDataADC, &adcResult, 0);
		GPIOB->ODR ^= GPIO_ODR_ODR12;
		vTaskDelay(200);
	}
}
/****************************************************/
void vTaskNextionHMI (void *argument){
	uint16_t adcResult;
	char adcResultStr[4];
	float stepADC = 0.0008650;
	float adcVoltage;
	char adcVoltageStr[5];
	char adcValueStr[3];
	
	while(1){
		if (uxQueueMessagesWaiting(SendDataADC) != 0){
			xQueueReceive(SendDataADC, &adcResult, 0);

/************************** Send ADC result *********************************/

			sprintf(adcResultStr, "%u", adcResult);
			SendStringUSART1("t1.txt=\"");
			SendStringUSART1(adcResultStr);
			SendStringUSART1("\"");
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			
/*************************** Send voltage ***********************************/

			adcVoltage = stepADC * (float)adcResult;
			sprintf(adcVoltageStr, "%.3f", adcVoltage);
			SendStringUSART1("t2.txt=\"");
			SendStringUSART1(adcVoltageStr);
			SendStringUSART1(" V\"");
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);

/**************************** Send value ************************************/

			sprintf(adcValueStr, "%u", adcResult/41);
			SendStringUSART1("j0.val=");
			SendStringUSART1(adcValueStr);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
		}
		vTaskDelay(100);
	}
}
/****************************************************/

/************************************************** Interrupt **************************************************/

void USART1_IRQHandler (void){

	if (USART1->SR & USART_CR1_RXNEIE){ //USART_CR1_RXNEIE - Read data register not empty
		
		USART1->SR &= ~USART_CR1_RXNEIE;
		
		if (USART1->DR == 0x01){
			GPIOB->BSRR |= GPIO_BSRR_BS8;
			GPIOB->BSRR |= GPIO_BSRR_BS9;
		}
		if (USART1->DR == 0x02){
			GPIOB->BSRR |= GPIO_BSRR_BR8;
			GPIOB->BSRR |= GPIO_BSRR_BR9;
		}
		if (USART1->DR == 0x03){
			GPIOB->BSRR |= GPIO_BSRR_BS10;
			GPIOB->BSRR |= GPIO_BSRR_BS11;
		}
		if (USART1->DR == 0x04){
			GPIOB->BSRR |= GPIO_BSRR_BR10;
			GPIOB->BSRR |= GPIO_BSRR_BR11;
		}
	}
	
}

