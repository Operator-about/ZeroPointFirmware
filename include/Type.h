#pragma once
#include<stdint.h>

typedef struct{
    uint64_t RESERVE_3;
    volatile uint8_t Start_LBA[4];
    uint32_t RESERVE_4;
}__attribute__((packed)) MBRP;

typedef struct{
    volatile uint8_t BootCode[440];
    volatile uint32_t UMBRDS;
    uint16_t RESERVE_1[1];
    MBRP PartionRecord[4];
    volatile uint8_t Signature[2];
}__attribute__((packed)) MBR;

typedef struct{
    uint8_t RESERVE_1[3];
    volatile uint8_t FileSystemName[8];
    uint8_t RESERVE_2[69];
    volatile uint32_t FATOffset;
    volatile uint32_t FATLength;
    volatile uint32_t ClusterHeapOffset;
    volatile uint32_t ClusterCount;
    volatile uint32_t FirstClusterRoot;
    uint8_t RESERVE_3[8];
    volatile uint8_t BytsPerSectorP;
    volatile uint8_t SectorsPerClusterP;
    volatile uint8_t NumberFAT;
}__attribute__((packed))exFAT;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t SecondaryCount;
    uint16_t RESERVE_1;
    volatile uint8_t FileAttributes[2];
    uint8_t RESERVE_2[26];
}__attribute__((packed))exFAT_FileDirectoryEntry;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t GeneralSecondaryFlags;
    uint8_t RESERVE_2[18];
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;
}__attribute__((packed))exFAT_StreamExtensionDirectoryEntry;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t GeneralSecondaryFlags;
    volatile uint8_t FileName[30];
}__attribute__((packed))exFAT_FileNameDirectoryEntry;

typedef struct{
    volatile uint32_t BytsPerSector;
    volatile uint32_t SectorsPerCluster;
    volatile uint32_t FirstClusterRoot;
    volatile uint32_t LBA_data_regione;
    volatile uint32_t LBA_FAT_regione;
    volatile uint32_t ClusterHeapOffset;
}__attribute__((packed)) exFAT_attribute;

typedef struct{
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;
    volatile uint8_t Name[30];
    volatile uint8_t NoFATChain;
}DataKernel;

typedef struct{
    volatile uint32_t GPFSEL0;
    volatile uint32_t GPFSEL1;
    volatile uint32_t GPFSEL2;
    volatile uint32_t GPFSEL3;
    volatile uint32_t GPFSEL4;
    volatile uint32_t GPFSEL5;
    uint32_t RESERVE_3[52];
    volatile uint32_t GPIO_PUP_PDN_CNTRL_REG0;
    volatile uint32_t GPIO_PUP_PDN_CNTRL_REG1;
    volatile uint32_t GPIO_PUP_PDN_CNTRL_REG2;
    volatile uint32_t GPIO_PUP_PDN_CNTRL_REG3;
}__attribute__((packed)) GPIOPI4;


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
}__attribute__((packed)) UARTPL011R;

typedef struct{
    UARTPL011R* UARTPL011_REGISTERS;
    int GIC_ID;
}__attribute__((packed)) UARTPL011;

typedef struct{
    uint8_t Buffer[32768];
    int BufferIndex;
    int BufferCurrentIndex;
}Data;

typedef struct{
    volatile uint32_t GICC_CTLR;
    volatile uint32_t GICC_PMR;
    uint32_t RESERVE_1[1];
    volatile uint32_t GICC_IAR;
    volatile uint32_t GICC_EOIR;
    uint32_t RESERVE_2[1019];
    volatile uint32_t GICC_DIR;
}__attribute__((packed)) GICC;

typedef struct{
    volatile uint32_t GICD_CTLR;
    volatile uint32_t GICD_TYPER;
    volatile uint32_t GICD_IIDR;
    uint32_t RESERVE_1[29];
    volatile uint32_t GICD_IGROUPR[32];
    volatile uint32_t GICD_ISENABLER[32];
    volatile uint32_t GICD_ICENABLER[32];
    volatile uint32_t GICD_ISPENDER[32];
    volatile uint32_t GICD_ICPENDER[32];
    uint32_t RESERVE_2[64];
    volatile uint32_t GICD_IPRIORITYR[255];
    uint32_t RESERVE_3[1];
    volatile uint32_t GICD_ITARGETSR[255];
    uint32_t RESERVE_4[1];
    volatile uint32_t GICD_ICFGR[64];
    volatile uint32_t GICD_IGRPMODR[64];
    uint32_t RESERVE_5[6152];
    volatile uint64_t GICD_IROUTER[1019];
}__attribute__((packed)) GICD;

