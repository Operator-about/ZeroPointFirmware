#pragma once

#include<Type.h>
#include<UART.h>
#include<SD.h>

extern uint32_t Buffer[32768] __attribute__((section(".kernel_buffer")));

extern exFAT_BPB exFAT;
extern exFAT_attribute exFAT_attr;

void exFAT_init();
void read_cluster(uint32_t _cluster, uint8_t _buffer[]);
void read_kernel(uint32_t _cluster, uint8_t _buffer[], int _index_data);
uint32_t walk_FAT_table(uint32_t _cluster);
KernelFile get_first_cluster_kernel(uint8_t _buffer[]);

int compare_ZeroPoint(uint8_t _buffer[], char _src[]);
int power_two(int _src);