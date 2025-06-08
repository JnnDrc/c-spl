#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../cspl.h"

int main(void){
    
    cspl_t* john = cspl_parse("tests/test.spl");

    char* proj = cspl_get(john,"project");
    printf("%s.\n",proj);
    int age = cspl_geti(john,"age");
    char* name = cspl_get(john,"name");
    printf("Happy birthday %s, ",name);
    printf("netx year you will be %d\n",age+1);
    
    printf("College grade was %f\n",cspl_getf(john,"grade"));

    printf("Latest error? %d\n",cspl_err());
    
    cspl_free(john);
    return 0;
}
