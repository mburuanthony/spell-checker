#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spellcheck.h"
#include "hashset.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage: %s -d dictionary file [-r] [-s] [-e]\n", argv[0]);
        return 1;
    }

    const char *dictionary_file = NULL;
    int suggest_replacement = 0;
    int suggest_swap = 0;
    int suggest_erase = 0;

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
                printf("missing argument for -d\n");
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

    if (dictionary_file == NULL)
    {
        printf("please provide a dictionary file using -d\n");
        return 1;
    }

    hashset dictionary = set_init();
    FILE *dictionary_fp = fopen(dictionary_file, "r");
    if (dictionary_fp == NULL)
    {
        printf("error opening dictionary file\n");
        return 1;
    }
    char word[MAX_WORD_LEN];
    while (fscanf(dictionary_fp, "%s", word) != EOF)
    {
        insert(&dictionary, word);
    }
    fclose(dictionary_fp);

    FILE *input = stdin;
    spell_check(input, &dictionary, suggest_replacement, suggest_swap, suggest_erase);

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
