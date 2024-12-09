const base_url = "https://127.0.0.1/agente/clisitef";
let session_id = "";
const IPServidor = "127.0.0.1";
const cupom = "1111111111";
const DataFiscal = "20230809";
const HoraFiscal = "133659";
const valor = "10,00";
const store_id = "00000000";
const terminal_id = "IP000001";

async function createSession() {
    console.log("Criando sessão...");
    const url = `${base_url}/session`;
    const data = new URLSearchParams({
        sitefIp: IPServidor,
        storeId: store_id,
        terminalId: terminal_id,
    });

    try {
        const response = await fetch(url, {
            method: "POST",
            body: data,
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
        });
        const json = await response.json();
        if (json.serviceStatus === 0) {
            session_id = json.sessionId;
            console.log(`Sessão criada: ${session_id}`);
        } else {
            throw new Error(`Erro ao criar sessão: ${json.serviceMessage}`);
        }
    } catch (error) {
        console.error(error.message);
    }
}

async function startTransaction() {
    console.log("Iniciando transação...");
    const url = `${base_url}/startTransaction`;
    const data = new URLSearchParams({
        sessionId: session_id,
        functionId: "3",
        trnAmount: valor,
        taxInvoiceNumber: cupom,
        taxInvoiceDate: DataFiscal,
        taxInvoiceTime: HoraFiscal,
        cashierOperator: "FULANO",
        trnAdditionalParameters: "",
    });

    try {
        const response = await fetch(url, {
            method: "POST",
            body: data,
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
        });
        const json = await response.json();
        if (json.serviceStatus === 0) {
            console.log("Transação iniciada com sucesso.");
            return json;
        } else {
            throw new Error(`Erro ao iniciar transação: ${json.serviceMessage}`);
        }
    } catch (error) {
        console.error(error.message);
    }
}

async function continueTransaction(buffer_data = "", continua_code = 0) {
    console.log("Continuando transação...");
    const url = `${base_url}/continueTransaction`;
    const data = new URLSearchParams({
        sessionId: session_id,
        data: buffer_data,
        continue: continua_code,
    });

    try {
        const response = await fetch(url, {
            method: "POST",
            body: data,
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
        });
        const json = await response.json();
        console.log("Resposta da transação continua:", json);
        return json;
    } catch (error) {
        console.error(error.message);
    }
}

async function finishTransaction() {
    console.log("Finalizando transação...");
    const url = `${base_url}/finishTransaction`;
    const data = new URLSearchParams({
        sessionId: session_id,
        taxInvoiceNumber: cupom,
        taxInvoiceDate: DataFiscal,
        taxInvoiceTime: HoraFiscal,
        confirm: "1",
    });

    try {
        const response = await fetch(url, {
            method: "POST",
            body: data,
            headers: { "Content-Type": "application/x-www-form-urlencoded" },
        });
        const json = await response.json();
        if (json.serviceStatus === 0) {
            console.log("Transação finalizada com sucesso.");
        } else {
            throw new Error(`Erro ao finalizar transação: ${json.serviceMessage}`);
        }
    } catch (error) {
        console.error(error.message);
    }
}

// Fluxo principal
(async function () {
    await createSession();

    const startResponse = await startTransaction();
    if (!startResponse) return;

    let res = startResponse.clisitefStatus;
    let buffer_data = "";
    let continua_code = 0;

    while (res === 10000) {
        const response = await continueTransaction(buffer_data, continua_code);
        if (!response) return;

        res = response.clisitefStatus;
        const mensagem = response.data;
        const comando = response.commandId;

        console.log(`Comando: ${comando}, Res: ${res}, Mensagem: ${mensagem}`);

        if (comando === 21) {
            buffer_data = prompt("Escolha uma opção:") || "";
        }
    }

    await finishTransaction();
})();
