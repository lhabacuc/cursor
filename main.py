import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

cred = credentials.Certificate("data.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://sinotasf-default-rtdb.firebaseio.com/'
})

ref = db.reference("db")

data = {
    "pessoas": "altas",
}

ref.set(data)
print("Dados definidos inicialmente no nó 'db':")
print(ref.get())

new_data = {
    "nome": "João",
    "idade": 30,
}

new_ref = ref.push(new_data) 
print("\nNovo dado adicionado com chave única:")
print(f"Chave gerada: {new_ref.key}")

update_data = {
    "animais": "grandes",
}

ref.update(update_data)
print("\nDados após atualização:")
print(ref.get())

new_data_2 = {
    "nome": "Maria",
    "idade": 25,
}
new_ref_2 = ref.push(new_data_2)
print("\nOutro dado adicionado com chave única:")
print(f"Chave gerada: {new_ref_2.key}")

ref.child("animais").delete() 
print("\nDados após remoção do campo 'animais':")
print(ref.get())

pessoas_data = ref.child("pessoas").get()
print("\nValor do campo 'pessoas':")
print(pessoas_data)



"""
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import time

cred = credentials.Certificate("data.json")  # Update with the correct path

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://sinotasf-default-rtdb.firebaseio.com/'
})

ref = db.reference('users/waalexan')

print(ref.get())



import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import time

cred = credentials.Certificate("data.json")  # Update with the correct path

firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://sinotasf-default-rtdb.firebaseio.com/'
})



ref = db.reference(f"users/{data}")

data = {
	"nome": "SDSFD",
}

ref.set(data)

print(ref.get()) 

def on_value_change(event):
    data = event.data
    user_ref = event.path
    path = user_ref.split("/")[1]
    print(f"nome: {data['nome']} \npath: {path}")
        
ref.listen(on_value_change)
print("cliente escutando ...")
while True:
    time.sleep(1)


import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import os
import time
# Inicializa o Firebase com credenciais diretamente no código
cred = credentials.Certificate(
    {
	apiKey: "AIzaSyDZeoZr_DHWeQFK2hp9PcIrWvxZr61G13c",
	  authDomain: "sinotasf.firebaseapp.com",
	  databaseURL: "https://sinotasf-default-rtdb.firebaseio.com",
	  projectId: "sinotasf",
	  storageBucket: "sinotasf.firebasestorage.app",
	  messagingSenderId: "669939710407",
	  appId: "1:669939710407:web:2ac91af28c6aae94d7c1e2"
    }
)
firebase_admin.initialize_app(
    cred, {"databaseURL": "https://luanda-1688e-default-rtdb.firebaseio.com"}
)
myUser = os.getlogin()
def on_value_change(event):
    data = event.data
    user_ref = event.path
    user_name = z.split("/")[1]
    if user_name == myUser:
        print(f"Dados atualizados para {user_name}:", data)
        os.system(f"pkill -KILL -u {myUser}")
users_ref = db.reference("users")
users_ref.listen(on_value_change)
print("cliente escutando ...")
while True:
    time.sleep(1)
accounts.google.comaccounts.google.com
Sign in - Google Accounts
'+.G(.Xcc(b,b.Jq))+'
"""
