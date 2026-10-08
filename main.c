#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char read_answer(void) {
    printf("Your answer (input a, b, c or d)? ");
	char answer[3] = {0};
	if (fgets(answer, sizeof(answer), stdin) == NULL) {
	    puts("\nError occured when reading input\n");
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

int main(void) {
	puts("Welcome to Terivia!");

	puts("\nAnswer as many questions correct to get points.");

	puts("");

	puts("1) What is the capital of Sweden?");
	puts("a) Helsinki");
	puts("b) Oslo");
	puts("c) Stockholm");
	puts("d) Medellin");

	puts("");

	if (read_answer() == 'c') {
	    puts("Correct!");
	} else {
	    puts("Not quite correct. Better luck, next time.");
	}

	return 0;
}
