#include <iostream>
#include <variant>
using namespace std;

template <size_t N, typename... Ts>
auto print_variant(ostream& os, const variant<Ts...>& v) -> enable_if_t<N == sizeof...(Ts)>;

template <size_t N, typename... Ts>
auto print_variant(ostream& os, const variant<Ts...>& v) -> enable_if_t<N < sizeof...(Ts)> {
    if (v.index() == N)
        os << get<N>(v);
    else
        print_variant<N + 1>(os, v);
}

// Definição do caso base — corpo vazio, encerra a recursão
template <size_t N, typename... Ts>
auto print_variant(ostream& os, const variant<Ts...>& v) -> enable_if_t<N == sizeof...(Ts)> {}

template <typename T, typename... Ts>            // T, Ts... evita instanciação de variant<> pelo GCC
ostream& operator<<(ostream& os, const variant<T, Ts...>& v) {
    print_variant<0>(os, v);
    return os;
}