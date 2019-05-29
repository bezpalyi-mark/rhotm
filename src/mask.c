#include "mask.h"

int mask(char * raw_data, int mode, struct Data * data){
    if(mode == 1){
        int position = 0;
        char tmp;
        while(data->mask[position] != '?'){
            position++;
        }
        tmp = raw_data[position];
        raw_data[position] = '?';
        if(strcmp(raw_data, data->mask) == 0){
            raw_data[position] = tmp;
            return 1;
        } else {
            raw_data[position] = tmp;
            return 0;
        }
    }

    if(mode == 2){
        char * tail = strchr(data->mask, '.');
        char * tail_file = strchr(raw_data, '.');
        if(tail_file == NULL){
            return 0;
        }
        if(strcmp(tail, tail_file) == 0){
            return 1;
        }
        else return 0;
    }

    return 1;
}
