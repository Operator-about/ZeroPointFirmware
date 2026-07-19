#include<exFAT.h>

uint8_t* Buffer;
uint32_t* Kernel_buffer;

void exFAT_init(){
    Kernel_buffer = (uint32_t*)KERNEL_ADDRESS;
    Buffer = (uint8_t*)0x00040000;

    read_block(0);
    MBR* _MBR;
    _MBR->PartionRecord[0].Start_LBA[0] = 0x0;
    _MBR->PartionRecord[1].Start_LBA[0] = 0x0;
    _MBR = (MBR*)DAT_buffer;

    volatile uint32_t _LBA = 0x0;
    exFAT_BPB* _exFAT_ptr;
    if(_MBR->PartionRecord[1].Start_LBA != 0x0){
        for(int _build_LBA = 0; _build_LBA < 4; _build_LBA++){
            _LBA |= ((uint32_t)_MBR->PartionRecord[1].Start_LBA[_build_LBA] << (8 * _build_LBA)); //Сборка первого абсолютного сектора exFAT
        }
    }

    read_block(_LBA);
    _exFAT_ptr = (exFAT_BPB*)DAT_buffer;
    
    exFAT_attr.BytsPerSector = power_two(_exFAT_ptr->BytsPerSectorP);
    exFAT_attr.SectorsPerCluster = power_two(_exFAT_ptr->SectorsPerClusterP);
    exFAT_attr.LBA_FAT_regione = _LBA + _exFAT_ptr->FATOffset;
    exFAT_attr.LBA_data_regione = _LBA + _exFAT_ptr->ClusterHeapOffset;
    exFAT_attr.FirstClusterRoot = _exFAT_ptr->FirstClusterRoot;
    exFAT_attr.ClusterHeapOffset = _exFAT_ptr->ClusterHeapOffset;

    walk_FAT_table(exFAT_attr.FirstClusterRoot);
    read_cluster(exFAT_attr.LBA_data_regione + ((exFAT_attr.FirstClusterRoot - 2) * exFAT_attr.SectorsPerCluster), 1); //Чтение Root-кластера
    KernelFile _kernel = get_first_cluster_kernel(); //Получение первого кластера ядра
    
    for(int _clear_buffer = 0; _clear_buffer < 32768; _clear_buffer++){
       Buffer[_clear_buffer] = 0x0;
    }
    
    if(_kernel.NoFATChain & (1ULL << 1)){
        for(int _get = 0; _get < (int)(_kernel.DataLength / (exFAT_attr.BytsPerSector * exFAT_attr.SectorsPerCluster)); _get++){
            read_cluster(exFAT_attr.LBA_data_regione + ((_kernel.FirstCluster - 2) * exFAT_attr.SectorsPerCluster), (int)(_kernel.DataLength / (exFAT_attr.BytsPerSector * exFAT_attr.SectorsPerCluster)));
        }
        _kernel.FirstCluster++;
    }

    to_kernel_buffer((int)(_kernel.DataLength / (exFAT_attr.BytsPerSector * exFAT_attr.SectorsPerCluster)));
}

void read_cluster(uint32_t _cluster, int _count_cluster){
    int _count = 0;

    for(int _index_count = 0; _index_count < _count_cluster; _index_count++){
        for(int _current_sector = 0; _current_sector < exFAT_attr.SectorsPerCluster; _current_sector++){
            read_block(_cluster);
            for(int _current_byte = 0; _current_byte < exFAT_attr.BytsPerSector; _current_byte++){
                Buffer[_count] = DAT_buffer[_current_byte];
                _count++;
            }
            _cluster++;
        }
    }
}

