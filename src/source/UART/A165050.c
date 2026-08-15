#include<A165050.h>

UART165050* UART165050M;

void UART165050_init(uint32_t _baud_rate, uint32_t _ppl){
    uint16_t _div = (_ppl / (16 * _baud_rate));
    UART165050M->UART_LCR |= (1ULL << 7);
    UART165050M->UART_TRD |= ((_div & 0x00FF) << 0);
    UART165050M->UART_DEI |= (((_div & 0xFF00) >> 8) << 0);
    UART165050M->UART_LCR &= ~(1ULL << 7);
    UART165050M->UART_FAR |= (1ULL << 0);
    UART165050M->UART_IIF |= (1ULL << 0) | (1ULL << 4) | (1ULL << 6);
    UART165050M->UART_FAR &= ~(1ULL << 0);
    UART165050M->UART_LCR |= (3ULL << 0);
}