from flask import Flask, render_template, request
from random import randint

app = Flask(__name__)


@app.route("/")
def main():
    return render_template("index.html")


@app.route("/report")
def report():
    token = request.headers.get("token")
    if token == "12345" or token == "meow":
        cars = [
            {
                "id": 1,
                "sid": 144,
                "income": 25243,
                "outcome": 4535,
                "general": 20798,
                "percent": 30,
                "fee": 6239,
                "profit": 14559,
            },
            {
                "id": 2,
                "sid": 120,
                "income": 38500,
                "outcome": 6000,
                "general": 32500,
                "percent": 25,
                "fee": 9625,
                "profit": 22875,
            },
            {
                "id": 3,
                "sid": 117,
                "income": 45987,
                "outcome": 7890,
                "general": 38097,
                "percent": 35,
                "fee": 16084,
                "profit": 22013,
            },
            {
                "id": 4,
                "sid": 555,
                "income": 29000,
                "outcome": 4500,
                "general": 24500,
                "percent": 20,
                "fee": 4900,
                "profit": 19600,
            },
            {
                "id": 5,
                "sid": 111,
                "income": 51000,
                "outcome": 10000,
                "general": 41000,
                "percent": 40,
                "fee": 16400,
                "profit": 24600,
            },
            {
                "id": 6,
                "sid": 877,
                "income": 31234,
                "outcome": 4567,
                "general": 26667,
                "percent": 28,
                "fee": 7467,
                "profit": 19199,
            },
            {
                "id": 7,
                "sid": 443,
                "income": 43950,
                "outcome": 8500,
                "general": 35450,
                "percent": 33,
                "fee": 11698,
                "profit": 23752,
            },
            {
                "id": 8,
                "sid": 996,
                "income": 47000,
                "outcome": 12000,
                "general": 35000,
                "percent": 45,
                "fee": 15750,
                "profit": 19250,
            },
            {
                "id": 9,
                "sid": 699,
                "income": 38450,
                "outcome": 7650,
                "general": 30800,
                "percent": 25,
                "fee": 7700,
                "profit": 23100,
            },
            {
                "id": 10,
                "sid": 223,
                "income": 36000,
                "outcome": 5000,
                "general": 31000,
                "percent": 20,
                "fee": 6200,
                "profit": 24800,
            },
        ]

        total = {
            "income": 386364,
            "outcome": 72142,
            "general": 314222,
            "cars": 10,
            "fee": 103063,
            "profit": 211159,
        }
        return {
            "cars": cars,
            "total": total,
        }
    else:
        return {"error": "Wrong token"}, 401


@app.route("/events")
def events():
    token = request.headers.get("token")
    if token == "12345" or token == "meow":
        return {
            "events": [
                {
                    "date": "12.02.2024 08:45",
                    "type": "Аренда",
                    "car": 333,
                    "driver": "Д",
                    "amount": 4200,
                    "description": "Аренда машины 3",
                },
                {
                    "date": "25.04.2024 09:30",
                    "type": "Штраф",
                    "car": 777,
                    "driver": "Г",
                    "amount": -750,
                    "description": "Штраф машины 7",
                },
                {
                    "date": "18.06.2024 16:10",
                    "type": "Ремонт",
                    "car": 555,
                    "driver": "А",
                    "amount": -300,
                    "description": "Ремонт машины 5",
                },
                {
                    "date": "03.05.2024 14:55",
                    "type": "Аренда",
                    "car": 888,
                    "driver": "В",
                    "amount": 2700,
                    "description": "Аренда машины 8",
                },
                {
                    "date": "15.08.2024 13:20",
                    "type": "Штраф",
                    "car": 699,
                    "driver": "Б",
                    "amount": -600,
                    "description": "Штраф машины 6",
                },
                {
                    "date": "07.11.2024 11:15",
                    "type": "Аренда",
                    "car": 117,
                    "driver": "Д",
                    "amount": 3900,
                    "description": "Аренда машины 1",
                },
                {
                    "date": "20.09.2024 19:00",
                    "type": "Ремонт",
                    "car": 222,
                    "driver": "Г",
                    "amount": -450,
                    "description": "Ремонт машины 2",
                },
                {
                    "date": "30.03.2024 17:45",
                    "type": "Аренда",
                    "car": 444,
                    "driver": "А",
                    "amount": 3300,
                    "description": "Аренда машины 4",
                },
                {
                    "date": "11.01.2024 10:20",
                    "type": "Штраф",
                    "car": 996,
                    "driver": "В",
                    "amount": -800,
                    "description": "Штраф машины 9",
                },
                {
                    "date": "22.02.2024 15:35",
                    "type": "Аренда",
                    "car": 699,
                    "driver": "Д",
                    "amount": 2800,
                    "description": "Аренда машины 6",
                },
                {
                    "date": "14.12.2024 09:15",
                    "type": "Ремонт",
                    "car": 1,
                    "driver": "А",
                    "amount": -500,
                    "description": "Ремонт машины 1",
                },
                {
                    "date": "28.07.2024 12:00",
                    "type": "Аренда",
                    "car": 555,
                    "driver": "Г",
                    "amount": 3200,
                    "description": "Аренда машины 5",
                },
                {
                    "date": "16.10.2024 11:50",
                    "type": "Штраф",
                    "car": 888,
                    "driver": "Б",
                    "amount": -700,
                    "description": "Штраф машины 8",
                },
                {
                    "date": "29.04.2024 08:30",
                    "type": "Аренда",
                    "car": 996,
                    "driver": "В",
                    "amount": 4500,
                    "description": "Аренда машины 9",
                },
                {
                    "date": "01.05.2024 13:10",
                    "type": "Ремонт",
                    "car": 333,
                    "driver": "Д",
                    "amount": -800,
                    "description": "Ремонт машины 3",
                },
                {
                    "date": "02.08.2024 18:05",
                    "type": "Аренда",
                    "car": 222,
                    "driver": "Г",
                    "amount": 3900,
                    "description": "Аренда машины 2",
                },
                {
                    "date": "08.09.2024 16:30",
                    "type": "Штраф",
                    "car": 444,
                    "driver": "А",
                    "amount": -200,
                    "description": "Штраф машины 4",
                },
            ],
            "total": {
                "amount": 23400,
            },
        }
    else:
        return {"error": "Wrong token"}, 401


if __name__ == "__main__":
    app.run(debug=True)
