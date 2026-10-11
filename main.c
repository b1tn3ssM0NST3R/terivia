#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "question.h"

typedef enum input_status {
    INPUT_OK,
    INPUT_END,
    INPUT_ERROR
} InputStatus;

InputStatus ask_replay(bool *reply) {
    printf("\n%s", "Do you want to play again (y/n)? ");
    char response[10];
    if (fgets(response, sizeof(response), stdin) == NULL) {
        *reply = false;
        return ferror(stdin) ? INPUT_ERROR : INPUT_END;
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
            *reply =  false;
            return ferror(stdin) ? INPUT_ERROR : INPUT_END;
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
        *reply = true;
        return INPUT_OK;
    }

    *reply = false;
    return INPUT_OK;
}

InputStatus read_answer(char *input) {
    printf("Your answer (input a, b, c or d)? ");
	char answer[3] = {0};
	if (fgets(answer, sizeof(answer), stdin) == NULL) {
		if (ferror(stdin)) {
	        puts("\nError occured when reading input");
		}
		return ferror(stdin) ? INPUT_ERROR : INPUT_END;
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
			return ferror(stdin) ? INPUT_ERROR : INPUT_END;
		};
	}

	*input = answer[0];
	return INPUT_OK;
}

InputStatus play_round(const Question questions[], size_t count, int *final_score) {
    int score = 0;
    for (size_t i = 0; i < count; i++) {
   	    printf("%zu) %s", i+1, questions[i].text);
  		for (size_t j = 0; j <= 3; j++) {
  		    printf("%c) %s", 'a' + (int)j, questions[i].choices[j]);
  		}

  		puts("");

        char answer = 0;
        InputStatus read_answer_status = read_answer(&answer);
        if (read_answer_status != INPUT_OK) {
            return read_answer_status;
        }

  		if (answer == questions[i].correct_answer) {
            puts("Correct!\n");
            score++;
  		} else {
            puts("Not quite correct. Better luck, next time.\n");
  		}
   	}


    *final_score = score;
    return INPUT_OK;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    const char *filename = "sample_questions/test_questions";
    if (argc == 2) {
        filename = argv[1];
    } else if (argc > 2) {
        fprintf(stderr, "usage: %s [questions_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

	puts("Welcome to Terivia!");

	puts("\nAnswer as many questions correct to get points.");

	puts("");

	size_t question_count = 0;
	Question *questions = load_questions(filename, &question_count);

	bool replay = false;

	do {
		shuffle_questions(questions, question_count);
		int score = 0;
		InputStatus play_round_status = play_round(questions, question_count, &score);

		if (play_round_status != INPUT_OK) {
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

        InputStatus replay_status = ask_replay(&replay);
        if (replay_status != INPUT_OK) {
            break;
        }
    } while (replay);


    free(questions);

	return ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS;
}
