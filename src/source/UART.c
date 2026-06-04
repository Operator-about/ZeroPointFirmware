#include<UART.h>

void UARTPL011_init(){
    UARTPL011M.UARTPL011_REGISTERS->UART_IBRD = 26;
    UARTPL011M.UARTPL011_REGISTERS->UART_FBRD = 3;
    UARTPL011M.UARTPL011_REGISTERS->UART_LCR_H |= (1ULL << 4) | (3ULL << 5);
    UARTPL011M.UARTPL011_REGISTERS->UART_IFLS |= (2ULL << 3);
    UARTPL011M.UARTPL011_REGISTERS->UART_IFLS &= ~(7ULL << 0);
    UARTPL011M.UARTPL011_REGISTERS->UART_CR |= (1ULL << 8) | (1ULL << 9);
    UARTPL011M.UARTPL011_REGISTERS->UART_CR |= (1ULL << 0); 
}

BRD_UART calculate_BRD(int _HGZ, int _BRR){
    BRD_UART _BRD_local = {.IBRD = (uint32_t)(_HGZ / (_BRR * 16)), 
        .FBRD = (uint32_t)(((((_HGZ % (_BRR * 16)) * 64) + 32) / 64) & 0x3F)};
    return _BRD_local;
}

void debug(char _buffer[]){
    while(UARTPL011M.UARTPL011_REGISTERS->UART_FR & (1ULL << 3)){
        __asm__("NOP");
    }
    
    int _index = 0;
    while(!(UARTPL011M.UARTPL011_REGISTERS->UART_FR & (1ULL << 5)) && _buffer[_index] != '\0'){
        UARTPL011M.UARTPL011_REGISTERS->UART_DR = _buffer[_index];
        _index++;
    }
}