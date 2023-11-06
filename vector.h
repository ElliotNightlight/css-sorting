#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "pretty.h"

typedef struct {
    UIntSize len;
    UIntSize cap;
    Int64 *arr;
} Vector;

static inline void vector_new(Vector *vec) {
    vec->len = 0;
    vec->cap = 0;
    vec->arr = NULL;
}

static inline void vector_extend(Vector *vec) {
    if(vec->len != vec->cap) return;
    if(vec->cap == 0) vec->cap = 1;
    else vec->cap *= 2;
    vec->arr = realloc(vec->arr, vec->cap * sizeof(Int64));
}

static inline void vec_free(Vector *vec) {
    free(vec->arr);
}

typedef struct {
    UIntSize len;
    UIntSize cap;
    char* arr;
} String;

static inline void string_new(String *str) {
    str->len = 0;
    str->cap = 0;
    str->arr = NULL;
}

static inline void string_extend(String *str) {
    if(str->len != str->cap) return;
    if(str->cap == 0) str->cap = 1;
    else str->cap *= 2;
    str->arr = realloc(str->arr, str->cap * sizeof(char));
}

static inline void string_free(String *str) {
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

#endif
