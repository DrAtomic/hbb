all:
	gcc -g -fsanitize=address -Wall -Wextra -std=c99 -pedantic main.c -o main -lm
