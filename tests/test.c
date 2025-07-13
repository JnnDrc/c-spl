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
    printf("let's change it!\n");
    cspl_edit(john,"grade","9.8f");
    printf("it's %f now!\n",cspl_getf(john,"grade"));
    
    
    printf("let's remove it!\n");
    cspl_delete(john,"grade");
    printf("it's %f now!\n",cspl_getf(john,"grade"));

    printf("Latest error? %d\n",cspl_err());
    cspl_perr("It is");
    
    cspl_add(john, "spouse","Marta");
    printf("\nHe is married to %s\n",cspl_get(john,"spouse"));
    
    cspl_insert(john,"age","height","172cm");
    cspl_write(john,"tests/testw.spl");

    cspl_free(john);
    return 0;
}
