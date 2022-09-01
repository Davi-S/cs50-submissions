import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, fprint, check_missing_form_data, db_buy, db_sell

# Configure application
app = Flask(__name__)


@app.route('/blogs')
def blog():
    app.logger.info('Info level log')
    app.logger.warning('Warning level log')
    return f"Welcome to the Blog"


app.run(host='localhost', debug=True)


# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    # get database values and add more information
    wallet = db.execute('SELECT symbol, shares FROM stock_wallet WHERE user_id = ?', user_id)

    total_price = 0
    for stock in wallet:
        stock['price_fs'] = lookup(stock['symbol'])['price']
        stock['price_ts'] = stock['price_fs'] * stock['shares']
        total_price += stock['price_ts']

    cash_available = db.execute('SELECT cash FROM users WHERE id IS ?', user_id)[0]['cash']

    # return template with values
    return render_template('index.html', wallet=wallet, total_price=total_price, cash_available=cash_available, total_balance=total_price + cash_available)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == 'GET':
        return render_template('buy.html')

    # requested by POST (via form)
    else:
        user_id = session["user_id"]
        # check if all values were input
        buy_data = dict(request.form)
        if missing := check_missing_form_data(buy_data):
            return apology(missing)

        # check symbol
        if not lookup(buy_data['symbol']):
            return apology('This stock does not exist')

        # check shares
        try:
            buy_data['shares'] = int(buy_data['shares'])
            if buy_data['shares'] <= 0 or buy_data['shares'] % 1 != 0:
                raise ValueError
        except ValueError:
            return apology('Shares amount must be a integer greater than zero')

        # change database buy data
        buy_order = db_buy(db, user_id, buy_data['symbol'], buy_data['shares'])
        if not buy_order[0]:
            return apology(buy_order[1])

        # final
        flash("Successfully bought!")
        return redirect("/buy")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session['user_id']
    transactions = db.execute(
        'SELECT action, symbol, shares, price, date FROM transactions WHERE user_id = ? ORDER BY date DESC', user_id)
    # add aditiona information
    for data in transactions:
        data['price_ts'] = data['price'] * data['shares']
    return render_template('history.html', transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":

        # if requested with no symbol parameter
        if not (stock_symbol := request.args.get('symbol')):
            return render_template('quote.html')

        # verify and get stock info
        if not (stock_info := lookup(stock_symbol)):
            return apology("This stock does not exist")

        # render template with info
        return render_template('quote.html', name=stock_info['name'], price=usd(stock_info['price']), symbol=stock_info['symbol'])

    # if requested via "POST" (by form)
    else:
        # check input
        quote_symbol = request.form.get('quote-symbol').strip().upper()
        if missing := check_missing_form_data({'quote_symbol': quote_symbol}):
            return apology(missing)

        # redirect to same page with url parameters
        return redirect(url_for('quote', symbol=quote_symbol))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template('register.html')

    # if requested via "POST" (by form)
    else:
        user_data = dict(request.form)
        if missing := check_missing_form_data(user_data):
            return apology(missing)

        # check username and password
        if db.execute('SELECT username FROM users WHERE username = ?', user_data['username']):
            return apology('Username already taken')
        if user_data['password'] != user_data['password-confirmation']:
            return apology('Password and password confimation must be the same')

        # insert values on database
        new_user_id = db.execute('INSERT INTO users (username, hash) VALUES (?, ?)',
                                 user_data['username'], generate_password_hash(user_data['password']))

        # log user in
        session["user_id"] = new_user_id
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "GET":
        own_stocks = db.execute('SELECT symbol FROM stock_wallet WHERE user_id = ?', user_id)
        return render_template('sell.html', own_stocks=own_stocks)

    # if requested via "POST" (by form)
    else:
        sell_data = dict(request.form)
        if missing := check_missing_form_data(sell_data):
            return apology(missing)

        # check symbol
        if not lookup(sell_data['symbol']):
            return apology('This stock does not exist')

        # check shares
        try:
            sell_data['shares'] = int(sell_data['shares'])
            if sell_data['shares'] <= 0 or sell_data['shares'] % 1 != 0:
                raise ValueError
        except ValueError:
            return apology('Shares amount must be a integer greater than zero')

        # change database sell data
        sell_order = db_sell(db, user_id, sell_data['symbol'], sell_data['shares'])
        if not sell_order[0]:
            return apology(sell_order[1])

        # final
        flash("Successfully sold!")
        return redirect("/sell")


# personal touch for:
# Allow users to change their passwords.
# Allow users to add additional cash to their account.
@app.route("/account")
@login_required
def account():
    """Show account info and options"""
    if request.args.get('success'):
        flash("Success")
    return render_template('account.html')


@app.route("/add_cash", methods=['POST'])
@login_required
def add_cash():
    """Add chash to user account"""
    user_id = session['user_id']

    cash_data = dict(request.form)
    if missing := check_missing_form_data(cash_data):
        return apology(missing)

    user_cash = db.execute('SELECT cash FROM users WHERE id = ?', user_id)[0]['cash']
    new_user_cash = user_cash + int(cash_data['cash_amount'])
    db.execute('UPDATE users SET cash = ? WHERE id = ?', new_user_cash, user_id)
    return redirect(url_for('account', success='true'))


@app.route("/change_password", methods=['POST'])
@login_required
def change_password():
    """Change_user_password"""
    user_id = session['user_id']

    password_data = dict(request.form)
    if missing := check_missing_form_data(password_data):
        return apology(missing)

    # Ensure old passwords match
    user_password = db.execute('SELECT hash FROM users WHERE id = ?', user_id)[0]['hash']
    if not check_password_hash(user_password, password_data['old_password']):
        return apology("Old password is not correct")

    # ensure new passwords match
    if not password_data['new_password'] == password_data['confirm_new_password']:
        return apology("New passwords must be the same")

    # change password hash
    try:
        db.execute('UPDATE users SET hash = ? WHERE id = ?', generate_password_hash(password_data['new_password']), user_id)
    except Exception as e:
        return apology(str(e))

    return redirect(url_for('account', success='true'))