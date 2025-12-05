#include "../include/utils.h"
#include "../include/preprocess.h"
#include "../include/sentiment.h"
#include "../include/parallel.h"      // <-- parallel analyzer header

#include <stdio.h>
#include <omp.h>

int main() 
{
    int total;

    // Load dictionaries once (thread-safe after this point)
    load_lexicons();

    // Load dataset
    char** messages = load_chat_dataset("data/sample_chatlog.txt", &total);

    // Preprocess all messages FIRST
    // (So serial and parallel comparison is fair)
    for (int i = 0; i < total; i++) {
        preprocess_message(messages[i]);
    }

    printf("\n=== SERIAL SENTIMENT ANALYSIS ===\n");

    double t1 = omp_get_wtime();
    SentimentResult serialRes = analyze_messages(messages, total);
    double t2 = omp_get_wtime();

    printf("Total messages: %d\n", total);
    printf("Positive:       %d\n", serialRes.positive);
    printf("Negative:       %d\n", serialRes.negative);
    printf("Spam:           %d\n", serialRes.spam);
    printf("Time (serial):  %.6f sec\n\n", t2 - t1);

    printf("====================================\n\n");

    // PARALLEL SECTION
    int threads = 8;   // <-- MANUAL THREAD COUNT
    omp_set_num_threads(threads);

    printf("=== PARALLEL SENTIMENT ANALYSIS (%d threads) ===\n", threads);

    double p1 = omp_get_wtime();
    ParallelResult parRes = analyze_messages_parallel(messages, total, threads);
    double p2 = omp_get_wtime();

    printf("Positive:       %d\n", parRes.positive);
    printf("Negative:       %d\n", parRes.negative);
    printf("Spam:           %d\n", parRes.spam);
    printf("Time (parallel): %.6f sec\n", p2 - p1);

    // SPEEDUP
    double speedup = (t2 - t1) / (p2 - p1);
    printf("Speedup: %.2fx\n", speedup);

    free_dataset(messages, total);
    return 0;
}
