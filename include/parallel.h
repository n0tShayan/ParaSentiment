#ifndef PARALLEL_H
#define PARALLEL_H

typedef struct {
    int positive;
    int negative;
    int spam;
    int emoji_score;
    double time_seconds;
} ParallelResult;

ParallelResult analyze_messages_parallel(char** messages, long total, int threadCount);

#endif
