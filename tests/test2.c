#include <stdio.h>

#include "../cspl.h"

int main(void){

    cspl_t* t1 = cspl_parse("tests/t1.spl");
    cspl_perr("error");
    cspl_t* t2 = cspl_get_and_parse(t1,"t2");
    cspl_perr("error");
    printf("%s",cspl_get(t2,"msg"));
    cspl_perr("error");

    return 0;
}
