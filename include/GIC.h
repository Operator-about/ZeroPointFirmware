#pragma once

#include<Type.h>
#include<UART.h>
#include<SD.h>

extern GIC GICM;

void switch_GICv2_to_GICv3();
void GICD_init();
void GICR_init();
void GICC_init();
void GICDv3_IROUTER_AFE();

void IRQ_set(int _index);

extern void Sync();