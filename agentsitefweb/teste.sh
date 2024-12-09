#!/bin/bash

# set -xe

# Configurações iniciais
base_url="${base_url:-https://127.0.0.1/agente/clisitef}"
session_id="${session_id:-}"
IPServidor="${IPServidor:-127.0.0.1}"
cupom="${cupom:-1111111111}"
DataFiscal="${DataFiscal:-20230809}"
HoraFiscal="${HoraFiscal:-133659}"
valor="${valor:-10,00}"
store_id="${store_id:-00000000}"
terminal_id="${terminal_id:-IP000001}"

# Funções para os endpoints da API com sessão

create_session() {
    echo "Criando sessão..."
    url="$base_url/session"
    data="sitefIp=$IPServidor&storeId=$store_id&terminalId=$terminal_id"
    response=$(curl -s -X POST -d "$data" "$url" -k)
    session_id=$(echo "$response" | jq -r '.sessionId')
    echo "Sessão criada: $session_id"
}

start_transaction() {
    url="$base_url/startTransaction"
    data="sessionId=$session_id&functionId=3&trnAmount=$valor&taxInvoiceNumber=$cupom&taxInvoiceDate=$DataFiscal&taxInvoiceTime=$HoraFiscal&cashierOperator=FULANO&trnAdditionalParameters="
    response=$(curl -s -X POST -d "$data" "$url" -k)

    # {"serviceStatus": 0, "sessionId": "dbf32f72", "clisitefStatus": 10000}

    # Captura o serviceStatus
    service_status=$(echo "$response" | jq -r '.serviceStatus')

    if [ "$service_status" -eq 0 ]; then
        echo "$response" # Imprime a resposta para ser capturada no fluxo principal
    else
        service_message=$(echo "$response" | jq -r '.serviceMessage')
        echo "Erro ao iniciar transação: $service_message" >&2
        exit 1 # Finaliza o script em caso de erro
    fi
}

continue_transaction() {
    buffer_data=$1
    continua_code=$2
    url="$base_url/continueTransaction"
    data="sessionId=$session_id&data=$buffer_data&continue=$continua_code"
    response=$(curl -s -X POST -d "$data" "$url" -k)
    echo "$response"
}

finish_transaction() {
    echo "Finalizando transação..."
    url="$base_url/finishTransaction"
    data="sessionId=$session_id&taxInvoiceNumber=$cupom&taxInvoiceDate=$DataFiscal&taxInvoiceTime=$HoraFiscal&confirm=1"
    response=$(curl -s -X POST -d "$data" "$url" -k)
    echo "$response"
}

# Fluxo de Execução
create_session

start_response=$(start_transaction)
echo "$start_response"
echo "$start_response"
res=$(echo "$start_response" | jq -r '.clisitefStatus')
echo "$start_response"
echo "$start_response"
buffer_data=""
continua_code=0

function ctrl_c() {
    echo "CTRL+C capturado. Encerrando..."
    continua_code=-1
    continue_transaction "$buffer_data" "$continua_code"
}

trap ctrl_c INT

# Loop para continuar a transação
while [ "$res" -eq 10000 ]; do
    response=$(continue_transaction "$buffer_data" "$continua_code")
    echo "$response"
    res=$(echo "$response" | jq -r '.clisitefStatus')
    mensagem=$(echo "$response" | jq -r '.data')
    comando=$(echo "$response" | jq -r '.commandId')

    echo "Comando: $comando, Res: $res, Mensagem: $mensagem"

    if [ "$comando" -eq 21 ]; then
        echo -n "Escolha uma opção: "
        read buffer_data
    fi
done

finish_transaction

