import requests
import time
import sys

# Define some constants
MAX_BUFFER_SIZE = 32 * 1024
IPServidor = "127.0.0.1"
cupom = "1111111111"
DataFiscal = "20230809"
HoraFiscal = "133659"

# Base URL for the Flask server
base_url = "http://localhost:5000"

# Call the endpoints
def configura_int_sit_tef_interativo():
    url = f"{base_url}/configura"
    data = {
        "IPServidor": IPServidor,
        "IdLoja": "00000000",
        "IdTerminal": "IP000001",
        "Reservado": None
    }
    response = requests.post(url, json=data)
    print("ConfiguraIntSiTefInterativo:", response.text)
    sys.stdout.flush()

def configura_int_sit_tef_interativo_ex():
    url = f"{base_url}/configura_ex"
    data = {
        "IPServidor": IPServidor,
        "IdLoja": "00000000",
        "IdTerminal": "IP000001",
        "Reservado": None,
        "ParametrosAdicionais": "[ParmsClient=1=31406434895111;2=12523654185985]"
    }
    response = requests.post(url, json=data)
    print("ConfiguraIntSiTefInterativo:", response.text)
    sys.stdout.flush()

def inicia_funcao_si_tef_interativo():
    url = f"{base_url}/inicia"
    data = {
        "funcao": 3,
        "valor": "10,00",
        "cupom_fiscal": cupom,
        "data_fiscal": DataFiscal,
        "hora_fiscal": HoraFiscal,
        "operador": "FULANO",
        "param_adic": ""
    }
    response = requests.post(url, json=data)
    print("IniciaFuncaoSiTefInterativo:", response.text)
    sys.stdout.flush()
    return response.json()

def continua_funcao_si_tef_interativo(comando, buffer_data):
    url = f"{base_url}/continua"
    data = {
        "comando": comando,
        "tipo_campo": None,
        "tam_minimo": None,
        "tam_maximo": None,
        "buffer": buffer_data,
        "tam_buffer": MAX_BUFFER_SIZE,
        "continua": 0
    }
    response = requests.post(url, json=data)
    print("ContinuaFuncaoSiTefInterativo:", response.text)
    sys.stdout.flush()
    return response.json()

def finaliza_funcao_si_tef_interativo():
    url = f"{base_url}/finaliza"
    data = {
        "confirma": 1,
        "cupom_fiscal": cupom,
        "data_fiscal": DataFiscal,
        "hora_fiscal": HoraFiscal,
        "param_adic": None
    }
    response = requests.post(url, json=data)
    print("FinalizaFuncaoSiTefInterativo:", response.text)
    sys.stdout.flush()

# Call the functions
configura_int_sit_tef_interativo_ex()
inicia_response = inicia_funcao_si_tef_interativo()
res = inicia_response.get('result')
buffer_data = ""

while res == 10000:
    comando = 0
    resposta_continua = continua_funcao_si_tef_interativo(comando, buffer_data)
    res = resposta_continua.get('result')
    comando = resposta_continua.get('comando')
    print(comando)
    if comando == 21:
        buffer_data = input("Escolha a opc: ")

finaliza_funcao_si_tef_interativo()
