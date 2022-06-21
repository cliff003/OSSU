#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Function prototypes
bool validate_key(string key);
void cypher(string text, string key);

int main(int argc, string argv[])
{
    // Check only one single command-line argument is used
    if (argc == 2)
    {
        if (!validate_key(argv[1]))
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        // Prompt user for input
        string text = get_string("plaintext:  ");

        // Use cypher function to get cyphertext
        cypher(text, argv[1]);

        // Print the output
        printf("ciphertext: %s\n", text);
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

void cypher(string text, string key)
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

    // Terminate the string
    c[m] = '\0';

    // Substitue original text
    strcpy(text, c);

    return;
}