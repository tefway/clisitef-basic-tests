#pragma once
#ifndef CLISITEF_WRAPPER_HPP
#define CLISITEF_WRAPPER_HPP

#include <string_view>

#ifdef _WIN32 // Windows specific
#include <windows.h>
#define STDCALL __stdcall
#else // Linux specific
#define STDCALL
#endif

typedef int(STDCALL *ConfiguraIntSiTefInterativo_t)(const char *IPSiTef,
                                                    const char *IdLoja,
                                                    const char *IdTerminal,
                                                    const char *Reservado);
typedef int(STDCALL *VerificaPresencaPinPad_t)();
typedef int(STDCALL *EscreveMensagemPinPad_t)(const char *Mensagem);
typedef int(STDCALL *IniciaFuncaoSiTefInterativo_t)(
    int Funcao, const char *Valor, const char *CupomFiscal,
    const char *DataFiscal, const char *HoraFiscal, const char *Operador,
    const char *ParamAdic);
typedef int(STDCALL *ContinuaFuncaoSiTefInterativo_t)(
    int *Comando, long *TipoCampo, short *TamMinimo, short *TamMaximo,
    char *Buffer, int TamBuffer, int Continua);
typedef void(STDCALL *FinalizaFuncaoSiTefInterativo_t)(short Confirma,
                                                       const char *CupomFiscal,
                                                       const char *DataFiscal,
                                                       const char *HoraFiscal,
                                                       const char *ParamAdic);

#ifdef _WIN32 // Windows specific
extern HMODULE clistefdll;
#else // Linux specific
extern void *clistefdll;
#endif

extern ConfiguraIntSiTefInterativo_t ConfiguraIntSiTefInterativo;
extern VerificaPresencaPinPad_t VerificaPresencaPinPad;
extern EscreveMensagemPinPad_t EscreveMensagemPermanentePinPad;
extern EscreveMensagemPinPad_t EscreveMensagemPinPad;
extern IniciaFuncaoSiTefInterativo_t IniciaFuncaoSiTefInterativo;
extern ContinuaFuncaoSiTefInterativo_t ContinuaFuncaoSiTefInterativo;
extern FinalizaFuncaoSiTefInterativo_t FinalizaFuncaoSiTefInterativo;

bool LoadSiTefLibrary(std::string_view path);

