#pragma once

#include<Type.h>
#include<SD-Standart.h>
#include<PL011.h>

extern uint32_t SD_RCA;
extern int GICSD_ID;
extern Data DataBuffer;
extern uint32_t SD_Standart;

void SD_card_init();
void CMD_send(SDCMD _CMD);
void read_single_block(uint32_t _block);
void read_multi_block(uint32_t _block);
void ACMD41_send(SDCMD* _ACMD41, SDCMD* _CMD55);
void RCA_init();
void clock_up();
void block_conf();
void close_NS_ES();
void wait(int _NS_wait);