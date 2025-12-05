#include "../include/utils.h"
#include "../include/preprocess.h"
#include "../include/sentiment.h"
#include "../include/parallel.h"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char** argv) 
{
    int total;

    // Load dictionaries (thread-safe after init)
    load_lexicons();

    // ----------------------------
    // MODE 1: Benchmark / Makefile mode
    // Format:
    //    sentiment_omp <dataset> <threads> <mode>
    // mode = 0 → serial
    // mode = 1 → parallel
    // ----------------------------
    if (argc == 4) 
    {
        char* file = argv[1];
        int threads = atoi(argv[2]);
        int mode = atoi(argv[3]);

        char** messages = load_chat_dataset(file, &total);

        // Preprocess BEFORE timing
        for (int i = 0; i < total; i++) {
            preprocess_message(messages[i]);
        }

        if (mode == 0) {
            // SERIAL
            double t1 = omp_get_wtime();
            SentimentResult res = analyze_messages(messages, total);
            double t2 = omp_get_wtime();

            // CSV FORMAT (mode, threads, time)
            printf("serial,%d,%.6f\n", threads, (t2 - t1));
        }
        else if (mode == 1) {
            // PARALLEL
            omp_set_num_threads(threads);

            double p1 = omp_get_wtime();
            ParallelResult res = analyze_messages_parallel(messages, total, threads);
            double p2 = omp_get_wtime();

            // CSV FORMAT
            printf("parallel,%d,%.6f\n", threads, (p2 - p1));
        }

        free_dataset(messages, total);
        return 0;
    }

    // ---------------------------------------------------------
    // MODE 2: Normal mode (running without args)
    // ---------------------------------------------------------
    printf("Running in NORMAL MODE (no CLI args detected)\n");

    char** messages = load_chat_dataset("data/sample_chatlog.txt", &total);

    // Preprocess all messages FIRST
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

    printf("==============================================\n\n");

    // DEFAULT THREADS FOR NORMAL MODE
    int threads = 8;
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
