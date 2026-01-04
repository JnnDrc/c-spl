#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../cspl.h"

#define ID 0
#define CREDS 1

int main(void){
    FILE* dbf = fopen("examples/writing/db.spl", "r");  
    FILE* dbf_n = fopen("examples/writing/db_new.spl","w");

    cspl_t* db = cspl_parse_file(dbf);

    cspl_add(db,"Jayy"," 2469f0fa, 666");

    cspl_insert(db,"John","Alex"," 34b035ba, 200");

    char* marta = cspl_getdup(db,"Marta");
    char* marta_cred = cspl_getarr(db,"Marta",CREDS);
    char* cred = strstr(marta,marta_cred);
    strcpy(cred,"400");

    cspl_edit(db, "Marta", marta);
    free(marta);
    free(marta_cred);

    cspl_delete(db,"Pablo");

    cspl_write_file(db,dbf_n);

    fclose(dbf);
    fclose(dbf_n);
    cspl_free(db);
    return 0;
}
