CC = gcc
CFLAGS = -Wall -Wextra -std=c99

terivia: main.c
	$(CC) $(CFLAGS) main.c -o terivia

.PHONY: clean

clean:
	rm -f terivia
