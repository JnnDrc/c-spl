#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cspl.h"

// global error state -----
int ___CSPL_ERR = CSPL_OK;
//-------------------------

// helper to trim strings
static void trim(char* str){
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
        char *k, *v;
        char is_comment = 0;
        // skip comments
        if(line[0] == COMMENT){
            k = NULL;
            v = line;
            trim(v);
            is_comment = 1;
        }else{
            // no separator, skip
            char* sep = strchr(line, ASSIGN_SEP);
            if(!sep) continue;

            *sep = '\0';
            k = line;
            v = sep+1;

            trim(k);
            trim(v);
        }

        struct cspl* pair = malloc(sizeof(struct cspl));
        if(!pair){
            ___CSPL_ERR = CSPL_ALLOC_FAIL;
            break;
        }
        
        pair->key = k ? strdup(k) : NULL;
        pair->value = v ? strdup(v) : NULL;
        if(((pair->key && is_comment) || (!pair->key && !is_comment)) || !pair->value){
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
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return;
    }
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
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return NULL;
    }

    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;
        
        // if key, not a comment
        if(cur->key){
            if(!strcmp(cur->key,key)) return cur->value;
        }
        
        cur = next;
    }

    ___CSPL_ERR = CSPL_KEY_NOT_FOUND;
    return NULL;
}
int cspl_geti(cspl_t* cspl, const char* key){
    char* k = cspl_get(cspl,key);
    return k ? atoi(k) : 0;
}
double cspl_getf(cspl_t* cspl, const char* key){
    char* k = cspl_get(cspl,key);
    return k ? atof(k) : 0.0;
}

int cspl_write(cspl_t* cspl, const char* spl){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return 0;
    }

    FILE* f = fopen(spl,"w");
    if(!f){
        ___CSPL_ERR = CSPL_FILE_NOT_FOUND;
        return 0;
    }

    int w = 0;
    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;

            if(!cur->key) w += fprintf(f,"%s\n",cur->value); // comment
            else w += fprintf(f,"%s: %s\n",cur->key,cur->value); // kv pair

        cur = next;
    }

    return w;
}

void cspl_edit(cspl_t* cspl, const char* key, const char* nval){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return;
    }

    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;
        
        // if key, not a comment
        if(cur->key){
            if(!strcmp(cur->key,key)){
                free(cur->value);
                cur->value = strdup(nval);
                trim(cur->value);
                return;
            }
        }
        
        cur = next;
    }

    ___CSPL_ERR = CSPL_KEY_NOT_FOUND;
}
int cspl_add(cspl_t* cspl, const char* key, const char* val){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return CSPL_NULL_POINTER;
    }

    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;
        if(next == NULL){
            cspl_t* pair = malloc(sizeof(cspl_t));
            if(!pair){
                ___CSPL_ERR = CSPL_ALLOC_FAIL;
                return CSPL_ALLOC_FAIL;
            }
            pair->key = strdup(key);
            pair->value = strdup(val);
            
            if(!pair->key || !pair->value){
                free(pair->key);
                free(pair->value);
                free(pair);
                ___CSPL_ERR = CSPL_ALLOC_FAIL;
                return CSPL_ALLOC_FAIL;
            }
            cur->next = pair;
            return CSPL_OK;
        }else{
            cur = next;
        }
    }
    return CSPL_UNKNOWN_ERROR;
}
int cspl_insert(cspl_t* cspl,const char* pkey, const char* key, const char* val){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return CSPL_NULL_POINTER;
    }

    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;
        if(!strcmp(cur->key,pkey)){
            cspl_t* pair = malloc(sizeof(cspl_t));
            if(!pair){
                ___CSPL_ERR = CSPL_ALLOC_FAIL;
                return CSPL_ALLOC_FAIL;
            }
            pair->key = strdup(key);
            pair->value = strdup(val);
            
            if(!pair->key || !pair->value){
                free(pair->key);
                free(pair->value);
                free(pair);
                ___CSPL_ERR = CSPL_ALLOC_FAIL;
                return CSPL_ALLOC_FAIL;
            }
            // cur -> next

            cur->next = pair;   // cur -> pair
            pair->next = next;  // pair -> next
            return CSPL_OK;
        }else{
            cur = next;
        }
    }
    return CSPL_KEY_NOT_FOUND;
}
void cspl_delete(cspl_t* cspl, const char* key){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return;
    }

    cspl_t* cur = cspl, *prev = NULL;
    while(cur){
        cspl_t *next = cur->next;
        
        // if key, not a comment
        if(cur->key){
            if(!strcmp(cur->key,key)){
                if(prev) prev->next = next;
                free(cur->next);
                free(cur->value);
                free(cur);
                return;
            }
        }
        prev = cur;
        cur = next;
    }

    ___CSPL_ERR = CSPL_KEY_NOT_FOUND;
}

int cspl_err(){
    return ___CSPL_ERR;
}
int cspl_perr(const char* s){
    switch ((enum cspl_err)(___CSPL_ERR)) {
        case CSPL_OK:
            break;
        case CSPL_FILE_NOT_FOUND:
            printf("%s: file was not found",s);
            break;
        case CSPL_KEY_NOT_FOUND:
            printf("%s: could not find desired key",s);
            break;
        case CSPL_ALLOC_FAIL:
            printf("%s: failed to allocate memory",s);
            break;
        case CSPL_NULL_POINTER:
            printf("%s: tried to pass a null pointer to a function",s);
        case CSPL_UNKNOWN_ERROR:
        default:
            printf("%s: unknown error",s);
            break;
    }
    return ___CSPL_ERR;
}
