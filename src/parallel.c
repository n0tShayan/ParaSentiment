#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/preprocess.h"
#include "../include/sentiment.h"

typedef struct {
    int positive;
    int negative;
    int spam;
    int emoji_score;
    double time_seconds;
} ParallelResult;

ParallelResult analyze_messages_parallel(char** messages, long total, int threadCount)
{
    ParallelResult R = {0, 0, 0, 0, 0.0};

    omp_set_num_threads(threadCount);

    double start = omp_get_wtime();

    int pos_local = 0;
    int neg_local = 0;
    int spam_local = 0;
    int emoji_local = 0;

    // -------------------------------
    // Main Parallel Region
    // -------------------------------
    #pragma omp parallel reduction(+:pos_local, neg_local, spam_local, emoji_local)
    {
        // Each thread handles its chunk of messages
        #pragma omp for
        for (long i = 0; i < total; i++) {

            char* msg = messages[i];

            preprocess_message(msg);

            if (is_spam(msg)) {
                spam_local++;
                continue;
            }

            int score = sentiment_score(msg);

            if (score > 0) pos_local++;
            else if (score < 0) neg_local++;
        }
    }

    double end = omp_get_wtime();

    R.positive = pos_local;
    R.negative = neg_local;
    R.spam     = spam_local;
    R.emoji_score = emoji_local;
    R.time_seconds = end - start;

    return R;
}
