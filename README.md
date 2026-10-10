# Terivia
A terminal trivia app written in C.

## Requirements
GCC (the version I used to compile was 16.2.1) or any modern C compiler

## Build
```sh
gcc -Wall -Wextra -o terivia main.c
```

## Run
```sh
./terivia
```

## Features
- Multiple-choice questions
- Score tracking
- Results screen
- Randomized question order
- Loading questions from file

## Question File
- The file should be named `questions` and must be in the directory where you run the program. If you want to use a
different name and want it to be in a different directory, you can edit it in the code.
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
- A maximum of 100 questions supported for now.
- The question text and possible answers can contain at most 254 characters.
- Editing the file doesn't require recompiling.