typedef struct{
    volatile uint32_t GICR_CTLR;
    volatile uint32_t GICR_IIDR;
    volatile uint32_t GICR_TYPER;
    uint32_t RESERVE_1[1];
    volatile uint32_t GICR_WAKER;
}__attribute__((packed)) GICR;

typedef struct{
    GICD* GICDR;
    GICR* GICRR;
    GICC* GICCR;
    volatile uint64_t AFE[4];
    int Mode;
}__attribute__((packed)) GIC;

typedef struct{
    volatile uint32_t IBRD;
    volatile uint32_t FBRD; 
}__attribute__((packed)) BRD_UART;

typedef struct{
    uint32_t RESERVE_1[1];
    volatile uint16_t BS_SD;
    volatile uint16_t BC_SD;
    volatile uint32_t ARG_SD;
    volatile uint32_t CMD_SD;
    volatile uint32_t RESP_SD[4];
    volatile uint32_t BDP_SD;
    volatile uint32_t PS_SD;
    volatile uint8_t HC_SD;
    volatile uint8_t PWC_SD;
    uint16_t RESERVE_4[1];
    volatile uint16_t CC_SD;
    uint8_t RESERVE_5[1];
    volatile uint8_t SR_SD;
    volatile uint16_t NS_SD;
    volatile uint16_t ERS_SD;
    volatile uint16_t NSE_SD;
    volatile uint16_t ERSE_SD;
    volatile uint16_t NSIE_SD;
    uint16_t RESERVE_6[2];
    volatile uint16_t HC2_SD;
    volatile uint64_t CB_SD;
}__attribute__((packed)) SDAR;

typedef struct{
    SDAR* SDAMR;
    int GIC_ID;
}__attribute__((packed)) SDA;

typedef struct{
    volatile uint32_t CTLR_SD;
    volatile uint32_t PWC_SD;
    volatile uint32_t CLKDIV_SD;
    volatile uint32_t CLKS_SD;
    volatile uint32_t CLKE_SD;
    volatile uint32_t TIME_SD;
    uint32_t RESERVE_1[1];
    volatile uint32_t BS_SD;
    volatile uint32_t BC_SD;
    volatile uint32_t IM_SD;
    volatile uint32_t ARG_SD;
    volatile uint32_t CMD_SD;
    volatile uint32_t RESP_SD[4];
    volatile uint32_t IMS_SD;
    uint32_t RESERVE_2[111];
    volatile uint32_t BDP_SD;
}__attribute__((packed)) SDRRock;

typedef struct{
    volatile uint32_t Arg;
    volatile uint32_t CMD;
}__attribute__((packed)) SDCMD;

typedef struct{
    volatile uint64_t SD;
    volatile uint64_t GICv2;
    volatile uint64_t UART;
    uint32_t UART_Standart;
    uint32_t SD_Standart;
    int UART_ID;
    int SD_ID;
}__attribute__((packed)) JumpData;

typedef struct{
    volatile uint32_t CRU_LPPL_CON[6];
    volatile uint32_t CRU_BPPL_CON[6];
    volatile uint32_t CRU_DPPL_CON[6];
    volatile uint32_t CRU_CPPL_CON[6];
    volatile uint32_t CRU_GPPL_CON[6];
    volatile uint32_t CRU_NPPL_CON[6];
    volatile uint32_t CRU_VPPL_CON[6];
    uint32_t RESERVE_1[10];
    volatile uint32_t CRU_CLKSEL_CON[108];
    uint32_t RESERVE_2[20];
    volatile uint32_t CRU_CLKGATE_CON[35];
}__attribute__((packed)) CRU;

typedef struct{
    volatile uint32_t UART_TRD;
    volatile uint32_t UART_DEI;
    volatile uint32_t UART_IIF;
    volatile uint32_t UART_LCR;
    volatile uint32_t UART_MCR;
    volatile uint32_t UART_LSR;
    uint32_t RESERVE_1[22];
    volatile uint32_t UART_FAR;
}__attribute__((packed)) UART165050;

typedef struct{
    uint32_t RESERVE_1[14335];
    volatile uint32_t GRF_GPIO2_IOMUX[4];
    volatile uint32_t GRF_GPIO3_IOMUX[4];
    volatile uint32_t GRF_GPIO4_IOMUX[4];
    uint32_t RESERVE_2[4];
    volatile uint32_t GRF_GPIO2_PULL[4];
    volatile uint32_t GRF_GPIO3_PULL[4];
    volatile uint32_t GRF_GPIO4_PULL[4];
    uint32_t RESERVE_3[4];
    volatile uint32_t GRF_GPIO2_SR[4];
    volatile uint32_t GRF_GPIO3_SR[4];
    volatile uint32_t GRF_GPIO4_SR[4];
}__attribute__((packed)) GRF;