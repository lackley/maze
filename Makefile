CC = gcc
GEN = generator
SOL = solver
SOL_FULL = solver_full
CFLAGS = -g -Wall -Wextra -std=c99

GEN_HEADERS = common.h generator.h
GEN_OBJS = generator.c

SOL_HEADERS = common.h solver.h
SOL_OBJS = solver.c

all: generator solver solver_full

generator: $(GEN_HEADERS) $(GEN_OBJS)
	$(CC) $(CFLAGS) -o $(GEN) $(GEN_OBJS)

solver: $(SOL_HEADERS) $(SOL_OBJS)
	$(CC) $(CFLAGS) -o $(SOL) $(SOL_OBJS)

solver_full: $(SOL_OBJS) $(SOL_HEADERS)
	$(CC) $(CFLAGS) -o $(SOL_FULL) -DFULL $(SOL_OBJS)

clean:
	rm -f $(GEN) $(SOL) $(SOL_FULL)
