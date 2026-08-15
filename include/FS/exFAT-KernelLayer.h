#pragma once

#include<Type.h>
#include<UART.h>
#include<exFAT-PhysicalLayer.h>

extern uint32_t* Kernel;

__attribute__((target("general-regs-only"))) //Запрет на использование Q регистров для данной функции
void search_kernel();

int this_kernel(uint8_t _buffer[], char _name[]);
void load_kernel(DataKernel _info);

void from_8_to_32();