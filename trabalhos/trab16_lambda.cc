#include <iostream>
#include <vector>
#include <type_traits>
#include <functional>
#include <utility>

using namespace std;

class X;
template <typename T> class Cte;
template <typename Op1, typename Op2> class Soma;
template <typename Op1, typename Op2> class Multiplica;
template <typename Op1, typename Op2> class Modulo;
template <typename Op1, typename Op2> class Igual;
template <typename Op1, typename Op2> class Diferente;
template <typename Left, typename Right> class print;
template <typename Op1, typename Op2> class Pipe;
template <typename Expr, typename IndexType> class Indexacao;

template <typename T> struct is_my_expr : false_type {};
template <> struct is_my_expr<X> : true_type {};
template <typename A, typename B> struct is_my_expr<Soma<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Multiplica<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Modulo<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Igual<A, B>> : true_type {};
template <typename A, typename B> struct is_my_expr<Diferente<A, B>> : true_type {};
template <typename T> struct is_my_expr<Cte<T>> : true_type {};
template <typename L, typename R> struct is_my_expr<print<L, R>> : true_type {};
template <typename A, typename B> struct is_my_expr<Pipe<A, B>> : true_type {};
template <typename E, typename I> struct is_my_expr<Indexacao<E, I>> : true_type {};

template <typename T>
inline constexpr bool is_my_expr_v = is_my_expr<decay_t<T>>::value;

template <typename T>
auto wrap(T&& val);

class X {
public:
    template <typename K>
    auto operator()( K valor ) { return valor; }

    template <typename T>
    auto operator[](T&& index);
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
    Soma(Op1 a, Op2 b) : a(a), b(b) {}

    template <typename K>
    auto operator()( K valor ) { return a( valor ) + b( valor ); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
class Multiplica {
public:
    Multiplica(Op1 a, Op2 b) : a(a), b(b) {}

    template <typename K>
    auto operator()( K valor ) { return a( valor ) * b( valor ); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
class Modulo {
public:
    Modulo(Op1 a, Op2 b) : a(a), b(b) {}

    template <typename K>
    auto operator()(K valor) { return a(valor) % b(valor); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
class Igual {
public:
    Igual(Op1 a, Op2 b) : a(a), b(b) {}

    template <typename K>
    auto operator()(K valor) { return a(valor) == b(valor); }
private:
    Op1 a; Op2 b;
};

template <typename Op1, typename Op2>
class Diferente {
public:
    Diferente(Op1 a, Op2 b) : a(a), b(b) {}
    template <typename K>
    auto operator()(K valor) { return a(valor) != b(valor); }
private:
    Op1 a; Op2 b;
};

template <typename Left, typename Right>
class print {
public:
    ostream& os;
    Left left;
    Right right;

    print(ostream& os, Left l, Right r) : os(os), left(l), right(r) {}

    template <typename K>
    void operator()(K valor) {
        if constexpr (!is_same_v<Left, nullptr_t>) {
            left(valor); 
        }
        if constexpr (is_invocable_v<Right, K>) {
            os << right(valor);
        } else {
            os << right;
        }
    }
};

template <typename Op1, typename Op2>
class Pipe {
public:
    Pipe(Op1 a, Op2 b) : a(a), b(b) {}
    template <typename K>
    auto operator()(K&& valor) { return a(forward<K>(valor)) | b; }
private:
    Op1 a; Op2 b;
};

template <typename Expr, typename IndexType>
class Indexacao {
public:
    Indexacao(Expr a, IndexType b) : a(a), b(b) {}
    template <typename K>
    auto operator()(K&& valor) {
        return a(forward<K>(valor))[b(forward<K>(valor))];
    }
private:
    Expr a; IndexType b;
};

template <typename T>
auto X::operator[](T&& index) {
    return Indexacao<X, decltype(wrap(forward<T>(index)))>(*this, wrap(forward<T>(index)));
}

template <typename R, typename = enable_if_t<is_my_expr_v<R>>>
auto operator<<(ostream& os, R&& right) {
    return print<nullptr_t, decay_t<R>>(os, nullptr, forward<R>(right));
}

template <typename L, typename R, typename T>
auto operator<<(print<L, R> left, T&& right) {
    return print<print<L, R>, decay_t<T>>(left.os, left, forward<T>(right));
}

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
    if constexpr (is_my_expr_v<T>) {
        return forward<T>(val);
    } else {
        return Cte<decay_t<T>>(forward<T>(val));
    }
}

template <typename Op1, typename Op2>
auto operator * ( Op1 a, Op2 b ) { return Multiplica< Op1, Op2 >( a, b ); }

template <typename Op1, typename Op2, typename = enable_if_t<is_my_expr_v<Op1> || is_my_expr_v<Op2>>>
auto operator + ( Op1&& a, Op2&& b ) { return Soma( wrap( forward<Op1>(a) ), wrap( forward<Op2>(b) ) ); }

template <typename Op1, typename Op2, typename = enable_if_t<is_my_expr_v<Op1> || is_my_expr_v<Op2>>>
auto operator % (Op1&& a, Op2&& b) { return Modulo(wrap(forward<Op1>(a)), wrap(forward<Op2>(b))); }

template <typename Op1, typename Op2, typename = enable_if_t<is_my_expr_v<Op1> || is_my_expr_v<Op2>>>
auto operator == (Op1&& a, Op2&& b) { return Igual(wrap(forward<Op1>(a)), wrap(forward<Op2>(b))); }

template <typename Op1, typename Op2, typename = enable_if_t<is_my_expr_v<Op1> || is_my_expr_v<Op2>>>
auto operator != (Op1&& a, Op2&& b) { return Diferente(wrap(forward<Op1>(a)), wrap(forward<Op2>(b))); }

template <typename Op1, typename Op2, typename = enable_if_t<is_my_expr_v<Op1>>>
auto operator | (Op1&& a, Op2&& b) { return Pipe(forward<Op1>(a), forward<Op2>(b)); }

X x;
