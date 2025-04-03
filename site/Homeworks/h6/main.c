// Exercise 4

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE 1000000

int main(int argc, const char *argv[]){
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        perror("Memory allocation failed");
        return 1;
    }

    for (int i = 0; i < ARRAY_SIZE; i += 4096 / sizeof(int)) {
        arr[i] = i;
    }

    free(array);
    return 0;
}
