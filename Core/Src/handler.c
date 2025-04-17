#include "R7FA6M5BH.h"

volatile uint32_t uwTick = 0;

void SysTick_Handler(void){
    uwTick++;
}
