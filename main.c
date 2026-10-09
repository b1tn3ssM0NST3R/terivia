#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct question {
    const char *text;
    const char *choices[4];
    char correct_answer;
} Question;

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
    srand(time(NULL));

    int score = 0;
	puts("Welcome to Terivia!");

	puts("\nAnswer as many questions correct to get points.");

	puts("");

	Question questions[] = {
	    {
	        "What is the capital of Sweden?",
			{"Helsinki", "Oslo", "Stockholm", "Medellin"},
			'c'
		},
		{
		    "When did World War I begin?",
			{"1901", "1914", "1939", "1857"},
			'b'
		},
		{
		    "Who became Pope in May 2025?",
			{"Pope Leo XIV", "Pope John Paul II", "Pope Francis", "Pope Benedict"},
			'a'
		},
		{
		    "Who won the 2026 FIFA World Cup?",
			{"Argentina", "England", "Spain", "France"},
			'c'
		},
		{
		    "Which of these is not a branch of mathematics?",
			{"Lie Algebra", "Topology", "Menace Theory", "Knot Theory"},
			'c'
		},
		{
		    "The first country to gain independence in sub-Saharan Africa is ___.",
			{"Sudan", "Ghana", "Nigeria", "Kenya"},
			'b'
		},
		{
		    "Who is regarded as the first computer programmer?",
			{"Grace Hopper", "Charles Babbage", "Linus Torvalds", "Ada Lovelace"},
			'd'
		},
		{
		    "What is second highest mountain in the world?",
			{"K2", "Mount Kilimanjaro", "Mount Everest", "Mount Kenya"},
			'a'
		}
	};

	size_t question_count = sizeof(questions) / sizeof(questions[0]);

	for (size_t i = question_count; i > 1; i--) {
	    size_t j = (size_t)rand() % i;
		Question temp = questions[j];
		questions[j] = questions[i-1];
		questions[i-1] = temp;
	}

	for (size_t i = 0; i < question_count; i++) {
	    printf("%zu) %s\n", i+1, questions[i].text);
		for (size_t j = 0; j <= 3; j++) {
		    printf("%c) %s\n", 'a' + (int)j, questions[i].choices[j]);
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

	return 0;
}
