#include<MBR.h>

MBR MBRM;
uint32_t LBA;

void MBR_init(){
    read_single_block(0);
    MBRM = *(MBR*)&DataBuffer.Buffer;
}

void LBA_for_exFAT(){
    int _partion_index = 0;
    LBA = 0x0;
    exFAT* exFATp;

    while(_partion_index < 4){
        for(int _build = 0; _build < 4; _build++){
            LBA |= (MBRM.PartionRecord[_partion_index].Start_LBA[_build] << (8 * _build));
        }

        read_single_block(LBA);
        exFATp = (exFAT*)DataBuffer.Buffer;

        if(compare_fs_name(exFATp->FileSystemName, "EXFAT   ") == 1){
            debug("[+]exFAT find\r\n");
            break;
        }

        _partion_index++;
        LBA = 0x0;
    }
}

int compare_fs_name(volatile uint8_t _buffer[], char _src_name[]){
    int _out = 0;
    int _index = 0;
    while(1){
        if(_src_name[_index] == '\0'){
            break;
        }

        if(_buffer[_index] == _src_name[_index]){
            _out = 1;
        }
        else{
            _out = 0;
            break;
        }

        _index++;
    }

    return _out;
}