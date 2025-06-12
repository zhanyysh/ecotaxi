from flask import Flask, render_template, request, redirect, url_for, flash, make_response
from flask_login import LoginManager, UserMixin, login_user, login_required, logout_user, current_user
import mysql.connector
from datetime import datetime, date
import hmac
import hashlib
from math import floor
from fpdf import FPDF

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
    # Получаем список машин инвестора (старый вариант)
    cursor.execute('SELECT * FROM cars WHERE investorId = %s', (current_user.id,))
    cars = cursor.fetchall()

    # Получаем даты из query string
    start_date = request.args.get('start_date')
    end_date = request.args.get('end_date')
    event_date_filter = ''
    charge_date_filter = ''
    params = [current_user.id]  # для event_stats
    charge_params = [current_user.id]  # для charge_stats
    if start_date and end_date:
        event_date_filter = ' AND events.date BETWEEN %s AND %s'
        charge_date_filter = ' AND charges.date BETWEEN %s AND %s'
        # Для event_stats
        params.append(start_date)
        end_date_dt = datetime.strptime(end_date, '%Y-%m-%d')
        end_date_dt = end_date_dt.replace(hour=23, minute=59, second=59)
        params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
        # Для charge_stats
        charge_params.append(start_date)
        charge_params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))

    # Новый отчет по всем машинам инвестора с раздельными фильтрами по датам
    query = f'''
        WITH event_stats AS (
            SELECT
                carId,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) as income,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05 as tax,
                SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) as outcome,
                SUM(COALESCE(dolg, 0)) as debt
            FROM events
            WHERE carId IN (SELECT id FROM cars WHERE investorId = %s){event_date_filter}
            GROUP BY carId
        ),
        charge_stats AS (
            SELECT
                carId,
                SUM(COALESCE(kwh, 0)) * 10 as kwh
            FROM charges
            WHERE carId IN (SELECT id FROM cars WHERE investorId = %s){charge_date_filter}
            GROUP BY carId
        )
        SELECT
            cars.id as car_id,
            cars.sid as car_sid,
            COALESCE(event_stats.income, 0) as income,
            FLOOR(COALESCE(event_stats.tax, 0)) as tax,
            COALESCE(charge_stats.kwh, 0) as kwh,
            COALESCE(event_stats.outcome, 0) as outcome,
            COALESCE(event_stats.debt, 0) as debt,
            FLOOR(COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) as total,
            COALESCE(cars.percentage, 0) as percent,
            FLOOR((COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) * (COALESCE(cars.percentage, 0) / 100)) as commission,
            FLOOR((COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) * ((100 - COALESCE(cars.percentage, 0)) / 100)) as to_investor
        FROM cars
        LEFT JOIN event_stats ON cars.id = event_stats.carId
        LEFT JOIN charge_stats ON cars.id = charge_stats.carId
        WHERE cars.investorId = %s
    '''
    # Объединяем параметры: сначала event_stats, потом charge_stats, потом основной запрос
    all_params = params + charge_params + [current_user.id]
    cursor.execute(query, tuple(all_params))
    cars_report = cursor.fetchall()

    # Итоговая строка (суммы по всем нужным полям)
    cars_report_total = {
        'income': sum(car['income'] or 0 for car in cars_report),
        'tax': sum(car['tax'] or 0 for car in cars_report),
        'kwh': sum(car['kwh'] or 0 for car in cars_report),
        'outcome': sum(car['outcome'] or 0 for car in cars_report),
        'debt': sum(car['debt'] or 0 for car in cars_report),
        'total': sum(car['total'] or 0 for car in cars_report),
        'commission': sum(car['commission'] or 0 for car in cars_report),
        'to_investor': sum(car['to_investor'] or 0 for car in cars_report),
        'count': len(cars_report)
    }
    cursor.close()
    conn.close()
    return render_template('dashboard.html', cars=cars, cars_report=cars_report, cars_report_total=cars_report_total, start_date=start_date, end_date=end_date)

