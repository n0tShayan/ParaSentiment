#ifndef SENTIMENT_H
#define SENTIMENT_H

typedef struct {
    int positive;
    int negative;
    int spam;
} SentimentResult;

void load_lexicons();
int is_spam(const char* msg);
int sentiment_score(const char* msg);
SentimentResult analyze_messages(char** messages, int total);

#endif
