#include "input.h"
#include <stdio.h>
#include <string.h>

InputStatus ask_replay(bool *replay) {
    printf("\n%s", "Do you want to play again (y/n)? ");
    char response[10];
    if (fgets(response, sizeof(response), stdin) == NULL) {
        *replay = false;
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
            *replay =  false;
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
        *replay = true;
        return INPUT_OK;
    }

    *replay = false;
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
