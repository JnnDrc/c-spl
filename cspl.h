/*
 *
 * CSPL
 * Simple Pairs Language, in C
 *
 * Simple Pairs Language is a config language of simple string pairs
 * this is an implementation in C
 *
 * */

#ifndef _CSPL_H
#define _CSPL_H

#include <stdio.h>
#include <stdbool.h>

// Version --------------------------------------------------------------------|
#define CSPL_MAJOR 1
#define CSPL_MINOR 2
// ----------------------------------------------------------------------------|

// Definitions ----------------------------------------------------------------|
#define MAX_LINE_SIZE 256
#define COMMENT '#'
#define ASSIGN_SEP ':'

#define invalid -1

// cspl linked list node
typedef struct cspl{
    char *key, *value;
    struct cspl* next;
}cspl_t;

// possible errors
typedef enum cspl_err{
    CSPL_OK = 0,
    CSPL_CANT_OPEN_FILE,
    CSPL_KEY_NOT_FOUND,
    CSPL_ALLOC_FAIL,
    CSPL_NULL_POINTER,
    CSPL_KEY_WITHOUT_VALUE,
    CSPL_UNKNOWN_ERROR = -1
}cspl_err_n;
extern int ___CSPL_ERR;

// Basics ---------------------------------------------------------------------|

// open and parse a spl file
cspl_t* cspl_parse(const char* filename);
// parse user managed file
cspl_t* cspl_parse_file(FILE* file);
// parse string as spl
cspl_t* cspl_parse_string(char* spl, size_t len);
// free the cspl list
void cspl_free(cspl_t* cspl);
// get the value of specified key and try to open it as a spl file path
cspl_t* cspl_get_parse(cspl_t* cspl, const char* key);

// File reading ---------------------------------------------------------------|

// get the value of the specified key
char* cspl_get(cspl_t* cspl, const char* key);
// get the value of a specified key and return a malloced copy of it (NEED TO BE FREED LATER)
char* cspl_getdup(cspl_t* cspl, const char* key);
// get the value of the specified key and converts to int
int cspl_geti(cspl_t* cspl, const char* key);
// get the value of the specified key and converts to double
double cspl_getf(cspl_t* cspl, const char* key);
// get true, false or invalid (1|0|-1) if string is respectively 'true', 'false' or neither
bool cspl_getb(cspl_t* cspl, const char* key);
// get the value at an index of a comma separated string and return a malloced copy of it (NEED TO BE FREED LATER)
char* cspl_getarr(cspl_t* cspl, const char* key, int index);

// File editing ---------------------------------------------------------------|

// Save the file changes
// WARN: FILE CHANGES WIPE OUT BLANK LINES
int cspl_write(cspl_t* cspl, const char* spl);
// Change the value of an entry
void cspl_edit(cspl_t* cspl, const char* key, const char* nval);
// Adds the entry at the bottom of the file
int cspl_add(cspl_t* cspl, const char* key, const char* val);
// Inserts the entry after the entry pkey
int cspl_insert(cspl_t* cspl,const char* pkey, const char* key, const char* val);
// Deletes an entry
void cspl_delete(cspl_t* cspl, const char* key);

// Utilities ------------------------------------------------------------------|

// check if a key exists or not
bool cspl_exists(cspl_t* cspl, const char* key);
// return the amount of pairs
unsigned int cspl_count(cspl_t* cspl);

// void cspl_comment(cspl_t* cspl, const char* key);
// void cspl_uncomment(cspl_t* cspl, int i);

// Error handling -------------------------------------------------------------|

// get the latest error
int cspl_err();
// get and print the latest error
int cspl_perr(const char* s);

#endif /* _CSPL_H */
