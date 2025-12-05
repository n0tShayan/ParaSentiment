#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char** load_chat_dataset(const char* filename, int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char** messages = malloc(sizeof(char*) * 1000000); // supports up to 1M messages
    char buffer[2048];
    int idx = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        messages[idx] = strdup(buffer);
        idx++;
    }

    fclose(fp);
    *count = idx;
    return messages;
}

void free_dataset(char** messages, int count) {
    for (int i = 0; i < count; i++)
        free(messages[i]);
    free(messages);
}
