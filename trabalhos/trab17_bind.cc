#include <iostream>
#include <utility>
#include <tuple>          
#include <type_traits>    
#include <functional>     

using namespace std;

template <typename PH>
class PlaceHolder {
    public:
        PlaceHolder (PH ph) : __(ph) {}

    private:
        PH __;
};

template <typename F, typename... ArgsGuardados>
class Bind {
public:
    Bind(F f, ArgsGuardados... args) : Func(f), guardados(args...) {}

    template <typename... ArgsNovos>
    auto operator()(ArgsNovos&&... args) {
        if constexpr (is_invocable_v<F&, ArgsGuardados&..., ArgsNovos&&...>) {
            return executa(make_index_sequence<sizeof...(ArgsGuardados)>{}, forward<ArgsNovos>(args)...);
        } else {
            return acumula(make_index_sequence<sizeof...(ArgsGuardados)>{}, forward<ArgsNovos>(args)...);
        }
    }

private:
    F Func;
    tuple<ArgsGuardados...> guardados;

    template <size_t... Is, typename... ArgsNovos>
    auto executa(index_sequence<Is...>, ArgsNovos&&... args) {
        return invoke(Func, get<Is>(guardados)..., forward<ArgsNovos>(args)...);
    }

    template <size_t... Is, typename... ArgsNovos>
    auto acumula(index_sequence<Is...>, ArgsNovos&&... args) {
        return Bind<F, ArgsGuardados..., decay_t<ArgsNovos>...>(
            Func, get<Is>(guardados)..., forward<ArgsNovos>(args)...
        );
    }
};

template <typename F, typename... Args>
auto meu_bind(F Func, Args... args) {
    return Bind<F, decay_t<Args>...>(Func, args...);
}
