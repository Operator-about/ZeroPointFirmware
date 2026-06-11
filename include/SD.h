#pragma once
#include<Type.h>
#include<UART.h>

extern SDR* SD_Registers;
extern uint32_t Response_CMD[4];

void SD_init();
void CMD_send(SDCMD _CMD);
void SD_get_error();
void SD_answer();