#include<GIC.h>

GIC GICM;

void GICD_init(){
    if(GICM.Mode == 2){
        GICM.GICDR->GICD_CTLR |= (1ULL << 1);
    }
    else if(GICM.Mode == 3){
        if(GICM.GICDR->GICD_TYPER & (1ULL << 10)){
            GICM.GICDR->GICD_CTLR |= (1ULL << 5) | (1ULL << 4) | (1ULL << 1);
        }
        else{
            GICM.GICDR->GICD_CTLR |= (1ULL << 4) | (1ULL << 1);
        }
    }

    int _count = (int)(GICM.GICDR->GICD_TYPER & 0x1F);
    _count = 32 * (_count + 1);
    for(int _index = 32; _index < 988; _index++){
        if(_index == GICUART_ID){
            IRQ_set(_index);
        }
        else if(_index == GICSD_ID){
            IRQ_set(_index);
        }
    }
}

void GICR_init(){
    if(GICM.GICRR->GICR_TYPER & (1ULL << 5)){
        GICM.GICRR->GICR_CTLR &= ~(1ULL << 25);
    }
    GICM.GICRR->GICR_WAKER &= ~(1ULL << 1);
    while(GICM.GICRR->GICR_WAKER & (1ULL << 2)){
        __asm__("NOP");
    }
}

void GICC_init(){
    switch(GICM.Mode){
        case 2:
            GICM.GICCR->GICC_CTLR |= (1ULL << 1) | (1ULL << 2);
            GICM.GICCR->GICC_PMR = 0xFF;
            break;
        case 3:
            volatile uint64_t GICC_CTLR;
            volatile uint64_t GICC_SRE;
            volatile uint64_t GICC_IGRPEN1;
            volatile uint64_t GICC_PMR;

            GICC_CTLR |= (1ULL << 6);
            GICC_IGRPEN1 |= (1ULL << 0);
            GICC_SRE |= (1ULL << 3) | (1ULL << 0);
            GICC_PMR = 0xFF;

            __asm__("MSR ICC_CTLR_EL3, %0" : :"r"(GICC_CTLR));
            __asm__("MSR ICC_SRE_EL3, %0" : :"r"(GICC_SRE));
            __asm__("MSR ICC_IGRPEN1_EL3, %0" : :"r"(GICC_IGRPEN1));
            __asm__("MSR ICC_PMR_EL1, %0" : :"r"(GICC_PMR));
            break;
    }
}

void GICDv3_IROUTER_AFE(){
    volatile uint64_t _MPIDR_EL1;
    __asm__("MRS %0, MPIDR_EL1" : "=r"(_MPIDR_EL1));

    for(int _index = 0; _index < 4; _index++){
        GICM.AFE[_index] = (_MPIDR_EL1 >> (8 * _index) & 0xFF);
    }
}

void IRQ_set(int _index){
    GICM.GICDR->GICD_IGROUPR[_index / 32] |= (1ULL << (_index % 32));
    GICM.GICDR->GICD_ICFGR[_index / 16] &= ~(1ULL << ((_index % 16) * 2 + 1));
    GICM.GICDR->GICD_IPRIORITYR[_index / 4] |= (1ULL << ((_index % 4) * 8));
    switch(GICM.Mode){
        case 2:
            GICM.GICDR->GICD_ITARGETSR[_index / 4] |= (0x01 << ((_index % 4) * 8));
            break;
        case 3:
            GICM.GICDR->GICD_IGRPMODR[_index / 32] &= ~(1ULL << (_index % 32));
            GICM.GICDR->GICD_IROUTER[_index] |= (GICM.AFE[0] << 0) | (GICM.AFE[1] << 8) | (GICM.AFE[2] << 16) | (GICM.AFE[3] << 32);
            GICM.GICDR->GICD_IROUTER[_index] &= ~(1ULL << 31);
            break;
    }
    GICM.GICDR->GICD_ISENABLER[_index / 32] |= (1ULL << (_index % 32));
}


void Sync(){
    uint64_t _ESR;
    uint64_t _ELR;
    uint64_t _FAR;

    __asm__("MRS %0, ESR_EL3" : "=r"(_ESR));
    __asm__("MRS %0, ELR_EL3" : "=r"(_ELR));
    __asm__("MRS %0, FAR_EL3" : "=r"(_FAR));

    debugh64(_ESR);
    debug("\r\n");
    debugh64(_ELR);
    debug("\r\n");
    debugh64(_FAR);
    debug("\r\n"); 
    
    //SD_sec_barrier(100);
}