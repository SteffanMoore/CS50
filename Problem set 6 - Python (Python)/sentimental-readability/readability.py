def main():
    text = input("Text: ")

    # Sets counts to zero
    letters = 0
    words = 1
    sentences = 0

    # Iterates through input text
    for i in text:
        if i in ["!", ".", "?"]:
            sentences += 1
        if i == " ":
            words += 1
        if i.isalpha():
            letters += 1

    # Finding the Coleman-Liau index
    L = (letters / words) * 100
    S = (sentences / words) * 100
    coleman_liau = round(0.0588 * L - 0.296 * S - 15.8)

    # Prints grade
    if coleman_liau >= 16:
        print("Grade 16+")
    elif coleman_liau < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {coleman_liau}")


main()
