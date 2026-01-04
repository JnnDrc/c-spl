#include <stdio.h>
#include "../../cspl.h"

int main(void){
    
    cspl_t* conf = cspl_parse("examples/basics/cc.spl");

    char* cc = cspl_get(conf,"cc");
    char* src = cspl_get(conf,"src");
    char* out = cspl_get(conf,"out");
    char* flags = cspl_get(conf,"flags");

    printf("%s %s -o %s %s",cc,src,out,flags);

    cspl_free(conf);
    return 0;
}
