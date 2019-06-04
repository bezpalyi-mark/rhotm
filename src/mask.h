#include "gtk_librarys.h"
#include "get_path.h"

#define SEP_SIZE 1000
#define ANY_SYMB 1
#define EXPECTED_SYMB 2
#define ONLY_ASTIRICS 3
#define ONLY_QUES_MARK 4
#define ASTIRICS_AND_QUES 5

/**
 * @file mask.h
 * File with mask to exclude some files.
 * @author Team RHotM
 * @version 1.1
 * @date 2019.05.31
 */

/*!
Function, which make exception for some files
@param char * raw_data - checking string
@param char * mask - rules
@return - is fitting string
*/
bool mask(char * raw_data, char * mask);