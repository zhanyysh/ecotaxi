from flask import Flask, render_template, request, redirect, url_for, flash, make_response
from flask_login import LoginManager, UserMixin, login_user, login_required, logout_user, current_user
import mysql.connector
from datetime import datetime, date
import hmac
import hashlib
from math import floor

app = Flask(__name__)
app.secret_key = 'your-secret-key'  # Измените на реальный секретный ключ

# Конфигурация базы данных
db_config = {
    'host': 'localhost',
    'user': 'root',  # Измените на вашего пользователя MySQL
    'password': '',  # Измените на ваш пароль MySQL
    'database': 'ecotaxidb'
}

# Настройка Flask-Login
login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'

class User(UserMixin):
    def __init__(self, id, username):
        self.id = id
        self.username = username

@login_manager.user_loader
def load_user(user_id):
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    cursor.execute('SELECT * FROM investors WHERE id = %s', (user_id,))
    user = cursor.fetchone()
    cursor.close()
    conn.close()
    if user:
        return User(user['id'], user['name'])
    return None

@app.route('/')
def index():
    if current_user.is_authenticated:
        return redirect(url_for('dashboard'))
    return redirect(url_for('login'))

def generate_hmac(data):
    key = "Il05EmiN3cR@ftMe0w$4shdi|3".encode()
    data_bytes = data.encode()
    h = hmac.new(key, data_bytes, hashlib.sha256)
    return h.hexdigest()

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        
        # Хэшируем введенный пароль используя тот же метод, что и в Qt
        hashed_password = generate_hmac(password)
        
        print(f"Attempting login with username: {username}")
        print(f"Input password: {password}")
        print(f"Generated hash: {hashed_password}")
        
        conn = mysql.connector.connect(**db_config)
        cursor = conn.cursor(dictionary=True)
        
        cursor.execute('SELECT * FROM investors WHERE name = %s', (username,))
        user = cursor.fetchone()
        
        if user:
            print(f"User found: {user}")
            print(f"Stored hash: {user['password']}")
            # Сравниваем хэшированные пароли
            if user['password'] == hashed_password:
                user_obj = User(user['id'], user['name'])
                login_user(user_obj)
                return redirect(url_for('dashboard'))
            else:
                print(f"Password mismatch. Expected: {user['password']}, Got: {hashed_password}")
        else:
            print(f"No user found with name: {username}")
            
        flash('Неверное имя пользователя или пароль')
        cursor.close()
        conn.close()
    
    return render_template('login.html')

