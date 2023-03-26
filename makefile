CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -lncurses
DEPS=

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

snake: snake.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o snake
