#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Get input text from user
    string text = get_string("Text: ");
    // Count letters from text
    int letters = count_letters(text);
    // Count words from text
    int words = count_words(text);
    // Count setences from text
    int sentences = count_sentences(text);
    // Compute Coleman-Liau index
    int index = round(0.0588 * 100.0 * letters / words - 0.296 * 100.0 * sentences / words - 15.8);
    // print grade
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Define function for counting letters
int count_letters(string text)
{
    int l = 0, i = 0;
    while (text[i])
    {
        if (isalpha(text[i]) != 0)
        {
            l++;
        }
        i++;
    }
    return l;
}

// Define function for counting words
int count_words(string text)
{
    int w = 1, j = 0;
    while (text[j])
    {
        if (isspace(text[j]) != 0)
        {
            w++;
        }
        j++;
    }
    return w;
}

// Define function for counting sentences
int count_sentences(string text)
{
    int s = 0, k = 0;
    while (text[k])
    {
        if (text[k] == 33 || text[k] == 46  || text[k] == 63)
        {
            s++;
        }
        k++;
    }
    return s;
}