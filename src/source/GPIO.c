#include<GPIO.h>

void UARTPL011_GPIO_PI4_init(){
    GPIO->GPFSEL1 &= ~(7ULL << 12);
    GPIO->GPFSEL1 &= ~(7ULL << 15);

    GPIO->GPFSEL1 |= (4ULL << 12);
    GPIO->GPFSEL1 |= (4ULL << 15);
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 &= ~(2ULL << 28);
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 &= ~(2ULL << 30);
}
