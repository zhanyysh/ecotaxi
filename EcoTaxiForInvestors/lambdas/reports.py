import json
import pymysql
import hmac
import hashlib
from datetime import datetime, timedelta


def lambda_handler(event, context):
    if not event:
        return {
            "statusCode": 400,
            "body": json.dumps({"message": "Not all requirements are provided"}),
        }

    conn = pymysql.connect(
        host="ecotaxidb.c9mc6ua2uofg.eu-north-1.rds.amazonaws.com",
        user="ecotaxiuser",
        password="EcoTaxi#2024",
        db="ecotaxidb",
    )
    cursor = conn.cursor()

    data = event

    if not "password" in data or not "from" in data or not "to" in data:
        return {
            "statusCode": 400,
            "body": json.dumps({"message": "Not all requiments are provided"}),
        }

    password = data["password"]

    uid = auth(cursor, password)

    if uid == 0:
        return {
            "statusCode": 401,
            "body": json.dumps({"message": "Wrong password"}),
        }

    fromDate = data["from"]
    toDate = data["to"]

    return {
        "statusCode": 200,
        "body": get_report(cursor, uid, fromDate, toDate),
    }


def auth(cursor, password):
    password = generate_hmac(password)
    cursor.execute(f"SELECT investors.id FROM investors WHERE password = '{password}'")
    data = cursor.fetchone()
    return data[0] if data else 0


def generate_hmac(data):
    key_bytes = "Il05EmiN3cR@ftMe0w$4shdi|3".encode("utf-8")
    data_bytes = data.encode("utf-8")

    hmac_result = hmac.new(key_bytes, data_bytes, hashlib.sha256).hexdigest()

    return hmac_result


def get_report(cursor, uid, fromDate, toDate):
    date_obj = datetime.strptime(toDate, "%Y-%m-%d")

    # Add one day using timedelta
    new_date_obj = date_obj + timedelta(days=1)

    # Convert back to string
    toDate = new_date_obj.strftime("%Y-%m-%d")
    query = """
            WITH event_stats AS (
                SELECT
                    carId,
                    SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) as income,
                    SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) as outcome
                FROM events
                WHERE date BETWEEN %s AND %s
                GROUP BY carId
            ),
            charge_stats AS (
                SELECT
                    carId,
                    SUM(COALESCE(kwh, 0)) * 10 as kwh
                FROM charges
                WHERE date BETWEEN %s AND %s
                GROUP BY carId
            ),
            car_stats AS (
                SELECT
                    cars.id as carId,
                    cars.sid as carSid,
                    COALESCE(event_stats.income, 0) as income,
                    COALESCE(event_stats.outcome, 0) as outcome,
                    COALESCE(charge_stats.kwh, 0) as kwh,
                    COALESCE(cars.percentage, 0) as percentage
                FROM investors
                JOIN cars ON cars.investorId = investors.id
                LEFT JOIN event_stats ON cars.id = event_stats.carId
                LEFT JOIN charge_stats ON cars.id = charge_stats.carId
                WHERE investors.id = %s
            )
            SELECT
                carId,
                carSid,
                income,
                kwh,
                outcome,
                FLOOR(income * 0.96 - kwh + outcome) as profit,
                percentage,
                FLOOR((income * 0.96 - kwh + outcome) * COALESCE(percentage, 0) / 100) as ourProfit,
                FLOOR((income * 0.96 - kwh + outcome) * COALESCE(100 - percentage, 0) / 100) as investorsProfit
            FROM car_stats
        """
    cursor.execute(query, (fromDate, toDate, fromDate, toDate, uid))
    cars = cursor.fetchall()

    query = """
            WITH event_stats AS (
                SELECT
                    carId,
                    SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) AS income,
                    SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) AS outcome
                FROM events
                WHERE date BETWEEN %s AND %s
                GROUP BY carId
            ),
            charge_stats AS (
                SELECT
                    carId,
                    SUM(COALESCE(kwh, 0)) * 10 AS kwh
                FROM charges
                WHERE date BETWEEN %s AND %s
                GROUP BY carId
            ),
            car_stats AS (
                SELECT
                    investors.name AS investorName,
                    cars.id AS carId,
                    COALESCE(event_stats.income, 0) AS income,
                    COALESCE(event_stats.outcome, 0) AS outcome,
                    COALESCE(charge_stats.kwh, 0) AS kwh,
                    COALESCE(cars.percentage, 0) AS percentage,
                    FLOOR((COALESCE(event_stats.income, 0) * 0.96 -
                          COALESCE(event_stats.outcome, 0) -
                          COALESCE(charge_stats.kwh, 0)) * (COALESCE(cars.percentage, 0) / 100)) AS ourProfit,
                    FLOOR((COALESCE(event_stats.income, 0) * 0.96 -
                          COALESCE(event_stats.outcome, 0) -
                          COALESCE(charge_stats.kwh, 0)) * ((100 - COALESCE(cars.percentage, 0)) / 100)) AS investorsProfit
                FROM investors
                LEFT JOIN cars ON cars.investorId = investors.id
                LEFT JOIN event_stats ON cars.id = event_stats.carId
                LEFT JOIN charge_stats ON cars.id = charge_stats.carId
                WHERE investors.id = %s
                GROUP BY cars.id, investors.name
            )
            SELECT
                investorName,
                FLOOR(SUM(income)) AS totalIncome,
                FLOOR(SUM(kwh)) AS totalKwh,
                FLOOR(SUM(outcome)) AS totalOutcome,
                FLOOR(SUM((income * 0.96) - outcome - kwh)) AS totalProfit,
                COUNT(carId) AS totalCarsCount,
                FLOOR(SUM(ourProfit)) AS totalOurIncome,
                FLOOR(SUM(investorsProfit)) AS totalInvestorsIncome
            FROM car_stats
            GROUP BY investorName
        """
    cursor.execute(query, (fromDate, toDate, fromDate, toDate, uid))
    total = cursor.fetchone()

    # Check if 'total' is None
    return {
        "cars": [
            {
                "name": car[1],
                "totalIncome": car[2],
                "totalKwh": car[3],
                "totalOutcome": car[4],
                "totalProfit": car[5],
                "percentage": car[6],
                "totalOurIncome": car[7],
                "totalInvestorsIncome": car[8],
            }
            for car in cars
        ],
        "total": {
            "totalIncome": total[1],
            "totalKwh": total[2],
            "totalOutcome": total[3],
            "totalProfit": total[4],
            "totalCarsCount": total[5],
            "totalOurIncome": total[6],
            "totalInvestorsIncome": total[7],
        },
    }

    # If 'total' is not None, return valid values
    return {
        "cars": [
            {
                "id": car[0],
                "sid": car[1],
                "income": car[2],
                "outcome": car[3],
                "general": car[4],
                "percent": car[5],
                "fee": car[6],
                "profit": car[7],
            }
            for car in cars
        ],
        "total": {
            "income": total[1],
            "outcome": total[2],
            "general": total[3],
            "cars": total[4],
            "fee": total[5],
            "profit": total[6],
        },
    }
