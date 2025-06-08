#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cspl.h"

int ___CSPL_ERR = CSPL_OK;

// helper to trim strings
void trim(char* str){
    if(!str) return;
    size_t s = 0, e = strlen(str) - 1;
    while(isspace(str[s])){
        s++;
    }
    while(e > s && isspace(str[e])){
        e--;
    }
    if(s > 0 || e < (strlen(str) - 1)){
        memmove(str,str+s,e - s + 1);
        str[e - s + 1] = '\0';
    }
}

cspl_t* cspl_parse(const char* spl){
    cspl_t *head = NULL;
    cspl_t *tail = NULL;
    // initialization
    FILE* f = fopen(spl,"r");
    if(!f){
        ___CSPL_ERR = CSPL_FILE_NOT_FOUND;
        return NULL;
    }

    // parsing
    

    char *line = malloc(MAX_LINE_SIZE);
    size_t len;
    while(getline(&line,&len,f) != -1){
        // skip comments
        if(line[0] == COMMENT) continue;
        // no separator, skip
        char* sep = strchr(line, ASSIGN_SEP);
        if(!sep) continue;

        *sep = '\0';
        char* k = line;
        char* v = sep+1;

        trim(k);
        trim(v);

        struct cspl* pair = malloc(sizeof(struct cspl));
        if(!pair){
            ___CSPL_ERR = CSPL_ALLOC_FAIL;
            break;
        }

        pair->key = strdup(k);
        pair->value = strdup(v);
        if(!pair->key || !pair->value){
            free(pair->key);
            free(pair->value);
            free(pair);
            ___CSPL_ERR = CSPL_ALLOC_FAIL;
            break;
        }
        
        pair->next = NULL;
        if(head == NULL){
            head = tail = pair;
        }else{
            tail->next = pair;
            tail = pair;
        }
    }

    // de-init
    free(line);
    fclose(f);
    return head;
}
void cspl_free(cspl_t* cspl){
    cspl_t* cur = cspl;
    while(cur){
        cspl_t* next = cur->next;
        
        free(cur->key);
        free(cur->value);
        free(cur);
        
        cur = next;
    }
}
char* cspl_get(cspl_t* cspl, const char* key){
    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;

        if(!strcmp(cur->key,key)) return cur->value;
        
        cur = next;
    }

    ___CSPL_ERR = CSPL_KEY_NOT_FOUND;
    return NULL;
}
int cspl_geti(cspl_t* cspl, const char* key){
    return atoi(cspl_get(cspl,key));
}
double cspl_getf(cspl_t* cspl, const char* key){
    return atof(cspl_get(cspl,key));
}


int cspl_err(){
    return ___CSPL_ERR;
}
int cspl_perr(const char* s){
    switch (___CSPL_ERR) {
        case CSPL_OK:
            break;
        case CSPL_FILE_NOT_FOUND:
            printf("%s: file was not found",s);
            break;
        case CSPL_ALLOC_FAIL:
            printf("%s: failed to allocate memory",s);
            break;
        case CSPL_KEY_NOT_FOUND:
            printf("%s: could not find desired key",s);
            break;
        default:
            printf("%s: unknown error",s);
            break;
    }
    return ___CSPL_ERR;
}
