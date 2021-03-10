#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_operations.h"

#define MAX_LINE_LENGTH 2000

// a-z sau A-Z
int is_letter(char c) {
	if (c >= 'a' && c <= 'z')
		return 1;
	else if (c >= 'A' && c <= 'Z')
		return 2;
	return 0;
}

// 0-9
int is_digit(char c) {
	return (c >= '0' && c <= '9') ? 1 : 0;
}

// Daca nu e parte din cuvant
// (Un cuvant = ([a-z] | [A-Z] | [0-9])* )
int is_space(char c) {
	return (is_digit(c) || is_letter(c)) ? 0 : 1;
}

// Orice litera, mai putin a, e, i, o, u
int is_consonant(char c) {
	if (!is_letter(c))
		return 0;
	else if (c == 'A' || c == 'a')
		return 0;
	else if (c == 'E' || c == 'e')
		return 0;
	else if (c == 'I' || c == 'i')
		return 0;
	else if (c == 'O' || c == 'o')
		return 0;
	else if (c == 'U' || c == 'u')
		return 0;
	return 1;
}

// a-z -> A-Z
char to_upper_case(char c) {
	if (c < 'a' || c > 'z')
		return c;
	return c - 'a' + 'A';
}

// A-Z -> a-z
char to_lower_case(char c) {
	if (c < 'A' || c > 'Z')
		return c;
	return c - 'A' + 'a';
}

void horror(char s[MAX_LINE_LENGTH]) {
	int n = strlen(s);

	char left[MAX_LINE_LENGTH], right[MAX_LINE_LENGTH];
	for (int i = 0; i < n; ++i) {
		
		if (!is_consonant(s[i]))
			continue;

		memcpy(left, s, i + 1);
		memcpy(right, s + i, n - i);

		memset(s, 0, MAX_LINE_LENGTH);
		right[0] = to_lower_case(right[0]);

		strcat(left, right);
		strcpy(s, left);

		memset(left, 0, MAX_LINE_LENGTH);
		memset(right, 0, MAX_LINE_LENGTH);

		++i; // trecem la urmatoarea litera
		++n; // Actualizarea lungimii sirului prelucrat
}
}

void comedy(char s[MAX_LINE_LENGTH]) {
	int position = 0;
	int j;
	for (int i = 0; i < strlen(s); ++i) {
		if (i == 0 || (is_space(s[i - 1]) && is_letter(s[i])) ) {
			position = 1; // Prima litera a cuvantului curent este la pozitia 1
			for (j = i; s[j] != ' ' && s[j] != '\n'; ++j)
				if (position++ % 2 == 0)
					s[j] = to_upper_case(s[j]);
			i = j; // check the following characters
		}
	}
}

void fantasy(char s[MAX_LINE_LENGTH]) {
	for (int i = 0; i < strlen(s); ++i)
		if ((i == 0 && is_letter(s[i])) // Daca primul caracter este litera
				// sau daca caracterul curent este inceputul unui cuvant
				|| s[i - 1] == ' ')
			s[i] = to_upper_case(s[i]);
}

void science_fiction(char s[MAX_LINE_LENGTH]) {
	int space_index;
	int space_index_changes;
	int start, mid, end;
	char c;

	// La inceputul liniei, avem space_index = 0
	space_index = 0;
	space_index_changes = 0;
	int i;

	for (i = 0; i < strlen(s); ++i) {
		// Se incepe un cuvant nou de la caracterul curent?
		if ((s[i] != ' ') && (i == 0 || s[i - 1] == ' ')) {
			space_index = i;
			++space_index_changes;
		}

		// In ce interval de indici trebuie sa inversam caracterele?
		if (space_index_changes == 7) {
			start = space_index;
			end = start + 1;
			while(s[end] != ' ' && s[end] != '\n' && s[end] != 0)
				++end;
			if (s[end] != '.' && s[end] != ',')
				--end;

			// Inversarea caracterelor cerute
			mid = start + (end - start) / 2;
			for (int j = start; j <= mid; ++j) {
				int j_rev = end - j + start;
				c = s[j];
				s[j] = s[j_rev];
				s[j_rev] = c;
			}

			// Cautarea urmatorului cuvant
			space_index_changes = 0;
			i = end;
		}
	}
}