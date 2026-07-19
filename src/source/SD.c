#include<SD.h>

void SD_controller_init(){
    SDM.SD_Registers->SR_SD |= (1ULL << 0) | (1ULL << 1) | (1ULL << 2); //Сброс линий карт
    SDM.SD_Registers->PWC_SD = 0x0; //Сброс питание карты
    if(SDM.SD_Registers->CB_SD & (1ULL << 24)){
        debug("[^]3.3v support\r\n");
        SDM.SD_Registers->PWC_SD |= (7ULL << 1); //Установка питания карты в 3.3v
    }
    else if(SDM.SD_Registers->CB_SD & (1ULL << 25)){
        debug("[^]3.0v support\r\n");
        SDM.SD_Registers->PWC_SD |= (6ULL << 1); //Установка питания карты в 3.0v
    }
    debug("[+]Voltage select\r\n");
    SDM.SD_Registers->PWC_SD |= (1ULL << 0);

    SDM.SD_Registers->NSE_SD = (1ULL << 0) | (1ULL << 1) | (1ULL << 5) | (1ULL << 3) | (1ULL << 5);
    SDM.SD_Registers->ERSE_SD = (1ULL << 0) | (1ULL << 1) | (1ULL << 2) | (1ULL << 3) | (1ULL << 4) | (1ULL << 5);
    SDM.SD_Registers->NSIE_SD = (1ULL << 5);

    SDM.SD_Registers->CC_SD &= ~(1ULL << 0);
    SDM.SD_Registers->CC_SD &= ~(1ULL << 0);
    SDM.SD_Registers->CC_SD &= ~(1ULL << 2);

    uint32_t _base_mhz;
    _base_mhz = (SDM.SD_Registers->CB_SD >> 8) & 0xFF;
    
    uint32_t _result = (_base_mhz * 1000) / (2 * 400);
    uint32_t _up;
    uint32_t _down;
    _up = ((_result & 0x300) >> 2);
    _down = ((_result & 0x7F) << 8);
    SDM.SD_Registers->CC_SD = _up | _down;

    SDM.SD_Registers->CC_SD = (1ULL << 0);
    do{
        debug("[^]Stable to 400 kHz\r\n");
        __asm__("NOP");
    }while(!(SDM.SD_Registers->CC_SD & (1ULL << 1)));
    SDM.SD_Registers->CC_SD |= (1ULL << 2);

    if(SDM.SD_Registers->CC_SD & (1ULL << 1)){
        debug("[+]Stable kHz complete\r\n");
    }

    SDM.SD_Registers->BS_SD = (0x0200 << 0);
    SDM.SD_Registers->BC_SD = (1ULL << 0);
}

