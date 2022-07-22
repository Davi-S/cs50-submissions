import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Wrong usage!")
        quit()

    # TODO: Read database file into a variable
    database = []
    with open(sys.argv[1], 'r') as f:
        f = csv.DictReader(f)
        for row in f:  # Read every row from file. All values come in string format
            for item in row.items():  # Convert string values into integers if possible (name field can't be conveted)
                try:
                    row[item[0]] = int(item[1])
                except ValueError as e:
                    continue
            database.append(row)  # append row into list

    # TODO: Read DNA sequence file into a variable
    dna_sequence = ""
    with open(sys.argv[2], 'r') as f:
        dna_sequence = f.read()

    # TODO: Find longest match of each STR in DNA sequence
    # Create a dict with all the STR names
    # Those STR names can be found in the database dict keys (among the person name)
    tandems = dict.fromkeys(database[0].keys())
    del tandems['name']  # delete the name field from the dict

    # Fill the dict with values
    for key in tandems:
        tandems[key] = longest_match(dna_sequence, key)

    # TODO: Check database for matching profiles
    tandems_len = len(tandems)
    for person in database:
        for idx, item in enumerate(person.items()):
            try:
                # Check for STR mismatch. If does not mach, break the loop and go to the next person
                if (item[1] != tandems[item[0]]):
                    break
            except KeyError as e:  # this happens when trying to match the name field
                continue  # continue to the next field

            if idx == tandems_len:  # last item in the loop
                # If there is no mismatch, print the person name and quit the program
                print(person['name'])
                quit()

    # If there is mismatches in all persons
    print('No match')
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
