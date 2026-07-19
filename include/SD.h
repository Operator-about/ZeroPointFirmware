#pragma once
#include<Type.h>
#include<UART.h>

extern volatile uint8_t* DAT_buffer;
extern SD SDM;
extern uint32_t SD_RCA;

void SD_controller_init();
void SD_card_init();
void SD_preparing();

void CMD_send(SDCMD _CMD);
void SD_sec_barrier(int _second);

void read_block(uint32_t _sector);