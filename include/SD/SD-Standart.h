#pragma once

#include<Type.h>
#include<UART.h>
#include<SD.h>
#include<exFAT-PhysicalLayer.h>

extern SDA SDAM;

void SDA_controller_init();
void SDA_read_single_block();
void SDA_read_multi_block();
void SDA_clock_conf(int _Hz, int _multi);
void SDA_wait_end_command();
void SDA_DAT_line_reset();
void SDA_RCA_init();
void SDA_CMD_send(SDCMD _CMD);
void SDA_ACMD41_init(SDCMD* _ACMD41, SDCMD* _CMD55);
void SDA_Block_conf();
void SDA_NS_ES_close();