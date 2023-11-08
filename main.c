#include <stdio.h>
#include "pretty.h"
#include "vector.h"
#include <ctype.h>
#include <string.h>

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
    WARNING_INVALID_INPUT    = 0,
    WARNING_BIG_INPUT        = 1,

    WARNING_CANNOT_READ      = 2,
    WARNING_ERRONEOUS_FILE   = 3,

    ERROR_NO_TEMPLATE  = 4,
    ERROR_CANNOT_WRITE = 5
};
const char* CASUAL_MESSAGE_LOOKUP[] = {
    "Did not find any numbers in the input files or all the provided numbers were in an incorrect format.",
    "There are too many numbers in the input files and the resulting compiled file may be heavy.",

    "Could not open one or more of the files with provided filenames.",
    "Content of one or more files was not entirely correct. The compiler might have been able to recover.",

    "Could not found the necessary template.",
    "Could not write to the file."
};

#define MAX_VALUE 1000000000ll
#define KILL_THRESHOLD 20

Vector numbers;
Int32 status;
Int32 codes;

void fprocess(const char* fname) {
    FILE *fptr = fopen(fname, "r");
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
        fclose(fptr);
    }
}

String pivotal;
String css_more;
String html_io;
String html_animation;

void compile() {
    string_new(&pivotal);
    string_append(&pivotal, ":root {\n");

    for(IntSize i = 0; i != (IntSize)numbers.len; ++i) {
        string_append(&pivotal, "    --a");
        string_append_base10(&pivotal, i);
        string_append(&pivotal, ": ");
        string_append_base10(&pivotal, numbers.arr[i]);
        string_append(&pivotal, ";\n");
    }

    UInt64 C = 0;
    UInt64 *lookup = (UInt64*)malloc(numbers.len * sizeof(UInt64));
    string_append(&pivotal, "\n");
    for(IntSize i = 0; i != (IntSize)numbers.len; ++i) {
        lookup[i] = C++;
        string_append(&pivotal, "    --");
        string_append_base26(&pivotal, lookup[i]);
        string_append(&pivotal, ": var(--a");
        string_append_base10(&pivotal, i);
        string_append(&pivotal, ");\n");
    }

    for(IntSize i = (IntSize)numbers.len-1; i >= 0; --i) {
        for(IntSize j = i; j + 1 < (IntSize)numbers.len; ++j) {
            string_append(&pivotal, "    --");
            string_append_base26(&pivotal, C);
            string_append(&pivotal, ": min(var(--");
            string_append_base26(&pivotal, lookup[j]);
            string_append(&pivotal, "), var(--");
            string_append_base26(&pivotal, lookup[j+1]);
            string_append(&pivotal, "));\n");

            string_append(&pivotal, "    --");
            string_append_base26(&pivotal, C+1);
            string_append(&pivotal, ": max(var(--");
            string_append_base26(&pivotal, lookup[j]);
            string_append(&pivotal, "), var(--");
            string_append_base26(&pivotal, lookup[j+1]);
            string_append(&pivotal, "));\n");

            lookup[j] = C;
            lookup[j+1] = C+1;
            C += 2;
        }
    }

    string_append(&pivotal, "\n");
    for(IntSize i = 0; i != (IntSize)numbers.len; ++i) {
        string_append(&pivotal, "    --b");
        string_append_base10(&pivotal, i);
        string_append(&pivotal, ": var(--");
        string_append_base26(&pivotal, lookup[i]);
        string_append(&pivotal, ");\n");
    }

    string_append(&pivotal, "}\n");
    free(lookup);

    string_new(&css_more);

    for(IntSize i = 0; i != (IntSize)numbers.len; ++i) {
        string_append(&css_more, "#aa span:nth-child(");
        string_append_base10(&css_more, i+1);
        string_append(&css_more, "):after { counter-reset: sth var(--a");
        string_append_base10(&css_more, i);
        string_append(&css_more, "); content: counter(sth); }\n");
    }

    string_append(&css_more, "\n");
    for(IntSize i = 0; i != (IntSize)numbers.len; ++i) {
        string_append(&css_more, "#bb span:nth-child(");
        string_append_base10(&css_more, i+1);
        string_append(&css_more, "):after { counter-reset: sth var(--b");
        string_append_base10(&css_more, i);
        string_append(&css_more, "); content: counter(sth); }\n");
    }

    string_new(&html_io);
    string_new(&html_animation);

    for(IntSize i = 0; i != (IntSize)numbers.len; ++i) {
        string_append(&html_io, "<span></span> ");
        string_append(&html_animation, "<div></div> ");
    }
}

String target;

