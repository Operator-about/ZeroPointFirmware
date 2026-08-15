#pragma once

#include<SD.h>
#include<Type.h>
#include<MBR.h>

extern exFAT* exFATp;
extern exFAT_attribute exFAT_attr;

void exFAT_init();
void read_cluster(uint32_t _cluster);
uint32_t walk_FAT(uint32_t _cluster);

__attribute__((target("general-regs-only"))) //Запрет на использование Q регистров для данной функции
DataKernel get_kernel();

int get_count_file();

int power_two(int _src);