import re


def main():
    while True:
        try:
            card_input = input("Number: ")
            card_number = int(card_input)
            break
        except ValueError:
            pass

    if luhn_algorithm(card_input) == False:
        print("INVALID")
        print("shit")
        return

    print(provider_check(card_input))


# Engages Luhn's algorithm to check for valid card
def luhn_algorithm(number):
    digits = []
    luhn_total = 0

    # Adds digits of calculated values to list as strings
    for i in range(len(number)):
        j = len(number) - (i + 1)

        if i % 2 != 0:
            digits.append(str(int(number[j]) * 2))
        else:
            digits.append(number[j])

    # Adds individual digits to total
    for i in digits:
        for j in i:
            luhn_total += int(j)

    # Checks whether algorithm is satisfied
    if luhn_total % 10 == 0:
        return True
    else:
        return False


# Verifies whether a number belongs to a provider using RegEx
def provider_check(card_input):
    # For Visa
    if (re.search("^4\d{15}$", card_input) != None) or (re.search("^4\d{12}$", card_input) != None):
        provider = "VISA"
        return provider
    # For American Express
    elif (re.search("^34\d{13}$", card_input) != None) or (re.search("^37\d{13}$", card_input) != None):
        provider = "AMEX"
        return provider
    # for Mastercard
    elif (re.search("^5[1-5]\d{14}$", card_input) != None) or (re.search("^2{3}\d{13}$", card_input) != None):
        provider = "MASTERCARD"
        return provider
    # If not a card
    else:
        provider = "INVALID"
        return provider


main()
