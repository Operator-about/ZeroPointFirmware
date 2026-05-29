#include<GIC.h>

void GICCv2_init(){
    GICv2_module.GICC->GICC_CTLR |= (1U << 1);
    GICv2_module.GICC->GICC_PMR = 0xFF;
}

void GICDv2_init(){
    GICv2_module.GICD->GICD_CTLR |= (1U << 1);
    
    int _interr_count = (int)(GICv2_module.GICD->GICD_TYPER & 0xF);
    __asm__("MOV X20, %0" : :"r"(_interr_count));

    GICv2_module.GICD->GICD_IGROUPER[121 / 32] |= (1U << (121 % 32));
    GICv2_module.GICD->GICD_ICFGR[153 / 16] &= ~(1U << ((153 % 16) * 2 + 1));
    GICv2_module.GICD->GICD_ITARGETSR[153 / 4] |= (0x01 << ((153 % 4) * 8));
    GICv2_module.GICD->GICD_IPRIORITYR[153 / 4] |= (1U << ((153 % 4) * 8));
    GICv2_module.GICD->GICD_ISENABLER[153 / 32] |= (1U << (153 % 32));
}

void GICDv2_clear_in(){
    GICv2_module.GICD->GICD_ICPENDER[153 / 32] |= (1U << (153 % 32));
}