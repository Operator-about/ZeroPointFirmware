#include<GIC.h>
#include<UART.h>
#include<GPIO.h>
#include<SD.h>
#include<exFAT.h>

GICv2 GICv2M;
GICv3 GICv3M;
UARTPL011 UARTPL011M;
SD SDM;
GPIOPI4* GPIO;
uint32_t SD_RCA;
volatile uint8_t* DAT_buffer;
exFAT_BPB exFAT;
exFAT_attribute exFAT_attr;
volatile uint32_t* Kernel;
JumpData Jump;


extern long jump_to_kernel();

int main_pi4(void){
    GPIO = (GPIOPI4*)0xFE200000;

    UARTPL011M.UARTPL011_REGISTERS = (UARTPL011R*)0xFE201000; 
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 0);
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 9);
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 8);
    UARTPL011M.GIC_ID = 153;
    UARTPL011_GPIO_PI4_init();
    UARTPL011_init();
    __asm__("ISB");
    debug("[+]UARTPL011 - +\r\n");

    SDM.GIC_ID = 158;
    GICv2M.GICD = (GICDv2*)0xFF841000;
    GICv2M.GICC = (GICCv2*)0xFF842000;
    GICDv2_clear_interrupts();
    GICDv2_init();
    GICCv2_init();
    debug("[+]GIC - +\r\n");

    debug("[+]SD init stage\r\n");
    SDM.SD_Registers = (SDR*)0xFE340000;
    SD_controller_init();
    SD_card_init();
    debug("[+]SD init stage done\r\n");

    __asm__("MSR DAIFClr, #2");

    exFAT_init();

    Jump.UART_Standart = 0x504C00B0;
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