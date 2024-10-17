import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate('iot-project-1995-firebase-adminsdk-494ho-1678ec9676.json')
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://iot-project-1995-default-rtdb.firebaseio.com'
})

ref = db.reference('test')
data = ref.get()
print(data)  # Esto debería mostrar los datos en la consola
