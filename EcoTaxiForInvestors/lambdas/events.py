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
    # First Query: Fetch events
    query = """
                SELECT
                events.id,
                events.date as time,
                CASE WHEN types.id IS NULL OR events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as typeName,
                CASE WHEN drivers.id IS NULL OR events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverId,
                CASE WHEN cars.id IS NULL OR events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.sid END END as carId,
                events.amount,
                events.description
                FROM events
                LEFT JOIN cars ON cars.id = events.carId
                LEFT JOIN drivers ON drivers.id = events.driverId
                LEFT JOIN types ON types.id = events.typeId
                WHERE cars.investorId = %s AND events.date BETWEEN %s AND %s
            """
    cursor.execute(query, (uid, fromDate, toDate))
    events = cursor.fetchall()

    # Second Query: Total amount
    query = """
                SELECT
                SUM(CASE WHEN events.amount > 0 THEN events.amount ELSE 0 END) as income,
                SUM(CASE WHEN events.amount < 0 THEN events.amount ELSE 0 END) as outcome,
                SUM(events.amount) as amount
                FROM events
                LEFT JOIN cars ON cars.id = events.carId
                WHERE cars.investorId = %s
                AND events.date BETWEEN %s AND %s
            """
    cursor.execute(query, (uid, fromDate, toDate))
    total = cursor.fetchone()

    return {
        "events": [
            {
                "id": event[0],
                "time": event[1].strftime("%d.%m.%Y %H:%M"),
                "typeName": event[2],
                "driverId": event[3],
                "carId": event[4],
                "amount": event[5],
                "description": event[6],
            }
            for event in events
        ],
        "total": {
            "income": total[0] if total[0] else 0,
            "outcome": total[1] if total[1] else 0,
            "amount": total[2] if total[2] else 0,
        },
    }