@app.route('/dashboard/pdf')
@login_required
def dashboard_pdf():
    # Получаем те же данные, что и для dashboard
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    # Получаем список машин инвестора (старый вариант)
    cursor.execute('SELECT * FROM cars WHERE investorId = %s', (current_user.id,))
    cars = cursor.fetchall()

    # Получаем даты из query string
    start_date = request.args.get('start_date')
    end_date = request.args.get('end_date')
    event_date_filter = ''
    charge_date_filter = ''
    params = [current_user.id]  # для event_stats
    charge_params = [current_user.id]  # для charge_stats
    if start_date and end_date:
        event_date_filter = ' AND events.date BETWEEN %s AND %s'
        charge_date_filter = ' AND charges.date BETWEEN %s AND %s'
        # Для event_stats
        params.append(start_date)
        end_date_dt = datetime.strptime(end_date, '%Y-%m-%d')
        end_date_dt = end_date_dt.replace(hour=23, minute=59, second=59)
        params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
        # Для charge_stats
        charge_params.append(start_date)
        charge_params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))

    # Новый отчет по всем машинам инвестора с раздельными фильтрами по датам
    query = f'''
        WITH event_stats AS (
            SELECT
                carId,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) as income,
                SUM(CASE WHEN amount > 0 THEN amount ELSE 0 END) * 0.05 as tax,
                SUM(CASE WHEN amount < 0 THEN amount ELSE 0 END) as outcome,
                SUM(COALESCE(dolg, 0)) as debt
            FROM events
            WHERE carId IN (SELECT id FROM cars WHERE investorId = %s){event_date_filter}
            GROUP BY carId
        ),
        charge_stats AS (
            SELECT
                carId,
                SUM(COALESCE(kwh, 0)) * 10 as kwh
            FROM charges
            WHERE carId IN (SELECT id FROM cars WHERE investorId = %s){charge_date_filter}
            GROUP BY carId
        )
        SELECT
            cars.id as car_id,
            cars.sid as car_sid,
            COALESCE(event_stats.income, 0) as income,
            FLOOR(COALESCE(event_stats.tax, 0)) as tax,
            COALESCE(charge_stats.kwh, 0) as kwh,
            COALESCE(event_stats.outcome, 0) as outcome,
            COALESCE(event_stats.debt, 0) as debt,
            FLOOR(COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) as total,
            COALESCE(cars.percentage, 0) as percent,
            FLOOR((COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) * (COALESCE(cars.percentage, 0) / 100)) as commission,
            FLOOR((COALESCE(event_stats.income, 0) * 0.95 + COALESCE(event_stats.outcome, 0) - COALESCE(charge_stats.kwh, 0)) * ((100 - COALESCE(cars.percentage, 0)) / 100)) as to_investor
        FROM cars
        LEFT JOIN event_stats ON cars.id = event_stats.carId
        LEFT JOIN charge_stats ON cars.id = charge_stats.carId
        WHERE cars.investorId = %s
    '''
    # Объединяем параметры: сначала event_stats, потом charge_stats, потом основной запрос
    all_params = params + charge_params + [current_user.id]
    cursor.execute(query, tuple(all_params))
    cars_report = cursor.fetchall()

    # Итоговая строка (суммы по всем нужным полям)
    cars_report_total = {
        'income': sum(car['income'] or 0 for car in cars_report),
        'tax': sum(car['tax'] or 0 for car in cars_report),
        'kwh': sum(car['kwh'] or 0 for car in cars_report),
        'outcome': sum(car['outcome'] or 0 for car in cars_report),
        'debt': sum(car['debt'] or 0 for car in cars_report),
        'total': sum(car['total'] or 0 for car in cars_report),
        'commission': sum(car['commission'] or 0 for car in cars_report),
        'to_investor': sum(car['to_investor'] or 0 for car in cars_report),
        'count': len(cars_report)
    }
    cursor.close()
    conn.close()
    pdf = FPDF()
    pdf.add_page()
    pdf.add_font('DejaVu', '', 'static/fonts/ttf/DejaVuSans.ttf', uni=True)
    pdf.set_font('DejaVu', '', 8)
    pdf.cell(0, 10, txt="Отчет по всем вашим машинам", ln=True, align='C')
    # Заголовки
    headers = ["ID", "Доход", "Долг", "Налог 5%", "KWH*10", "Расход", "Общий", "%", "Комиссия", "Инвестору"]
    for h in headers:
        pdf.cell(20, 10, h, 1)
    pdf.ln()
    # Данные
    for car in cars_report:
        pdf.cell(20, 10, str(car['car_sid']), 1)
        pdf.cell(20, 10, str(car['income']), 1)
        pdf.cell(20, 10, str(car['debt']), 1)
        pdf.cell(20, 10, str(car['tax']), 1)
        pdf.cell(20, 10, str(car['kwh']), 1)
        pdf.cell(20, 10, str(car['outcome']), 1)
        pdf.cell(20, 10, str(car['total']), 1)
        pdf.cell(20, 10, str(car['percent']), 1)
        pdf.cell(20, 10, str(car['commission']), 1)
        pdf.cell(20, 10, str(car['to_investor']), 1)
        pdf.ln()
    # Итог
    pdf.cell(20, 10, "Итого", 1)
    pdf.cell(20, 10, str(cars_report_total['income']), 1)
    pdf.cell(20, 10, str(cars_report_total['debt']), 1)
    pdf.cell(20, 10, str(cars_report_total['tax']), 1)
    pdf.cell(20, 10, str(cars_report_total['kwh']), 1)
    pdf.cell(20, 10, str(cars_report_total['outcome']), 1)
    pdf.cell(20, 10, str(cars_report_total['total']), 1)
    pdf.cell(20, 10, str(cars_report_total['count']), 1)
    pdf.cell(20, 10, str(cars_report_total['commission']), 1)
    pdf.cell(20, 10, str(cars_report_total['to_investor']), 1)
    response = make_response(pdf.output(dest='S').encode('latin-1', 'replace'))
    response.headers['Content-Type'] = 'application/pdf'
    response.headers['Content-Disposition'] = 'attachment; filename=report.pdf'
    return response

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
        # включительно до конца дня
        end_date_dt = datetime.strptime(end_date, '%Y-%m-%d')
        end_date_dt = end_date_dt.replace(hour=23, minute=59, second=59)
        params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
    # Получаем события по машине (с фильтром по датам)
    cursor.execute(f'''
        SELECT events.date, types.name AS type, drivers.name AS driver, events.amount, events.dolg, events.description
        FROM events
        LEFT JOIN types ON events.typeId = types.id
        LEFT JOIN drivers ON events.driverId = drivers.id
        WHERE events.carId = %s{date_filter}
        ORDER BY events.date DESC
    ''', tuple(params))
    events = cursor.fetchall()
    # Итоговая строка по событиям и зарядкам для этой машины
    # Считаем суммы по событиям
    income = sum(event['amount'] or 0 for event in events if event['amount'] and event['amount'] > 0)
    tax = int(income * 0.05)
    outcome = sum(event['amount'] or 0 for event in events if event['amount'] and event['amount'] < 0)
    debt = sum(event['dolg'] or 0 for event in events)
    total = int(income * 0.95 + outcome)  # Общая прибыль по событиям
    # Получаем kwh по зарядкам
    kwh = 0
    cursor2 = conn.cursor(dictionary=True)
    charge_date_filter = ''
    charge_params = [car_id]
    if start_date and end_date:
        charge_date_filter = ' AND charges.date BETWEEN %s AND %s'
        charge_params.append(start_date)
        charge_params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))
    cursor2.execute(f'''SELECT SUM(COALESCE(kwh, 0)) as kwh FROM charges WHERE carId = %s{charge_date_filter}''', tuple(charge_params))
    kwh_row = cursor2.fetchone()
    kwh = int((kwh_row['kwh'] or 0) * 10)
    cursor2.close()
    # Общая с учетом kwh
    total_with_kwh = total - kwh
    percent = car['percentage'] or 0
    commission = int(total_with_kwh * percent / 100) if total_with_kwh > 0 else 0
    to_investor = int(total_with_kwh * (100 - percent) / 100) if total_with_kwh > 0 else total_with_kwh
    events_total = {
        'count': len(events),
        'income': income,
        'tax': tax,
        'kwh': kwh,
        'outcome': outcome,
        'debt': debt,
        'total': total_with_kwh,
        'percent': percent,
        'commission': commission,
        'to_investor': to_investor
    }
    cursor.close()
    conn.close()
    return render_template(
        'car_details.html',
        car=car,
        events=events,
        events_total=events_total,
        start_date=start_date,
        end_date=end_date
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

@app.route('/car/<int:car_id>/pdf')
@login_required
def car_details_pdf(car_id):
    # Получите данные по машине и событиям (скопируйте логику из car_details)
    conn = mysql.connector.connect(**db_config)
    cursor = conn.cursor(dictionary=True)
    cursor.execute('SELECT * FROM cars WHERE id = %s AND investorId = %s', (car_id, current_user.id))
    car = cursor.fetchone()
    if not car:
        cursor.close()
        conn.close()
        return "Машина не найдена", 404

    start_date = request.args.get('start_date')
    end_date = request.args.get('end_date')
    date_filter = ''
    params = [car_id]
    if start_date and end_date:
        date_filter = ' AND events.date BETWEEN %s AND %s'
        params.append(start_date)
        from datetime import datetime
        end_date_dt = datetime.strptime(end_date, '%Y-%m-%d')
        end_date_dt = end_date_dt.replace(hour=23, minute=59, second=59)
        params.append(end_date_dt.strftime('%Y-%m-%d %H:%M:%S'))

    cursor.execute(f'''
        SELECT events.date, types.name AS type, drivers.name AS driver, events.amount, events.dolg, events.description
        FROM events
        LEFT JOIN types ON events.typeId = types.id
        LEFT JOIN drivers ON events.driverId = drivers.id
        WHERE events.carId = %s{date_filter}
        ORDER BY events.date DESC
    ''', tuple(params))
    events = cursor.fetchall()
    # ... (расчёт итогов, как в car_details) ...

    # Пример генерации PDF
    pdf = FPDF()
    pdf.add_page()
    pdf.add_font('DejaVu', '', 'static/fonts/ttf/DejaVuSans.ttf', uni=True)
    pdf.set_font('DejaVu', '', 6)
    pdf.cell(0, 10, txt=f"Отчет по машине {car['model']} ({car['licensePlate']})", ln=True, align='C')
    pdf.ln(5)
    # Таблица событий
    headers = ["Дата", "Тип", "Водитель", "Сумма", "Долг", "Описание"]
    col_widths = [30, 25, 30, 20, 20, 60]
    for i, h in enumerate(headers):
        pdf.cell(col_widths[i], 8, h, 1)
    pdf.ln()
    for event in events:
        pdf.cell(col_widths[0], 8, str(event['date']), 1)
        pdf.cell(col_widths[1], 8, str(event['type']), 1)
        pdf.cell(col_widths[2], 8, str(event['driver']), 1)
        pdf.cell(col_widths[3], 8, str(event['amount']), 1)
        pdf.cell(col_widths[4], 8, str(event['dolg']), 1)
        pdf.cell(col_widths[5], 8, str(event['description']), 1)
        pdf.ln()
    response = make_response(pdf.output(dest='S').encode('latin-1', 'replace'))
    response.headers['Content-Type'] = 'application/pdf'
    response.headers['Content-Disposition'] = f'attachment; filename=car_{car_id}_report.pdf'
    return response

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True) 