#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "spellcheck.h"
#include "hashset.h"

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

void spell_check(const char *input_text, hashset *dictionary, int suggest_replacement, int suggest_swap, int suggest_erase)
{
    int line_number = 0;
    const char *ptr = input_text;

    while (*ptr != '\0')
    {
        int column_number = 1;

        while (*ptr != '\n' && *ptr != '\0')
        {
            while (isspace(*ptr))
            {
                column_number++;
                ptr++;
            }

            const char *start = ptr;
            while (isalpha(*ptr))
            {
                ptr++;
            }
            const char *end = ptr;

            char *cleaned_word = malloc((end - start + 1) * sizeof(char));
            if (cleaned_word == NULL)
            {
                printf("Memory allocation failed\n");
                return;
            }
            strncpy(cleaned_word, start, end - start);
            cleaned_word[end - start] = '\0';

            for (char *ch = cleaned_word; *ch != '\0'; ch++)
            {
                *ch = tolower(*ch);
            }

            if (!search(*dictionary, cleaned_word))
            {
                if (suggest_replacement)
                {
                    _suggest_replacement(dictionary, cleaned_word, line_number, column_number);
                }
                if (suggest_swap)
                {
                    _suggest_swap(dictionary, cleaned_word, line_number, column_number);
                }
                if (suggest_erase)
                {
                    _suggest_erase(dictionary, cleaned_word, line_number, column_number);
                }
            }

            column_number += end - start;

            free(cleaned_word);
        }

        if (*ptr != '\0')
        {
            line_number++;
            ptr++;
        }
    }
}
