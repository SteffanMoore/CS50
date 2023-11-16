import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Reads database file into variable
    matches = []
    with open(sys.argv[1], "r") as database_file:
        database = csv.DictReader(database_file)
        STR = database.fieldnames
        for i in database:
            matches.append(i)

    # Reads DNA sequence file into a variable
    with open(sys.argv[2], "r") as sequence_file:
        sequence = csv.reader(sequence_file)
        for i in sequence:
            dna_sequence = i

    # Find longest match of each STR in DNA sequence
    STR_dict = {}

    for i in STR:
        match_len = longest_match(dna_sequence[0], i)
        STR_dict[i] = match_len

    # Check database for matching profiles
    for i in matches:
        match_check = True

        # Iterates through STR's to see if all match
        for j in STR:
            try:
                if int(i[j]) != int(STR_dict[j]):
                    match_check = False
                    break
            except ValueError:
                pass

        # Only returns a name if all STR's are matched
        if match_check == True:
            print(i["name"])
            return

    print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
