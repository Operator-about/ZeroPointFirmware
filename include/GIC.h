#pragma once

#include<Type.h>
#include<UART.h>

extern GICv2 GICv2M;
extern GICv3 GICv3M;

void GICDv2_init();
void GICCv2_init();
void GICDv2_clear_interrupts();

void GICDv3_init();
void GICRv3_init();
void GICCv3_init();
void GICDv3_clear_interrupts();
void GICDv3_IROUTER_AFE();