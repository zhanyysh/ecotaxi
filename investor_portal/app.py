from flask import Flask, render_template, request, redirect, url_for, flash
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
    cursor.close()
    conn.close()
    return render_template('dashboard.html', cars=cars)

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
    # Период отчета (за все время)
    start_date = '2000-01-01'
    end_date = date.today().strftime('%Y-%m-%d')
    # Доход (totalIncome)
    cursor.execute('SELECT COALESCE(SUM(amount),0) as income FROM events WHERE carId = %s AND amount > 0', (car_id,))
    income = cursor.fetchone()['income'] or 0
    # Налог 5% (totalIncomeTax)
    tax = income * 0.05
    # Расход (totalOutcome)
    cursor.execute('SELECT COALESCE(SUM(amount),0) as outcome FROM events WHERE carId = %s AND amount < 0', (car_id,))
    outcome = cursor.fetchone()['outcome'] or 0
    # KWH x 10 (totalKwh)
    cursor.execute('SELECT COALESCE(SUM(kwh),0) as kwh FROM charges WHERE carId = %s', (car_id,))
    kwh = (cursor.fetchone()['kwh'] or 0) * 10
    # totalEventProfit = Доход * 0.95
    total_event_profit = income * 0.95
    # Общий (totalAmount)
    total_amount = floor(total_event_profit - kwh + outcome)
    # % (процент)
    percent = car['percentage']
    # Комиссия (ourProfit)
    if total_amount < 0:
        commission = 0
    else:
        commission = floor(total_amount * percent / 100)
    # Инвестору (investorsProfit)
    if total_amount < 0:
        to_investor = total_amount
    else:
        to_investor = floor(total_amount * (100 - percent) / 100)
    cursor.close()
    conn.close()
    return render_template(
        'car_details.html',
        car=car,
        report={
            'income': income,
            'tax': tax,
            'kwh': kwh,
            'outcome': outcome,
            'total': total_amount,
            'percent': percent,
            'commission': commission,
            'to_investor': to_investor
        }
    )

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

@app.route('/car/<int:car_id>/report')
@login_required
def car_report(car_id):
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    # Проверяем, принадлежит ли машина инвестору
    cursor.execute('SELECT * FROM cars WHERE id = %s AND investorId = %s', (car_id, current_user.id))
    car = cursor.fetchone()
    if not car:
        flash('Машина не найдена')
        cursor.close()
        conn.close()
        return redirect(url_for('dashboard'))
    # Получаем события по машине
    cursor.execute('''
        SELECT events.date, types.name AS type, drivers.name AS driver, events.amount, events.description
        FROM events
        LEFT JOIN types ON events.typeId = types.id
        LEFT JOIN drivers ON events.driverId = drivers.id
        WHERE events.carId = %s
        ORDER BY events.date DESC
    ''', (car_id,))
    events = cursor.fetchall()
    cursor.close()
    conn.close()
    return render_template('car_report.html', car=car, events=events)

if __name__ == '__main__':
    app.run(debug=True) 