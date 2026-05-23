#include<GIC.h>

void GICCv2_init(){
    GICv2_module.GICC->GICC_CTLR |= (1ULL << 1) | (1ULL << 10);
    GICv2_module.GICC->GICC_PMR = 0xFF;
}

void GICDv2_init(){
    GICv2_module.GICD->GICD_CTLR |= (1ULL << 1);
    for(int _ID = 32; _ID < 200; _ID++){
        GICv2_module.GICD->GICD_IGROUPER[_ID / 32] |= (1ULL << (_ID % 32));
        GICv2_module.GICD->GICD_ICFGR[_ID / 16] &= ~(1ULL << ((_ID % 16) * 2 + 1));
        GICv2_module.GICD->GICD_IPRIORITYR[_ID / 4] |= (1ULL << ((_ID % 4) * 8));
        GICv2_module.GICD->GICD_ITARGETSR[_ID / 4] |= (0x01 << ((_ID % 4) * 8));
        GICv2_module.GICD->GICD_ISENABLER[_ID / 32] |= (1ULL << (_ID % 32));
    }
}