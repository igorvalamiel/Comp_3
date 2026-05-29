#include <iostream>
#include <utility>

using namespace std;

template <typename F, typename... ArgsGuardados>
class Bind;

template <typename F, typename... ArgsGuardados>
Bind<F, ArgsGuardados...> bind_aux(F f, ArgsGuardados... args) {
    return Bind<F, ArgsGuardados...>(f, args...);
}

template <typename F, typename... ArgsGuardados>
class Bind {
public:
    Bind(F f, ArgsGuardados... args) : Func(f), guardados(args...) {}

    template <typename... ArgsNovos>
    auto operator()(ArgsNovos... args) {
        if constexpr (requires { Func(get<ArgsGuardados>(guardados)..., args...); }) {
            return chama_funcao(index_sequence_for<ArgsGuardados...>{}, args...);
        } else {
            return junta_e_cria_novo_bind(index_sequence_for<ArgsGuardados...>{}, args...);
        }
    }

private:
    F Func;
    tuple<ArgsGuardados...> guardados;

    template <size_t... Is, typename... ArgsNovos>
    auto chama_funcao(index_sequence<Is...>, ArgsNovos... args) {
        return Func(get<Is>(guardados)..., args...);
    }

    template <size_t... Is, typename... ArgsNovos>
    auto junta_e_cria_novo_bind(index_sequence<Is...>, ArgsNovos... args) {
        return bind_aux(Func, get<Is>(guardados)..., args...);
    }
};

template <typename F, typename... Args>
auto bind(F Func, Args... args) {
    return Bind<F, Args...>(Func, args...);
}