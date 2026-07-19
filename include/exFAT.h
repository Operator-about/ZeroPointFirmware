#pragma once

#include<stdalign.h>
#include<Type.h>
#include<UART.h>
#include<SD.h>

#define KERNEL_ADDRESS 0x00080000

extern uint8_t* Buffer;
extern uint32_t* Kernel_buffer;

extern exFAT_BPB exFAT;
extern exFAT_attribute exFAT_attr;

void exFAT_init();
void read_cluster(uint32_t _cluster, int _count);
uint32_t walk_FAT_table(uint32_t _cluster);
KernelFile get_first_cluster_kernel();

void to_kernel_buffer(int _multi);
int compare_ZeroPoint(uint8_t _buffer[], char _src[]);
int power_two(int _src);