@app.route('/dashboard')
@login_required
def dashboard():
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    # Получаем список машин инвестора
    cursor.execute('SELECT * FROM cars WHERE investorId = %s', (current_user.id,))
    cars = cursor.fetchall()

    # Получаем даты из query string
    start_date = request.args.get('start_date')
    end_date = request.args.get('end_date')
    date_filter = ''
    params = [current_user.id]
    if start_date and end_date:
        date_filter = ' AND events.date BETWEEN %s AND %s'
        params.append(start_date)
        end_date_dt = datetime.strptime(end_date, '%Y-%m-%d')
        end_date_dt = end_date_dt.replace(hour=23, minute=59, second=59)
        params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
    else:
        # Если даты не заданы, используем весь диапазон
        date_filter = ''

    # Новый отчет по всем машинам инвестора (логика как в Qt)
    query = f'''
        WITH event_stats AS (
            SELECT
                carId,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) as income,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05 as incomeTax,
                SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) as outcome,
                SUM(COALESCE(dolg, 0)) as totalDebt
            FROM events
            WHERE carId IN (SELECT id FROM cars WHERE investorId = %s){date_filter}
            GROUP BY carId
        ),
        charge_stats AS (
            SELECT
                carId,
                SUM(COALESCE(kwh, 0)) as kwh,
                SUM(COALESCE(kwh * kwh_multiplier, 0)) as kwh_with_multiplier
            FROM charges
            WHERE carId IN (SELECT id FROM cars WHERE investorId = %s){date_filter}
            GROUP BY carId
        ),
        car_stats AS (
            SELECT
                cars.id as carId,
                cars.sid as carSid,
                COALESCE(event_stats.income, 0) as income,
                FLOOR(COALESCE(event_stats.incomeTax, 0)) as incomeTax,
                COALESCE(event_stats.outcome, 0) as outcome,
                COALESCE(charge_stats.kwh, 0) as kwh,
                COALESCE(charge_stats.kwh_with_multiplier, 0) as kwh_with_multiplier,
                COALESCE(cars.percentage, 0) as percentage,
                COALESCE(event_stats.totalDebt, 0) as totalDebt
            FROM cars
            LEFT JOIN event_stats ON cars.id = event_stats.carId
            LEFT JOIN charge_stats ON cars.id = charge_stats.carId
            WHERE cars.investorId = %s
        )
        SELECT
            carId,
            carSid,
            income,
            incomeTax,
            kwh,
            kwh_with_multiplier,
            outcome,
            FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt) as profit,
            percentage,
            CASE 
                WHEN (FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt) < 0) THEN 0
                ELSE FLOOR((income * 0.95 - kwh_with_multiplier + outcome - totalDebt) * COALESCE(percentage, 0) / 100)
            END AS ourProfit,
            CASE 
                WHEN (FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt) < 0) THEN FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt)
                ELSE FLOOR((income * 0.95 - kwh_with_multiplier + outcome - totalDebt) * (100 - COALESCE(percentage, 0)) / 100)
            END AS investorsProfit,
            totalDebt
        FROM car_stats;
    '''
    # Параметры: для event_stats, charge_stats, car_stats
    all_params = [current_user.id] + [current_user.id] + [current_user.id]
    if start_date and end_date:
        all_params = [current_user.id, start_date, end_date_dt.strftime('%Y-%m-%d %H:%M:%S'),
                      current_user.id, start_date, end_date_dt.strftime('%Y-%m-%d %H:%M:%S'),
                      current_user.id]
    cursor.execute(query, tuple(all_params))
    cars_report = cursor.fetchall()

    # Итоговая строка (суммы по всем машинам)
    cars_report_total = {
        'income': sum(car['income'] or 0 for car in cars_report),
        'incomeTax': sum(car['incomeTax'] or 0 for car in cars_report),
        'kwh': sum(car['kwh'] or 0 for car in cars_report),
        'kwh_with_multiplier': sum(car['kwh_with_multiplier'] or 0 for car in cars_report),
        'outcome': sum(car['outcome'] or 0 for car in cars_report),
        'profit': sum(car['profit'] or 0 for car in cars_report),
        'ourProfit': sum(car['ourProfit'] or 0 for car in cars_report),
        'investorsProfit': sum(car['investorsProfit'] or 0 for car in cars_report),
        'totalDebt': sum(car['totalDebt'] or 0 for car in cars_report),
        'count': len(cars_report)
    }
    cursor.close()
    conn.close()
    return render_template('dashboard.html', cars=cars, cars_report=cars_report, cars_report_total=cars_report_total, start_date=start_date, end_date=end_date)

@app.route('/payments')
@login_required
def payments():
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    # Получаем историю выплат для инвестора
    cursor.execute('SELECT * FROM payments WHERE investor_id = %s ORDER BY date DESC', (current_user.id,))
    payments = cursor.fetchall()
    cursor.close()
    conn.close()
    return render_template('payments.html', payments=payments)

@app.route('/logout')
@login_required
def logout():
    logout_user()
    return redirect(url_for('login'))

