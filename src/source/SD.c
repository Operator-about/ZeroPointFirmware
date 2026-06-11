#include<SD.h>

uint32_t Response_CMD[4];

void SD_init(){
    if(SD_Registers->CB_SD & (1ULL << 24)){
        debug("3.3v support\r\n");
        SD_Registers->PWC_SD = (7ULL << 1);
    }
    else if(SD_Registers->CB_SD & (1ULL << 25)){
        debug("3.0v support\r\n");
        SD_Registers->PWC_SD = (6ULL << 1);
    }
    debug("Voltage select\r\n");
    SD_Registers->PWC_SD |= (1ULL << 0);

    SD_Registers->HC_SD &= ~(1ULL << 1);

    SD_Registers->NSE_SD = (1ULL << 0);
    SD_Registers->ERSE_SD = (1ULL << 0) | (1ULL << 1) | (1ULL << 2) | (1ULL << 3) | (1ULL << 4) | (1ULL << 5);

    uint64_t _base_mhz;
    _base_mhz = (SD_Registers->CB_SD >> 8) & 0xFF;
    
    int _result = ((int)_base_mhz * 1000) / (2 * 400);
    uint16_t _up;
    uint16_t _down;
    _up = ((_result & 0x300) >> 2);
    _down = (_result & 0xFF);
    SD_Registers->CC_SD = _up | _down;

    SD_Registers->CC_SD = (1ULL << 0);
    while((SD_Registers->CC_SD & (1ULL << 1)) == 0){
        debug("Stable...\r\n");
        __asm__("NOP");
    }
    SD_Registers->CC_SD |= (1ULL << 2);

}
