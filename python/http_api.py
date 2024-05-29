#!/usr/bin/env python

import ctypes
import time
from ctypes import wintypes

# Incomplete code

# Define the function prototypes
ConfiguraIntSiTefInterativo_t = ctypes.WINFUNCTYPE(
    ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p
)
ConfiguraIntSiTefInterativoEx_t = ctypes.WINFUNCTYPE(
    ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p
)
VerificaPresencaPinPad_t = ctypes.WINFUNCTYPE(ctypes.c_int)
EscreveMensagemPinPad_t = ctypes.WINFUNCTYPE(ctypes.c_int, ctypes.c_char_p)
IniciaFuncaoSiTefInterativo_t = ctypes.WINFUNCTYPE(
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.c_char_p,
)
ContinuaFuncaoSiTefInterativo_t = ctypes.WINFUNCTYPE(
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_long),
    ctypes.POINTER(ctypes.c_short),
    ctypes.POINTER(ctypes.c_short),
    ctypes.c_char_p,
    ctypes.c_int,
    ctypes.c_int,
)
FinalizaFuncaoSiTefInterativo_t = ctypes.WINFUNCTYPE(
    None,
    ctypes.c_short,
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.c_char_p,
    ctypes.c_char_p,
)

# Load the DLL
clistefdll = ctypes.WinDLL("./CliSiTef64I.dll")

# Set up function prototypes
ConfiguraIntSiTefInterativo = ConfiguraIntSiTefInterativo_t(
    ("ConfiguraIntSiTefInterativo", clistefdll)
)
ConfiguraIntSiTefInterativoEx = ConfiguraIntSiTefInterativoEx_t(
    ("ConfiguraIntSiTefInterativoEx", clistefdll)
)
VerificaPresencaPinPad = VerificaPresencaPinPad_t(
    ("VerificaPresencaPinPad", clistefdll)
)
EscreveMensagemPermanentePinPad = EscreveMensagemPinPad_t(
    ("EscreveMensagemPermanentePinPad", clistefdll)
)
EscreveMensagemPinPad = EscreveMensagemPinPad_t(("EscreveMensagemPinPad", clistefdll))
IniciaFuncaoSiTefInterativo = IniciaFuncaoSiTefInterativo_t(
    ("IniciaFuncaoSiTefInterativo", clistefdll)
)
ContinuaFuncaoSiTefInterativo = ContinuaFuncaoSiTefInterativo_t(
    ("ContinuaFuncaoSiTefInterativo", clistefdll)
)
FinalizaFuncaoSiTefInterativo = FinalizaFuncaoSiTefInterativo_t(
    ("FinalizaFuncaoSiTefInterativo", clistefdll)
)


def sitef():
    # Define some constants
    MAX_BUFFER_SIZE = 32 * 1024
    IPServidor = b"127.0.0.1"
    cupom = "1111111111"
    DataFiscal = "20230809"
    HoraFiscal = "133659"

    # Call the functions
    res = ConfiguraIntSiTefInterativo(IPServidor, b"00000000", b"IP000001", None)
    print("ConfiguraIntSiTefInterativo:", res)

    pinpadPresente = VerificaPresencaPinPad()
    print("Pinpad presente?", pinpadPresente)

    if pinpadPresente > 0:
        respEscritaMsg = EscreveMensagemPermanentePinPad(b"TEFWAY")
        print("Resposta de escrever mensagem:", respEscritaMsg)

    placeholder = b""

    res = IniciaFuncaoSiTefInterativo(
        3,
        b"10,00",
        cupom.encode(),
        DataFiscal.encode(),
        HoraFiscal.encode(),
        b"FULANO",
        placeholder,
    )
    print("IniciaFuncaoSiTefInterativo:", res)

    buffer = ctypes.create_string_buffer(MAX_BUFFER_SIZE)
    while res == 10000:
        comando = wintypes.INT()
        TipoCampo = wintypes.LONG()
        TamMinimo = wintypes.SHORT()
        TamMaximo = wintypes.SHORT()

        res = ContinuaFuncaoSiTefInterativo(
            ctypes.byref(comando),
            ctypes.byref(TipoCampo),
            ctypes.byref(TamMinimo),
            ctypes.byref(TamMaximo),
            buffer,
            MAX_BUFFER_SIZE,
            0,
        )
        print(
            "ContinuaFuncaoSiTefInterativo:",
            res,
            "comando:",
            comando.value,
            "tipo campo:",
            TipoCampo.value,
            "tam minimo:",
            TamMinimo.value,
            "tam maximo:",
            TamMaximo.value,
            "buffer:",
            buffer.value.decode(),
        )

        if comando.value == 21:
            userOpc = input("Escolha a opc: ")
            buffer.value = userOpc.encode()

        time.sleep(0.01)

    FinalizaFuncaoSiTefInterativo(
        1, cupom.encode(), DataFiscal.encode(), HoraFiscal.encode(), None
    )


from flask import Flask, request, jsonify

app = Flask(__name__)


