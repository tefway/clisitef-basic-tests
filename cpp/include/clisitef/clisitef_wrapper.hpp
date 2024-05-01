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
typedef void(STDCALL *FinalizaFuncaoSiTefInterativo_t)(
    short Confirma, const char *CupomFiscal, const char *DataFiscal,
    const char *HoraFiscal, const char *ParamAdic);

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

#endif // CLISITEF_WRAPPER_HPP
