#include<cs50.h>
#include<stdio.h>

int main(void)
{
    // Get height from user input.
    int n;
    do
    {
        n = get_int("What's the height? (only 1 to 8)\n");
    }
    // Only allow a integer from 1 to 8.
    while (n < 1 || n > 8);
    // Start drawing.
    // First loop for change rows.
    for (int i = 0; i < n; i++)
    {
        // Second loop for drawing space before #.
        for (int j = n - i; j > 1; j--)
        {
            printf(" ");
        }
        // Third loop for drawing gap.
        for (int l = 0; l < 2; l++)
        {
            // Forth loop for drawing #.
            for (int k = n - i; k <= n; k++)
            {
                printf("#");
            }
            // Prevent extra gap after #.
            if (l < 1)
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}
