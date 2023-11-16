def main():
    # Loops until int recieved
    while True:
        try:
            height = int(input("Height:"))
            if 0 < height < 9:
                break
        except ValueError:
            pass

    # Prints the pyramid
    for i in range(height):
        print(" " * (height - (i + 1)) + "#" * (i + 1) + "  " + "#" * (i + 1) + "\n".rstrip(), sep=" ")


main()