#include "tests.h"

int main(){
    FILE * file = freopen("tests_result.txt", "w", stderr);
    if(test_mask()){
        fprintf(stderr, "\nALL TESTS PASSED\n");
    }
    fclose(file);
}
