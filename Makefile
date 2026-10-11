CC = gcc
CFLAGS = -Wall -Wextra -std=c99

terivia: main.c question.c question.h
	$(CC) $(CFLAGS) main.c question.c -o terivia

.PHONY: clean

clean:
	rm -f terivia

.PHONY: test

test: terivia
	sh tests/correct_answer.sh
	sh tests/invalid_answer.sh
	sh tests/replay.sh
	sh tests/incomplete_question.sh
	sh tests/question_dynamic_storage.sh
	sh tests/argument_question_file.sh
	sh tests/eof_answer.sh