@app.route('/car/<int:car_id>')
@login_required
def car_details(car_id):
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    # Получаем машину
    cursor.execute('SELECT * FROM cars WHERE id = %s AND investorId = %s', (car_id, current_user.id))
    car = cursor.fetchone()
    if not car:
        flash('Машина не найдена')
        cursor.close()
        conn.close()
        return redirect(url_for('dashboard'))
    # Получаем даты из query string
    start_date = request.args.get('start_date')
    end_date = request.args.get('end_date')
    date_filter = ''
    params = [car_id]
    if start_date and end_date:
        date_filter = ' AND events.date BETWEEN %s AND %s'
        params.append(start_date)
        end_date_dt = datetime.strptime(end_date, '%Y-%m-%d')
        end_date_dt = end_date_dt.replace(hour=23, minute=59, second=59)
        params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
    else:
        date_filter = ''

    # Новый отчет по одной машине (логика как в Qt)
    query = f'''
        WITH event_stats AS (
            SELECT
                carId,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) as income,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05 as incomeTax,
                SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) as outcome,
                SUM(COALESCE(dolg, 0)) as totalDebt
            FROM events
            WHERE carId = %s{date_filter}
            GROUP BY carId
        ),
        charge_stats AS (
            SELECT
                carId,
                SUM(COALESCE(kwh, 0)) as kwh,
                SUM(COALESCE(kwh * kwh_multiplier, 0)) as kwh_with_multiplier
            FROM charges
            WHERE carId = %s{date_filter}
            GROUP BY carId
        ),
        car_stats AS (
            SELECT
                cars.id as carId,
                cars.sid as carSid,
                COALESCE(event_stats.income, 0) as income,
                FLOOR(COALESCE(event_stats.incomeTax, 0)) as incomeTax,
                COALESCE(event_stats.outcome, 0) as outcome,
                COALESCE(charge_stats.kwh, 0) as kwh,
                COALESCE(charge_stats.kwh_with_multiplier, 0) as kwh_with_multiplier,
                COALESCE(cars.percentage, 0) as percentage,
                COALESCE(event_stats.totalDebt, 0) as totalDebt
            FROM cars
            LEFT JOIN event_stats ON cars.id = event_stats.carId
            LEFT JOIN charge_stats ON cars.id = charge_stats.carId
            WHERE cars.id = %s
        )
        SELECT
            carId,
            carSid,
            income,
            incomeTax,
            kwh,
            kwh_with_multiplier,
            outcome,
            FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt) as profit,
            percentage,
            CASE 
                WHEN (FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt) < 0) THEN 0
                ELSE FLOOR((income * 0.95 - kwh_with_multiplier + outcome - totalDebt) * COALESCE(percentage, 0) / 100)
            END AS ourProfit,
            CASE 
                WHEN (FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt) < 0) THEN FLOOR(income * 0.95 - kwh_with_multiplier + outcome - totalDebt)
                ELSE FLOOR((income * 0.95 - kwh_with_multiplier + outcome - totalDebt) * (100 - COALESCE(percentage, 0)) / 100)
            END AS investorsProfit,
            totalDebt
        FROM car_stats;
    '''
    # Параметры: для event_stats, charge_stats, car_stats
    all_params = [car_id] + [car_id] + [car_id]
    if start_date and end_date:
        all_params = [car_id, start_date, end_date_dt.strftime('%Y-%m-%d %H:%M:%S'),
                      car_id, start_date, end_date_dt.strftime('%Y-%m-%d %H:%M:%S'),
                      car_id]
    cursor.execute(query, tuple(all_params))
    car_report = cursor.fetchone() or {}

    # Получаем события по машине (для таблицы событий)
    events = []
    event_params = [car_id]
    event_date_filter = ''
    if start_date and end_date:
        event_date_filter = ' AND events.date BETWEEN %s AND %s'
        event_params.append(start_date)
        event_params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
    cursor.execute(f'''
        SELECT events.date, types.name AS type, drivers.name AS driver, events.amount, events.dolg, events.description
        FROM events
        LEFT JOIN types ON events.typeId = types.id
        LEFT JOIN drivers ON events.driverId = drivers.id
        WHERE events.carId = %s{event_date_filter}
        ORDER BY events.date DESC
    ''', tuple(event_params))
    events = cursor.fetchall()
    cursor.close()
    conn.close()
    return render_template(
        'car_details.html',
        car=car,
        car_report=car_report,
        events=events,
        start_date=start_date,
        end_date=end_date
    )

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True) 