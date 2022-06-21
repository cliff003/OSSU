# Main drawing "#"
def main():
    n = get_height()
    for i in range(n):
        print(" " * (n - i - 1) + "#" * (i + 1) + "  " + "#" * (i + 1))


# Get height and avoid invalid value
def get_height():
    while True:
        try:
            n = int(input("Height: "))
        except ValueError:
            print("That's not an integer!")
        else:
            if n > 0 and n < 9:
                return n


# Run main
main()
