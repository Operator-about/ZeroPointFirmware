#include<GIC.h>
#include<PL011.h>
#include<GPIO.h>
#include<SD-Standart.h>
#include<SD.h>
#include<MBR.h>
#include<exFAT-PhysicalLayer.h>
#include<exFAT-KernelLayer.h>

JumpData Jump;

extern long jump_to_kernel();

int GICSD_ID;
int GICUART_ID;

int main_pi4(void){
    GPIO = (GPIOPI4*)0xFE200000;

    UARTPL011M.UARTPL011_REGISTERS = (UARTPL011R*)0xFE201000; 
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 0);
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 9);
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 8);
    GICUART_ID = 153;
    UARTPL011_GPIO_PI4_init();
    UARTPL011_init(UARTPL011_calculate_DR(48000000, 115200));
    __asm__("ISB");
    debug("[+]UARTPL011 - +\r\n");

    GICSD_ID = 158;
    GICM.GICDR = (GICD*)0xFF841000;
    GICM.GICCR = (GICC*)0xFF842000;
    GICM.Mode = 2;
    GICD_init();
    GICC_init();
    debug("[+]GIC - +\r\n");

    SDAM.SDAMR = (SDAR*)0xFE340000;
    SD_Standart = (uint32_t)0x5354414E;
    SDA_controller_init();
    SD_card_init();

    MBR_init();
    LBA_for_exFAT();
    exFAT_init();
    
    search_kernel();

    Jump.UART_Standart = 0x504C00B0;
    Jump.SD_Standart = 0x5354414E;
    Jump.UART = (uint64_t)0xFE201000;
    Jump.SD = (uint64_t)0xFE340000;
    Jump.GICv2 = (uint64_t)0xFF842000;
    Jump.SD_ID = 158;
    Jump.UART_ID = 153;

    __asm__("MOV X10, %0" : : "r"((uint64_t)&Jump));

    debug("[+]Jump struct build\r\n");

    __asm__("MSR DAIFSet, #2");
    jump_to_kernel(); //Переход к ядру
}   