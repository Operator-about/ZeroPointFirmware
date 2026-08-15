#include<GPIO.h>

GPIOPI4* GPIO;
GRF* GPIO_ROCK;

void UARTPL011_GPIO_PI4_init(){
    GPIO->GPFSEL1 &= ~(7ULL << 12);
    GPIO->GPFSEL1 &= ~(7ULL << 15);

    GPIO->GPFSEL1 |= (4ULL << 12);
    GPIO->GPFSEL1 |= (4ULL << 15);
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 &= ~(2ULL << 28);
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 &= ~(2ULL << 30);
}

void UART165050_GPIO_ROCK_init(){
    GPIO_ROCK->GRF_GPIO4_IOMUX[2] |= (1ULL << 6) | (1ULL << 8) | (1ULL << 22) | (1ULL << 24);
    GPIO_ROCK->GRF_GPIO4_PULL[2] |= ~((3ULL << 6) | (3ULL << 8) | (3ULL << 22) | (3ULL << 24));
    GPIO_ROCK->GRF_GPIO4_SR[2] |= (1ULL << 0) | (1ULL << 16);
}