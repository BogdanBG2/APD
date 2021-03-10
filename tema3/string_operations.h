#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef STR_OPS
#define STR_OPS

#define MAX_LINE_LENGTH 2000

int is_letter(char c);
int is_digit(char c);
int is_space(char c);
int is_consonant(char c);

char to_upper_case(char c);
char to_lower_case(char c);

void horror(char s[MAX_LINE_LENGTH]);

void comedy(char s[MAX_LINE_LENGTH]);

void fantasy(char s[MAX_LINE_LENGTH]);

void science_fiction(char s[MAX_LINE_LENGTH]);

#endif