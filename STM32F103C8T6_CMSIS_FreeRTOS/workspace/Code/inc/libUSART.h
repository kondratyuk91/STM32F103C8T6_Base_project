/*************************************************** Library ***************************************************/

#include "stm32f10x.h"

/************************************************ Use functions ************************************************/

void InitUART(void);

/*************************************************** USART1 ****************************************************/

void InitUSART1(void);
void SendUSART1(char chr);
void SendDataUSART1(uint8_t data);
void SendStringUSART1(char* str);

/*************************************************** USART2 ****************************************************/

void InitUSART2(void);
void SendUSART2(char chr);
void SendDataUSART2(uint8_t data);
void SendStringUSART2(char* str);