namespace FuncoesSitef {
/*
 * @brief estes enums foram gerados automaticamente, portanto podem conter erros
 */
enum FunctionCode : int {
    PagamentoGenerico = 0,
    Cheque = 1,
    Debito = 2,
    Credito = 3,
    Fininvest = 4,
    CartaoBeneficio = 5,
    CreditoCentralizado = 6,
    CartaoCombustivel = 7,
    ParceleMaisRedecard = 8,
    BeneficioRefeicaoWappa = 10,
    BeneficioAlimentacaoWappa = 11,
    CartaoInfocard = 12,
    PayPass = 13,
    VendaCartaoGift = 15,
    DebitoParaPagamentoCarnê = 16,
    CreditoParaPagamentoCarnê = 17,
    Telemarketing = 100,
    AbreMenuTransacoesGerenciais = 110,
    TesteComunicacaoSiTef = 111,
    MenuReimpressao = 112,
    ReimpressaoComprovanteEspecifico = 113,
    ReimpressaoUltimoComprovante = 114,
    PreAutorizacao = 115,
    CapturaPreAutorizacao = 116,
    AjustePreAutorizacao = 117,
    ConsultaPreAutorizacao = 118,
    ConsultaTransacoesPendentesTerminal = 130,
    ConsultaTransacoesPendentesDocumentoFiscalEspecifico = 131,
    ConsultaBonus = 150,
    ConsultaSaldoCartaoPresente = 151,
    ConsultaSaldoCartaoGift = 152,
    ConsultasCartaoEMS = 160,
    VendasCartaoEMS = 161,
    CancelamentoNormal = 200,
    CancelamentoTelemarketing = 201,
    CancelamentoPreAutorizacao = 202,
    CancelamentoCapturaPreAutorizacao = 203,
    CancelamentoVendaCartaoCredito = 210,
    CancelamentoVendaCartaoDebito = 211,
    CancelamentoVendaCartaoCombustivel = 212,
    CancelamentoVendaCartaoGift = 213,
    CancelamentoConsultaBonus = 250,
    CancelamentoRecargaCartaoPresente = 251,
    CancelamentoAcumuloPontosCartaoBonus = 253,
    ResgatePontosCartaoBonus = 254,
    CancelamentoResgatePontosCartaoBonus = 255,
    AcumuloPontosCartaoBonus = 256,
    CancelamentoRecargaCartaoGift = 257,
    RecargaCartaoGift = 264,
    AtivacaoPagamentoVinculadoCartaoGift = 265,
    ConsultaCartaoGift = 266,
    AtivacaoCartaoGiftSemPagamento = 267,
    AtivacaoCartaoGiftComPagamento = 268,
    AtivacaoDesvinculadaCartaoGift = 269,
    CorrespondeBancario = 310,
    PagamentoContasComSaque = 311,
    ConsultaPagamentoDesvinculado = 312,
    PagamentoDesvinculado = 313,
    RecargaPrePagoCorbanSEComSaque = 314,
    SaqueParaPagamento = 315,
    CancelamentoPagamentoDesvinculado = 316,
    ConsultaLimitesCorrespondenteBancario = 317,
    RecargaPrePagoBradesco = 318,
    RecargaPrePagoBradescoDesvinculadaPagamento = 319,
    RecargaPrePagoCorbanSE = 320,
    RecargaPrePagoCorbanSEDesvinculadaPagamento = 321,
    DepositoIdentificado = 322,
    TransferenciaEntreContas = 323,
    PagueFacil = 324,
    RevalidacaoSenhaINSS = 325,
    VendaProdutoSemValor = 350,
    CancelamentoVendaProdutoSemValor = 351,
    ValeGas = 400,
    ValidacaoValeGas = 401,
    TrocoSurpresa = 410,
    AdesaoSeguro = 422,
    LeCartaoSeguro = 430,
    LeTrilhaChip = 431,
    ConsultaDetalhadaACSP = 500,
    ConsultaDetalhadaSerasa = 501,
    ConsultaSaldo = 600,
    ConsultaSaldoCartaoDebito = 601,
    ConsultaSaldoCartaoCredito = 602,
    SaqueCreditoTransferencia = 657,
    SaqueCredito = 658,
    MenuSaqueIBI = 660,
    ConsultaSaqueBancoIBI = 661,
    SaqueBancoIBI = 662,
    SaqueGetNet = 663,
    CancelamentoSaqueGetNet = 664,
    ResgatePontos = 665,
    EmissaoPontos = 667,
    CancelamentoEmissaoPontos = 668,
    CargaPrePago = 669,
    CancelamentoCargaPrePago = 670,
    ConsultaSaqueSaqueBancoIBI = 671,
    CancelamentoSaqueBancoIBI = 672,
    ConsultaSaldoPrePago = 680,
    SaqueDebito = 698,
    VendaOiPaggo = 700,
    CancelamentoOiPaggo = 701,
    PagamentoContas = 702,
    CancelamentoPagamentoCartaoBeneficio = 703,
    PagamentoFatura = 705,
    ConsultaParcelasCreditoAdm = 740,
    CargaTabelasPinpad2 = 770,
    CargaForcadaTabelasPinpadLocal = 771,
    CargaForcadaTabelasPinpadSiTef = 772,
    ObtencaoInformacoesPinpad3 = 775,
    RecargaCartaoCredito = 899,
    CancelamentoRecargaCartaoCredito = 900,
    AlteracaoPreAutorizacao = 913,
    CancelamentoDebitoPagamentoCarneRedeForcada = 928,
    Fechamento = 999,
    VoucherPapel = 1000
};
} // namespace FuncoesSitef

#endif // CLISITEF_WRAPPER_HPP
