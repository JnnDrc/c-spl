# Simple Pairs Language

SPL is a simple configuration language it just have one thing: strings

## How it works

SPL works with pairs of strings like:
```spl
key: value
key: value
```
both key and value are strings of text in a supported character-set

It does not have numbers, or tables, or arrays, or anything.
Just strings.

It also have line comments using '#', like:
```spl
# comment
key: value
# comment
key: value
```
It does not accept comments and pairs in same line, a comment should get the
entire line

and this is it, that's all of SPL standard

## But how about other types?

If you want to use numbers, just use strings and convert them
```c
int val = atoi(cspl_get(file, "key"));
```
Arrays can be used as strings in a value separated by a delimiter 
```spl
names: john, jacob, may, april, aethelred
```

```c
cspl_t *group = cspl_parse("group.spl");
const char* names = cspl_get(group, "names");
// long parsing here
```
Tables can just be the name of other spl file
```c
char* user = cspl_get(users,"john");
cspl_t *john = cspl_parse("user");
```
At the end, it's just strings, use it as you think is best

## Other languages

currently there is only this C implementation, but as it's made in C, you can  
easily interface it with any good language
