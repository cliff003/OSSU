from cs50 import get_string
import re

# Process text
text = get_string("Text: ")
words = len(text.split())
sentences = len(re.split(r'[.!?]+', text)) - 1
letters = len(re.sub(r'[^\w]', '', text))

# Calculate Coleman-Liau index
index = round(0.0588 * 100 * letters / words - 0.296 * 100 * sentences / words - 15.8)

# Determine the grade
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")