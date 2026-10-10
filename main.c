#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct question {
    char text[256];
    char choices[4][256];
    char correct_answer;
} Question;

bool file_answer_check(const char *read_answer) {
    return strlen(read_answer) == 1 && read_answer[0] >= 'a' && read_answer[0] <= 'd';
}

bool ask_replay(void) {
    printf("\n%s", "Do you want to play again (y/n)? ");
    char response[10];
    if (fgets(response, sizeof(response), stdin) == NULL) {
        // puts("\nError occured reading your response");
        // exit(EXIT_FAILURE);
        return false; // I think Ctrl-D triggers this branch, so it is valid to have it quite
    }

    char user_reply = response[0];
    if (strchr(response, '\n') == NULL) {
        int ch = 0;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            // consume what is left
        }
    }

    // Just asking for replay so validation can be relaxed over here
    while (response[0] != 'y' && response[0] != 'n') {
        puts("Invalid response. You need to input either 'y' or 'n'.");
        printf("%s", "Do you want to play again (y/n)? ");

        if (fgets(response, sizeof(response), stdin) == NULL) {
            // puts("\nError occured reading your response");
            // exit(EXIT_FAILURE);
            return false; // check if fgets above
        }

        user_reply = response[0];
        if (strchr(response, '\n') == NULL) {
            int ch = 0;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                // consume what is left
            }
        }
    }

    if (user_reply == 'y') {
        puts(""); // adds new line if you answer correctly to space out nicely
        return true;
    }

    return false;
}

char read_answer(void) {
    printf("Your answer (input a, b, c or d)? ");
	char answer[3] = {0};
	if (fgets(answer, sizeof(answer), stdin) == NULL) {
		if (ferror(stdin)) {
	        puts("\nError occured when reading input");
		}
		return '\0';
	}

	while (answer[1] != '\n' || answer[0] < 'a' || answer[0] > 'd') {
	    puts("Answer should be a, b, c or d. You typed something else.");
	    printf("\nYour answer (input a, b, c or d)? ");
		int ch = 0;
		if (strchr(answer, '\n') == NULL) {
		    while ((ch = getchar()) != '\n' && ch != EOF); // consumes any leftovers if there are any
		}
		if (fgets(answer, sizeof(answer), stdin) == NULL) {
		    if (ferror(stdin)) {
				puts("\nError occured when reading input\n");
			}
			return '\0';
		};
	}

	return answer[0];
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

void shuffle_questions(Question questions[], size_t count) {
    for (size_t i = count; i > 1; i--) {
   	    size_t j = (size_t)rand() % i;
        Question temp = questions[j];
        questions[j] = questions[i-1];
        questions[i-1] = temp;
   	}
}

int play_round(const Question questions[], size_t count) {
    int score = 0;
    for (size_t i = 0; i < count; i++) {
   	    printf("%zu) %s", i+1, questions[i].text);
  		for (size_t j = 0; j <= 3; j++) {
  		    printf("%c) %s", 'a' + (int)j, questions[i].choices[j]);
  		}

  		puts("");

        char answer = read_answer();
        // this is so that by inputing Ctrl-D, execution reaches the freeing memory code
        if (answer == '\0') {
            return -1;
        }

  		if (answer == questions[i].correct_answer) {
            puts("Correct!\n");
            score++;
  		} else {
            puts("Not quite correct. Better luck, next time.\n");
  		}
   	}

    return score;
}

int main(void) {
    srand(time(NULL));

	puts("Welcome to Terivia!");

	puts("\nAnswer as many questions correct to get points.");

	puts("");

	size_t question_count = 0;
	Question *questions = load_questions("questions", &question_count);

	do {
		shuffle_questions(questions, question_count);
		int score = play_round(questions, question_count);

		// check play round function
		if (score == -1) {
		    break;
		}

		printf("Your score: %d out of %zu\n", score, question_count);
       	if ((size_t)score == question_count) {
       	    puts("Well done!");
       	} else if ((size_t)score > question_count/2) {
       	    puts("Not bad.");
       	} else {
       	    puts("You can do better.");
       	}
    } while (ask_replay());

    free(questions);

	return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
}
