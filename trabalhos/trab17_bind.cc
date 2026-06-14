#include <iostream>
#include <utility>
#include <tuple>          
#include <type_traits>    
#include <functional>     

using namespace std;


class PlaceHolder {};
constexpr PlaceHolder __ {};


template <typename Tuple, size_t... Is>
constexpr size_t count_placeholders_impl(index_sequence<Is...>) {
    return (0 + ... + (is_same_v<tuple_element_t<Is, decay_t<Tuple>>, PlaceHolder> ? 1 : 0));
}

template <typename Tuple, size_t UpTo>
constexpr size_t count_placeholders_before() {
    return count_placeholders_impl<Tuple>(make_index_sequence<UpTo>{});
}

template <typename Tuple, size_t... Is>
constexpr size_t find_last_concrete_index(std::index_sequence<Is...>) {
    size_t last = 0;
    ( (last = (!std::is_same_v<std::tuple_element_t<Is, std::decay_t<Tuple>>, PlaceHolder> ? Is + 1 : last)), ... );
    return last;
}

template <typename Tuple, size_t... Is>
auto slice_tuple(Tuple&& t, index_sequence<Is...>) {
    return make_tuple(get<Is>(forward<Tuple>(t))...);
}

template <typename Tuple>
auto truncate_tuple(Tuple&& t) {
    using PureT = decay_t<Tuple>;
    constexpr size_t sz = tuple_size_v<PureT>;
    constexpr size_t last_idx = find_last_concrete_index<PureT>(make_index_sequence<sz>{});
    return slice_tuple(forward<Tuple>(t), make_index_sequence<last_idx>{});
}

template <typename F, typename Tuple, size_t... Is>
constexpr bool is_invocable_with_tuple_impl(index_sequence<Is...>) {
    return is_invocable_v<F&, tuple_element_t<Is, decay_t<Tuple>>...>;
}

template <typename F, typename Tuple>
constexpr bool is_invocable_with_tuple() {
    using PureT = decay_t<Tuple>;
    return is_invocable_with_tuple_impl<F, PureT>(make_index_sequence<tuple_size_v<PureT>>{});
}



template <typename F, typename StoredTuple>
class Bind {
private:
    F Func;
    StoredTuple guardados;

public:
    Bind(F f, StoredTuple t) : Func(f), guardados(move(t)) {}

    template <typename... ArgsNovos>
    auto operator()(ArgsNovos&&... args) {
        auto novos_tuple = forward_as_tuple(forward<ArgsNovos>(args)...);
        
        using GuardadosT = decay_t<StoredTuple>;
        using NovosT = decltype(novos_tuple);
        
        constexpr size_t g_sz = tuple_size_v<GuardadosT>;
        constexpr size_t n_sz = tuple_size_v<NovosT>;
        constexpr size_t total_phs = count_placeholders_before<GuardadosT, g_sz>();
        
        constexpr size_t num_leftovers = (n_sz > total_phs) ? (n_sz - total_phs) : 0;

        auto substituted_tuple = substitute_and_append(
            guardados, 
            novos_tuple, 
            make_index_sequence<g_sz>{}, 
            make_index_sequence<num_leftovers>{}
        );

        auto tupla_final = truncate_tuple(move(substituted_tuple));
        using FinalTupleT = decltype(tupla_final);

        constexpr bool has_ph = count_placeholders_before<FinalTupleT, tuple_size_v<FinalTupleT>>() > 0;
        constexpr bool invocable = is_invocable_with_tuple<F, FinalTupleT>();

        if constexpr (!has_ph && invocable) {
            return apply(Func, move(tupla_final));
        } else {
            return Bind<F, FinalTupleT>(Func, move(tupla_final));
        }
    }

private:
    template <typename Guardados, typename Novos, size_t... GIs, size_t... NIs>
    auto substitute_and_append(Guardados&& g, Novos&& n, index_sequence<GIs...>, index_sequence<NIs...>) {
        using GuardadosPure = decay_t<Guardados>;
        constexpr size_t total_phs = count_placeholders_before<GuardadosPure, sizeof...(GIs)>();
        
        return tuple_cat(
            make_tuple(
                [&](auto idx_constant) -> decltype(auto) {
                    constexpr size_t GI = decltype(idx_constant)::value;
                    using GI_type = tuple_element_t<GI, GuardadosPure>;
                    
                    if constexpr (is_same_v<GI_type, PlaceHolder>) {
                        constexpr size_t n_idx = count_placeholders_before<GuardadosPure, GI>();
                        if constexpr (n_idx < tuple_size_v<decay_t<Novos>>) {
                            return get<n_idx>(forward<Novos>(n));
                        } else {
                            return PlaceHolder{};
                        }
                    } else {
                        return get<GI>(forward<Guardados>(g));
                    }
                }(integral_constant<size_t, GIs>{})...
            ),
            make_tuple(
                get<total_phs + NIs>(forward<Novos>(n))...
            )
        );
    }
};

template <typename F, typename... Args>
auto bind(F Func, Args&&... args) {
    auto t = make_tuple(forward<Args>(args)...);
    auto truncated = truncate_tuple(move(t));
    using TruncatedT = decltype(truncated);
    return Bind<F, TruncatedT>(Func, move(truncated));
}
