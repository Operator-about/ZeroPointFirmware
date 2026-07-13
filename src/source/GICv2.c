#include<GIC.h>

void GICCv2_init(){
    GICv2M.GICC->GICC_CTLR |= (1U << 1) | (1U << 2);
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
        else if(_index == 158){
            GICv2M.GICD->GICD_IGROUPER[_index / 32] |= (1ULL << (_index % 32));
            GICv2M.GICD->GICD_ICFGR[_index / 16] &= ~(1ULL << ((_index % 16) * 2 + 1));
            GICv2M.GICD->GICD_IPRIORITYR[_index / 4] |= (2ULL << ((_index % 4) * 8));
            GICv2M.GICD->GICD_ITARGETSR[_index / 4] |= (0x01 << ((_index % 4) * 8));
            GICv2M.GICD->GICD_ISENABLER[_index / 32] |= (1ULL << (_index % 32));
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

void IRQh_handel(){
    volatile uint32_t _IAR = GICv2M.GICC->GICC_IAR;
    
    if((_IAR & 0x3FF) < 1022){
        if((_IAR & 0x3FF) == 158){
            volatile uint32_t _tempory_buffer[128];
            for(int _clear = 0; _clear < 128; _clear++){
                _tempory_buffer[_clear] = 0x0;
            }

            for(int _get = 0; _get < 128; _get++){
                _tempory_buffer[_get] = SD_Registers->BDP_SD;
            }

            DAT_buffer = (uint8_t*)_tempory_buffer;

            SD_Registers->NS_SD &= ~(1ULL << 1);
            SD_Registers->NS_SD &= ~(1ULL << 5);
        }
    }

    GICv2M.GICC->GICC_EOIR = _IAR;
}