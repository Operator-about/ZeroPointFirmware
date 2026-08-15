#include<Freq.h>

CRU* CRUM;

void CRU_Rock_init(){
    //Настройка GPPL генератора
    CRUM->CRU_GPPL_CON[0] = (99ULL << 0) | (99ULL << 16);
    CRUM->CRU_GPPL_CON[1] = (1ULL << 0) | (4ULL << 8) | (1ULL << 12) | (1ULL << 16) | (4ULL << 24) | (1ULL << 28);
    CRUM->CRU_GPPL_CON[3] |= (1ULL << 8) | (1ULL << 24);
    CRUM->CRU_GPPL_CON[3] &= ~((1ULL << 0) | (1ULL << 16));

    //Частота для UART
    CRUM->CRU_CLKSEL_CON[35] &= ~((1ULL << 0) | (1ULL << 16));
    CRUM->CRU_CLKSEL_CON[35] |= (2ULL << 8) | (2ULL << 24);
    CRUM->CRU_CLKGATE_CON[9] |= (1ULL << 4) | (1ULL << 20);

    //Частота для SD
    CRUM->CRU_CLKSEL_CON[15] |= (11ULL << 0) | (1ULL << 8) | (11ULL << 16) | (1ULL << 24);
    CRUM->CRU_CLKSEL_CON[13] |= (11ULL << 8) | (1ULL << 15) | (11ULL << 24) | (1ULL << 31);
    CRUM->CRU_CLKGATE_CON[12] |= (1ULL << 13) | (1ULL << 29);
    CRUM->CRU_CLKGATE_CON[6] |= (1ULL << 1) | (1ULL << 17);

    //Частота для GIC-500
    CRUM->CRU_CLKSEL_CON[56] |= (1ULL << 8) | (1ULL << 15) | (1ULL << 24) | (1ULL << 31);
    CRUM->CRU_CLKGATE_CON[12] |= (1ULL << 12) | (1ULL << 28);
}