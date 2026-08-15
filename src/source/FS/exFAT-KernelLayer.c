#include<exFAT-KernelLayer.h>

uint32_t* Kernel;
int CurrentKernelIndex;

void search_kernel(){
    uint32_t _current_cluster = exFAT_attr.FirstClusterRoot;
    DataKernel _info;

    Kernel = (uint32_t*)0x00080000;

    while(_current_cluster != 0xFFFFFFFF){
        read_cluster(_current_cluster);
        for(int _index_file = 0; _index_file < get_count_file(); _index_file++){
            _info = get_kernel();
            
            if(this_kernel(_info.Name, "ZeroPoint.bin") == 1){
                load_kernel(_info);
                return;
            }
        }
        _current_cluster = walk_FAT(_current_cluster);
    }
}

void load_kernel(DataKernel _info){
    CurrentKernelIndex = 0;

    if(_info.NoFATChain & (1ULL << 1)){
        for(int _get = 0; _get <= (_info.DataLength / (exFAT_attr.BytsPerSector * exFAT_attr.SectorsPerCluster)); _get++){
            read_cluster(_info.FirstCluster);
            from_8_to_32();
            _info.FirstCluster++;
        }
    }
}

void from_8_to_32(){
    for(int _export = 0; _export < (DataBuffer.BufferIndex / 4); _export++){
        Kernel[_export] = *(uint32_t*)&DataBuffer.Buffer[CurrentKernelIndex];
        CurrentKernelIndex += 4;
    }
}


int this_kernel(uint8_t _buffer[], char _name[]){
    int _index = 0;
    int _out = 0;

    while(1){
        if(_name[_index] == '\0'){
            break;
        }


        if(_buffer[_index] == _name[_index]){
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