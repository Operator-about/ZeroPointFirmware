#pragma once
#include<stdint.h>

typedef struct{
    uint8_t RESERVE_3[8];
    volatile uint8_t Start_LBA[4];
    uint32_t RESERVE_4[1];
}__attribute__((packed)) MBRP;

typedef struct{
    volatile uint8_t BootCode[440];
    volatile uint32_t UMBRDS;
    uint16_t RESERVE_1[1];
    MBRP PartionRecord[4];
    volatile uint8_t Signature[2];
}__attribute__((packed)) MBR;

typedef struct{
    uint8_t RESERVE_1[80];
    volatile uint32_t FATOffset;
    volatile uint32_t FATLength;
    volatile uint32_t ClusterHeapOffset;
    volatile uint32_t ClusterCount;
    volatile uint32_t FirstClusterRoot;
    uint8_t RESERVE_2[8];
    volatile uint8_t BytsPerSectorP;
    volatile uint8_t SectorsPerClusterP;
    volatile uint8_t NumberFAT;
}__attribute__((packed))exFAT_BPB;

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
    uint8_t RESERVE_1;
    volatile uint8_t NameLength;
    uint8_t RESERVE_2[16];
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;
}__attribute__((packed))exFAT_StreamExtensionDirectoryEntry;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t GeneralSecondaryFlags;
    volatile uint8_t FileName[30];
}__attribute__((packed))exFAT_FileNameDirectoryEntry;

typedef struct{
    volatile uint8_t EntryType;
    volatile uint8_t BitmapFlags;
    uint8_t RESERVE_2[18];
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;
}__attribute__((packed))exFAT_AllocationBitmapDirectoryEntry;

typedef struct{
    volatile uint8_t EntryType;
    uint8_t RESERVE_1[3];
    volatile uint32_t TableCheckSum;
    uint8_t RESERVE_2[12];
    volatile uint32_t FirstCluster;
    volatile uint64_t DataLength;
}__attribute__((packed))exFAT_UpCaseTableDirectoryEntry;

typedef struct{
    volatile uint32_t BytsPerSector;
    volatile uint32_t SectorsPerCluster;
    volatile uint32_t FirstClusterRoot;
    volatile uint32_t LBA_data_regione;
    volatile uint32_t LBA_FAT_regione;
    volatile uint32_t ClusterHeapOffset;
}exFAT_attribute;

typedef struct{
    uint32_t FirstCluster;
    uint32_t DataLength;
    uint8_t NoFATChain;
}KernelFile;

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
}SDR;

typedef struct{
    SDR* SD_Registers;
    int GIC_ID;
}SD;

typedef struct{
    volatile uint32_t Arg;
    volatile uint32_t CMD;
}SDCMD;

