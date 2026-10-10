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
### Multiple-choice questions
### Score tracking
### Results screen
### Randomized question order
### Loading questions from file
Questions can be loaded from file with this format
```
Question text
Possible answer 1
Possible answer 2
Possible answer 3
Possible answer 4
Correct answer
```
with no space between consecutive questions.
