#ifndef MAIN_H
#define MAIN_H


#include <stdio.h>      // standard input output
#include <stdlib.h>     // standard library
#include <string.h>     // string
#include <time.h>       // time


typedef int (*compare_fn)(const void *, const void *);

typedef enum DataType {
    NO_TYPE = 0,
    CHAR_TYPE = 1,
    INT_TYPE = 2,
    DOUBLE_TYPE = 3
} DataType;

typedef enum SortType {
    NO_SORT = 0,
    INCR_SORT = 1,
    DECR_SORT = 2,
    RAND_SORT = 3,
} SortType;

typedef struct Node {
    char *key;
    void *value;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    size_t size;
} List;

const char *sort_type_to_string(SortType sort_type) {
    switch (sort_type) {
        case INCR_SORT: return "inc";
        case DECR_SORT: return "dec";
        case RAND_SORT: return "rand";
        default: return "unknown";
    }
}

const char *data_type_to_string(DataType data_type) {
    switch (data_type) {
        case CHAR_TYPE: return "char*";
        case INT_TYPE: return "int";
        case DOUBLE_TYPE: return "double";
        default: return "unknown";
    }
}

DataType get_data_type(char *arg);
SortType get_SortType(char *arg);

struct Node *make_node(char *key, void *value);

void read_from_file(List *list, const char *file_name, DataType data_type);
void add_node(Node **head, char *key, void *value);
void free_list(List *list);

int compare_int_asc(const void *a, const void *b);
int compare_int_desc(const void *a, const void *b);
int compare_double_asc(const void *a, const void *b);
int compare_double_desc(const void *a, const void *b);
int compare_char_asc(const void *a, const void *b);
int compare_char_desc(const void *a, const void *b);
int compare_rand(const void *a, const void *b);

void sort_list(List *list, compare_fn cmp);

struct Node *find_value(List *list, void *answer, DataType data_type);
void write_to_file(List *list, SortType sort_type, DataType data_type);


#endif