void SD_card_init(){
    SDCMD _CMD0; //Отправка карты в IDLE
    _CMD0.Arg = 0x0;
    _CMD0.CMD = 0x0;
    CMD_send(_CMD0);
    SD_sec_barrier(500);

    SDCMD _CMD8; //Базовая проверка карты
    _CMD8.Arg = 0x0;
    _CMD8.Arg |= (1ULL << 8) | (0xAA << 0); //Установка паттерна(0xAA) и OCR(приблизительный) - рабочее напряжение карты
    _CMD8.CMD = 0x0;
    //Начинается с 16 т.к. одна часть регистра - Transmition, а другая - Command
    _CMD8.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (8ULL << 24); //Установка типа ответа R
    CMD_send(_CMD8);
    SD_sec_barrier(500);

    if(((SDM.SD_Registers->RESP_SD[0] >> 8) & 0xF) == 1){
        __asm__("NOP");
    }

    SDCMD _CMD55; //Команда для сигнализации о ACMD
    _CMD55.Arg = 0x0;
    _CMD55.CMD = 0x0;
    _CMD55.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (55ULL << 24);

    SDCMD _ACMD41; //Настройка SD карты
    _ACMD41.Arg = 0x0;
    _ACMD41.Arg |= (0x1FF << 15); //Настройка точного VDD(OCR) 
    _ACMD41.Arg |= (1ULL << 30); //Установка поддержки быстрых карт
    _ACMD41.Arg &= ~(1ULL << 24); //Отключение режима 1.8v
    _ACMD41.Arg &= ~(1ULL << 28); //Установка режима обычной работы
    _ACMD41.CMD = 0x0;
    _ACMD41.CMD |= (2ULL << 16) | (41ULL << 24);
    
    while((SDM.SD_Registers->RESP_SD[0] & (1ULL << 31)) == 0){ //Цикл ожидания завершения инцилизации карты
        CMD_send(_CMD55);
        SD_sec_barrier(100);
        CMD_send(_ACMD41);
        SD_sec_barrier(100);
    }

    SDM.SD_Registers->ERS_SD &= ~(1ULL << 1); //Сброс ошибки CRC

    SDCMD _CMD2; //Перевод карты в режим индификации
    _CMD2.Arg = 0x0;
    _CMD2.CMD = 0x0;
    _CMD2.CMD |= (1ULL << 16) | (1ULL << 19) | (2ULL << 24);
    CMD_send(_CMD2);
    SD_sec_barrier(500);

    SDCMD _CMD3; //Выдача карте специального RCA адреса
    _CMD3.Arg = 0x0;
    _CMD3.CMD = 0x0;
    _CMD3.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (3ULL << 24);
    CMD_send(_CMD3);
    SD_sec_barrier(500);
    SD_RCA = SDM.SD_Registers->RESP_SD[0] & 0xFFFF0000; //Запись адреса 

    SDCMD _CMD7; //Перевод карты в режим транспортировки данных 
    _CMD7.Arg = 0x0;
    _CMD7.Arg |= SD_RCA;
    _CMD7.CMD = 0x0;
    _CMD7.CMD |= (3ULL << 16) | (1ULL << 19) | (1ULL << 20) | (7ULL << 24);
    CMD_send(_CMD7);
    SD_sec_barrier(500);

    SDM.SD_Registers->SR_SD |= (1ULL << 2); //Сброс DAT линии

    SDM.SD_Registers->HC_SD |= (1ULL << 2); //Включение поддержки 50мГц

    SDM.SD_Registers->CC_SD &= ~(1ULL << 0);
    SDM.SD_Registers->CC_SD &= ~(1ULL << 0);
    SDM.SD_Registers->CC_SD &= ~(1ULL << 2);

    //Смена частоты с 400кГц на 50мГц
    uint32_t _base_mhz;
    _base_mhz = (SDM.SD_Registers->CB_SD >> 8) & 0xFF;
    
    uint32_t _result = (_base_mhz / (2 * 50));
    uint32_t _up;
    uint32_t _down;
    _up = ((_result & 0x300) >> 2);
    _down = ((_result & 0x7F) << 8);
    SDM.SD_Registers->CC_SD = _up | _down;
    SDM.SD_Registers->CC_SD = (1ULL << 0);
    do{
        __asm__("NOP");
    }while(!(SDM.SD_Registers->CC_SD & (1ULL << 1)));
    SDM.SD_Registers->CC_SD |= (1ULL << 2);

    debug("[+]SD card - +\r\n");
}

void SD_preparing(){
    SDM.SD_Registers->HC_SD |= (1ULL << 1);
    
    SDCMD _CMD55;
    _CMD55.Arg = 0x0;
    _CMD55.Arg |= SD_RCA;
    _CMD55.CMD = 0x0;
    _CMD55.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (55ULL << 24);

    SDCMD _ACMD6; //Активация 4-битного режима для DAT линии
    _ACMD6.Arg = 0x0;
    _ACMD6.Arg |= (2ULL << 0);
    _ACMD6.CMD = 0x0;
    _ACMD6.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (6ULL << 24);

    CMD_send(_CMD55);
    SD_sec_barrier(50);
    CMD_send(_ACMD6);

    debug("[+]4 DAT line is active\r\n");
}

void read_block(uint32_t _sector){
    SD_sec_barrier(50);
    SDCMD _CMD17; //Команда для чтения блока данных с карты
    _CMD17.Arg = 0x0;
    _CMD17.Arg |= _sector;
    _CMD17.CMD = 0x0;
    _CMD17.CMD |= (1ULL << 1) | (1ULL << 4);
    _CMD17.CMD &= ~(1ULL << 0);
    _CMD17.CMD |= (2ULL << 16) | (1ULL << 19) | (1ULL << 20) | (1ULL << 21) | (17ULL << 24);
    CMD_send(_CMD17);
    SD_sec_barrier(50);

    volatile uint32_t _tempory_buffer[128];
    for(int _index = 0; _index < 128; _index++){
        _tempory_buffer[_index] = SDM.SD_Registers->BDP_SD;
    }
    DAT_buffer = (uint8_t*)_tempory_buffer;
}

void CMD_send(SDCMD _CMD){
    SDM.SD_Registers->ARG_SD = _CMD.Arg;
    SDM.SD_Registers->CMD_SD = _CMD.CMD;
}

void SD_sec_barrier(int _second){
    int _total = ((60 * 60) * _second);
    for(int i = 0; i < _total; i++){
        __asm__("NOP");
    }
}