#include<SD.h>

uint32_t SD_RCA;
Data DataBuffer;
uint32_t SD_Standart;

void SD_card_init(){
    SDCMD _CMD0;
    _CMD0.Arg = 0x0;
    _CMD0.CMD = 0x0;
    CMD_send(_CMD0);
    wait(0);

    SDCMD _CMD8;
    _CMD8.Arg = 0x0;
    _CMD8.Arg |= (0xAA << 0) | (1ULL << 8);
    _CMD8.CMD = 0x0;
    _CMD8.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (8ULL << 24);
    CMD_send(_CMD8);
    wait(0);

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

    ACMD41_send(&_ACMD41, &_CMD55);
    SDAM.SDAMR->ERS_SD |= (1ULL << 1);

    SDCMD _CMD2;
    _CMD2.Arg = 0x0;
    _CMD2.CMD = 0x0;
    _CMD2.CMD |= (1ULL << 16) | (1ULL << 19) | (2ULL << 24);
    CMD_send(_CMD2);
    wait(0);

    SDCMD _CMD3;
    _CMD3.Arg = 0x0;
    _CMD3.CMD = 0x0;
    _CMD3.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (3ULL << 24);
    CMD_send(_CMD3);
    wait(0);

    RCA_init();

    SDCMD _CMD7;
    _CMD7.Arg = 0x0;
    _CMD7.Arg = SD_RCA;
    _CMD7.CMD = 0x0;
    _CMD7.CMD |= (3ULL << 16) | (1ULL << 19) | (1ULL << 20) | (7ULL << 24);
    CMD_send(_CMD7);
    wait(0);
    wait(0);

    clock_up();

    debug("[+]SD card init\r\n");
}

void read_single_block(uint32_t _block){
    SDCMD _CMD17;
    _CMD17.Arg = 0x0;
    _CMD17.Arg |= _block;
    _CMD17.CMD = 0x0;
    _CMD17.CMD &= ~(1ULL << 0);
    _CMD17.CMD &= ~(1ULL << 5);
    _CMD17.CMD |= (1ULL << 4) | (1ULL << 1);
    _CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24);
    CMD_send(_CMD17);
    wait(0);

    switch(SD_Standart){
        case 0x5354414E:
            SDA_read_single_block();
            break;
        default:
            break;
    }
}

void read_multi_block(uint32_t _block){
    block_conf();

    SDCMD _CMD23;
    _CMD23.Arg = 0x0;
    _CMD23.Arg |= 64;
    _CMD23.CMD = 0x0;
    _CMD23.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (23ULL << 24);

    SDCMD _CMD18;
    _CMD18.Arg = 0x0;
    _CMD18.Arg |= _block;
    _CMD18.CMD = 0x0;
    _CMD18.CMD &= ~(1ULL << 0);
    _CMD18.CMD |= (1ULL << 1) | (1ULL << 4) | (1ULL << 5) | (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (18ULL << 24);

    CMD_send(_CMD23);
    wait(1);
    CMD_send(_CMD18);
    wait(0);

    switch(SD_Standart){
        case 0x5354414E:
            SDA_read_multi_block();
            break;
        default:
            break;
    }
}

void CMD_send(SDCMD _CMD){
    switch(SD_Standart){
        case 0x5354414E:
            SDA_CMD_send(_CMD);
            break;
        default:
            break;
    }
}

void wait(int _NS_wait){
    if(_NS_wait == 1){
        switch(SD_Standart){
            case 0x5354414E:
                SDA_wait_end_command();
                break;
            default:
                break;
        }
    }
    else{
        for(int _wait = 0; _wait < (60 * 60 * 100); _wait++){
            __asm__("NOP");
        }
    }

    close_NS_ES();
}

void ACMD41_send(SDCMD* _ACMD41, SDCMD* _CMD55){
    switch(SD_Standart){
        case 0x5354414E:
            SDA_ACMD41_init(_ACMD41, _CMD55);
            break;
        default:
            break;
    }
}

void RCA_init(){
    switch(SD_Standart){
        case 0x5354414E:
            SDA_RCA_init();
            break;
        default:
            break;
    }
}

void clock_up(){
    switch(SD_Standart){
        case 0x5354414E:
            SDA_clock_conf(50, 1);
            break;
        default:
            break;
    }
}

void block_conf(){
    switch(SD_Standart){
        case 0x5354414E:
            SDA_Block_conf();
            break;
        default:
            break;
    }
}

void close_NS_ES(){
        switch(SD_Standart){
        case 0x5354414E:
            SDA_NS_ES_close();
            break;
        default:
            break;
    }
}