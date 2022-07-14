// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned long long N = 456976; // pow(26, 4) -> aaaa, aaab ... zzzy, zzzz
// this is the max value of an expoent to fit in a unsigned int

// Hash table
node *table[N];

// Number of words in the dictionary
int count_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *cursor = table[hash(word)];
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int expoent = log(N) / log(26); // find the expoent used to find the N const
    int index = 0;
    int word_len = strlen(word);

    // multiply all char values (a = 0, b = 1, ...) by 26 but the last one, which is added
    for (int i = 0; i < expoent && i < word_len; i++)
    {
        int value = toupper(word[i]) - 'A';
        if (value == -26) // apostrophe
        {
            continue; // ignore
        }
        if (i == expoent - 1 || i == word_len - 1)
        {
            index += value;
            break;
        }
        index += 26 * value;
    }
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dict_file = fopen(dictionary, "r");

    // Read strings from file one at a time
    char word[LENGTH + 1];
    while (fscanf(dict_file, "%s", word) != EOF)
    {
        // Create a new node for each word
        node *n = malloc(sizeof(node));

        // Hash a word to obtain a hash value
        int hash_value = hash(word);

        // Insert node into a hash table at that location
        strcpy(n->word, word);
        n->next = table[hash_value]; // appending at the beginning of the hash table
        table[hash_value] = n;

        count_words++; // Updating the global number of words in the dictionary
    }
    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (count_words > 0)
    {
        return count_words;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate throgout the table
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}