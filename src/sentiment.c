#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/sentiment.h"

#define MAX_WORDS   5000
#define MAX_EMOJIS  100

char* positive_words[MAX_WORDS];
char* negative_words[MAX_WORDS];
char* spam_patterns[MAX_WORDS];
char* emojis[MAX_EMOJIS];
int emoji_weights[MAX_EMOJIS];

int pos_count = 0, neg_count = 0, spam_count = 0, emoji_count = 0;

void load_list(char* filename, char** list, int* count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) { printf("Error reading %s\n", filename); exit(1); }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        list[*count] = strdup(buffer);
        (*count)++;
    }
    fclose(fp);
}

void load_emoji_weights() {
    FILE* fp = fopen("dictionaries/emoji_weights.txt", "r");
    if (!fp) { printf("Error reading emoji_weights.txt\n"); exit(1); }

    char emoji[32];
    int weight;
    while (fscanf(fp, "%s %d", emoji, &weight) != EOF) {
        emojis[emoji_count] = strdup(emoji);
        emoji_weights[emoji_count] = weight;
        emoji_count++;
    }
    fclose(fp);
}

void load_lexicons() {
    load_list("dictionaries/positive_words.txt", positive_words, &pos_count);
    load_list("dictionaries/negative_words.txt", negative_words, &neg_count);
    load_list("dictionaries/spam_patterns.txt", spam_patterns, &spam_count);
    load_emoji_weights();
}

int contains_word(const char* msg, char** list, int count) {
    for (int i = 0; i < count; i++) {
        if (strstr(msg, list[i])) {
            return 1;
        }
    }
    return 0;
}

int is_spam(const char* msg) {
    return contains_word(msg, spam_patterns, spam_count);
}

int emoji_sentiment(const char* msg) {
    int total = 0;
    for (int i = 0; i < emoji_count; i++) {
        if (strstr(msg, emojis[i])) {
            total += emoji_weights[i];
        }
    }
    return total;
}

int sentiment_score(const char* msg) {
    int score = 0;

    if (contains_word(msg, positive_words, pos_count)) score++;
    if (contains_word(msg, negative_words, neg_count)) score--;

    score += emoji_sentiment(msg);

    return score;
}

SentimentResult analyze_messages(char** messages, int total) {
    SentimentResult result = {0, 0, 0};

    for (int i = 0; i < total; i++) {
        if (is_spam(messages[i])) {
            result.spam++;
            continue;
        }

        int score = sentiment_score(messages[i]);

        if (score > 0)      result.positive++;
        else if (score < 0) result.negative++;
    }

    return result;
}
