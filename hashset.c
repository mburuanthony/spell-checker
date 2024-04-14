#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashset.h"

hashset set_init()
{
  hashset h = {(node **)malloc(BINS * sizeof(node *)), 0, BINS};
  for (int i = 0; i < BINS; i++)
    h.table[i] = NULL;
  return h;
}

static int hash(char *key, int bins)
{
  unsigned hashval = 0;
  for (int i = 0; i < (int)strlen(key); i++)
    hashval = 31 * hashval + tolower(key[i]);
  return hashval % bins;
}

static void rehash(hashset *);

int insert(hashset *h, char *val)
{
  if (search(*h, val))
    return 0;
  int index = hash(val, h->bins);
  if (h->size >= h->bins)
    rehash(h);
  node *new_element = (node *)malloc(sizeof(node));
  new_element->next = h->table[index];
  new_element->value = strdup(val);
  h->table[index] = new_element;
  h->size++;
  return 1;
}

void print_all(hashset h)
{
  for (int i = 0; i < h.bins; i++)
    for (node *it = h.table[i]; it; it = it->next)
      printf("%s ", it->value), fflush(stdout);
}

int eliminate(hashset *h, char *val)
{
  if (!search(*h, val))
    return 0;
  int index = hash(val, h->bins);
  node *prev = NULL, *it;
  for (it = h->table[index]; it->next; it = it->next)
    if (!strcasecmp(it->value, val))
      break;
    else
      prev = it;
  if (prev)
    prev->next = it->next;
  else
    h->table[index] = it->next;
  free(it);
  h->size--;
  return 1;
}

static int next_prime(int min)
{
  while (1)
  {
    int prime = 1;
    for (int div = 2; div <= sqrt(min); div++)
      if (min % div == 0)
      {
        prime = 0;
        break;
      }
    if (prime)
      return min;
    else
      min++;
  }
  return min;
}

static void rehash(hashset *h)
{
  node **oldtable = h->table;
  int old_bins = h->bins;
  h->table = (node **)malloc(next_prime(2 * h->bins) * sizeof(node *));
  h->size = 0;
  h->bins = next_prime(2 * h->bins);
  for (int i = 0; i < old_bins; i++)
    for (node *it = oldtable[i]; it; it = it->next)
      insert(h, it->value);
}

int search(hashset h, char *val)
{
  int index = hash(val, h.bins);
  for (node *iterator = h.table[index]; iterator; iterator = iterator->next)
    if (!strcasecmp(iterator->value, val))
      return 1;
  return 0;
}
