from flask import Flask, render_template
import firebase_admin
from firebase_admin import credentials, db

def initialize_firebase():
    # Cargar las credenciales
    cred = credentials.Certificate('iot-project-1995-firebase-adminsdk-494ho-1678ec9676.json')
    
    # Inicializar la aplicación
    firebase_admin.initialize_app(cred, {

       
        'databaseURL': ' https://iotproyceto-default-rtdb.firebaseio.com/'
    })

# Inicializa Firebase antes de crear la aplicación Flask
initialize_firebase()

app = Flask(__name__)

@app.route('/')
def index():
    ref = db.reference('test')
    data = ref.get()
    print(data)  # Agregado para depuración
    return render_template('index.html', data=data)

if __name__ == '__main__':
    app.run(debug=True)