KernelFile get_first_cluster_kernel(){
    //Поиск файла kernel8.img
    uint8_t _tempory_buffer[32];

    int _tempory_buffer_index = 0;
    exFAT_FileDirectoryEntry _kernel;
    _kernel.SecondaryCount = 0;
    exFAT_StreamExtensionDirectoryEntry _kernel_stream;
    exFAT_FileNameDirectoryEntry _kernel_name[7];
    int _name_count = 0;
    int _name_index = 0;

    int _secondary_index = -1;
    char _name[260];

    for(int _clear = 0; _clear < 32; _clear++){
        _tempory_buffer[_clear] = 0x0;
    }

    for(int _index = 0; _index < (int)(exFAT_attr.BytsPerSector * exFAT_attr.SectorsPerCluster); _index++){
        _tempory_buffer[_tempory_buffer_index] = Buffer[_index]; //Сохранение значения 32 байт каждый раз
        _tempory_buffer_index++;
        if(_tempory_buffer_index == 32){
            //Проверка на файл
            if(_tempory_buffer[0] == 0x85){ //Если дескриптор - FileDirectoryEntry
                _secondary_index = 0;
                _name_count = 0;
                _kernel.EntryType = _tempory_buffer[0]; //Сохранения типа дескриптора
                _kernel.SecondaryCount = _tempory_buffer[1]; //Сохранение кол-во вторичных дескрипторов для это-го файла(Stream, Name)
                _kernel.FileAttributes[0] = _tempory_buffer[4]; //Аттрибуты файла
                _kernel.FileAttributes[1] = _tempory_buffer[5]; //Аттрибуты файла
            }

            if(_secondary_index < _kernel.SecondaryCount && _tempory_buffer[0] == 0xC0){ //Если дескриптор - StreamExtensions
                _kernel_stream.EntryType = _tempory_buffer[0]; //Сохранения типа дескриптора
                _kernel_stream.GeneralSecondaryFlags = _tempory_buffer[1]; //Сохранение флагов файла: NoFATChain к примеру
                _kernel_stream.NameLength = _tempory_buffer[3]; //Сохранение длины имени(в байтах)
                for(int _build_index = 0; _build_index < 4; _build_index++){
                    _kernel_stream.FirstCluster |= ((uint32_t)_tempory_buffer[20 + _build_index] << (8 * _build_index)); //Сборка значения для первого кластера файла
                }

                for(int _build_length = 0; _build_length < 8; _build_length++){
                    _kernel_stream.DataLength |= ((uint64_t)_tempory_buffer[24 + _build_length] << (8 * _build_length)); //Сборка длины файла(в байтах)
                }

                _secondary_index++;
            }

            if(_secondary_index < _kernel.SecondaryCount && _tempory_buffer[0] == 0xC1){ //Если дескриптор - FileName
                _kernel_name[_name_count].EntryType = _tempory_buffer[0]; //Сохранение типа дескриптора
                _kernel_name[_name_count].GeneralSecondaryFlags = _tempory_buffer[1]; //Сохранение флагов файла
                for(int i = 0; i < 30; i++){
                    _kernel_name[_name_count].FileName[i] = _tempory_buffer[i+2]; //i+2 т.к. NameFile начинается с 2 байта
                }
                _name_count++;
                _secondary_index++;
            }

            if(_secondary_index >= _kernel.SecondaryCount){
                int _jump_index = 0;
                while(_name_index < _name_count){
                    for(int j = 0; j < 30; j++){
                        _name[j + _jump_index] = (char)_kernel_name[_name_index].FileName[j];
                        SD_sec_barrier(10);
                    }
                    _jump_index+=30;
                    _name_index++;
                }
                if(compare_ZeroPoint(_name, "ZeroPoint.bin") == 1){
                    return (KernelFile){.DataLength = _kernel_stream.DataLength, .FirstCluster = _kernel_stream.FirstCluster, .NoFATChain = _kernel_stream.GeneralSecondaryFlags};
                }
                _name_index = 0;
                _name_count = 0;
                _secondary_index = -1;

                _kernel_stream.FirstCluster = 0x0;
            }
            _tempory_buffer_index = 0;
        }
    }
}

uint32_t walk_FAT_table(uint32_t _cluster){
    uint32_t _start_LBA_FAT = exFAT_attr.LBA_FAT_regione + ((_cluster * 4) / exFAT_attr.BytsPerSector);
    read_block(_start_LBA_FAT);

    uint32_t _tempory_cluster = 0x0;
    for(int _build = 0; _build < 4; _build++){
        _tempory_cluster |= ((uint32_t)DAT_buffer[(((_cluster * 4) % exFAT_attr.BytsPerSector) + _build)] << (8 * _build));
    }
    
    if(_tempory_cluster >= 0xFFFFFFF8 && _tempory_cluster <= 0xFFFFFFFF){
        return (uint32_t)1;
    }
    else if(_tempory_cluster == 0x0){
        return 0x0;
    }
    else{
        return _tempory_cluster;
    }
}

void to_kernel_buffer(int _multi){
    volatile uint32_t _tempory_value = 0x0;
    int _current8_index = 0;

    for(int _current32_index = 0; _current32_index < (8912 * _multi); _current32_index++){
        for(int _get = 0; _get < 4; _get++){
            _tempory_value |= ((uint32_t)Buffer[_get + _current8_index] << (_get * 8));
        }
        Kernel_buffer[_current32_index] = _tempory_value;
        _tempory_value = 0x0;
        _current8_index+=4;
    }
}

int compare_ZeroPoint(uint8_t _buffer[], char _src[]){
    int _index_src = 0;
    int _index_buffer = 0;
    int _result = 0;
    while(1){

        if(_buffer[_index_buffer] == 0x0){
            _index_buffer++;
        }

        if(_buffer[_index_buffer] == (uint8_t)_src[_index_src]){
            _result = 1;
        }
        else{
            _result = 0;
            break;
        }

        if(_src[_index_src] == '\0'){
            break;
        }
        _index_src++;
        _index_buffer++;
    }

    return _result;
}

int power_two(int _src){
    int _result = 1;
    for(int _power_index = 1; _power_index <= _src; _power_index++){
        _result = _result * 2;
    }
    return _result;
}