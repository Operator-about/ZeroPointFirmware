#include<GIC.h>
#include<UART.h>

GICv2 GICv2_module;
UARTPL011* UARTPL011_module;
GPIOPI4* GPIO;

extern long jump_in_kernel();

int main_pi4(void){

    UARTPL011_module = (UARTPL011*)0xFE201000; 
    UARTPL011_module->UART_DR &= ~(1ULL << 0);
    UARTPL011_module->UART_DR &= ~(1ULL << 9);
    UARTPL011_module->UART_DR &= ~(1ULL << 8);
    GPIO = (GPIOPI4*)0xFE200000;
    UARTPL011_GPIO_PI4_init();
    UARTPL011_init();
    __asm__("ISB");
    if(UARTPL011_module->UART_RIS & (1ULL << 5)){
        //debug("RIS - active\r\n");
    }
    debug("UARTPL011 - +\r\n");
    
    GICv2_module.GICD = (GICDv2*)0xFF841000;
    GICv2_module.GICC = (GICCv2*)0xFF842000;
    GICDv2_init();
    GICCv2_init();
    debug("GIC - +\r\n");

    // debug("========================================\r\n");
    // debug("Info about this firmware:\r\n");
    // debug("Firmware version: 0.0.2\r\n");
    // debug("Name firmware: ZeroPointFirmware-Pi4\r\n");
    // debug("Status - ok!\r\n");
    // debug("========================================\r\n");
    // debug("Go to kernel...\r\n");
    __asm__("ISB");

    jump_in_kernel(); //Переход к ядру
}   