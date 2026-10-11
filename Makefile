CC = gcc
CFLAGS = -Wall -Wextra -std=c99
DEPFLAGS = -MMD -MP

terivia: main.o question.o input.o
	$(CC) $(CFLAGS) main.o question.o input.o -o terivia

main.o: main.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c main.c -o main.o

question.o: question.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c question.c -o question.o

input.o: input.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c input.c -o input.o

-include main.d question.d input.d

.PHONY: clean

clean:
	rm -f terivia main.o question.o input.o main.d question.d input.d

.PHONY: test

test: terivia
	sh tests/correct_answer.sh
	sh tests/invalid_answer.sh
	sh tests/replay.sh
	sh tests/incomplete_question.sh
	sh tests/question_dynamic_storage.sh
	sh tests/argument_question_file.sh
	sh tests/eof_answer.sh
