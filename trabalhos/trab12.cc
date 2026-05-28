#include <iostream>
#include <vector>
#include <type_traits>
#include <functional>
#include <utility>

using namespace std;

// === 1. Suas classes base (Corrigidas) ===

class X {
public: // CORRIGIDO: Precisava ser público
    template <typename K>
    auto operator()( K valor ) { return valor; }
};

template <typename T>
class Cte {
public:
    Cte( const T& valor ): valor( valor ) {}
    template<typename K>
    T operator()( K ignorar ) { return valor; }
private:
    T valor;
};

template <typename Op1, typename Op2>
class Soma {
public:
    // CORRIGIDO: Adicionado o construtor para inicializar as operações
    Soma(Op1 a, Op2 b) : a(a), b(b) {}

    auto operator()( auto valor ) { return a( valor ) + b( valor ); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
auto operator + ( Op1 a, Op2 b ) {
    return Soma< Op1, Op2 >( a, b );
}

template <typename Op1, typename Op2>
class Multiplica {
public:
    Multiplica(Op1 a, Op2 b) : a(a), b(b) {}

    auto operator()( auto valor ) { return a( valor ) * b( valor ); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
auto operator * ( Op1 a, Op2 b ) {
    return Multiplica< Op1, Op2 >( a, b );
}

template <typename Op1, typename Op2>
class Modulo {
public:
    Modulo(Op1 a, Op2 b) : a(a), b(b) {}
    auto operator()(auto valor) { return a(valor) % b(valor); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
class Igual {
public:
    Igual(Op1 a, Op2 b) : a(a), b(b) {}
    auto operator()(auto valor) { return a(valor) == b(valor); }
private:
    Op1 a; Op2 b;
};

X x;

// === 2. A Engrenagem Mágica: print ===
// Permite que o "cout << expressão" vire um objeto chamável (Functor)

template <typename Left, typename Right>
class print {
public:
    ostream& os;
    Left left;
    Right right;

    print(ostream& os, Left l, Right r) : os(os), left(l), right(r) {}

    // Quando o pipe chamar a função, ela executa a corrente de prints
    void operator()(auto valor) {
        if constexpr (!is_same_v<Left, nullptr_t>) {
            left(valor); // Executa o que estava à esquerda primeiro
        }
        if constexpr (requires { right(valor); }) {
            os << right(valor); // Se for uma expressão (ex: x+x), avalia e imprime
        } else {
            os << right; // Se for um literal (ex: ' '), imprime direto
        }
    }
};

// Traits e Concepts para capturar apenas as NOSSAS expressões e não quebrar o cout normal
template <typename T> struct is_my_expr : false_type {};
template <> struct is_my_expr<X> : true_type {};
template <typename A, typename B> struct is_my_expr<Soma<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Multiplica<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Modulo<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Igual<A, B>> : true_type {};
template <typename T> struct is_my_expr<Cte<T>> : true_type {};
template <typename L, typename R> struct is_my_expr<print<L, R>> : true_type {};

template <typename T>
concept MyExpr = is_my_expr<decay_t<T>>::value;

// Sobrecarga para: cout << expressao_customizada
template <MyExpr R>
auto operator<<(ostream& os, R&& right) {
    return print<nullptr_t, decay_t<R>>(os, nullptr, forward<R>(right));
}

// Sobrecarga para encadear: (cout << expressao) << ' '
template <typename L, typename R, typename T>
auto operator<<(print<L, R> left, T&& right) {
    return print<print<L, R>, decay_t<T>>(left.os, left, forward<T>(right));
}

// === 3. Seu operador de Pipe original ===

template<typename I, typename F>
auto operator | (const I& item, F funcao) {

    using T = decay_t<decltype(*begin(item))>;
    using RetornoFuncao = invoke_result_t<F, T>;

    if constexpr (is_same_v<RetornoFuncao, bool>) {
        vector<T> return_list;
        for (const auto& i : item) {
            if (invoke(funcao, i)) {
                return_list.push_back(i);
            }
        }
        return return_list;
    } 
    else if constexpr (is_same_v<RetornoFuncao, void>) {
        for (const auto& i : item) {
            invoke(funcao, i);
        }
    }
    else {
        vector<RetornoFuncao> return_list;
        for (const auto& i : item) {
            return_list.push_back(invoke(funcao, i));
        }
        return return_list;
    }
}

template <typename T>
auto wrap(T&& val) {
    if constexpr (MyExpr<T>) {
        return forward<T>(val);
    } else {
        return Cte<decay_t<T>>(forward<T>(val));
    }
}

template <typename Op1, typename Op2>
requires (MyExpr<Op1> || MyExpr<Op2>)
auto operator % (Op1&& a, Op2&& b) { 
    return Modulo(wrap(forward<Op1>(a)), wrap(forward<Op2>(b))); 
}

template <typename Op1, typename Op2>
requires (MyExpr<Op1> || MyExpr<Op2>)
auto operator == (Op1&& a, Op2&& b) { 
    return Igual(wrap(forward<Op1>(a)), wrap(forward<Op2>(b))); 
}

// === 4. Main ===

int main(){
    vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    v | (x % 2 == 1) | cout << x << ' ';

    return 0;
}