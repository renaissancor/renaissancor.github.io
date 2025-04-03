#include "main.h"

/**
 * @argv[0]: h3
 * @argv[1]: Name of Input File
 * @argv[2]: Sort Type: int char double
 */

int main(int argc, char *argv[]){
    srand((unsigned)time(0));
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <SortType>\n", argv[0]);
        return EXIT_FAILURE;
    }

    DataType data_type = get_data_type(argv[1]);
    SortType sort_type = get_SortType(argv[2]);

    if (data_type == NO_TYPE || sort_type == NO_SORT) {
        fflush(stderr);
        fprintf(stderr, "ERROR: Invalid arguments.\n");
        return EXIT_FAILURE;
    }

    List *list = malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    read_from_file(list, argv[1], data_type);

    if (sort_type == RAND_SORT) {
        sort_list(list, compare_rand);  // Random sort for all data types
    } else {
        if (data_type == INT_TYPE) {
            if (sort_type == INCR_SORT) {
                sort_list(list, compare_int_asc);
            } else /*if (sort_type == DECR_SORT)*/ {
                sort_list(list, compare_int_desc);
            }
        } else if (data_type == DOUBLE_TYPE) {
            if (sort_type == INCR_SORT) {
                sort_list(list, compare_double_asc);
            } else /*if (sort_type == DECR_SORT)*/ {
                sort_list(list, compare_double_desc);
            }
        } else /*if (data_type == CHAR_TYPE) */ {
            if (sort_type == INCR_SORT) {
                sort_list(list, compare_char_asc);
            } else /*if (sort_type == DECR_SORT)*/  {
                sort_list(list, compare_char_desc);
            }
        }
    }

    write_to_file(list, sort_type, data_type);
    free_list(list);
    return 0;
}

enum DataType get_data_type(char *arg2) {
    if (strstr(arg2, "_int") != NULL) {
        return INT_TYPE;
    } else if (strstr(arg2, "_double") != NULL) {
        return DOUBLE_TYPE;
    } else if (strstr(arg2, "_char") != NULL) {
        return CHAR_TYPE;
    } else {
        return NO_TYPE;
    }
}

enum SortType get_SortType(char *arg1) {
    if (strcmp(arg1, "inc") == 0) return INCR_SORT;
    if (strcmp(arg1, "dec") == 0) return DECR_SORT;
    if (strcmp(arg1, "rand") == 0) return RAND_SORT;
    return NO_SORT;
}


struct Node *make_node(char *key, void *value){
    Node *new_node = (struct Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failure\n");
        exit(EXIT_FAILURE);
    }
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

void add_node(Node **head, char *key, void *value) {
    char *key_copy = malloc(strlen(key)+1);
    if (!key_copy) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(key_copy, key);

    Node *new_node = make_node(key_copy, value);

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node *itr = *head;
        while (itr->next != NULL) {
            itr = itr->next;
        }
        itr->next = new_node;
    }
}

