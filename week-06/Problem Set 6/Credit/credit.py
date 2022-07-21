def main():
    # all checks are made with strings
    input_cc_number = input("Credit card number: ")

    # return after any mach, given that there cant be more tham one mach per card
    if luhn_algorithm(input_cc_number):
        if check_AMEX(input_cc_number):
            print("AMEX")
            return

        elif check_MASTERCARD(input_cc_number):
            print("MASTERCARD")
            return

        elif check_VISA(input_cc_number):
            print("VISA")
            return

    print("INVALID")  # if none of the maches are true
    return


def luhn_algorithm(cc_number: str):
    even_digit_product = ""
    odd_digit_product_sum = 0
    for idx, itb in enumerate(cc_number[-1::-1]):
        if int(idx) % 2 != 0:  # Filter even and odd indexes
            even_digit_product += str(int(itb)*2)
            continue
        odd_digit_product_sum += int(itb)

    even_digit_product_sum = 0
    for itb in even_digit_product:
        even_digit_product_sum += int(itb)

    if (even_digit_product_sum + odd_digit_product_sum) % 10 == 0:  # last digit of final sum
        return True
    return False


def check_AMEX(cc_number: str):
    if len(cc_number) == 15 and int(cc_number[0:2]) in [34, 37]:
        return True
    return False


def check_MASTERCARD(cc_number: str):
    if len(cc_number) == 16 and int(cc_number[0:2]) in range(50, 57):  # 57 not include
        return True
    return False


def check_VISA(cc_number: str):
    if (len(cc_number) == 13 or len(cc_number) == 16) and int(cc_number[0]) == 4:
        return True
    return False


if __name__ == "__main__":
    main()