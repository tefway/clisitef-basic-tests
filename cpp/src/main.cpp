#include "stdafx.hpp"
#include <clisitef/clisitef_wrapper.hpp>
#include <string_view>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string_view libraryPath =
#ifdef _WIN32
        "CliSiTef32I.dll";
#else
        "./libclisitef.so";
#endif

    if (!LoadSiTefLibrary(libraryPath)) {
        std::cerr << "Erro ao carregar a biblioteca " << libraryPath
                  << std::endl;
        return 1;
    }

    using namespace std::string_view_literals;

    const auto IPServidor = "127.0.0.1"sv;

    int res = ConfiguraIntSiTefInterativo(IPServidor.data(), "00000000",
                                          "IP000001", nullptr);

    std::cout << res << std::endl;

    auto pinpadPresente = VerificaPresencaPinPad();
    std::cout << "Pinpad presente?  " << pinpadPresente << std::endl;

    if (pinpadPresente > 0) {
        int respEscritaMsg = EscreveMensagemPermanentePinPad("TEFWAY");

        std::cout << "Resposta de escrever mensagem: " << respEscritaMsg
                  << std::endl;
    }

    std::string cupom = "1111111111";
    std::string DataFiscal = "20230809";
    std::string HoraFiscal = "133659";

    const char *placeholder = "";

    res = IniciaFuncaoSiTefInterativo(3, "10,00", cupom.c_str(),
                                      DataFiscal.c_str(), HoraFiscal.c_str(),
                                      "FULANO", placeholder);

    std::cout << "IniciaFuncaoSiTefInterativo: " << res << std::endl;

    std::vector<char> buffer;
    buffer.resize(32 * 1024);
    buffer[0] = '\0';
    std::string userOpc;

    while (res == 10000) {
        int comando = 0;
        long TipoCampo{};
        short TamMinimo{}, TamMaximo{};

        res = ContinuaFuncaoSiTefInterativo(&comando, &TipoCampo, &TamMinimo,
                                            &TamMaximo, buffer.data(),
                                            static_cast<int>(buffer.size()), 0);
        std::cout << "ContinuaFuncaoSiTefInterativo: " << res << " comando "
                  << comando << "  tipo campo " << TipoCampo << "  tam minimo "
                  << TamMinimo << "  tam maximo " << TamMaximo << "   buffer "
                  << buffer.data() << std::endl;

        if (comando == 21) {
            std::cout << "Escolha a opc: " << buffer.data() << " ";

            userOpc.clear();
            std::cin >> userOpc;
            std::copy_n(userOpc.c_str(),
                        std::min(buffer.size(), userOpc.size() + 1),
                        buffer.begin());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    FinalizaFuncaoSiTefInterativo(1, cupom.c_str(), DataFiscal.c_str(),
                                  HoraFiscal.c_str(), nullptr);
    return 0;
}
