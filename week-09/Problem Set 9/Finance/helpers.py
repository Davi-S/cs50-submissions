import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

#
# functions made myself
#


def check_missing_form_data(form_data: dict, message: str = None):
    """Generate a error message for the fist input of a form that is missing."""
    def _missing_form_data(_form_data: dict):
        for key, value in _form_data.items():
            if value == '':
                return key
        return False

    if not (missing := _missing_form_data(form_data)):  # if there is no missing input, return None
        return None

    # Try to format the custom message. If there is any error while formating, send the defalt one
    defalt_message = 'Must provide {}'
    try:
        formated_message = message.format(missing)
    except Exception:
        return defalt_message.format(missing)
    return formated_message


def db_buy(database, user_id, symbol, shares):
    # get info to check order
    stock_info = lookup(symbol)
    total_price = stock_info['price'] * shares
    user_cash = database.execute('SELECT cash FROM users WHERE id = ?', user_id)[0]['cash']

    if total_price > user_cash:
        return False, 'You dont have enough cash to do this transaction'

    # database changes
    try:
        database.execute('UPDATE users SET cash = ? WHERE id = ?', user_cash - total_price, user_id)
        database.execute('INSERT INTO transactions (user_id, action, symbol, shares, price) VALUES (?, ?, ?, ?, ?)',
                         user_id, "buy", symbol, shares, stock_info['price'])
        # check if user already has one of this stock in his wallet
        wallet_info = database.execute('SELECT shares FROM stock_wallet WHERE symbol = ? AND user_id = ?', symbol, user_id)
        if wallet_info:  # if has stock: update
            database.execute('UPDATE stock_wallet SET shares = ? WHERE symbol = ? AND user_id = ?',
                             wallet_info[0]['shares'] + shares, symbol, user_id)
        else:  # if do not has stock: create
            database.execute('INSERT INTO stock_wallet (user_id, symbol, shares) VALUES (?, ?, ?)', user_id, symbol, shares)
    except Exception as e:
        return False, str(e)

    return True, ''


def db_sell(database, user_id, symbol, shares):
    # check symbol
    try:
        wallet_info = database.execute('SELECT symbol, shares FROM stock_wallet WHERE user_id = ? AND symbol = ?', user_id, symbol)
    except Exception as e:
        return False, e

    if not wallet_info:
        return False, 'You don\'t have this stock'

    if wallet_info[0]['shares'] < shares:
        return False, 'You don\'t have this many shares to sell'

    # check price and cash
    stock_info = lookup(symbol)
    total_price = stock_info['price'] * shares
    user_cash = database.execute('SELECT cash FROM users WHERE id = ?', user_id)[0]['cash']

    # database changes
    try:
        database.execute('UPDATE users SET cash = ? WHERE id = ?', user_cash - total_price, user_id)
        database.execute('INSERT INTO transactions (user_id, action, symbol, shares, price) VALUES (?, ?, ?, ?, ?)',
                         user_id, "sell", symbol, shares, stock_info['price'])
        database.execute('UPDATE stock_wallet SET shares = ? WHERE symbol = ? AND user_id = ?',
                         wallet_info[0]['shares'] - shares, symbol, user_id)
        # delete if shares is 0
        database.execute('DELETE FROM stock_wallet WHERE shares = 0')
    except Exception as e:
        return False, e
    return True, ''


def fprint(data):
    with open('log.txt', 'a') as f:
        f.write(f'{str(data)}\n')