#include<SD.h>

void SD_controller_init(){
    SD_Registers->SR_SD |= (1ULL << 0) | (1ULL << 1) | (1ULL << 2);
    SD_Registers->PWC_SD = 0x0;
    if(SD_Registers->CB_SD & (1ULL << 24)){
        debug("[^]3.3v support\r\n");
        SD_Registers->PWC_SD |= (7ULL << 1);
    }
    else if(SD_Registers->CB_SD & (1ULL << 25)){
        debug("[^]3.0v support\r\n");
        SD_Registers->PWC_SD |= (6ULL << 1);
    }
    debug("[+]Voltage select\r\n");
    SD_Registers->PWC_SD |= (1ULL << 0);

    SD_Registers->NSE_SD = (1ULL << 0) | (1ULL << 1) | (1ULL << 5) | (1ULL << 3) | (1ULL << 5);
    SD_Registers->ERSE_SD = (1ULL << 0) | (1ULL << 1) | (1ULL << 2) | (1ULL << 3) | (1ULL << 4) | (1ULL << 5);
    SD_Registers->NSIE_SD = (1ULL << 5);

    SD_Registers->CC_SD &= ~(1ULL << 0);
    SD_Registers->CC_SD &= ~(1ULL << 0);
    SD_Registers->CC_SD &= ~(1ULL << 2);

    uint32_t _base_mhz;
    _base_mhz = (SD_Registers->CB_SD >> 8) & 0xFF;
    
    uint32_t _result = (_base_mhz * 1000) / (2 * 400);
    uint32_t _up;
    uint32_t _down;
    _up = ((_result & 0x300) >> 2);
    _down = ((_result & 0x7F) << 8);
    SD_Registers->CC_SD = _up | _down;

    SD_Registers->CC_SD = (1ULL << 0);
    do{
        debug("[^]Stable to 400 kHz\r\n");
        __asm__("NOP");
    }while(!(SD_Registers->CC_SD & (1ULL << 1)));
    SD_Registers->CC_SD |= (1ULL << 2);

    if(SD_Registers->CC_SD & (1ULL << 1)){
        debug("[+]Stable kHz complete\r\n");
    }

    SD_Registers->BS_SD = (0x0200 << 0);
    SD_Registers->BC_SD = (1ULL << 0);
}

