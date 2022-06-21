// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 52;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Compare the word with values in hash table
    for (node *tmp = table[hash(word)]; tmp != NULL; tmp = tmp->next)
    {

        // Use strcasecmp() for case-insensitive
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Use first 2 character to generate hash number
    int number = 0;
    if (toupper(word[1]) - 'A' >= 0)
    {
        number = toupper(word[0]) + toupper(word[1]) - 'A' - 'A';
    }
    else
    {
        number = toupper(word[0]) - 'A';
    }
    return number;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        return false;
    }

    // Check if there is an error
    if (ferror(input))
    {
        fclose(input);
        return false;
    }

    char dictionary_word[LENGTH + 1];

    // Load dictionary word
    while (fscanf(input, "%s", dictionary_word) != EOF)
    {
        // Assign a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(table[hash(dictionary_word)]);
            return false;
        }

        // Add data to the new node
        strcpy(n->word, dictionary_word);

        // If there is a node in hash table, insert it to the front
        if (table[hash(dictionary_word)] != NULL)
        {
            n->next = table[hash(dictionary_word)];
        }

        // If there is NOT a node in hash table, create a new node
        else
        {
            n->next = NULL;
        }

        table[hash(dictionary_word)] = n;
    }

    // Close dictionary
    fclose(input);
    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Initialize size
    int size = 0;

    // Loop thourgh all the bracket in the hash table
    for (int i = 0; i < N; i++)
    {
        // Add 1 to size if a node is found
        for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
        {
            size++;
        }
    }
    return size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop thourgh all the bracket in the hash table
    for (int i = 0; i < N; i++)
    {

        // Recursively free all the memory
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }

    }

    // Check if all values are freed
    int count = 0;
    for (int j = 0; j < N; j++)
    {
        if (table[j] == NULL)
        {
            count++;
        }
    }
    if (count == N)
    {
        return true;
    }
    return false;
}