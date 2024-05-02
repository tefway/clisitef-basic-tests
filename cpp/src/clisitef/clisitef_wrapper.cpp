#include <cassert>
#include <clisitef/clisitef_wrapper.hpp>

#ifdef _WIN32 // Windows specific
HMODULE clistefdll{};
#else // Linux specific

#include <dlfcn.h>

void *clistefdll{};
#endif

ConfiguraIntSiTefInterativo_t ConfiguraIntSiTefInterativo{};
VerificaPresencaPinPad_t VerificaPresencaPinPad{};
EscreveMensagemPinPad_t EscreveMensagemPermanentePinPad{};
EscreveMensagemPinPad_t EscreveMensagemPinPad{};
IniciaFuncaoSiTefInterativo_t IniciaFuncaoSiTefInterativo{};
ContinuaFuncaoSiTefInterativo_t ContinuaFuncaoSiTefInterativo{};
FinalizaFuncaoSiTefInterativo_t FinalizaFuncaoSiTefInterativo{};

#ifdef _WIN32
#define LOAD_FUNCTION(name)                                                    \
    name = reinterpret_cast<decltype(name)>(                                   \
        reinterpret_cast<void *>(GetProcAddress(clistefdll, #name)));          \
    assert(name != nullptr);
#else
#define LOAD_FUNCTION(name)                                                    \
    name = reinterpret_cast<decltype(name)>(dlsym(clistefdll, #name));         \
    assert(name != nullptr);
#endif

bool LoadSiTefLibrary(std::string_view path) {
#ifdef _WIN32
    clistefdll = LoadLibraryA(path.data());
#else
    clistefdll = dlopen(path.data(), RTLD_LAZY);
#endif

    if (clistefdll == nullptr) {
        return false;
    }

    LOAD_FUNCTION(ConfiguraIntSiTefInterativo)
    LOAD_FUNCTION(VerificaPresencaPinPad)
    LOAD_FUNCTION(EscreveMensagemPermanentePinPad)
    LOAD_FUNCTION(EscreveMensagemPinPad)
    LOAD_FUNCTION(IniciaFuncaoSiTefInterativo)
    LOAD_FUNCTION(ContinuaFuncaoSiTefInterativo)
    LOAD_FUNCTION(FinalizaFuncaoSiTefInterativo)

    return true;
}
