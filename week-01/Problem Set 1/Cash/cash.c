#include <cs50.h>
#include <stdio.h>

int get_cents(void);

// all the "calculate" functions work on the same following pseudo code:

// take away the coin value from cents
// add one to counter
// repeat until it is no longer possible to remove the value
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    int cents;
    // loop to get only positive inputs
    do
    {
        cents = get_int("How many cents? ");
    }
    while (cents < 0);
    return cents;
}

int calculate_quarters(int cents)
{
    int quantity = 0;

    while (cents >= 25)
    {
        cents -= 25;
        quantity++;
    }

    return quantity;
}

int calculate_dimes(int cents)
{
    int quantity = 0;

    while (cents >= 10)
    {
        cents -= 10;
        quantity++;
    }

    return quantity;
}

int calculate_nickels(int cents)
{
    int quantity = 0;

    while (cents >= 5)
    {
        cents -= 5;
        quantity++;
    }

    return quantity;
}

int calculate_pennies(int cents)
{
    int quantity = 0;

    while (cents >= 1)
    {
        cents -= 1;
        quantity++;
    }

    return quantity;
}
