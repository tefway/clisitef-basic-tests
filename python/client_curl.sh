#!/bin/bash

# Define some constants
IPServidor="127.0.0.1"
cupom="1111111111"
DataFiscal="20230809"
HoraFiscal="133659"
ParametrosAdicionais="[ParmsClient=1=31406434895111;2=12523654185985]"

# Base URL for the Flask server

if [[ -z "$base_url" ]]; then
    base_url="http://localhost:5000"
fi

# Call the endpoints
configura_int_sit_tef_interativo() {
    url="$base_url/configura"
    data='{"IPServidor":"'$IPServidor'","IdLoja":"00000000","IdTerminal":"IP000001","Reservado":null}'
    response=$(curl -s -X POST -H 'Content-Type: application/json' -d "$data" "$url")
    echo "$response"
}

configura_int_sit_tef_interativo_ex() {
    url="$base_url/configura_ex"
    data='{"IPServidor":"'$IPServidor'","IdLoja":"00000000","IdTerminal":"IP000001","Reservado":null, "ParametrosAdicionais":"'$ParametrosAdicionais'"}'
    response=$(curl -s -X POST -H 'Content-Type: application/json' -d "$data" "$url")
    echo "$response"
}

escreve_mensagem_permanente_pinpad() {
    url="$base_url/escreve_mensagem_permanente_pinpad"
    data='{"mensagem":"'${1}'"}'
    response=$(curl -s -X POST -H 'Content-Type: application/json' -d "$data" "$url")
    echo "$response"
}

inicia_funcao_si_tef_interativo() {
    url="$base_url/inicia"
    data='{"funcao":3,"valor":"10,00","cupom_fiscal":"'$cupom'","data_fiscal":"'$DataFiscal'","hora_fiscal":"'$HoraFiscal'","operador":"FULANO","param_adic":""}'

    response=$(curl -s -X POST -H 'Content-Type: application/json' -d "$data" "$url")
    echo "$response"
}

continua_funcao_si_tef_interativo() {
    url="$base_url/continua"
    data='{"buffer":"'$1'"}'
    response=$(curl -s -X POST -H 'Content-Type: application/json' -d "$data" "$url")
    echo "$response"
}

finaliza_funcao_si_tef_interativo() {
    url="$base_url/finaliza"
    data='{"confirma":1,"cupom_fiscal":"'$cupom'","data_fiscal":"'$DataFiscal'","hora_fiscal":"'$HoraFiscal'","param_adic":null}'
    response=$(curl -s -X POST -H 'Content-Type: application/json' -d "$data" "$url")
    echo "$response"
}

# Call the functions
configura_int_sit_tef_interativo

escreve_mensagem_permanente_pinpad "TESTE DE MENSAGEM PERMANENTE"

inicia_response=$(inicia_funcao_si_tef_interativo)
res=$(echo "$inicia_response" | jq -r '.result')
buffer_data=""

while [ "$res" -eq 10000 ]; do
    comando=0
    resposta_continua=$(continua_funcao_si_tef_interativo "$buffer_data")

    res=$(echo "$resposta_continua" | jq -r '.result')
    mensagem=$(echo "$resposta_continua" | jq -r '.mensagem')
    tamanho_maximo=$(echo "$resposta_continua" | jq -r '.tamanho_maximo')
    tamanho_minimo=$(echo "$resposta_continua" | jq -r '.tamanho_minimo')
    tipo_campo=$(echo "$resposta_continua" | jq -r '.tipo_campo')
    comando=$(echo "$resposta_continua" | jq -r '.comando')

    echo "$comando $res $mensagem $tamanho_maximo $tamanho_minimo $tipo_campo"
    if [ "$comando" -eq 21 ]; then
        echo -n "Escolha a opc: "
        read buffer_data
    fi
done

finaliza_funcao_si_tef_interativo
