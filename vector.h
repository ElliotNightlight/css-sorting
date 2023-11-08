#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "pretty.h"

typedef struct {
    UIntSize len;
    UIntSize cap;
    Int64 *arr;
} Vector;

inline void vector_new(Vector *vec) {
    vec->len = 0;
    vec->cap = 0;
    vec->arr = NULL;
}

inline void vector_extend(Vector *vec) {
    if(vec->len != vec->cap) return;
    if(vec->cap == 0) vec->cap = 1;
    else vec->cap *= 2;
    vec->arr = (Int64*)realloc(vec->arr, vec->cap * sizeof(Int64));
}

inline void vector_free(Vector *vec) {
    free(vec->arr);
}

typedef struct {
    UIntSize len;
    UIntSize cap;
    char* arr;
} String;

inline void string_new(String *str) {
    str->len = 0;
    str->cap = 0;
    str->arr = NULL;
}

inline void string_extend(String *str) {
    if(str->len != str->cap) return;
    if(str->cap == 0) str->cap = 1;
    else str->cap *= 2;
    str->arr = (char*)realloc(str->arr, str->cap * sizeof(char));
}

inline void string_free(String *str) {
    free(str->arr);
}

void string_append(String *str, const char* ext) {
    char c;
    const char *s = ext;
    while( (c = *(s++)) ) {
        string_extend(str);
        str->arr[str->len++] = c;
    }
    string_extend(str);
    str->arr[str->len] = 0;
}

void string_append_base26(String *str, UInt64 nmb) {
    do {
        string_extend(str);
        str->arr[str->len++] = nmb % 26 + 'A';
        nmb /= 26;
    } while(nmb > 0);
}

void string_append_base10(String *str, Int64 nmb) {
    Int64 sign = 1;
    if(nmb < 0) {
        sign *= -1;
        nmb *= -1;
    }
    char s[30];
    char* top = s;
    do {
        *(top++) = nmb % 10 + '0';
        nmb /= 10;
    } while(nmb > 0);
    if(sign < 0) {
        string_extend(str);
        str->arr[str->len++] = '-';
    }
    while(top != s) {
        string_extend(str);
        str->arr[str->len++] = *(--top);
    }
}

#endif
