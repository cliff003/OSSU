from cs50 import get_int

# Get input from user
number = get_int("Number: ")
digit = [int(d) for d in str(number)]
n = len(digit)
sum = 0

# Apply Luhn’s Algorithm
for i in range(n - 1, -1, -1):
    if (n - i) % 2 == 0:
        if digit[i] * 2 > 9:
            sum += digit[i] * 2 % 10 + 1
        else:
            sum += digit[i] * 2
    else:
        sum += digit[i]

# Validate and decide the type of credit card
if sum % 10 == 0:
    if digit[0] == 4 and n == 16 or n == 13:
        print("VISA")
    elif digit[0] == 3 and n == 15 and digit[1] == 4 or digit[1] == 7:
        print("AMEX")
    elif digit[0] == 5 and digit[1] > 0 and digit[1] < 6 and n == 16:
        print("MASTERCARD")
    else:
        print("INVALID")
else:
    print("INVALID")