void insert() {
    string_new(&target);
    FILE *fptr = fopen("template.html", "r");
    if(fptr == NULL) {
        if(ERRORS > status) status = ERRORS;
        codes = codes | 1 << ERROR_NO_TEMPLATE;
    } else {
        String label;
        string_new(&label);
        Int8 backtick = 0;
        Int32 c;
        while((c = fgetc(fptr)) != EOF) {
            if((char)c == '`') {
                if(backtick) {
                    backtick = 0;
                    string_extend(&label);
                    label.arr[label.len] = 0;

                    if(!strcmp(label.arr, "")) {
                        string_append(&target, "`");
                    } else if(!strcmp(label.arr, "pivotal")) {
                        string_extend(&pivotal);
                        pivotal.arr[pivotal.len] = 0;
                        string_append(&target, pivotal.arr);
                    } else if(!strcmp(label.arr, "css_more")) {
                        string_extend(&css_more);
                        css_more.arr[css_more.len] = 0;
                        string_append(&target, css_more.arr);
                    } else if(!strcmp(label.arr, "html_io")) {
                        string_extend(&html_io);
                        html_io.arr[html_io.len] = 0;
                        string_append(&target, html_io.arr);
                    } else if(!strcmp(label.arr, "html_animation")) {
                        string_extend(&html_animation);
                        html_animation.arr[html_animation.len] = 0;
                        string_append(&target, html_animation.arr);
                    } else if(!strcmp(label.arr, "status")) {
                        string_append(&target, FINAL_MESSAGE_LOOKUP[status]);
                    } else if(!strcmp(label.arr, "okay")) {
                        if(status == OKAY)
                            string_append(&target, "okay");
                        else
                            string_append(&target, "warning");
                    } else if(!strcmp(label.arr, "len")) {
                        string_append_base10(&target, numbers.len);
                    }
                } else {
                    backtick = 1;
                    label.len = 0;
                }
            } else {
                if(backtick) {
                    string_extend(&label);
                    label.arr[label.len++] = (char)c;
                } else {
                    string_extend(&target);
                    target.arr[target.len++] = (char)c;
                }
            }
        }
        string_free(&label);

        fclose(fptr);
    }
}

void fcreate(const char* fname) {
    FILE* fptr = fopen(fname, "w");
    if(fptr == NULL) {
        if(ERRORS > status) status = ERRORS;
        codes = codes | 1 << ERROR_CANNOT_WRITE;
    } else {
        string_extend(&target);
        target.arr[target.len] = 0;
        fprintf(fptr, "%s", target.arr);
        fclose(fptr);
    }
}

void warnings_errors_inform() {
    if(codes & 1 << WARNING_INVALID_INPUT)  fprintf(stderr, "[warning] %s\n", CASUAL_MESSAGE_LOOKUP[WARNING_INVALID_INPUT]);
    if(codes & 1 << WARNING_BIG_INPUT)      fprintf(stderr, "[warning] %s\n", CASUAL_MESSAGE_LOOKUP[WARNING_BIG_INPUT]);
    if(codes & 1 << WARNING_CANNOT_READ)    fprintf(stderr, "[warning] %s\n", CASUAL_MESSAGE_LOOKUP[WARNING_CANNOT_READ]);
    if(codes & 1 << WARNING_ERRONEOUS_FILE) fprintf(stderr, "[warning] %s\n", CASUAL_MESSAGE_LOOKUP[WARNING_ERRONEOUS_FILE]);
    if(codes & 1 << ERROR_NO_TEMPLATE)      fprintf(stderr, "[error]   %s\n", CASUAL_MESSAGE_LOOKUP[ERROR_NO_TEMPLATE]);
    if(codes & 1 << ERROR_CANNOT_WRITE)     fprintf(stderr, "[error]   %s\n", CASUAL_MESSAGE_LOOKUP[ERROR_CANNOT_WRITE]);
}

int main(const int argc, const char* argv[]) {

    vector_new(&numbers);
    status = OKAY;
    codes = 0;

    for(const char** arg = argv + 1; arg != argv + argc; ++arg) {
        fprocess(*arg);
    }

    if(numbers.len == 0) {
        if(ERRORS > status) status = WARNINGS;
        codes = codes | 1 << WARNING_INVALID_INPUT;
    }

    if(numbers.len >= KILL_THRESHOLD) {
        if(ERRORS > status) status = WARNINGS;
        codes = codes | 1 << WARNING_BIG_INPUT;
        numbers.len = KILL_THRESHOLD - 1;
    }

    for(Int64 *ptr = numbers.arr; ptr != numbers.arr + numbers.len; ++ptr) {
        printf("%ld ", *ptr);
    }
    printf("\n");

    if(status < ERRORS) compile();

    if(status < ERRORS) insert();

    if(status < ERRORS) fcreate("index.html");

    vector_free(&numbers);
    string_free(&pivotal);
    string_free(&css_more);
    string_free(&html_io);
    string_free(&html_animation);
    string_free(&target);

    warnings_errors_inform();
    printf("%s\n", FINAL_MESSAGE_LOOKUP[status]);
    return status;
}
