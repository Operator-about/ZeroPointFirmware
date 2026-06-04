#pragma once
#include<stdint.h>

typedef struct{
    uint32_t RESERVE_1[1];
    volatile uint32_t GPFSEL1;
    uint32_t RESERVE_2[55];
    volatile uint32_t GPIO_PUP_PDN_CNTRL_REG0;
}GPIOPI4;

typedef struct{
    volatile uint32_t GICD_CTLR;
    volatile uint32_t GICD_TYPER;
    volatile uint32_t GICD_IIDR;
    uint32_t RESERVE_1[29];
    volatile uint32_t GICD_IGROUPER[32];
    volatile uint32_t GICD_ISENABLER[32];
    volatile uint32_t GICD_ICENABLER[32];
    volatile uint32_t GICD_ISPENDER[32];
    volatile uint32_t GICD_ICPENDER[32];
    uint32_t RESERVE_4[64];
    volatile uint32_t GICD_IPRIORITYR[255];
    uint32_t RESERVE_5[1];
    volatile uint32_t GICD_ITARGETSR[255];
    uint32_t RESERVE_6[1];
    volatile uint32_t GICD_ICFGR[64];
}GICDv2;

typedef struct{
    volatile uint32_t GICC_CTLR;
    volatile uint32_t GICC_PMR;
    uint32_t RESERVE_1[1];
    volatile uint32_t GICC_IAR;
    volatile uint32_t GICC_EOIR;
    uint32_t RESERVE_2[1019];
    volatile uint32_t GICC_DIR;
}GICCv2;

typedef struct{
    volatile uint32_t UART_DR; 
    uint32_t RESERVE_1[5]; 
    volatile uint32_t UART_FR; 
    uint32_t RESERVE_2[2]; 
    volatile uint32_t UART_IBRD; 
    volatile uint32_t UART_FBRD; 
    volatile uint32_t UART_LCR_H; 
    volatile uint32_t UART_CR; 
    volatile uint32_t UART_IFLS; 
    volatile uint32_t UART_IMSC; 
    volatile uint32_t UART_RIS; 
    volatile uint32_t UART_MIS;
    volatile uint32_t UART_ICR;
}UARTPL011R;

typedef struct{
    UARTPL011R* UARTPL011_REGISTERS;
    int GIC_ID;
}UARTPL011;

typedef struct{
    GICDv2* GICD;
    GICCv2* GICC;
}GICv2;

typedef struct{
    volatile uint32_t GICD_CTLR;
    volatile uint32_t GICD_TYPER;
    volatile uint32_t GICD_IIDR;
    uint32_t RESERVE_1[29];
    volatile uint32_t GICD_IGROUPR[32];
    volatile uint32_t GICD_ISENABLER[32];
    volatile uint32_t GICD_ICENABLER[32];
    volatile uint32_t GICD_ISPENDER[32];
    uint32_t RESERVE_3[96];
    volatile uint32_t GICD_IPRIORITYR[256];
    uint32_t RESERVE_4[256];
    volatile uint32_t GICD_ICFGR[64];
    volatile uint32_t GICD_IGRPMODR[64];
    uint32_t RESERVE_5[6152];
    volatile uint64_t GICD_IROUTER[1019];
}GICDv3;

typedef struct{
    volatile uint32_t GICR_CTLR;
    volatile uint32_t GICR_IIDR;
    volatile uint32_t GICR_TYPER;
    uint32_t RESERVE_1[1];
    volatile uint32_t GICR_WAKER;
}GICRv3;

typedef struct{
    GICDv3* GICDR;
    GICRv3* GICRR;
    volatile uint64_t AFE[4];
}GICv3;

typedef struct{
    volatile uint32_t IBRD;
    volatile uint32_t FBRD; 
}BRD_UART;