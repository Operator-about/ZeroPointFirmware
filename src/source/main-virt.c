#include<UART.h>
#include<GIC.h>

GIC GICM;
UARTPL011 UARTPL011M;
JumpData Jump;
SD SDM;

extern void jump_to_kernel();

void main_virt(void){
    GICM.GICDR = (GICD*)0x08000000;
    GICM.GICRR = (GICR*)0x08100000;
    GICM.Mode = 2;
    UARTPL011M.GIC_ID = 33;
    SDM.GIC_ID = 40;
    //switch_GICv2_to_GICv3();
    GICC_init();
    GICD_init();
    //UARTPL011_init();


    *(volatile uint32_t*)0x09000000 = 'L';

    UARTPL011M.UARTPL011_REGISTERS->UART_DR = 'P';
}