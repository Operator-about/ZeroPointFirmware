#include<GIC.h>

void GICCv2_init(){
    GICv2M.GICC->GICC_CTLR |= (1U << 1);
    GICv2M.GICC->GICC_PMR = 0xFF;
}

void GICDv2_init(){
    GICv2M.GICD->GICD_CTLR |= (1U << 1);
    
    int _count = (int)(GICv2M.GICD->GICD_TYPER & 0x1F);
    
    _count = 32 * (_count + 1);
    
    for(int _index = 32; _index < _count; _index++){
        if(_index == UARTPL011M.GIC_ID){
            GICv2M.GICD->GICD_IGROUPER[UARTPL011M.GIC_ID / 32] |= (1U << (153 % 32));
            GICv2M.GICD->GICD_ICFGR[_index / 16] &= ~(1U << ((_index % 16) * 2 + 1));
            GICv2M.GICD->GICD_ITARGETSR[_index / 4] |= (0x01 << ((_index % 4) * 8));
            GICv2M.GICD->GICD_IPRIORITYR[_index / 4] |= (1U << ((_index % 4) * 8));
            GICv2M.GICD->GICD_ISENABLER[_index / 32] |= (1U << (_index % 32));
        }
    }
}

void GICDv2_clear_interrupts(){
    int _count = (int)(GICv2M.GICD->GICD_TYPER & 0x1F);
    
    _count = 32 * (_count + 1);
    for(int _index = 32; _index < _count; _index++){
        GICv2M.GICD->GICD_ICENABLER[_index / 32] |= (1U << (_index % 32));
    }
}