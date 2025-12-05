#ifndef UTILS_H
#define UTILS_H

char** load_chat_dataset(const char* filename, int* count);
void free_dataset(char** messages, int count);

#endif
