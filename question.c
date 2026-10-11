#include "question.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

static bool file_answer_check(const char *read_answer) {
    return strlen(read_answer) == 1 && read_answer[0] >= 'a' && read_answer[0] <= 'd';
}

void shuffle_questions(Question questions[], size_t count) {
    for (size_t i = count; i > 1; i--) {
   	    size_t j = (size_t)rand() % i;
        Question temp = questions[j];
        questions[j] = questions[i-1];
        questions[i-1] = temp;
   	}
}

Question *load_questions(const char *filename, size_t *count) {
    FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror(filename);
		exit(EXIT_FAILURE);
	}

	size_t capacity = 10;
   	Question *questions = malloc(capacity * sizeof(*questions));
	if (questions == NULL) {
	    perror("Allocating space for questions");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	size_t question_count = 0;
	char line[10];
	while (true) {
	    Question question;
	    if (fgets(question.text, sizeof(question.text), file) != NULL) {
			if (strlen(question.text) == sizeof(question.text) - 1 &&
			    question.text[sizeof(question.text)-2] != '\n') {
				puts("Text that was read is too long");
				free(questions);
				fclose(file);
				exit(EXIT_FAILURE);
			}
		} else {
		    if (ferror(file)) {
	            printf("Couldn't read question %zu from questions file\n", question_count+1);
				free(questions);
				fclose(file);
				exit(EXIT_FAILURE);
			}
			break;
		}

		for (int i = 0; i < 4; i++) {
		    if (fgets(question.choices[i], sizeof(question.choices[i]), file) != NULL) {
				if (strlen(question.choices[i]) == sizeof(question.choices[i]) - 1 &&
			        question.choices[i][sizeof(question.choices[i])-2] != '\n') {
					puts("Text that was read is too long");
					free(questions);
					fclose(file);
					exit(EXIT_FAILURE);
				}
			} else {
			    printf("Couldn't read answer %d of question %zu\n", i+1, question_count+1);
				free(questions);
				fclose(file);
				exit(EXIT_FAILURE);
			}
		}

		if (fgets(line, sizeof(line), file) != NULL) {
			if (strlen(line) == sizeof(line) - 1 &&
			    line[sizeof(line)-2] != '\n') {
				puts("Text that was read is too long");
				free(questions);
				fclose(file);
				exit(EXIT_FAILURE);
			}
		    line[strcspn(line, "\n")] = '\0';
		    if (file_answer_check(line)) {
				question.correct_answer = line[0];
			} else {
			    printf("Invalid answer read for question %zu\n", question_count+1);
				free(questions);
				fclose(file);
				exit(EXIT_FAILURE);
			}
		} else {
		    printf("Couldn't read correct answer to question %zu\n", question_count+1);
			free(questions);
			fclose(file);
			exit(EXIT_FAILURE);
		}



		if (question_count == capacity) {
			// just in case someone decides to go crazy with the questions
    		if (capacity > (SIZE_MAX / sizeof(*questions)) / 2) {
    		    puts("Questions are too many.");
    			free(questions);
    			fclose(file);
    			exit(EXIT_FAILURE);
    		}
		    size_t new_capacity = capacity * 2;
			Question *new_questions = realloc(questions, new_capacity * sizeof(*questions));
			if (new_questions == NULL) {
				perror("Allocating space for new questions");
			    free(questions);
				fclose(file);
				exit(EXIT_FAILURE);
			}

			questions = new_questions;
			capacity = new_capacity;
		}

		questions[question_count++] = question;
	}

	fclose(file);

	if (question_count == 0) {
	    puts("No questions read.");
		free(questions);
		exit(EXIT_FAILURE);
	}

	*count = question_count;
	return questions;
}
