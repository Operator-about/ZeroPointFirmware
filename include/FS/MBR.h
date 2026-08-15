#pragma once
#include<Type.h>
#include<UART.h>
#include<SD.h>

extern MBR MBRM;
extern uint32_t LBA;

void MBR_init();
void LBA_for_exFAT();

int compare_fs_name(volatile uint8_t _buffer[], char _src_name[]);