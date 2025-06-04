from flask import Flask, render_template, request, redirect, url_for, flash
from flask_login import LoginManager, UserMixin, login_user, login_required, logout_user, current_user
import mysql.connector
from datetime import datetime
import hmac
import hashlib

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
    # Проверяем, принадлежит ли машина текущему инвестору
    cursor.execute('SELECT * FROM cars WHERE id = %s AND investorId = %s', (car_id, current_user.id))
    car = cursor.fetchone()
    if not car:
        flash('Машина не найдена')
        cursor.close()
        conn.close()
        return redirect(url_for('dashboard'))
    cursor.close()
    conn.close()
    return render_template('car_details.html', car=car)

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

if __name__ == '__main__':
    app.run(debug=True) 