CC = gcc
CFLAGS = -Wall -Wextra -std=c99

terivia: main.o question.o input.o
	$(CC) $(CFLAGS) main.o question.o input.o -o terivia

main.o: main.c question.h input.h
	$(CC) $(CFLAGS) -c main.c -o main.o

question.o: question.c question.h
	$(CC) $(CFLAGS) -c question.c -o question.o

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c -o input.o

.PHONY: clean

clean:
	rm -f terivia main.o question.o input.o

.PHONY: test

test: terivia
	sh tests/correct_answer.sh
	sh tests/invalid_answer.sh
	sh tests/replay.sh
	sh tests/incomplete_question.sh
	sh tests/question_dynamic_storage.sh
	sh tests/argument_question_file.sh
	sh tests/eof_answer.sh
