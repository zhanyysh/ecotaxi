import json
import pymysql
import hmac
import hashlib


def lambda_handler(event, context):
    if "password" not in event.headers:
        return {
            "statusCode": 400,
            "body": json.dumps({"message": "Password not provided"}),
        }

    conn = pymysql.connect(host="localhost", user="root", password="", db="ecotaxi")
    cursor = conn.cursor()

    password = event.headers["password"]

    uid = auth(cursor, password)

    if uid == 0:
        return {
            "statusCode": 401,
            "body": json.dumps({"message": "Wrong password"}),
        }

    fromDate = event.get("from")
    toDate = event.get("to")

    return {
        "statusCode": 200,
        "body": json.dumps(get_report(cursor, uid, fromDate, toDate)),
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
    query = """
                SELECT
                events.id,
                events.date as time,
                CASE WHEN types.id IS NULL OR events.typeId = 0 THEN '-' ELSE CASE WHEN types.id IS NULL THEN 'удален' ELSE types.name END END as typeName,
                CASE WHEN drivers.id IS NULL OR events.driverId = 0 THEN '-' ELSE CASE WHEN drivers.id IS NULL THEN 'удален' ELSE drivers.name END END as driverId,
                CASE WHEN cars.id IS NULL OR events.carId = 0 THEN '-' ELSE CASE WHEN cars.id IS NULL THEN 'удалена' ELSE cars.sid END END as carId,
                events.amount,
                events.description FROM events LEFT JOIN cars ON cars.id = events.carId
                LEFT JOIN drivers ON drivers.id = events.driverId
                LEFT JOIN types ON types.id = events.typeId
                WHERE cars.investorId = %s AND events.date BETWEEN %s AND %s
            """
    cursor.execute(query, (fromDate, toDate, uid))
    events = cursor.fetchall()

    query = """
                SELECT
                SUM(events.amount) as amount
                FROM events
                LEFT JOIN cars ON cars.id = events.carId
                WHERE cars.investorId = %s
                AND events.date BETWEEN %s AND %s
            """
    cursor.execute(query, (fromDate, toDate, uid))
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
            "amount": total[0] if total[0] else 0,
        },
    }
