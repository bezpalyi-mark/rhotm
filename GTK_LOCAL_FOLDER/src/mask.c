#include "mask.h"

bool mask_func(char *raw_data, char *mask)
{
	int mode = 0;
	char temp[STR_SIZE];
	strcpy(temp, mask);

	if (strstr(mask, "?*") || strstr(mask, "*?")) {
		if (strchr(mask, '.') == NULL) {
			int count_q = 0;
			size_t size_mask = strlen(mask) - 1;
			for (int i = 0; i < size_mask; i++) {
				if (mask[i] == '?') {
					count_q++;
				}
			}
			if (strlen(raw_data) >= count_q) {
				return true;
			} else {
				return false;
			}
		} else {
			mode = MERGE_Q_AND_A;
		}
	} else if (strcmp(mask, "*") == 0) {
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
				bool some_dots = false;
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
						some_dots = true;
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
						for (int i = 0;
						     i < count_subs &&
						     some_dots;
						     i++) {
							raw_data[associate[i]] =
								pos_else_dot[i];
						}
						return false;
					}
					parts_of_mask = strtok(NULL, sep);
					parts_of_raw = strtok(NULL, sep);
				}
				for (int i = 0; i < count_subs && some_dots;
				     i++) {
					raw_data[associate[i]] =
						pos_else_dot[i];
				}
			} else {
				return false;
			}
		}

		if (mode == MERGE_Q_AND_A) {
			bool result;
			bool some_dots = false;
			bool is_dot_exist = true;

			if (strchr(raw_data, '.') == NULL) {
				is_dot_exist = false;
			}

			if (is_dot_exist) {
				size_t mask_size = strlen(mask) - 1;
				size_t raw_size = strlen(raw_data) - 1;
				for (int i = 0; i < raw_size && i < mask_size;
				     i++) {
					if (mask[i] == '?' || mask[i] == '*') {
						break;
					}
					if (mask[i] != raw_data[i]) {
						return false;
					}
				}
				int pos_else_dot[SUB_SIZE];
				int associate[SUB_SIZE];
				int count_subs = 0;
				char sub = '-';
				size_t pos_last_dot = strlen(raw_data) - 1;
				while (raw_data[pos_last_dot] != '.') {
					pos_last_dot--;
				}
				pos_last_dot--;
				int pos_last_dot_mask = strlen(mask) - 1;
				while (mask[pos_last_dot_mask] != '.') {
					pos_last_dot_mask--;
				}
				pos_last_dot_mask--;
				while (pos_last_dot != 1) {
					if (raw_data[pos_last_dot] == '.') {
						pos_else_dot[count_subs] =
							raw_data[pos_last_dot];
						raw_data[pos_last_dot] = sub;
						associate[count_subs] =
							pos_last_dot;
						count_subs++;
						some_dots = true;
					}
					pos_last_dot--;
				}

				bool first_part = false;
				char *which_part = strstr(mask, "?*");
				if (which_part == NULL) {
					which_part = strstr(mask, "*?");
				}
				size_t size_part = strlen(which_part) - 1;
				for (int i = 0; i < size_part; i++) {
					if (which_part[i] == '.') {
						first_part = true;
					}
				}
				if (first_part) {
					char temp_for_raw[STR_SIZE];
					strcpy(temp_for_raw, raw_data);
					char sep[SEP_SIZE] = ".";
					char sep2[SEP_SIZE] = ".";
					char *str;
					char *p_mask;
					str = strtok(temp_for_raw, sep);
					int count_q = 0;
					for (int i = 0;
					     i < pos_last_dot_mask + 1; i++) {
						if (mask[i] != '*') {
							count_q++;
						}
					}
					if (strlen(str) >= count_q) {
						str = strtok(NULL, sep);
						p_mask = strtok(temp, sep2);
						p_mask = strtok(NULL, sep2);
						if (mask_func(str, p_mask)) {
							result = true;
						} else {
							result = false;
						}
					} else {
						result = false;
					}
					for (int i = 0;
					     i < count_subs && some_dots; i++) {
						raw_data[associate[i]] =
							pos_else_dot[i];
					}
					return result;
				} else {
					char temp_for_raw[STR_SIZE];
					strcpy(temp_for_raw, raw_data);
					char sep[SEP_SIZE] = ".";
					char *str = strtok(temp_for_raw, sep);
					char *p_mask = strtok(temp, sep);
					int count_q = 0;
					char *dub_str = strstr(mask, ".");
					for (int i = 0; dub_str[i] != '\0';
					     i++) {
						if (dub_str[i] != '*') {
							count_q++;
						}
					}
					if (mask_func(str, p_mask)) {
						str = strtok(NULL, sep);
						if (strlen(str) >= count_q) {
							result = true;
						} else {
							result = false;
						}
					} else {
						result = false;
					}
					for (int i = 0;
					     i < count_subs && some_dots; i++) {
						raw_data[associate[i]] =
							pos_else_dot[i];
					}
					return result;
				}
			} else {
				return false;
			}
		}
	}

	return true;
}
