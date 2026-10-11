CC = gcc
CFLAGS = -Wall -Wextra -std=c99

terivia: main.c
	$(CC) $(CFLAGS) main.c -o terivia

.PHONY: clean

clean:
	rm -f terivia

.PHONY: test

test: terivia
	sh tests/correct_answer.sh
	sh tests/invalid_answer.sh
	sh tests/replay.sh
	sh tests/incomplete_question.sh
