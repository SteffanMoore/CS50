import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# dictionary of number of days in each month
month_days = {"January": 31, "February": 29, "March": 31,
              "April": 30, "May": 31, "June": 30, "July": 31,
              "August": 31, "September": 30, "October": 31,
              "November": 30, "December": 31}

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # TODO: Add the user's entry into the database

        valid_entry = True

        # get info from the form
        name = request.form.get("name")
        month = request.form.get("month")
        day = int(request.form.get("day"))

        # check name
        if name.isalpha() == False:
            valid_entry = False

        # check month and day
        try:
            max_days = month_days[month]

            if day <= max_days and day >= 0:
                pass
            else:
                valid_entry = False

        except KeyError:
            valid_entry = False

        # if the entry is still valid then it is added to the database
        if valid_entry == True:
            db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", name, (list(month_days).index(month) + 1), day)

        return redirect("/")


    else:
        # gets results from database and changes numerical month value for string month
        entries = db.execute("SELECT * FROM birthdays")
        for entry in entries:
            entry["month"] = list(month_days.keys())[entry["month"] - 1]

        return render_template("index.html", entries=entries)


