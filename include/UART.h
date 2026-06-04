#pragma once
#include<Type.h>

extern UARTPL011 UARTPL011M;


void UARTPL011_init();

BRD_UART calculate_BRD(int _HGZ, int _BRR);
void debug(char _buffer[]);