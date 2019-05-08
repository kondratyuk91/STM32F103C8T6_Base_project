/*************************************************** Library ***************************************************/

#include "stm32f10x.h"

/*************************************************** Defines ***************************************************/

#define INT_FLASH_KEY1				((uint32_t)0x45670123)
#define INT_FLASH_KEY2				((uint32_t)0xCDEF89AB)

/************************************************ Use functions ************************************************/

void RCCInit (void);
void GenMCO (void);
void GPIOInit (void);

void flashUnlock (void);
void flashlock (void);
void flashEraseAll (void);

uint32_t flashReadData (uint32_t address);
void flashWriteData(uint32_t address, uint32_t data);
