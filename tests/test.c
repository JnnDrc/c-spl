#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../cspl.h"

int main(void){
    
    cspl_t* john = cspl_parse("tests/test.spl");

    char* proj = cspl_get(john,"project");
    printf("%d",proj[strlen(proj) -1]);
    printf("%s!",proj);
    int age = cspl_geti(john,"age");
    printf("netx year john will be %d\n",age+1);
    
    printf("Latest error? %d\n",cspl_err());

    cspl_free(john);
    return 0;
}
