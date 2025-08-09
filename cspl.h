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
#include <stdlib.h>

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
    CSPL_KEY_NOT_FOUND,
    CSPL_ALLOC_FAIL,
    CSPL_NULL_POINTER,
    CSPL_UNKNOWN_ERROR = -1
};
extern int ___CSPL_ERR;

// Create and Destroy ----------------------------------------------------------

// open and parse a spl file
cspl_t* cspl_parse(const char* spl);
// free the cspl list
void cspl_free(cspl_t* cspl);

// File reading ----------------------------------------------------------------

// get the value of the specified key
char* cspl_get(cspl_t* cspl, const char* key);
// get the value of the specified key and converts to int
int cspl_geti(cspl_t* cspl, const char* key);
// get the value of the specified key and converts to double
double cspl_getf(cspl_t* cspl, const char* key);

// File editing ----------------------------------------------------------------
// WARN: FILE CHANGES WIPE OUT LINE FORMATING (BLANK LINES)
// WARN: YOU NEED TO USE WRITE TO SAVE THE CHANGES

// save the file changes
int cspl_write(cspl_t* cspl, const char* spl);
// Change the value of an entry
void cspl_edit(cspl_t* cspl, const char* key, const char* nval);
// Adds the entry at the bottom of the file
int cspl_add(cspl_t* cspl, const char* key, const char* val);
// Inserts the entry after the entry pkey
int cspl_insert(cspl_t* cspl,const char* pkey, const char* key, const char* val);
// Deletes an entry
void cspl_delete(cspl_t* cspl, const char* key);

// Error handling --------------------------------------------------------------

// get the latest error
int cspl_err();
// get and print the latest error
int cspl_perr(const char* s);

#endif /* _CSPL_H */
