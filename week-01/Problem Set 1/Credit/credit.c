#include <cs50.h>
#include <stdio.h>
#include <math.h>
int main(void)
{
    long input_CC_number = get_long("Type the credit card number: ");

    // copy original number for loop
    // on the loop, the number will be modified, so is better to have a copy
    long testing_CC_number = input_CC_number;

    int sum_of_products_digits = 0, sum_without_product = 0, length = 0;
    for (int i = 0; testing_CC_number > 0; i++)
    {
        int last_digit = testing_CC_number % 10; // get the last digit of the number

        if (i % 2 != 0) // get only the odds index from the number
        {
            // add the sum of the digits of the product
            sum_of_products_digits += ((last_digit * 2) % 10) + ((last_digit * 2) / 10);
        }

        else // get all even index from number
        {
            sum_without_product += last_digit;
        }

        testing_CC_number = testing_CC_number / 10; // take of the last digit that was used to continue the loop
        length++; // add to the length of digits on the number
    }
    int sum = sum_of_products_digits + sum_without_product;
    int first_two_digits = input_CC_number / pow(10, length - 2);

    // print variables to debug
    printf("\n");
    printf("input_CC_number: %li\n", input_CC_number);
    printf("testing_CC_number: %li\n", testing_CC_number);
    printf("sum_of_products_digits: %i\n", sum_of_products_digits);
    printf("sum_without_product: %i\n", sum_without_product);
    printf("sum: %i\n", sum);
    printf("length: %i\n", length);
    printf("first_two_digits: %i\n", first_two_digits);
    printf("sum %% 10: %i\n", sum % 10);
    printf("first_two_digits / 10: %i\n", first_two_digits % 10);
    printf("\n");

    // final checks
    if (sum % 10 == 0) // valid card
    {
        if (length == 15 && (first_two_digits == 34 || first_two_digits == 37))
        {
            printf("AMEX\n");
        }

        else if (length == 16 && (50 < first_two_digits && first_two_digits < 56))
        {
            printf("MASTERCARD\n");
        }

        else if ((length == 13 || length == 16) && first_two_digits / 10 == 4)
        {
            printf("VISA\n");
        }

        else // valid card but no flag check
        {
            printf("INVALID\n");
        }
    }

    else // not valid card
    {
        printf("INVALID\n");
    }


}