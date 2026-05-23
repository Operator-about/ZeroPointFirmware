#include<GIC.h>
#include<UART.h>

GICv2 GICv2_module;
UARTPL011* UARTPL011_module;
GPIOPI4* GPIO;

extern long EL3_to_EL1();

int main_pi4(void){

    UARTPL011_module = (UARTPL011*)0xFE201000; 
    UARTPL011_module->UART_DR &= ~(1ULL << 0);
    UARTPL011_module->UART_DR &= ~(1ULL << 9);
    UARTPL011_module->UART_DR &= ~(1ULL << 8);
    GPIO = (GPIOPI4*)0xFE200000;
    UARTPL011_GPIO_PI4_init();
    UARTPL011_init();
    __asm__("ISB");
    
    GICv2_module.GICD = (GICDv2*)0xFF841000;
    GICv2_module.GICC = (GICCv2*)0xFF842000;
    GICDv2_init();
    GICCv2_init();

    EL3_to_EL1(); //Переход к ядру
}   