#include<cs50.h>
#include<stdio.h>
#include<math.h>

int main(void)
{
    long a, b, c12 = 0, e = 0, f = 0, n = 0;
    // Prompt user for credit card number.
    long c = get_long("Please input a credit card number:");
    long d = c;
    // Count number of digits.
    do
    {
        d /= 10;
        n++;
    }
    while (d != 0);
    // Validate credit card.
    for (long i = 1; i <= n; i += 2)
    {
        // Find the number of each digit. After the final loop, a will be one of the first two digits.
        a = (c % (long) pow(10, i) - c % (long) pow(10, i - 1)) / (long) pow(10, i - 1);
        // Add the digits together.
        e += a;
    }
    for (long j = 2; j <= n; j += 2)
    {
        // Find the product of every other digit after multiply by 2. And b/2 will be one of the first two digits.
        b = (c % (long) pow(10, j) - c % (long) pow(10, j - 1)) / (long) pow(10, j - 1) * 2;
        // Add the digits of products together. If the product > 9, it will never reach 20, so only need to add 1 if b > 9.
        if (b > 9)
        {
            f += b % 10 + 1;
        }
        else
        {
            f += b;
        }
    }
    // Validate the credit card using the sum.
    if ((e + f) % 10 == 0)
    {
        //Look for the first 2 digits of credit card.
        if (n % 2 == 0)
        {
            c12 = b / 2 * 10 + a;
        }
        else
        {
            c12 = b / 2 + (a * 10);
        }
        // Decide the type of credit card, and validate the number of digits.
        if ((c12 == 34 || c12 == 37) && n == 15)
        {
            printf("AMEX\n");
        }
        else if (c12 > 50 && c12 < 56 && n == 16)
        {
            printf("MASTERCARD\n");
        }
        else if (c12 > 39 && c12 < 50 && (n == 13 || n == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}