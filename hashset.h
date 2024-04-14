#ifndef HASHSET_H
#define HASHSET_H

#define BINS 101

typedef struct node
{
    struct node *next;
    char *value;
} node;

typedef struct
{
    node **table;
    int size;
    int bins;
} hashset;

hashset set_init();
int insert(hashset *h, char *val);
void print_all(hashset h);
int eliminate(hashset *h, char *val);
int search(hashset h, char *val);

#endif