void SD_card_init(){
    SDCMD _CMD0;
    _CMD0.Arg = 0x0;
    _CMD0.CMD = 0x0;
    CMD_send(_CMD0);
    SD_sec_barrier(50);

    SDCMD _CMD8;
    _CMD8.Arg = 0x0;
    _CMD8.Arg |= (1ULL << 8) | (0xAA << 0);
    _CMD8.CMD = 0x0;
    _CMD8.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (8ULL << 24);
    CMD_send(_CMD8);
    SD_sec_barrier(50);

    if(((SD_Registers->RESP_SD[0] >> 8) & 0xF) == 1){
        debug("[+]2.7 ~ 3.6v supported\r\n");
    }
    else{
        debug("[*]Unsupported SD card\r\n");
        return;
    }

    SDCMD _CMD55;
    _CMD55.Arg = 0x0;
    _CMD55.CMD = 0x0;
    _CMD55.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (55ULL << 24);

    SDCMD _ACMD41;
    _ACMD41.Arg = 0x0;
    _ACMD41.Arg |= (0x1FF << 15);
    _ACMD41.Arg |= (1ULL << 30);
    _ACMD41.Arg &= ~(1ULL << 24);
    _ACMD41.Arg &= ~(1ULL << 28);
    _ACMD41.CMD = 0x0;
    _ACMD41.CMD |= (2ULL << 16) | (41ULL << 24);
    
    while((SD_Registers->RESP_SD[0] & (1ULL << 31)) == 0){
        debug("[^]Card init. Please wait\r\n");
        CMD_send(_CMD55);
        SD_sec_barrier(50);
        CMD_send(_ACMD41);
        SD_sec_barrier(50);
    }

    SD_Registers->ERS_SD &= ~(1ULL << 1);

    if(SD_Registers->RESP_SD[0] & (1ULL << 30)){
        debug("[+]Card version >= 3.0\r\n");
    }

    SDCMD _CMD2;
    _CMD2.Arg = 0x0;
    _CMD2.CMD = 0x0;
    _CMD2.CMD |= (1ULL << 16) | (1ULL << 19) | (2ULL << 24);
    CMD_send(_CMD2);
    SD_sec_barrier(50);

    SDCMD _CMD3;
    _CMD3.Arg = 0x0;
    _CMD3.CMD = 0x0;
    _CMD3.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (3ULL << 24);
    CMD_send(_CMD3);
    SD_sec_barrier(50);
    SD = SD_Registers->RESP_SD[0] & 0xFFFF0000;

    SDCMD _CMD7;
    _CMD7.Arg = 0x0;
    _CMD7.Arg |= SD;
    _CMD7.CMD = 0x0;
    _CMD7.CMD |= (3ULL << 16) | (1ULL << 19) | (1ULL << 20) | (7ULL << 24);
    CMD_send(_CMD7);
    SD_sec_barrier(50);

    SD_Registers->SR_SD |= (1ULL << 2);

    SD_Registers->HC_SD |= (1ULL << 2);

    SD_Registers->CC_SD &= ~(1ULL << 0);
    SD_Registers->CC_SD &= ~(1ULL << 0);
    SD_Registers->CC_SD &= ~(1ULL << 2);

    uint32_t _base_mhz;
    _base_mhz = (SD_Registers->CB_SD >> 8) & 0xFF;
    
    uint32_t _result = (_base_mhz / (2 * 50));
    uint32_t _up;
    uint32_t _down;
    _up = ((_result & 0x300) >> 2);
    _down = ((_result & 0x7F) << 8);
    SD_Registers->CC_SD = _up | _down;
    SD_Registers->CC_SD = (1ULL << 0);
    do{
        debug("[^]Stable to 50 mHz\r\n");
        __asm__("NOP");
    }while(!(SD_Registers->CC_SD & (1ULL << 1)));
    SD_Registers->CC_SD |= (1ULL << 2);

    debug("[+]SD card - +\r\n");
}

void read_block(uint32_t _sector){
    SD_sec_barrier(50);
    SDCMD _CMD17;
    _CMD17.Arg = 0x0;
    _CMD17.Arg |= _sector;
    _CMD17.CMD = 0x0;
    _CMD17.CMD |= (1ULL << 1) | (1ULL << 4);
    _CMD17.CMD &= ~(1ULL << 0);
    _CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24);
    CMD_send(_CMD17);
    //SD_get_status();
    SD_sec_barrier(50);
}

void CMD_send(SDCMD _CMD){
    SD_Registers->ARG_SD = _CMD.Arg;
    SD_Registers->CMD_SD = _CMD.CMD;
}

void SD_sec_barrier(int _second){
    int _total = ((60 * 60) * _second);
    for(int i = 0; i < _total; i++){
        __asm__("NOP");
    }
}

void SD_get_status(){
    if(SD_Registers->NS_SD & (1ULL << 0)){
        debug("[^]CDM complet\r\n");
    }

    if(SD_Registers->ERS_SD & (1ULL << 0)){
        debug("[*]Timeout error!\r\n");
    }
    else if(SD_Registers->ERS_SD & (1ULL << 1)){
        debug("[*]CRC error!\r\n");
    }
    else if(SD_Registers->ERS_SD & (1ULL << 4)){
        debug("[*]Data timeout error\r\n");
    }
    else if(SD_Registers->ERS_SD & (1ULL << 5)){
        debug("[*]CRC data error\r\n");
    }
    else{
        debug("[^]Error no detected\r\n");
    }
}

void DAT_clear(){
    for(int _clear = 0; _clear < 128; _clear++){
        DAT_buffer[_clear] = 0x0;
    }
}