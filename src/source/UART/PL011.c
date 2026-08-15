#include<PL011.h>

UARTPL011 UARTPL011M;

void UARTPL011_init(BRD_UART _BBR){
    UARTPL011M.UARTPL011_REGISTERS->UART_IBRD = _BBR.IBRD;
    UARTPL011M.UARTPL011_REGISTERS->UART_FBRD = _BBR.FBRD;
    UARTPL011M.UARTPL011_REGISTERS->UART_LCR_H = (1ULL << 4) | (3ULL << 5);
    UARTPL011M.UARTPL011_REGISTERS->UART_IFLS = (2ULL << 3);
    UARTPL011M.UARTPL011_REGISTERS->UART_IFLS &= ~(7ULL << 0);
    UARTPL011M.UARTPL011_REGISTERS->UART_CR = (1ULL << 8) | (1ULL << 9);
    UARTPL011M.UARTPL011_REGISTERS->UART_CR |= (1ULL << 0); 
}

BRD_UART UARTPL011_calculate_DR(int _Hz, int _BRR){
    BRD_UART _out;
    _out.IBRD = (uint32_t)(_Hz / (_BRR * 16));
    _out.FBRD = (uint32_t)(((_Hz / (_BRR * 16)) * 64) + 0.5);
    return _out;
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

void debugh(uint8_t _src){
    const char _hex_table[] = "0123456789ABCDEF";

    //В данном случаи операции над _src - это индекс в массиве. Т.е. если в _src будет после операции к примеру 14 - E, то это будет считано как индекс
    UARTPL011M.UARTPL011_REGISTERS->UART_DR = _hex_table[(_src >> 4) & 0x0F];
    UARTPL011M.UARTPL011_REGISTERS->UART_DR = _hex_table[_src & 0x0F];
}

void debugh64(uint64_t _src){
    for(int _index = 7; _index >= 0; _index--){
        uint8_t _next = ((_src >> (_index * 8)) & 0xFF);
        debugh(_next);
    }
}

void debugh32(uint32_t _src){
    for(int _index = 3; _index >= 0; _index--){
        uint8_t _next = ((_src >> (_index * 8)) & 0xFF);
        debugh(_next);
    }
}
