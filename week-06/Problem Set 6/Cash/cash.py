QUARTERS = 0.25
DIMES = 0.10
NICKELS = 0.05
PENNIES = 0.01


def main():
    change: float = get_valid_float("Change owed: U$")

    quarters = calculate_change(change, QUARTERS)
    change = round(change - quarters * QUARTERS, 2)

    dimes = calculate_change(change, DIMES)
    change = round(change - dimes * DIMES, 2)

    nickels = calculate_change(change, NICKELS)
    change = round(change - nickels * NICKELS, 2)

    pennies = calculate_change(change, PENNIES)
    change = round(change - pennies * PENNIES, 2)

    total_coins = quarters + dimes + nickels + pennies  # calculate all coins
    print(f"{total_coins}")


def get_valid_float(prompt: str):
    while True:  # loop until get a valid input
        try:
            user_input = float(input(prompt))
        except Exception as e:
            continue
        if user_input >= 0:
            return user_input


def calculate_change(change: float, coin_value: float):
    quantity = 0
    while change >= coin_value:
        change = round(change - coin_value, 2)  # round to 2 decimal points
        quantity += 1
    return quantity


if __name__ == "__main__":
    main()