#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "question.h"
#include "input.h"

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
	if (questions == NULL) {
	    return EXIT_FAILURE;
	}

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
