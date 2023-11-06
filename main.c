#include <stdio.h>
#include "pretty.h"
#include "vector.h"
#include <ctype.h>

enum {
    OKAY     = 0,
    WARNINGS = 1,
    ERRORS   = 2
};
const char* FINAL_MESSAGE_LOOKUP[] = {
    "Compilation successful. Sorting now...",
    "Compiled with warnings. Sorting...",
    "Compilation failed. Cannot sort."
};

enum {
    ERROR_INVALID_INPUT    = 0,
    ERROR_BIG_INPUT        = 1,

    WARNING_CANNOT_READ    = 2,
    WARNING_ERRONEOUS_FILE = 3
};
const char* CASUAL_MESSAGE_LOOKUP[] = {
    "Did not find any numbers in the input files or all the provided numbers were in an incorrect format.",
    "There were too many numbers in the input files and the resulting compiled file would be inacceptably heavy.",

    "Could not open one or more of the files with provided filenames.",
    "Content of one or more files was not entirely correct. The compiler might have been able to recover."
};

#define MAX_VALUE 1000000000ll
#define KILL_THRESHOLD 20

int main(const int argc, const char* argv[]) {

    Vector numbers;
    vector_new(&numbers);

    Int32 status = OKAY;
    Int32 codes = 0;

    for(const char** fname = argv + 1; fname != argv + argc; ++fname) {
        FILE *fptr = fopen(*fname, "r");
        if(fptr == NULL) {
            if(WARNINGS > status) status = WARNINGS;
            codes = codes | 1 << WARNING_CANNOT_READ;
        } else {
            Int8 await_whitespace = 0;
            Int8 holds_value = 0;
            Int64 accumulator = 0;
            Int64 sign = 1;
            Int32 c;
            do {
                c = fgetc(fptr);
                if(c != EOF && !isspace(c) && (char)c != '-' && !((char)c >= '0' && (char)c <= '9')) {
                    if(WARNINGS > status) status = WARNINGS;
                    codes = codes | 1 << WARNING_ERRONEOUS_FILE;
                    holds_value = 0;
                    await_whitespace = 1;
                    continue;
                }

                if(await_whitespace) {
                    if(isspace(c)) {
                        holds_value = 0;
                        await_whitespace = 0;
                    }
                    continue;
                }

                if((char)c == '-') {
                    if(holds_value) {
                        if(WARNINGS > status) status = WARNINGS;
                        codes = codes | 1 << WARNING_ERRONEOUS_FILE;
                        holds_value = 0;
                        await_whitespace = 1;
                        continue;
                    }
                    accumulator = 0;
                    sign *= -1;
                    holds_value = 1;
                } else if((char)c >= '0' && (char)c <= '9') {
                    if(!holds_value) {
                        accumulator = 0;
                        sign = 1;
                        holds_value = 1;
                    }
                    accumulator *= 10;
                    accumulator += (char)c - '0';
                    if(accumulator > MAX_VALUE) {
                        if(WARNINGS > status) status = WARNINGS;
                        codes = codes | 1 << WARNING_ERRONEOUS_FILE;
                        accumulator = MAX_VALUE;
                    }
                } else if(holds_value) {
                    holds_value = 0;
                    vector_extend(&numbers);
                    numbers.arr[numbers.len++] = sign * accumulator;
                }
            } while(c != EOF);

        }
    }

    if(numbers.len == 0) {
        if(ERRORS > status) status = ERRORS;
        codes = codes | 1 << ERROR_INVALID_INPUT;
    }

    if(numbers.len >= KILL_THRESHOLD) {
        if(ERRORS > status) status = ERRORS;
        codes = codes | 1 << ERROR_BIG_INPUT;
    }

    for(Int64 *ptr = numbers.arr; ptr != numbers.arr + numbers.len; ++ptr) {
        printf("%ld ", *ptr);
    }
    printf("\n");

    if(codes & 1 << ERROR_INVALID_INPUT)    fprintf(stderr, "[error]   %s\n", CASUAL_MESSAGE_LOOKUP[ERROR_INVALID_INPUT]);
    if(codes & 1 << ERROR_BIG_INPUT)        fprintf(stderr, "[error]   %s\n", CASUAL_MESSAGE_LOOKUP[ERROR_BIG_INPUT]);
    if(codes & 1 << WARNING_CANNOT_READ)    fprintf(stderr, "[warning] %s\n", CASUAL_MESSAGE_LOOKUP[WARNING_CANNOT_READ]);
    if(codes & 1 << WARNING_ERRONEOUS_FILE) fprintf(stderr, "[warning] %s\n", CASUAL_MESSAGE_LOOKUP[WARNING_ERRONEOUS_FILE]);

    printf("%s\n", FINAL_MESSAGE_LOOKUP[status]);
    return status;
}
