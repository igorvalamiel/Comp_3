#include <iostream>
#include <variant>
// #include "sua_implementacao.h" ← será adicionado pelo avaliador
using namespace std;

template <size_t N, typename Ts>
auto print_variant(ostream& os, const variant<Ts...>& v) -> enable_if_t<N == sizeof...(Ts)>{}

template <size_t N, typename... Ts>
auto print_variant(ostream& os, const variant<Ts...>& v) -> enable_if_t<N < sizeof...(Ts)> {
    if (v.index() == N)
        os << get<N>(v);
    else
        print_variant<N + 1>(os, v);
}

template <typename... Ts>
ostream& operator<<(ostream& os, const variant<Ts...>& v){
    print_variant<0>(os, v);
    return os;
}

int main() {

    variant<int,char,string> v = 42;
    cout << v << "\n";

    return 0;
}
