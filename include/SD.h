#pragma once
#include<Type.h>
#include<UART.h>

extern volatile uint8_t* DAT_buffer;
extern SDR* SD_Registers;
extern uint32_t SD;

void SD_controller_init();
void SD_card_init();
void CMD_send(SDCMD _CMD);
void SD_barrier();
void SD_sec_barrier(int _second);

void SD_get_status();

void SD_get_status();

void DAT_clear();
void read_block(uint32_t _sector);