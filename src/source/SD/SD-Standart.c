#include<SD-Standart.h>

SDA SDAM;

void SDA_controller_init(){
    SDAM.SDAMR->SR_SD |= (1ULL << 0) | (1ULL << 2) | (1ULL << 1);
    
    debug("[^]Wait for reset\r\n");
    wait(0);

    SDAM.SDAMR->PWC_SD = (7ULL << 1) | (1ULL << 0);

    SDAM.SDAMR->NSE_SD = 0xFFFF;
    SDAM.SDAMR->ERSE_SD = 0xFFFF;
    SDAM.SDAMR->NSIE_SD = (1ULL << 5);

    SDAM.SDAMR->BS_SD = 512;
    SDAM.SDAMR->BC_SD = 1;

    SDAM.SDAMR->HC_SD |= (1ULL << 2);
    
    SDA_clock_conf(400, 1000);

    debug("[+]Controller done\r\n");
}

void SDA_clock_conf(int _Hz, int _multi){
    SDAM.SDAMR->CC_SD &= ~(1ULL << 0);
    SDAM.SDAMR->CC_SD &= ~(1ULL << 0);
    SDAM.SDAMR->CC_SD &= ~(1ULL << 2);

    uint16_t _clock = ((((SDAM.SDAMR->CB_SD >> 8) & 0xFF) * _multi) / (2 * _Hz));
    uint16_t _up = ((_clock & 0x300) >> 2);
    uint16_t _bottom = ((_clock & 0x7F) << 8);
    SDAM.SDAMR->CC_SD = _up | _bottom;
    
    SDAM.SDAMR->CC_SD |= (1ULL << 0);
    while((SDAM.SDAMR->CC_SD & (1ULL << 1)) == 0){
        __asm__("NOP");
    }
    SDAM.SDAMR->CC_SD |= (1ULL << 2);
}

void SDA_wait_end_command(){
    while((SDAM.SDAMR->NS_SD & (1ULL << 0)) == 0){
        __asm__("NOP");
    }
}

void SDA_read_single_block(){
    DataBuffer.BufferIndex = 0;
    DataBuffer.BufferCurrentIndex = 0;

    uint32_t _tempory = 0x0;
    for(int _index = 0; _index < 128; _index++){
        _tempory = SDAM.SDAMR->BDP_SD;
        for(int _build = 0; _build < 4; _build++){
            DataBuffer.Buffer[DataBuffer.BufferIndex] = 0x0;
            DataBuffer.Buffer[DataBuffer.BufferIndex] = (_tempory >> (8 * _build));
            DataBuffer.BufferIndex++;
        }
        _tempory = 0x0;
    }
}

void SDA_read_multi_block(){
    DataBuffer.BufferIndex = 0;
    DataBuffer.BufferCurrentIndex = 0;

    uint32_t _tempory = 0x0;
    for(int _index = 0; _index < 8192; _index++){
        _tempory = SDAM.SDAMR->BDP_SD;
        for(int _build = 0; _build < 4; _build++){
            DataBuffer.Buffer[DataBuffer.BufferIndex] = 0x0;
            DataBuffer.Buffer[DataBuffer.BufferIndex] = (_tempory >> (8 * _build));
            DataBuffer.BufferIndex++;
        }
        _tempory = 0x0;
    }
}

void SDA_DAT_line_reset(){
    SDAM.SDAMR->SR_SD |= (1ULL << 2);
    SDAM.SDAMR->NS_SD |= (1ULL << 1);
}

void SDA_ACMD41_init(SDCMD* _ACMD41, SDCMD* _CMD55){
    while((SDAM.SDAMR->RESP_SD[0] & (1ULL << 31)) == 0){
        CMD_send(*_CMD55);
        wait(0);
        CMD_send(*_ACMD41);
        wait(0);
    }
}

void SDA_RCA_init(){
    SD_RCA = SDAM.SDAMR->RESP_SD[0] & 0xFFFF0000;
}

void SDA_CMD_send(SDCMD _CMD){
    SDAM.SDAMR->ARG_SD = _CMD.Arg;
    SDAM.SDAMR->CMD_SD = _CMD.CMD;
}

void SDA_Block_conf(){
    SDAM.SDAMR->BC_SD = 64;
    SDAM.SDAMR->BS_SD = 512;
}

void SDA_NS_ES_close(){
    SDAM.SDAMR->NS_SD = 0xFFFF;
    SDAM.SDAMR->ERS_SD = 0xFFFF;
}