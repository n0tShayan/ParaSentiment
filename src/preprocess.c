#include <ctype.h>
#include <string.h>

void preprocess_message(char* msg) {

    // Remove newline
    msg[strcspn(msg, "\n")] = '\0';

    // Convert to lowercase
    for (int i = 0; msg[i]; i++) {
        msg[i] = tolower(msg[i]);
    }

    // Trim leading spaces
    char* start = msg;
    while (*start == ' ') start++;
    
    // Move cleaned string back
    if (start != msg) memmove(msg, start, strlen(start) + 1);

    // Remove extra spaces between words
    int i = 0, j = 0;
    int space_flag = 0;

    while (msg[i]) {
        if (msg[i] == ' ') {
            if (!space_flag) msg[j++] = msg[i];
            space_flag = 1;
        } else {
            msg[j++] = msg[i];
            space_flag = 0;
        }
        i++;
    }
    msg[j] = '\0';
}
