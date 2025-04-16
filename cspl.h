
#ifndef _CSPL_H
#define _CSPL_H

#include "stdlib.h"

#define MAX_LINE_SIZE 256
#define COMMENT '#'
#define ASSIGN_SEP ':'

// cspl linked list node
typedef struct cspl{
    char *key, *value;
    struct cspl* next;
}cspl_t;
// possible errors
enum cspl_err{
    CSPL_OK = 0,
    CSPL_FILE_NOT_FOUND,
    CSPL_ALLOC_FAIL,
    CSPL_KEY_NOT_FOUND,
};
extern int ___CSPL_ERR;

// open and parse a spl file
cspl_t* cspl_parse(const char* spl);
// free the cspl list
void cspl_free(cspl_t* cspl);

// get the value of the specified key
char* cspl_get(cspl_t* cspl, const char* key);
// get the value of the specified key and converts to int
int cspl_geti(cspl_t* cspl, const char* key);
// get the value of the specified key and converts to double
double cspl_getf(cspl_t* cspl, const char* key);

// get the latest error
int cspl_err();


#endif /* _CSPL_H */
