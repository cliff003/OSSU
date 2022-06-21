#include <stdio.h>
#include <cs50.h>

int main(void)
{
    /* Get name from user. */
    string answer = get_string("What's your name? ");
    /* Print Hello with user name. */
    printf("hello, %s\n", answer);
}