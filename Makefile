# -------------------------
# COMPILER SETTINGS
# -------------------------
CC = gcc
CFLAGS = -O3 -fopenmp -Iinclude

# Source files
SRC = src/main.c src/utils.c src/preprocess.c src/sentiment.c src/parallel.c
OUT = sentiment_omp

# Dataset for benchmarks
DATASET = data/sample_chatlog.txt

# Thread counts for benchmarking
THREADS = 1 2 4 8 12 16

# Number of repeated runs per thread
RUNS = 3

# -------------------------
# BUILD EXECUTABLE
# -------------------------
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

# -------------------------
# CLEAN BUILD
# -------------------------
clean:
	del /Q $(OUT) 2> NUL || true
	del /Q results.csv 2> NUL || true
	del /Q *.png 2> NUL || true

# -------------------------
# BENCHMARK (Step 5)
# - runs parallel mode several times for each thread count
# - runs serial mode once per thread count
# - outputs CSV lines
# -------------------------
benchmark: $(OUT)
	@echo mode,threads,time> results.csv
	@for %%t in ($(THREADS)) do ( \
		echo ----- THREAD %%t ----- && \
		for /L %%r in (1,1,$(RUNS)) do ( \
			$(OUT) $(DATASET) %%t 1 >> results.csv \
		) \
		&& $(OUT) $(DATASET) %%t 0 >> results.csv \
	)

# -------------------------
# GRAPH GENERATION
# python script must be named: plot_results.py
# -------------------------
graphs: benchmark
	python plot_results.py

# -------------------------
# RUN NORMAL MODE
# -------------------------
run:
	$(OUT)
