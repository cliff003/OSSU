import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, time_now, convert_time

# Configure application
app = Flask(__name__)

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
    # Create a dictionary
    total = {}
    total["total_value"] = 0
    total["current_value"] = 0

    # Find cash of the user
    total["cash"] = db.execute("SELECT cash from users WHERE id = ?", session["user_id"])[0]["cash"]

    # Find portfolio
    rows = db.execute(
        "SELECT stock_name, stock_symbol, SUM(stock_quantity) AS stock_quantity, SUM(total_price) AS total_value FROM transactions WHERE user_id = ? GROUP BY stock_symbol ORDER BY total_value DESC", session["user_id"])

    for row in rows:

        # Calculate price of each stock
        stock = lookup(row["stock_symbol"])
        row["stock_price"] = stock["price"]
        row["current_value"] = row["stock_price"] * row["stock_quantity"]
        row["difference"] = row["current_value"] - row["total_value"]
        if row["total_value"] != 0:
            row["percentage"] = row["difference"]/row["total_value"] * 100
        else:
            row["percentage"] = 0.0

        # Calculate total value
        total["total_value"] += row["total_value"]
        total["current_value"] += row["current_value"]

        # Change format
        for key in row:
            if isinstance(row[key], float):
                if key == "percentage":
                    row[key] = f"{row[key]:.2f}%"

    # Calculate total values
    total["balance"] = total["cash"] + total["current_value"]
    total["difference"] = total["current_value"] - total["total_value"]
    if total["total_value"] != 0:
        total["percentage"] = total["difference"]/total["total_value"] * 100
    else:
        total["percentage"] = 0.0

    for key in total:
        if isinstance(total[key], float):
            if key == "percentage":
                total[key] = f"{total[key]:.2f}%"

    return render_template("index.html", now=time_now("EST"), total=total, rows=rows)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock symbol", 400)
        if not request.form.get("shares"):
            return apology("must provide stock shares", 400)
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("invalid stock shares", 400)
        if shares <= 0:
            return apology("shares must be positive", 400)

        # Calculate total price
        price = stock["price"] * shares

        # Validate user money
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]
        if price > cash:
            return apology("Not enough cash", 400)

        # Spend cash
        cash = cash - price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

        # log transaction
        db.execute("INSERT INTO transactions (user_id, stock_name, stock_symbol, stock_price, stock_quantity, total_price, UNIX_time) VALUES(?, ?, ?, ?, ?, ?, strftime('%s','now'))",
                   session["user_id"], stock["name"], stock["symbol"], stock["price"], shares, price)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        try:
            symbol = request.url.split('=')[1]
            return render_template("buy.html", now=time_now("EST"), symbol=symbol)
        except IndexError:
            return render_template("buy.html", now=time_now("EST"), symbol="symbol")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Find transactions from database
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY UNIX_time DESC", session["user_id"])

    for transaction in transactions:
        # Convert time to EST time
        transaction["EST_time"] = convert_time(int(transaction["UNIX_time"]))

    return render_template("history.html", now=time_now("EST"), transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username").lower())

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html", now=time_now("EST"))


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
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock symbol", 400)
        return render_template("quoted.html", stock=stock, now=time_now("EST"))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html", now=time_now("EST"))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        username = request.form.get("username").lower()

        # Check username availability
        rows = db.execute("SELECT username FROM users WHERE username = ?", username)
        if rows != []:
            return apology("username already exists", 400)

        # Ensure password was entered
        if not request.form.get("password"):
            return apology("must provide password", 400)

        password = request.form.get("password")

        # Confirm password
        if password != request.form.get("confirmation"):
            return apology("password must be the same", 400)

        # Insert username and password_hash into database
        password_hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, password_hash)

        # Redirect user to login page
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html", now=time_now("EST"))


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        symbol = request.form.get("symbol")
        stock = lookup(symbol)
        if not stock:
            return apology("invalid stock symbol", 400)
        if not request.form.get("shares"):
            return apology("must provide stock shares", 400)
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("invalid stock shares", 400)
        if shares <= 0:
            return apology("shares must be positive", 400)

        # Find stocks in user's portfolio
        stocks = db.execute("SELECT stock_symbol FROM transactions WHERE user_id = ? AND stock_symbol = ?",
                            session["user_id"], symbol)
        if stocks == []:
            return apology("You do not own this stock", 400)

        # Validate user shares
        rows = db.execute(
            "SELECT SUM(stock_quantity) AS shares FROM transactions WHERE user_id = ? AND stock_symbol = ?", session["user_id"], symbol)
        print(rows)
        if shares > rows[0]["shares"]:
            return apology("You do not own enough shares", 400)

        shares = shares * -1
        sold = stock["price"] * shares

        # Gain cash
        cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        cash = cash - sold
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])

        # log transaction
        db.execute("INSERT INTO transactions (user_id, stock_name, stock_symbol, stock_price, stock_quantity, total_price, UNIX_time) VALUES(?, ?, ?, ?, ?, ?, strftime('%s','now'))",
                   session["user_id"], stock["name"], stock["symbol"], stock["price"], shares, sold)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # Create stocks list from database
        stocks = []
        rows = db.execute("SELECT DISTINCT stock_symbol FROM transactions WHERE user_id = ? ORDER BY stock_symbol ASC",
                          session["user_id"])
        for row in rows:
            stocks.append(row["stock_symbol"])

        try:

            # Pre-select symbol if using sell buttons from index.html
            symbol = request.url.split('=')[1]
            stocks.remove(symbol)
            return render_template("sell.html", now=time_now("EST"), symbol=symbol, stocks=stocks)
        except IndexError:
            return render_template("sell.html", now=time_now("EST"), symbol="symbol", stocks=stocks)
