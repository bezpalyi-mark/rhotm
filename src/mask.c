#include "mask.h"

bool mask(char * raw_data, char * mask){
    int mode = 0;
    if(strchr(mask, '?') != NULL){
        mode = 1;
    } else if(strstr(mask, "*") != NULL) {
        mode = 2;
    }

    char tok_mask[SEP_SIZE];
    strcpy(tok_mask, mask);

    if(mode == 1){
        int position = 0;
        char tmp;
        while(mask[position] != '?'){
            position++;
        }
        tmp = raw_data[position];
        raw_data[position] = '?';
        if(strcmp(raw_data, mask) == 0){
            raw_data[position] = tmp;
            return true;
        } else {
            raw_data[position] = tmp;
            return false;
        }
    }

    if(mode == 2){
        char sep[SEP_SIZE] ="*";
        char * cmp_str;
        cmp_str = strtok(tok_mask, sep);
        if(strstr(raw_data, cmp_str) == NULL){
            return false;
        }
        while(cmp_str != NULL){
            if(!strstr(raw_data, cmp_str)){
                return false;
            }
            cmp_str = strtok(NULL, sep);
        }

        size_t size_data = strlen(raw_data);
        size_t size_mask = strlen(mask);

        while(size_data >= 0 && size_mask >= 0 && mask[size_mask-1] != '*'){
            if(raw_data[size_data-1] != mask[size_mask-1])
                return false;
            size_data--;
            size_mask--;
        }
    }

    return true;
}
