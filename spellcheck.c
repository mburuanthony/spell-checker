#include "spellcheck.h"
#include "hashset.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void _suggest_replacement(hashset *dictionary, const char *misspelled_word, int line_number, int column_number)
{
    char suggestions[SUGGEST_BUFFER_SIZE] = "";
    for (int i = 0; i < (int)strlen(misspelled_word); i++)
    {
        for (char c = 'a'; c <= 'z'; c++)
        {
            if (c != misspelled_word[i])
            {
                char corrected_word[MAX_WORD_LEN];
                strcpy(corrected_word, misspelled_word);
                corrected_word[i] = c;
                if (search(*dictionary, corrected_word) && strcmp(corrected_word, misspelled_word) != 0)
                {
                    snprintf(suggestions + strlen(suggestions), SUGGEST_BUFFER_SIZE - strlen(suggestions), "%s%s", strlen(suggestions) > 0 ? "," : "", corrected_word);
                }
            }
        }
    }
    if (strlen(suggestions) > 0)
    {
        printf("%s,%d,%d,%s\n", misspelled_word, line_number, column_number, suggestions);
    }
}

void _suggest_swap(hashset *dictionary, const char *misspelled_word, int line_number, int column_number)
{
    char suggestions[SUGGEST_BUFFER_SIZE] = "";
    for (int i = 0; i < (int)strlen(misspelled_word) - 1; i++)
    {
        char corrected_word[MAX_WORD_LEN];
        strcpy(corrected_word, misspelled_word);
        char temp = corrected_word[i];
        corrected_word[i] = corrected_word[i + 1];
        corrected_word[i + 1] = temp;
        if (search(*dictionary, corrected_word) && strcmp(corrected_word, misspelled_word) != 0)
        {
            snprintf(suggestions + strlen(suggestions), SUGGEST_BUFFER_SIZE - strlen(suggestions), "%s%s", strlen(suggestions) > 0 ? "," : "", corrected_word);
        }
    }
    if (strlen(suggestions) > 0)
    {
        printf("%s,%d,%d,%s\n", misspelled_word, line_number, column_number, suggestions);
    }
}

void _suggest_erase(hashset *dictionary, const char *misspelled_word, int line_number, int column_number)
{
    char suggestions[SUGGEST_BUFFER_SIZE] = "";
    for (int i = 0; i < (int)strlen(misspelled_word); i++)
    {
        char corrected_word[MAX_WORD_LEN];
        strcpy(corrected_word, misspelled_word);
        memmove(corrected_word + i, corrected_word + i + 1, strlen(corrected_word) - i);
        if (search(*dictionary, corrected_word) && strcmp(corrected_word, misspelled_word) != 0)
        {
            snprintf(suggestions + strlen(suggestions), SUGGEST_BUFFER_SIZE - strlen(suggestions), "%s%s", strlen(suggestions) > 0 ? "," : "", corrected_word);
        }
    }
    if (strlen(suggestions) > 0)
    {
        printf("%s,%d,%d,%s\n", misspelled_word, line_number, column_number, suggestions);
    }
}

void spell_check(FILE *input, hashset *dictionary, int suggest_replacement, int suggest_swap, int suggest_erase)
{
    char line[MAX_LINE_LEN];
    int line_number = 0;

    while (fgets(line, MAX_LINE_LEN, input) != NULL)
    {
        line_number++;

        char *token = strtok(line, " \t\n");

        int column_number = 0;
        while (token != NULL)
        {
            column_number += strlen(token) + 1;

            // Remove punctuation
            char *cleaned_word = malloc(MAX_WORD_LEN * sizeof(char));
            int cleaned_index = 0;
            for (int i = 0; i < strlen(token); i++)
            {
                if (isalpha(token[i]))
                {
                    cleaned_word[cleaned_index++] = tolower(token[i]);
                }
            }
            cleaned_word[cleaned_index] = '\0';

            if (!search(*dictionary, cleaned_word))
            {
                // Handle misspelled word
                char suggestions[SUGGEST_BUFFER_SIZE] = "";

                // Suggest replacements if enabled
                if (suggest_replacement)
                {
                    _suggest_replacement(dictionary, cleaned_word, line_number, column_number);
                }

                // Suggest swaps if enabled
                if (suggest_swap)
                {
                    _suggest_swap(dictionary, cleaned_word, line_number, column_number);
                }

                // Suggest erasures if enabled
                if (suggest_erase)
                {
                    _suggest_erase(dictionary, cleaned_word, line_number, column_number);
                }
            }

            free(cleaned_word);

            token = strtok(NULL, " \t\n");
        }
    }
}
