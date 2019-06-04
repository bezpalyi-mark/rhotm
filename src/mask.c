#include "mask.h"

bool mask_func(char *raw_data, char *mask)
{
	int mode = 0;
	char temp[STR_SIZE];
	strcpy(temp, mask);

	if (strcmp(mask, "*") == 0) {
		mode = ONLY_ASTIRICS;
	} else if (strchr(mask, '*') && strchr(mask, '?')) {
		mode = ASTIRICS_AND_QUES;
	} else if (strchr(mask, '*') && strchr(mask, '?') == NULL) {
		mode = EXPECTED_SYMB;
	} else if (strchr(mask, '*') == NULL && strchr(mask, '?')) {
		if (strtok(temp, "?") == NULL) {
			mode = ONLY_QUES_MARK;
		} else {
			mode = ANY_SYMB;
		}
	}

	strcpy(temp, mask);

	if (mode == ONLY_QUES_MARK) {
		if (strlen(mask) != strlen(raw_data))
			return false;
	} else if (mode != ONLY_ASTIRICS) {
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
			if (cmp_str == NULL ||
			    strstr(raw_data, cmp_str) == NULL) {
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
				if (raw_data[size_data - 1] !=
				    mask[size_mask - 1])
					return false;
				size_data--;
				size_mask--;
			}
		}

		if (mode == ASTIRICS_AND_QUES) {
			bool is_dot_exist = true;
			if (strchr(raw_data, '.') == NULL) {
				is_dot_exist = false;
			}
			if (is_dot_exist) {
				char *parts_of_raw;
				char *parts_of_mask;
				int pos_else_dot[SUB_SIZE];
				int associate[SUB_SIZE];
				int count_subs = 0;
				char sub = '-';
				size_t pos_last_dot = strlen(raw_data) - 1;
				while (raw_data[pos_last_dot] != '.') {
					pos_last_dot--;
				}
				pos_last_dot--;
				while (pos_last_dot != 1) {
					if (raw_data[pos_last_dot] == '.') {
						pos_else_dot[count_subs] =
							raw_data[pos_last_dot];
						raw_data[pos_last_dot] = sub;
						associate[count_subs] =
							pos_last_dot;
						count_subs++;
					}
					pos_last_dot--;
				}
				char temp_for_raw[STR_SIZE];
				strcpy(temp_for_raw, raw_data);
				char sep[SEP_SIZE] = ".";

				parts_of_raw = strtok(temp_for_raw, sep);

				parts_of_mask = strtok(temp, sep);

				while (parts_of_mask && parts_of_raw) {
					if (!mask_func(parts_of_raw,
						       parts_of_mask)) {
						return false;
					}
					parts_of_mask = strtok(NULL, sep);
					parts_of_raw = strtok(NULL, sep);
				}
				for (int i = 0; i < count_subs; i++) {
					raw_data[associate[i]] =
						pos_else_dot[i];
				}
			} else {
				return false;
			}
		}
	}

	return true;
}
