#include "mask.h"

bool mask(char *raw_data, char *mask)
{
	int mode = 0;
    if(strtok(mask, "*") == NULL)
        mode = ONLY_ASTIRICS;
    else if(strtok(mask, "?") == NULL)
        mode = ONLY_QUES_MARK;
    else if(strchr(mask, '?') != NULL && strchr(mask, '*') != NULL)
    {
        mode = ASTIRICS_AND_QUES;
    }
	else if (strchr(mask, '?') != NULL) {
		mode = ANY_SYMB;
	} else if (strstr(mask, "*") != NULL) {
		mode = EXPECTED_SYMB;
	}


    if(mode == ONLY_QUES_MARK)
    {
        if(strlen(mask) != strlen(raw_data))
            return false;
    }
    else if(mode != ONLY_ASTIRICS)
    {
        char tok_mask[SEP_SIZE];
        strcpy(tok_mask, mask);
        if (mode == ANY_SYMB) {
            int position = 0;
            char tmp;
            while (mask[position] != '?') {
                position++;
            }
            tmp = raw_data[position];
            raw_data[position] = '?';
            if (strcmp(raw_data, mask) == 0) {
                raw_data[position] = tmp;
                return true;
            } else {
                raw_data[position] = tmp;
                return false;
            }
        }

        if (mode == EXPECTED_SYMB) {
            char sep[SEP_SIZE] = "*";
            char *cmp_str;
            cmp_str = strtok(tok_mask, sep);
            if (cmp_str == NULL || strstr(raw_data, cmp_str) == NULL) {
                return false;
            }
            while (cmp_str != NULL) {
                if (!strstr(raw_data, cmp_str)) {
                    return false;
                }
                cmp_str = strtok(NULL, sep);
            }

            size_t size_data = strlen(raw_data);
            size_t size_mask = strlen(mask);

            while (size_data >= 0 && size_mask >= 0 &&
                   mask[size_mask - 1] != '*') {
                if (raw_data[size_data - 1] != mask[size_mask - 1])
                    return false;
                size_data--;
                size_mask--;
            }
        }

        if(mode == ASTIRICS_AND_QUES)
        {
            char * result_tok = strtok(raw_data, ".");
            if(result_tok == NULL)
                return false;

            int count_symb_data = 0;
            while(result_tok[count_symb_data] != "\n")
            {
                count_symb_data++;
            }

            char * result_tok_mask = strtok(mask, ".");
            int count_symb_mask = 0;
            while(result_tok_mask[count_symb_mask] != "\n")
                count_symb_mask++;
            free(result_tok);
            free(result_tok_mask);
            if(count_symb_data != count_symb_mask)
                return false;
        }
    }

	return true;
}
