#include<GIC.h>
#include<UART.h>
#include<GPIO.h>
#include<SD.h>

GICv2 GICv2M;
GICv3 GICv3M;
UARTPL011 UARTPL011M;
SDR* SD_Registers;
GPIOPI4* GPIO;

extern long jump_to_kernel();

int main_pi4(void){
    UARTPL011M.UARTPL011_REGISTERS = (UARTPL011R*)0xFE201000; 
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 0);
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 9);
    UARTPL011M.UARTPL011_REGISTERS->UART_DR &= ~(1ULL << 8);
    UARTPL011M.GIC_ID = 153;
    GPIO = (GPIOPI4*)0xFE200000;
    UARTPL011_GPIO_PI4_init();
    UARTPL011_init();
    __asm__("ISB");
    debug("UARTPL011 - +\r\n");
    
    GICv2M.GICD = (GICDv2*)0xFF841000;
    GICv2M.GICC = (GICCv2*)0xFF842000;
    GICDv2_clear_interrupts();
    GICDv2_init();
    GICCv2_init();
    debug("GIC - +\r\n");

    debug("SD init stage:\r\n");
    SD_Registers = (SDR*)0xFE340000;
    *(volatile uint16_t*)0xFE3400FC &= ~(0x3F << 0);
    SD_init();

    __asm__("ISB");
    __asm__("DSB SY");
    jump_to_kernel(); //Переход к ядру
}   