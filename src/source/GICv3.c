#include<GIC.h>

void GICDv3_init(){
    if(GICv3M.GICDR->GICD_TYPER & (1ULL << 10)){
        GICv3M.GICDR->GICD_CTLR |= (1ULL << 5) | (1ULL << 4) | (1ULL << 1);
    }
    else{
        GICv3M.GICDR->GICD_CTLR |= (1ULL << 1) | (1ULL << 4);
    }

    GICDv3_IROUTER_AFE();

    int _count = (int)(GICv3M.GICDR->GICD_TYPER & 0x1F);
    _count = 32 * (_count + 1);
    for(int _index = 32; _index < _count; _index++){
        if(_index == UARTPL011M.GIC_ID){
            GICv3M.GICDR->GICD_IGROUPR[_index / 32] |= (1ULL << (_index % 32));
            GICv3M.GICDR->GICD_IGRPMODR[_index / 32] &= ~(1ULL << (_index % 32));
            GICv3M.GICDR->GICD_ICFGR[_index / 16] &= ~(1ULL << ((_index % 16) * 2 + 1));
            GICv3M.GICDR->GICD_IPRIORITYR[_index / 4] |= (1ULL << ((_index % 4) * 8));
            GICv3M.GICDR->GICD_IROUTER[_index] |= (GICv3M.AFE[0] << 0) | (GICv3M.AFE[1] << 8) | (GICv3M.AFE[2] << 16) | (GICv3M.AFE[3] << 32);
            GICv3M.GICDR->GICD_IROUTER[_index] &= ~(1ULL << 31);
            GICv3M.GICDR->GICD_ISENABLER[_index / 32] |= (1ULL << (_index % 32));
        }
    }
}

void GICRv3_init(){
    if(GICv3M.GICRR->GICR_TYPER & (1ULL << 5)){
        GICv3M.GICRR->GICR_CTLR &= ~(1ULL << 25);
    }
    GICv3M.GICRR->GICR_WAKER &= ~(1ULL << 1);
    while(GICv3M.GICRR->GICR_WAKER & (1ULL << 2)){
        __asm__("NOP");
    }
}

void GICCv3_init(){
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
}

void GICDv3_clear_interrupts(){
    int _count = (int)(GICv3M.GICDR->GICD_TYPER & 0x1F);
    _count = 32 * (_count + 1);
    
    for(int _index = 0; _index < _count; _index++){
        GICv3M.GICDR->GICD_ICENABLER[_index / 32] |= (1ULL << (_index % 32));
    }
}

void GICDv3_IROUTER_AFE(){
    volatile uint64_t _afe_local;
    volatile int _LSR = 0;
    for(int _index = 0; _index < 4; _index++){
        __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_afe_local));
        __asm__ volatile("LSR %0, %0, %0" : "=r"(_afe_local) : "r"(_afe_local), "r"(_LSR));
        __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_afe_local) : "r"(_afe_local));
        GICv3M.AFE[_index] = _afe_local;
        _LSR+=8;
    }
}