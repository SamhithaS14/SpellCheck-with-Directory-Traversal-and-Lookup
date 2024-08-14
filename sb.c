#include "sb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h> // Include for perror

void sb_init(strbuf_t *B, unsigned size) {
    B->capacity = size + 1;
    B->length = 0;
    //if (DEBUG)  printf("sb_init malloc: %d\n", B->capacity);
    B->data = malloc(B->capacity);
    if (B->data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    B->data[0] = '\0'; // Initialize buffer with null-terminated string
}

void sb_destroy(strbuf_t *B) {
    free(B->data);
}

void sb_ensure(strbuf_t *B, unsigned size) {
    if (size <= B->capacity) return;
    unsigned newcap = B->capacity * 2;
    if (newcap < size) newcap = size;
    //if(DEBUG)   printf("sb_ensure realloc: %d\n", newcap);
    char *buf = realloc(B->data, newcap);
    if (!buf) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    B->data = buf;
    B->capacity = newcap;
}

void sb_append(strbuf_t *B, char *str) {
    unsigned len = strlen(str);
    sb_ensure(B, B->length + len + 1);
    memcpy(B->data + B->length, str, len + 1); // Append including null terminator
    B->length += len;
}