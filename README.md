# Terivia
A terminal trivia app written in C.

## Requirements
GCC (the version I used to compile was 16.2.1) or any modern C compiler
Make

## Build
```sh
gcc -Wall -Wextra main.c question.c -o terivia
```
or
```sh
make
```

## Run
```sh
./terivia
```
To this to load default questions (`test_questions`) found in the sample questions directory.

or

```sh
./terivia questions_file
```
To load a questions file of your choice.

## Features
- Multiple-choice questions
- Score tracking
- Results screen
- Randomized question order
- Loading questions from file
- Replay without restarting program

## Questions File
- Each question must occupy six lines: question text, four possible answers and then the correct answer(either a, 
b, c or d).
```
What is the capital of Sweden?
Helsinki
Oslo
Stockholm
Medellin
c
```
- No blank lines between questions and no extra blank lines at the end.
- You can load as many questions as you want theoritically, but you are limited by the memory you have so don't go
too crazy.
- The question text and possible answers can contain at most 254 characters.
- Editing the file doesn't require recompiling.
- Sample question files can be found in the `sample_questions` directory.
