#ifndef TERIVIA_INPUT_H
#define TERIVIA_INPUT_H

#include <stdbool.h>

typedef enum input_status {
    INPUT_OK,
    INPUT_END,
    INPUT_ERROR
} InputStatus;

InputStatus read_answer(char *input);
InputStatus ask_replay(bool *replay);

#endif
