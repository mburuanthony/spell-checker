#ifndef SPELLCHECK_H
#define SPELLCHECK_H

#include <stdio.h>
#include "hashset.h"

#define MAX_WORD_LEN 59
#define MAX_LINE_LEN 1000
#define SUGGEST_BUFFER_SIZE 1000

void _suggest_replacement(hashset *dictionary, const char *misspelled_word, int line_number, int column_number);
void _suggest_swap(hashset *dictionary, const char *misspelled_word, int line_number, int column_number);
void _suggest_erase(hashset *dictionary, const char *misspelled_word, int line_number, int column_number);

void spell_check(FILE *input, hashset *dictionary, int suggest_replacement, int suggest_swap, int suggest_erase);

#endif
