#pragma once
#include<Type.h>

extern UARTPL011* UARTPL011_module;
extern GPIOPI4* GPIO;

void UARTPL011_init();
void UARTPL011_GPIO_PI4_init();

BRD_UART calculate_BRD(int _HGZ, int _BRR);