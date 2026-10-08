#include <stdio.h>
#include <string.h>

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

	printf("Your answer (input a, b, c or d)? ");
	char answer[128] = {0};
	if (scanf("%127s", answer) != 1) {
	    puts("Error occured on scanf\n");
		return -1;
	}

	while (strlen(answer) != 1 || (strlen(answer) == 1 && (answer[0] < 'a' || answer[0] > 'd'))) {
	    puts("Answer should be a, b, c or d. You typed something else.");
	    printf("\nYour answer (input a, b, c or d)? ");
		if (scanf("%127s", answer) == 1) {
		    puts("Error occured on scanf\n");
			return -1;
		};
	}

	if (answer[0] == 'c') {
	    puts("Correct!");
	} else {
	    puts("Not quite correct. Better luck, next time.");
	}

	return 0;
}
