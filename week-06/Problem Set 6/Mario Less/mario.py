# TODO

def main():
    height = get_valid_int_input("Pyramid height: ")

    for i in range(1, height + 1):
        for j in range(height, i, -1):
            print(" ", end="")
        for k in range(i):
            print("#", end="")
        print()


def get_valid_int_input(prompt):
    while (True):  # loop to get valid input
        try:
            user_input = int(input(prompt))
        except Exception as e:  # exeption to get a valid int
            continue
        if 1 <= user_input <= 8:
            return user_input


if __name__ == "__main__":
    main()