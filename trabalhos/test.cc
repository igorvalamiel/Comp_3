#include <iostream>
#include <utility>
#include <tuple>          
#include <type_traits>    
#include <functional>     

using std::cout;

// O Nosso Placeholder simples e sua definição global
class PlaceHolder {};
constexpr PlaceHolder __ {};

// --- FERRAMENTAS AUXILIARES DE METAPROGRAMAÇÃO ---

template <typename Tuple, size_t... Is>
constexpr size_t count_placeholders_impl(std::index_sequence<Is...>) {
    return (0 + ... + (std::is_same_v<std::tuple_element_t<Is, std::decay_t<Tuple>>, PlaceHolder> ? 1 : 0));
}

template <typename Tuple, size_t UpTo>
constexpr size_t count_placeholders_before() {
    return count_placeholders_impl<Tuple>(std::make_index_sequence<UpTo>{});
}

// PASSO 4 CORRIGIDO: Usando operador ternário e dobra por vírgula válido no C++17
template <typename Tuple, size_t... Is>
constexpr size_t find_last_concrete_index(std::index_sequence<Is...>) {
    size_t last = 0;
    ( (last = (!std::is_same_v<std::tuple_element_t<Is, std::decay_t<Tuple>>, PlaceHolder> ? Is + 1 : last)), ... );
    return last;
}

template <typename Tuple, size_t... Is>
auto slice_tuple(Tuple&& t, std::index_sequence<Is...>) {
    return std::make_tuple(std::get<Is>(std::forward<Tuple>(t))...);
}

template <typename Tuple>
auto truncate_tuple(Tuple&& t) {
    using PureT = std::decay_t<Tuple>;
    constexpr size_t sz = std::tuple_size_v<PureT>;
    constexpr size_t last_idx = find_last_concrete_index<PureT>(std::make_index_sequence<sz>{});
    return slice_tuple(std::forward<Tuple>(t), std::make_index_sequence<last_idx>{});
}

template <typename F, typename Tuple, size_t... Is>
constexpr bool is_invocable_with_tuple_impl(std::index_sequence<Is...>) {
    return std::is_invocable_v<F&, std::tuple_element_t<Is, std::decay_t<Tuple>>...>;
}

template <typename F, typename Tuple>
constexpr bool is_invocable_with_tuple() {
    using PureT = std::decay_t<Tuple>;
    return is_invocable_with_tuple_impl<F, PureT>(std::make_index_sequence<std::tuple_size_v<PureT>>{});
}

// --- CLASSE BIND PRINCIPAL ---

template <typename F, typename StoredTuple>
class Bind {
private:
    F Func;
    StoredTuple guardados; 

public:
    Bind(F f, StoredTuple t) : Func(f), guardados(std::move(t)) {}

    template <typename... ArgsNovos>
    auto operator()(ArgsNovos&&... args) {
        auto novos_tuple = std::forward_as_tuple(std::forward<ArgsNovos>(args)...);
        
        using GuardadosT = std::decay_t<StoredTuple>;
        using NovosT = decltype(novos_tuple);
        
        constexpr size_t g_sz = std::tuple_size_v<GuardadosT>;
        constexpr size_t n_sz = std::tuple_size_v<NovosT>;
        constexpr size_t total_phs = count_placeholders_before<GuardadosT, g_sz>();
        
        constexpr size_t num_leftovers = (n_sz > total_phs) ? (n_sz - total_phs) : 0;

        auto substituted_tuple = substitute_and_append(
            guardados, 
            novos_tuple, 
            std::make_index_sequence<g_sz>{}, 
            std::make_index_sequence<num_leftovers>{}
        );

        auto tupla_final = truncate_tuple(std::move(substituted_tuple));
        using FinalTupleT = decltype(tupla_final);

        constexpr bool has_ph = count_placeholders_before<FinalTupleT, std::tuple_size_v<FinalTupleT>>() > 0;
        constexpr bool invocable = is_invocable_with_tuple<F, FinalTupleT>();

        if constexpr (!has_ph && invocable) {
            return std::apply(Func, std::move(tupla_final));
        } else {
            return Bind<F, FinalTupleT>(Func, std::move(tupla_final));
        }
    }

private:
    template <typename Guardados, typename Novos, size_t... GIs, size_t... NIs>
    auto substitute_and_append(Guardados&& g, Novos&& n, std::index_sequence<GIs...>, std::index_sequence<NIs...>) {
        using GuardadosPure = std::decay_t<Guardados>;
        constexpr size_t total_phs = count_placeholders_before<GuardadosPure, sizeof...(GIs)>();
        
        return std::tuple_cat(
            std::make_tuple(
                [&](auto idx_constant) -> decltype(auto) {
                    constexpr size_t GI = decltype(idx_constant)::value;
                    using GI_type = std::tuple_element_t<GI, GuardadosPure>;
                    
                    if constexpr (std::is_same_v<GI_type, PlaceHolder>) {
                        constexpr size_t n_idx = count_placeholders_before<GuardadosPure, GI>();
                        if constexpr (n_idx < std::tuple_size_v<std::decay_t<Novos>>) {
                            return std::get<n_idx>(std::forward<Novos>(n));
                        } else {
                            return PlaceHolder{}; 
                        }
                    } else {
                        return std::get<GI>(std::forward<Guardados>(g));
                    }
                }(std::integral_constant<size_t, GIs>{})...
            ),
            std::make_tuple(
                std::get<total_phs + NIs>(std::forward<Novos>(n))...
            )
        );
    }
};

template <typename F, typename... Args>
auto bind(F Func, Args&&... args) {
    auto t = std::make_tuple(std::forward<Args>(args)...);
    auto truncated = truncate_tuple(std::move(t)); 
    using TruncatedT = decltype(truncated);
    return Bind<F, TruncatedT>(Func, std::move(truncated));
}

// --- FUNÇÃO DE TESTE MDC ---
long int mdc(long int a, long int b) {
    return b == 0 ? a : mdc(b, a % b);
}

int main() {
    auto f2 = ::bind( mdc, __ );
    auto f1 = ::bind( f2, __, 12 );
    for( int i = 2; i <= 12; i++ )
      cout << f1( i ) <<  " ";
    cout << "\n";
    return 0;
}