void read_from_file(List *list, const char *file_name, DataType data_type) {
    if(list == NULL) return;
    FILE *file_read = fopen(file_name, "r");
    if (file_read == NULL) {
        fprintf(stderr, "Error: Unable to open file %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    printf("reading %s\n", file_name);

    char *line = NULL;
    size_t len = 0;


    while (getline(&line, &len, file_read) != -1) {
        char *key = strtok(line, "=");
        char *value_str = strtok(NULL, "\n");

        if (!key || !value_str) {
            fprintf(stderr, "Invalid line format\n");
            continue;
        }

        void *value;
        if (data_type == INT_TYPE) {
            value = malloc(sizeof(int));
            *(int *)value = atoi(value_str);
        } else if (data_type == DOUBLE_TYPE) {
            value = malloc(sizeof(double));
            *(double *)value = atof(value_str);
        } else /*if (data_type == CHAR_TYPE)*/ {
            value = malloc(strlen(value_str) + 1);
            strcpy((char *)value, value_str);
        }

        add_node(&list->head, key, value);
        list->size++;
    }
    fclose(file_read);
}

int compare_int_asc(const void *a, const void *b) {
    const Node *node1 = *(const Node **)a;
    const Node *node2 = *(const Node **)b;
    return (*(int *)node1->value) - (*(int *)node2->value);
}

int compare_int_desc(const void *a, const void *b) {
    const Node *node1 = *(const Node **)a;
    const Node *node2 = *(const Node **)b;
    return (*(int *)node2->value) - (*(int *)node1->value);
}

int compare_double_asc(const void *a, const void *b) {
    const Node *node1 = *(const Node **)a;
    const Node *node2 = *(const Node **)b;
    return (*(double *)node1->value > *(double *)node2->value) -
           (*(double *)node1->value < *(double *)node2->value);
}

int compare_double_desc(const void *a, const void *b) {
    const Node *node1 = *(const Node **)a;
    const Node *node2 = *(const Node **)b;
    return (*(double *)node2->value > *(double *)node1->value) -
           (*(double *)node2->value < *(double *)node1->value);
}

int compare_char_asc(const void *a, const void *b) {
    const Node *node1 = *(const Node **)a;
    const Node *node2 = *(const Node **)b;
    return strcmp((char *)node1->value, (char *)node2->value);
}

int compare_char_desc(const void *a, const void *b) {
    const Node *node1 = *(const Node **)a;
    const Node *node2 = *(const Node **)b;
    return strcmp((char *)node2->value, (char *)node1->value);
}

int compare_rand(const void *a, const void *b) {
    if (a == NULL || b == NULL)
    {
        fprintf(stderr, "Error: invalid value\n");
        exit(1);
    }
    return rand() % 2 ? 1 : -1;
}

void sort_list(List *list, compare_fn cmp) {
    if(list == NULL) return;
    if (list->size < 2) return;
    Node **node_array = malloc(list->size * sizeof(Node *));
    Node *itr = list->head;
    for (size_t i = 0; i < list->size; ++i) {
        node_array[i] = itr;
        itr = itr->next;
    }

    qsort(node_array, list->size, sizeof(Node *), cmp);

    list->head = node_array[0];
    for (size_t i = 1; i < list->size; ++i) {
        node_array[i - 1]->next = node_array[i];
    }
    node_array[list->size - 1]->next = NULL;

    free(node_array);
}


void free_list(List *list) {
    Node *itr = list->head;
    while (itr != NULL) {
        Node *temp = itr;
        itr = itr->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
    free(list);
}

/* Ex 2.2 Find value function
 *
 * */
struct Node *find_value(List *list, void *answer, DataType data_type){
    struct Node *itr = list->head;
    while (itr != NULL)
    {
        switch(data_type){
            case CHAR_TYPE: {
                if (strcmp(itr->value, (char*)answer) == 0) return itr;
            } break;
            case INT_TYPE: {
                if (*((int *) itr->value) == *((int *) answer)) return itr;
            } break;
            case DOUBLE_TYPE: {
                if (*((double *) itr->value) == *((double *) answer)) return itr;
            } break;
            default: break;
        }
        itr = itr->next;
    }
    return NULL;
}

void write_to_file(List *list, SortType sort_type, DataType data_type){
    char *write_file_name = malloc(sizeof(char) * 256);
    if (write_file_name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    snprintf(write_file_name, 256, "%s_%s.txt",
             sort_type_to_string(sort_type),
             data_type_to_string(data_type));

    FILE *write_file = fopen(write_file_name, "w");
    if (write_file == NULL) {
        fprintf(stderr, "Error: Unable to open write file %s\n", write_file_name);
        fclose(write_file);
        exit(EXIT_FAILURE);
    }

    printf("Writing %s\n", write_file_name);

    Node *itr = list->head;
    while (itr != NULL) {
        if (data_type == INT_TYPE) {
            fprintf(write_file, "%s=%d\n", itr->key, *(int *)(itr->value));
        } else if (data_type == DOUBLE_TYPE) {
            fprintf(write_file, "%s=%lf\n", itr->key, *(double *)(itr->value));
        } else  if (data_type == CHAR_TYPE)  {
            fprintf(write_file, "%s=%s\n", itr->key, (char *)(itr->value));
        }
        itr = itr->next;
    }

    fclose(write_file);
    free(write_file_name);
}

// End with \n
