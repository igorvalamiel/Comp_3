#include <iostream>
#include <tuple>
#include <utility>

using namespace std;

template <typename F, typename... Args>
auto call(F&& f, Args&&... args) -> decltype(forward<F>(f)(forward<Args>(args)...)) {
    return forward<F>(f)(forward<Args>(args)...);
}

template <typename M, typename T, typename... Args>
auto call(M&& m, T&& obj, Args&&... args) -> decltype((forward<T>(obj).*forward<M>(m))(forward<Args>(args)...)) {
    return (forward<T>(obj).*forward<M>(m))(forward<Args>(args)...);
}

template <typename M, typename T, typename... Args>
auto call(M&& m, T* obj, Args&&... args) -> decltype((obj->*forward<M>(m))(forward<Args>(args)...)) {
    return (obj->*forward<M>(m))(forward<Args>(args)...);
}

template <typename F, typename... ArgsGuardados>
class Bind {
public:
    Bind(F f, ArgsGuardados... args) : Func(f), guardados(args...) {}

    template <typename... ArgsNovos>
    auto operator()(ArgsNovos&&... args) {
        if constexpr (requires { call(Func, get<ArgsGuardados>(guardados)..., forward<ArgsNovos>(args)...); }) {
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
        return call(Func, get<Is>(guardados)..., forward<ArgsNovos>(args)...);
    }

    template <size_t... Is, typename... ArgsNovos>
    auto acumula(index_sequence<Is...>, ArgsNovos&&... args) {
        return Bind<F, ArgsGuardados..., decay_t<ArgsNovos>...>(
            Func, get<Is>(guardados)..., forward<ArgsNovos>(args)...
        );
    }
};

template <typename F, typename... Args>
auto bind(F Func, Args... args) {
    return Bind<F, decay_t<Args>...>(Func, args...);
}