#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spellcheck.h"
#include "hashset.h"

int main(int argc, char *argv[])
{
    const char *dictionary_file = NULL;
    int suggest_replacement = 0;
    int suggest_swap = 0;
    int suggest_erase = 0;

    if (argc == 1)
    {
        printf("Enter The Dictionary File: ");
        char file_input[MAX_WORD_LEN];
        fgets(file_input, MAX_WORD_LEN, stdin);
        strtok(file_input, "\n");
        dictionary_file = file_input;

        printf("Enable Suggestions For Replacement? (1/0): ");
        scanf("%d", &suggest_replacement);

        printf("Enable Suggestions For Swap? (1/0): ");
        scanf("%d", &suggest_swap);

        printf("Enable Suggestions For Erase? (1/0): ");
        scanf("%d", &suggest_erase);
    }
    else
    {
        if (argc < 3)
        {
            printf("Usage: %s -d dictionary file [-r] [-s] [-e]\n", argv[0]);
            return 1;
        }

        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-d") == 0)
            {
                if (i + 1 < argc)
                {
                    dictionary_file = argv[i + 1];
                }
                else
                {
                    printf("Missing Argument For -d\n");
                    return 1;
                }
            }
            else if (strcmp(argv[i], "-r") == 0)
            {
                suggest_replacement = 1;
            }
            else if (strcmp(argv[i], "-s") == 0)
            {
                suggest_swap = 1;
            }
            else if (strcmp(argv[i], "-e") == 0)
            {
                suggest_erase = 1;
            }
        }
    }

    if (dictionary_file == NULL)
    {
        printf("Please Provide A Dictionary File Using -d\n");
        return 1;
    }

    hashset dictionary = set_init();
    FILE *dictionary_fp = fopen(dictionary_file, "r");
    if (dictionary_fp == NULL)
    {
        printf("Error Opening Dictionary file\n");
        return 1;
    }

    char word[MAX_WORD_LEN];

    while (fscanf(dictionary_fp, "%s", word) != EOF)
    {
        insert(&dictionary, word);
    }
    fclose(dictionary_fp);

    char *input_text = malloc(MAX_LINE_LEN * sizeof(char));

    if (input_text == NULL)
    {
        printf("Memory Allocation Failed\n");
        return 1;
    }
    input_text[0] = '\0';

    char buffer[MAX_WORD_LEN];

    printf("Enter text to spell check (end: Ctrl+D):\n");

    while (fgets(buffer, MAX_WORD_LEN, stdin) != NULL)
    {
        buffer[MAX_WORD_LEN - 1] = '\0';
        strcat(input_text, buffer);
    }

    spell_check(input_text, &dictionary, suggest_replacement, suggest_swap, suggest_erase);

    free(input_text);

    for (int i = 0; i < dictionary.bins; i++)
    {
        node *current = dictionary.table[i];
        while (current != NULL)
        {
            node *temp = current;
            current = current->next;
            free(temp->value);
            free(temp);
        }
    }

    free(dictionary.table);

    return 0;
}
