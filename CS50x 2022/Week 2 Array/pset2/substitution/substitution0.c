#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool validate_key(string key);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        if (validate_key(argv[1]) == false)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        string text = get_string("plaintext:  ");
        string key = argv[1];
        const int m = strlen(text);
        int cypher[m], code[26];
        // Calculate cypher code, can be used to cypher the text
        for (int i = 0, j = 65; i < 26; i++, j++)
        {
            code[i] = toupper(key[i]) - j;
        }
        // Cypher the text
        for (int k = 0; k < 26; k++)
        {
            for (int l = 0; l < m; l++)
            {
                // If the character is not an alphabet, leave it unchange
                if (isalpha(text[l]) == 0)
                {
                    cypher[l] = text[l];
                }
                // Convert the alphabet to corresponding character
                else if (toupper(text[l]) - k == 65)
                {
                    cypher[l] = text[l] + code[k];
                }
            }
        }
        printf("ciphertext: ");
        // Print cypher text one by one
        for (int o = 0; o < m; o++)
        {
            printf("%c", cypher[o]);
        }
        printf("\n");
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