# Define the Configura endpoint to call ConfiguraIntSiTefInterativo
@app.route("/configura", methods=["POST"])
def configura_si_tef():
    # Parse request data
    data = request.json

    # Extract data from JSON payload
    IPServidor = data.get("IPServidor").encode()
    IdLoja = data.get("IdLoja").encode()
    IdTerminal = data.get("IdTerminal").encode()

    # Call ConfiguraIntSiTefInterativo
    res = ConfiguraIntSiTefInterativo(IPServidor, IdLoja, IdTerminal, None)

    return jsonify({"result": res})

@app.route('/configura_ex', methods=['POST'])
def configura_si_tef_ex():
    data = request.json

    IPServidor = data.get('IPServidor').encode()
    IdLoja = data.get('IdLoja').encode()
    IdTerminal = data.get('IdTerminal').encode()
    ParametrosAdicionais = data.get('ParametrosAdicionais').encode()

    res = ConfiguraIntSiTefInterativoEx(IPServidor, IdLoja, IdTerminal, ParametrosAdicionais)

    return jsonify({'result': res})

@app.route('/escreve_mensagem_pinpad', methods=['POST'])
def escreve_mensagem_pinpad():
    data = request.json
    mensagem = data.get('mensagem').encode()
    res = EscreveMensagemPinPad(mensagem)
    return jsonify({'result': res})

@app.route('/escreve_mensagem_permanente_pinpad', methods=['POST'])
def escreve_mensagem_permanente_pinpad():
    data = request.json
    mensagem = data.get('mensagem').encode()
    res = EscreveMensagemPermanentePinPad(mensagem)
    return jsonify({'result': res})

@app.route("/inicia", methods=["POST"])
def inicia_funcao_si_tef_interativo():
    # Parse request data
    data = request.json
    funcao = data.get("funcao", 0)
    valor = data.get("valor", "0,00")
    cupom_fiscal = data.get("cupom_fiscal", "")
    data_fiscal = data.get("data_fiscal", "")
    hora_fiscal = data.get("hora_fiscal", "")
    operador = data.get("operador", "")
    param_adic = data.get("param_adic", "")

    # Call the function with the parsed data
    res = IniciaFuncaoSiTefInterativo(
        funcao,
        valor.encode(),
        cupom_fiscal.encode(),
        data_fiscal.encode(),
        hora_fiscal.encode(),
        operador.encode(),
        param_adic.encode(),
    )

    # Return the result as JSON response
    return jsonify({"result": res})


@app.route("/continua", methods=["POST"])
def continua_funcao_si_tef_interativo():
    # Parse request data
    data = request.json
    buffer_data = data.get("buffer")
    MAX_BUFFER_SIZE = 32 * 1024

    comando = wintypes.INT()
    TipoCampo = wintypes.LONG()
    TamMinimo = wintypes.SHORT()
    TamMaximo = wintypes.SHORT()

    buffer = ctypes.create_string_buffer(MAX_BUFFER_SIZE)
    buffer.value = buffer_data.encode()

    res = ContinuaFuncaoSiTefInterativo(
        ctypes.byref(comando),
        ctypes.byref(TipoCampo),
        ctypes.byref(TamMinimo),
        ctypes.byref(TamMaximo),
        buffer,
        MAX_BUFFER_SIZE,
        0,
    )

    bufferdata = buffer.value.decode()
    print(
        "ContinuaFuncaoSiTefInterativo:",
        res,
        "comando:",
        comando.value,
        "tipo campo:",
        TipoCampo.value,
        "tam minimo:",
        TamMinimo.value,
        "tam maximo:",
        TamMaximo.value,
        "buffer:",
        bufferdata,
    )

    return jsonify(
        {
            "result": res,
            "comando": comando.value,
            "tipo_campo": TipoCampo.value,
            "tamanho_minimo": TamMinimo.value,
            "tamanho_maximo": TamMaximo.value,
            "mensagem": bufferdata,
        }
    )


# Define route
@app.route("/finaliza", methods=["POST"])
def finaliza_funcao_si_tef_interativo():
    # Parse request data
    data = request.json
    confirma = data.get("confirma")
    cupom_fiscal = data.get("cupom_fiscal")
    data_fiscal = data.get("data_fiscal")
    hora_fiscal = data.get("hora_fiscal")
    param_adic = data.get("param_adic")

    if param_adic is None:
        param_adic = ""

    # Call FinalizaFuncaoSiTefInterativo
    FinalizaFuncaoSiTefInterativo(
        ctypes.c_short(confirma),
        ctypes.c_char_p(cupom_fiscal.encode()),
        ctypes.c_char_p(data_fiscal.encode()),
        ctypes.c_char_p(hora_fiscal.encode()),
        ctypes.c_char_p(param_adic.encode()),
    )

    # Return JSON result
    return jsonify({"message": "FinalizaFuncaoSiTefInterativo called successfully"})


if __name__ == "__main__":
    app.run(debug=True)  # Run the server
