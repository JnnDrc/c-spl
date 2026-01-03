#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

static char* strgets(char* buf, size_t max, char** cur, char* end){
    if(*cur >= end || max == 0) return NULL;

    size_t remain = end - *cur;
    size_t maxcpy = max - 1;
    if(remain < maxcpy) maxcpy = remain;

    char* l = memchr(*cur,'\n',maxcpy);
    size_t llen = l ? (size_t)((l - *cur) + 1) : maxcpy;
    
    memcpy(buf,*cur,llen);
    buf[llen] = '\0';
    *cur += llen;
    
    return buf;
}

static int parse(char* line, cspl_t** head, cspl_t** tail){
        char *k, *v;
        char is_comment = 0;
        // store comments
        if(line[0] == COMMENT){
            k = NULL;
            v = line;
            trim(v);
            is_comment = 1;
        }else if(line[0] == '\n'){
            // TODO: check if key exists, even with identation
            return CSPL_OK;
        }else{
            // no separator, skip
            char* sep = strchr(line, ASSIGN_SEP);
            if(!sep){
                ___CSPL_ERR = CSPL_KEY_WITHOUT_VALUE;
                return ___CSPL_ERR;
            }

            *sep = '\0';
            k = line;
            v = sep+1;

            trim(k);
            trim(v);
        }

        struct cspl* pair = malloc(sizeof(struct cspl));
        if(!pair){
            ___CSPL_ERR = CSPL_ALLOC_FAIL;
            return ___CSPL_ERR;
        }
        
        pair->key = k ? strdup(k) : NULL;
        pair->value = v ? strdup(v) : NULL;
        if(((pair->key && is_comment) || (!pair->key && !is_comment)) || !pair->value){
            free(pair->key);
            free(pair->value);
            free(pair);
            ___CSPL_ERR = CSPL_ALLOC_FAIL;
            return ___CSPL_ERR;
        }
        
        pair->next = NULL;
        if(*head == NULL){
            *head = *tail = pair;
        }else{
            (*tail)->next = pair;
            *tail = pair;
        }
    return CSPL_OK;
}

cspl_t* cspl_parse(const char* filename){
    if(!filename){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return NULL;
    }
    cspl_t *head = NULL;
    cspl_t *tail = NULL;
    // initialization
    FILE* f = fopen(filename,"r");
    if(!f){
        ___CSPL_ERR = CSPL_CANT_OPEN_FILE;
        return NULL;
    }

    // parsing
    
    char line[MAX_LINE_SIZE];
    while(fgets(line,sizeof(line),f)){
        if(parse(line,&head,&tail)) break;
    } 

    // de-init
    fclose(f);
    return head;
}

cspl_t* cspl_parse_file(FILE* file){
    if(!file){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return NULL;
    }
    cspl_t *head = NULL;
    cspl_t *tail = NULL;

    // parsing
    char line[MAX_LINE_SIZE];
    while(fgets(line,sizeof(line),file)) {
        if(parse(line,&head,&tail)) break;
    }

    return head;
}

cspl_t* cspl_parse_string(char* spl, size_t len){
    if(!spl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return NULL;
    }
    cspl_t *head = NULL;
    cspl_t *tail = NULL;

    // parsing
    
    char line[MAX_LINE_SIZE];
    char* cur = spl;
    char* end = spl + len;
    while(strgets(line,sizeof(line),&cur,end)) {
        if(parse(line,&head,&tail)) break;
    }

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
cspl_t* cspl_get_parse(cspl_t* cspl, const char* key){
    char* v = cspl_get(cspl,key);
    if(!v) return NULL;

    cspl_t* ncspl = cspl_parse(v);
    if(!ncspl) return NULL;
    
    return ncspl;
}

char* cspl_get(cspl_t* cspl, const char* key){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return NULL;
    }

    cspl_t* cur = cspl;
    while(cur){
        cspl_t *next = cur->next;
        
        // if has key, is not a comment
        if(cur->key){
            if(!strcmp(cur->key,key)) return cur->value;
        }
        
        cur = next;
    }

    ___CSPL_ERR = CSPL_KEY_NOT_FOUND;
    return NULL;
}
char* cspl_getdup(cspl_t* cspl, const char* key){
    char* v = cspl_get(cspl,key);
    if(!v) return NULL;
    char* s = malloc(strlen(v) + 1);
    if(!s){
        ___CSPL_ERR = CSPL_ALLOC_FAIL;
        return NULL;
    }
    strcpy(s,v);
    return s;
}
int cspl_geti(cspl_t* cspl, const char* key){
    char* k = cspl_get(cspl,key);
    return k ? atoi(k) : -1;
}
double cspl_getf(cspl_t* cspl, const char* key){
    char* k = cspl_get(cspl,key);
    return k ? atof(k) : -1.0;
}
bool cspl_getb(cspl_t* cspl, const char* key){
    const char* v = cspl_get(cspl,key);
    if(!v) return invalid;
    
    if(!strcmp(v,"true")) return true;
    else if(!strcmp(v,"false")) return false;

    return invalid;
}

char* cspl_getarr(cspl_t* cspl, const char* key, int index){
    char* v  = cspl_get(cspl, key);
    if(!v) return NULL;
    int cur = 0;
    char* start = v;
    while(*v){
        if(*v == ','){
            if(cur == index) break;
            cur++;
            v++;
            while(*v == ' ') v++;
            start = v;
            continue;
        }
        v++;
    }

    if(cur != index){
        //index out of bounds
        return NULL;
    }

    size_t len = v - start;
    char* vi = malloc(len + 1);
    if(!vi){
        ___CSPL_ERR = CSPL_ALLOC_FAIL;
        return NULL;
    }

    memcpy(vi,start,len);
    vi[len] = '\0';
    return vi;
}

int cspl_write(cspl_t* cspl, const char* spl){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return 0;
    }

    FILE* f = fopen(spl,"w");
    if(!f){
        ___CSPL_ERR = CSPL_CANT_OPEN_FILE;
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
    return;
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
            pair->next = NULL;
            
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
    return;
}

bool cspl_exists(cspl_t* cspl, const char* key){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return false;
    }
    cspl_t* cur = cspl;
    while(cur){
        if(!strcmp(cur->key,key)) return true;

        cur = cur->next;
    }
    return false;
}
unsigned int cspl_count(cspl_t* cspl){
    if(!cspl){
        ___CSPL_ERR = CSPL_NULL_POINTER;
        return 0;
    }
    cspl_t* cur = cspl;
    unsigned int count = 0;
    while(cur){
        if(cur->key) count ++;
        cur = cur->next;
    }
    return count;
}

int cspl_err(){
    return ___CSPL_ERR;
}
int cspl_perr(const char* s){
    switch ((enum cspl_err)(___CSPL_ERR)) {
        case CSPL_OK:
            break;
        case CSPL_CANT_OPEN_FILE:
            printf("%s: couldn't open file",s);
            break;
        case CSPL_KEY_NOT_FOUND:
            printf("%s: could not find desired key",s);
            break;
        case CSPL_ALLOC_FAIL:
            printf("%s: failed to allocate memory",s);
            break;
        case CSPL_NULL_POINTER:
            printf("%s: tried to pass a null pointer to a function",s);
            break;
        case CSPL_KEY_WITHOUT_VALUE:
            printf("%s: find a key with no value when trying to parse file",s);
            break;
        case CSPL_UNKNOWN_ERROR:
        default:
            printf("%s: unknown error",s);
            break;
    }
    return ___CSPL_ERR;
}
