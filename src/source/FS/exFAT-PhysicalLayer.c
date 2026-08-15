#include<exFAT-PhysicalLayer.h>

exFAT* exFATp;
exFAT_attribute exFAT_attr;

void exFAT_init(){
    read_single_block(LBA);
    exFATp = (exFAT*)DataBuffer.Buffer;

    exFAT_attr.BytsPerSector = power_two(exFATp->BytsPerSectorP);
    exFAT_attr.SectorsPerCluster = power_two(exFATp->SectorsPerClusterP);
    exFAT_attr.FirstClusterRoot = exFATp->FirstClusterRoot;
    exFAT_attr.LBA_data_regione = LBA + exFATp->ClusterHeapOffset;
    exFAT_attr.LBA_FAT_regione = LBA + exFATp->FATOffset;
}

void read_cluster(uint32_t _cluster){
    uint32_t _LBA_sector = exFAT_attr.LBA_data_regione + ((_cluster - 2) * exFAT_attr.SectorsPerCluster);
    read_multi_block(_LBA_sector);
}

uint32_t walk_FAT(uint32_t _cluster){
    uint32_t _sector = exFAT_attr.LBA_FAT_regione + ((_cluster * 4) / 512);
    read_single_block(_sector);

    uint32_t _next_cluster = 0x0;
    for(int _build = 0; _build < 4; _build++){
        _next_cluster |= (DataBuffer.Buffer[((_cluster * 4) % 512) + _build] << (8 * _build));
    }

    if(_next_cluster >= 0xFFFFFFF8 && _next_cluster <= 0xFFFFFFFF){
        return 0xFFFFFFFF;
    }
    else if(_next_cluster == 0x00000000){
        return 0x00000000;
    }
    else{
        return _next_cluster;
    }
}

__attribute__((target("general-regs-only")))
DataKernel get_kernel(){
    uint8_t _tempory_buffer[32];
    DataKernel _out;

    for(int _clear = 0; _clear < 32; _clear++){
        _tempory_buffer[_clear] = 0x0;
    }
    int _tempory_index = 0;

    for(; DataBuffer.BufferCurrentIndex < DataBuffer.BufferIndex; DataBuffer.BufferCurrentIndex++){
        _tempory_buffer[_tempory_index] = DataBuffer.Buffer[DataBuffer.BufferCurrentIndex];
        _tempory_index++;
        if(_tempory_index == 32){
            _tempory_index = 0;
            switch(_tempory_buffer[0]){
                case 0xC0:
                    _out.FirstCluster = ((exFAT_StreamExtensionDirectoryEntry*)_tempory_buffer)->FirstCluster;
                    _out.NoFATChain = ((exFAT_StreamExtensionDirectoryEntry*)_tempory_buffer)->GeneralSecondaryFlags;
                    _out.DataLength = ((exFAT_StreamExtensionDirectoryEntry*)_tempory_buffer)->DataLength;
                    break;
                case 0xC1:
                    DataBuffer.BufferCurrentIndex++;
                    int _name_index = 0;
                    for(int _name_des_index = 0; _name_des_index < 30; _name_des_index++){
                        if(_tempory_buffer[2 + _name_des_index] == 0x0){
                            continue;
                        }
                        else{
                            _out.Name[_name_index] = 0x0;
                            _out.Name[_name_index] = _tempory_buffer[2 + _name_des_index];
                            _name_index++;
                        }
                    }
                    return _out;
                    break;
                default:
                    break;
            }
        }
    }
}

int get_count_file(){
    int _out = 0;

    for(int _index = 0; _index < DataBuffer.BufferIndex; _index++){
        if(DataBuffer.Buffer[_index] == 0x85){
            _out++;
        }
    }

    return _out;
}

int power_two(int _src){
    int _result = 1;
    for(int _power_index = 1; _power_index <= _src; _power_index++){
        _result = _result * 2;
    }
    return _result;
}