# C-SPL

Simple Pairs Language implementation in C

## SPL

SPL is a config language, see here [SPL](SPL.md)

## API

it have 7 functions
- cspl\_parse: parse the file
- cspl\_free: free the spl list
- cspl\_get: get value of desired key
- cspl\_geti: atoi of cspl\_get
- cspl\_getf: atof of cspl\_get
- cspl\_write: write file changes
- cspl\_edit: change value of desired key
- cspl\_add: add key at bottom of file
- cspl\_insert: insert key after other key
- cspl\_delete: delete key
- cspl\_err: get the value of the last error
- cspl\_perr: get the value and print the string description of the last error

see: [cspl.h]
