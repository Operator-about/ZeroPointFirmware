#include<Freq.h>
#include<GIC.h>
#include<A165050.h>
#include<GPIO.h>


void main_rock(void){
    CRUM = (CRU*)0xFF760000;
    CRU_Rock_init();

    GPIO_ROCK = (GRF*)0xFF770000;
    UART165050_GPIO_ROCK_init();

    UART165050M = (UART165050*)0xFF1A0000;
    UART165050_init(115200, 24000000);
    UART165050M->UART_TRD = 'K';

    while(1){
        __asm__("NOP");
    }

    GICM.GICDR = (GICD*)0xFEE00000;
    GICM.GICRR = (GICR*)0xFEF00000;
    GICM.Mode = 3;
    GICD_init();
    GICR_init();
    GICC_init();
}