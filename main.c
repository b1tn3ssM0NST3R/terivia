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
            return false; // check line 23
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
	    puts("\nError occured when reading input");
		exit(EXIT_FAILURE);
	}

	while (answer[1] != '\n' || answer[0] < 'a' || answer[0] > 'd') {
	    puts("Answer should be a, b, c or d. You typed something else.");
	    printf("\nYour answer (input a, b, c or d)? ");
		int ch = 0;
		if (strchr(answer, '\n') == NULL) {
		    while ((ch = getchar()) != '\n' && ch != EOF); // consumes any leftovers if there are any
		}
		if (fgets(answer, sizeof(answer), stdin) == NULL) {
			puts("\nError occured when reading input\n");
			exit(EXIT_FAILURE);
		};
	}

	return answer[0];
}

size_t load_questions(const char *filename, Question questions[], size_t capacity) {
 	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		perror(filename);
		exit(EXIT_FAILURE);
	}

	size_t question_count = 0;
	char line[10];
	while (question_count < capacity) {
	    if (fgets(questions[question_count].text, sizeof(questions[question_count].text), file) != NULL) {
			if (strlen(questions[question_count].text) == sizeof(questions[question_count].text) - 1 &&
			    questions[question_count].text[sizeof(questions[question_count].text)-2] != '\n') {
				puts("Text that was read is too long");
				fclose(file);
				exit(EXIT_FAILURE);
			}
		} else {
		    if (ferror(file)) {
	            printf("Couldn't read question %zu from questions file\n", question_count+1);
				fclose(file);
				exit(EXIT_FAILURE);
			}
			break;
		}

		for (int i = 0; i < 4; i++) {
		    if (fgets(questions[question_count].choices[i], sizeof(questions[question_count].choices[i]), file) != NULL) {
				if (strlen(questions[question_count].choices[i]) == sizeof(questions[question_count].choices[i]) - 1 &&
			        questions[question_count].choices[i][sizeof(questions[question_count].choices[i])-2] != '\n') {
					puts("Text that was read is too long");
					fclose(file);
					exit(EXIT_FAILURE);
				}
			} else {
			    printf("Couldn't read answer %d of question %zu\n", i+1, question_count+1);
				fclose(file);
				exit(EXIT_FAILURE);
			}
		}

		if (fgets(line, sizeof(line), file) != NULL) {
			if (strlen(line) == sizeof(line) - 1 &&
			    line[sizeof(line)-2] != '\n') {
				puts("Text that was read is too long");
				fclose(file);
				exit(EXIT_FAILURE);
			}
		    line[strcspn(line, "\n")] = '\0';
		    if (file_answer_check(line)) {
				questions[question_count].correct_answer = line[0];
			} else {
			    printf("Invalid answer read for question %zu\n", question_count+1);
				fclose(file);
				exit(EXIT_FAILURE);
			}
		} else {
		    printf("Couldn't read correct answer to question %zu\n", question_count+1);
			fclose(file);
			exit(EXIT_FAILURE);
		}
		question_count++;
	}

	if (question_count == capacity) {
	    int extra = getc(file);

		if (extra != EOF) {
		    printf("Too many questions, maximum is %zu\n", capacity);
			fclose(file);
			exit(EXIT_FAILURE);
		}

		if (ferror(file)) {
		    puts("Error reading questions file");
			fclose(file);
			exit(EXIT_FAILURE);
		}
	}

	fclose(file);

	if (question_count == 0) {
	    puts("No questions read.");
		exit(EXIT_FAILURE);
	}

	return question_count;
}

int main(void) {
    srand(time(NULL));

	puts("Welcome to Terivia!");

	puts("\nAnswer as many questions correct to get points.");

	puts("");

	Question questions[100];
	size_t capacity = sizeof(questions) / sizeof(questions[0]);
	size_t question_count = load_questions("questions", questions, capacity);

	do {
	    int score = 0;

    	for (size_t i = question_count; i > 1; i--) {
    	    size_t j = (size_t)rand() % i;
    		Question temp = questions[j];
    		questions[j] = questions[i-1];
    		questions[i-1] = temp;
    	}

    	for (size_t i = 0; i < question_count; i++) {
    	    printf("%zu) %s", i+1, questions[i].text);
    		for (size_t j = 0; j <= 3; j++) {
    		    printf("%c) %s", 'a' + (int)j, questions[i].choices[j]);
    		}

    		puts("");

    		if (read_answer() == questions[i].correct_answer) {
                puts("Correct!\n");
                score++;
    		} else {
                puts("Not quite correct. Better luck, next time.\n");
    		}
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

	return 0;
}
