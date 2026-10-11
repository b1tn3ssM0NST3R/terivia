#ifndef TERIVIA_QUESTION_H
#define TERIVIA_QUESTION_H

#include <stddef.h>

typedef struct question {
    char text[256];
    char choices[4][256];
    char correct_answer;
} Question;

void shuffle_questions(Question questions[], size_t count);


Question *load_questions(const char *filename, size_t *count);

#endif
