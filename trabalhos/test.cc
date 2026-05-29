#include <iostream>
#include <utility>

using namespace std;

// Declaramos a classe Bind na frente para que o operator() possa conhecê-la
template <typename F, typename... ArgsGuardados>
class Bind;

// Função auxiliar que cria o Bind de forma limpa
template <typename F, typename... ArgsGuardados>
Bind<F, ArgsGuardados...> bind_aux(F f, ArgsGuardados... args) {
    return Bind<F, ArgsGuardados...>(f, args...);
}

template <typename F, typename... ArgsGuardados>
class Bind {
public:
    // Construtor que aceita a função e QUALQUER quantidade de argumentos guardados até agora
    Bind(F f, ArgsGuardados... args) : Func(f), guardados(args...) {}

    // O operador() mágico
    template <typename... ArgsNovos>
    auto operator()(ArgsNovos... args) {
        // Se a função PODE ser chamada com os argumentos guardados + os novos:
        if constexpr (requires { Func(std::get<ArgsGuardados>(guardados)..., args...); }) {
            // Executa a função original
            return chama_funcao(std::index_sequence_for<ArgsGuardados...>{}, args...);
        } else {
            // Caso contrário, NÃO executa. Cria um NOVO Bind acumulando os argumentos!
            return junta_e_cria_novo_bind(std::index_sequence_for<ArgsGuardados...>{}, args...);
        }
    }

private:
    F Func;
    std::tuple<ArgsGuardados...> guardados; // Guardamos os parâmetros em uma tupla

    // Auxiliar 1: Desembrulha a tupla e executa a função original
    template <size_t... Is, typename... ArgsNovos>
    auto chama_funcao(std::index_sequence<Is...>, ArgsNovos... args) {
        return Func(std::get<Is>(guardados)..., args...);
    }

    // Auxiliar 2: Desembrulha a tupla, junta com os novos argumentos e gera um novo Bind
    template <size_t... Is, typename... ArgsNovos>
    auto junta_e_cria_novo_bind(std::index_sequence<Is...>, ArgsNovos... args) {
        return bind_aux(Func, std::get<Is>(guardados)..., args...);
    }
};

// A função bind principal que o seu trabalho expõe
template <typename F, typename... Args>
auto bind(F Func, Args... args) {
    return Bind<F, Args...>(Func, args...);
}