#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

//Number of strings allocated by cypher.
static size_t allocations = 0;

//Array of strings allocated by cypher.
static string *strings = NULL;

bool validate_key(string key);
string cypher(string text, string key);

int main(int argc, string argv[])
{
    // Check only one single command-line argument is used
    if (argc == 2)
    {
        if (validate_key(argv[1]) == false)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        // Prompt user for input
        string plain_text = get_string("plaintext:  ");

        // Use cypher function to get cyphertext
        string cypher_text = cypher(plain_text, argv[1]);

        // Print the output
        printf("ciphertext: %s\n", cypher_text);
        return 0;
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}

// Validate Key
bool validate_key(string key)
{
    int n = strlen(key);
    // Check the length of the key
    if (n != 26)
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        // Check the key that it only contains alphabets
        if (isalpha(key[i]) == 0)
        {
            return false;
        }
        // Check the key that it only contains each letter once
        for (int j = i + 1; j < n; j++)
        {
            if (key[i] == key[j] || key[i] == key[j] + 32 || key[i] == key[j] - 32)
            {
                return false;
            }
        }
    }
    return true;
}

string cypher(string text, string key)
{
    const int m = strlen(text), n = 26;
    int code[n];
    char c[m];
    // Calculate cypher code, can be used to cypher the text
    for (int i = 0, j = 65; i < n; i++, j++)
    {
        code[i] = toupper(key[i]) - j;
    }
    // Cypher the text
    for (int k = 0; k < n; k++)
    {
        for (int l = 0; l < m; l++)
        {
            // If the character is not an alphabet, leave it unchange
            if (isalpha(text[l]) == 0)
            {
                c[l] = text[l];
            }
            // Convert the alphabet to corresponding character
            else if (toupper(text[l]) - k == 65)
            {
                c[l] = text[l] + code[k];
            }
        }
    }
    // Use some code from get_string function
    // Check whether we have room for another string
    if (allocations == SIZE_MAX / sizeof(string))
    {
        return NULL;
    }

    // Growable buffer for characters
    string buffer = NULL;

    // Capacity of buffer
    size_t capacity = 0;

    // Number of characters actually in buffer
    size_t size = 0;

    // Iteratively get characters
    int p = 0;
    while (c[p])
    {
        // Grow buffer if necessary
        if (size + 1 > capacity)
        {
            // Increment buffer's capacity if possible
            if (capacity < SIZE_MAX)
            {
                capacity++;
            }
            else
            {
                free(buffer);
                return NULL;
            }

            // Extend buffer's capacity
            string temp = realloc(buffer, capacity);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        // Append current character to buffer
        buffer[size++] = c[p++];
    }
    // Minimize buffer
    string s = realloc(buffer, size + 1);
    if (s == NULL)
    {
        free(buffer);
        return NULL;
    }
    // Terminate string
    s[size] = '\0';

    // Resize array so as to append string
    string *tmp = realloc(strings, sizeof(string) * (allocations + 1));
    if (tmp == NULL)
    {
        free(s);
        return NULL;
    }
    strings = tmp;

    // Append string to array
    strings[allocations] = s;
    allocations++;

    // Return string
    return s;
}