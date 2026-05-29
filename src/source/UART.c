#include<UART.h>

void UARTPL011_init(){
    UARTPL011_module->UART_IBRD = 26;
    UARTPL011_module->UART_FBRD = 3;
    UARTPL011_module->UART_LCR_H |= (1ULL << 4) | (3ULL << 5);
    UARTPL011_module->UART_IFLS |= (2ULL << 0);
    UARTPL011_module->UART_IFLS &= ~(7ULL << 3);
    UARTPL011_module->UART_CR |= (1ULL << 8) | (1ULL << 9);
    UARTPL011_module->UART_CR |= (1ULL << 0); 
}

BRD_UART calculate_BRD(int _HGZ, int _BRR){
    BRD_UART _BRD_local = {.IBRD = (uint32_t)(_HGZ / (_BRR * 16)), 
        .FBRD = (uint32_t)(((((_HGZ % (_BRR * 16)) * 64) + 32) / 64) & 0x3F)};
    return _BRD_local;
}

void UARTPL011_GPIO_PI4_init(){
    GPIO->GPFSEL1 &= ~(7ULL << 12);
    GPIO->GPFSEL1 &= ~(7ULL << 15);

    GPIO->GPFSEL1 |= (4ULL << 12);
    GPIO->GPFSEL1 |= (4ULL << 15);
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 &= ~(1ULL << 28);
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 &= ~(1ULL << 30);
}

void debug(char _buffer[]){
    int _index = 0;
    while(_buffer[_index] != '\0'){
        UARTPL011_module->UART_DR = _buffer[_index];
        _index++;
    }
}