#include <iostream>
#include <vector>
#include <functional>
using namespace std;

// ─── Intervalo ────────────────────────────────────────────────────────────────

class Intervalo {
public:
    Intervalo(int value_a) : begin_value(value_a), end_value(value_a - 2) {}
    Intervalo(int value_a, int value_b) : begin_value(value_a), end_value(value_b) {}

    class Iterador {
    public:
        Iterador(int v) : atual(v) {}
        int operator*() const { return atual; }
        Iterador& operator++() { ++atual; return *this; }
        Iterador  operator++(int) { Iterador tmp = *this; ++atual; return tmp; }
        bool operator!=(const Iterador& i) const { return atual < i.atual; }
    private:
        int atual;
    };

    Iterador begin() const { return begin_value; }
    Iterador end()   const { return end_value;   }

private:
    int begin_value, end_value;
};

// ─── operator| for function/predicate stages ─────────────────────────────────

template<typename I, typename F>
auto operator|(const I& item, F funcao) {
    using T = decay_t<decltype(*begin(item))>;
    using RetornoFuncao = invoke_result_t<F, T>;

    if constexpr (is_same_v<RetornoFuncao, bool>) {
        vector<T> return_list;
        for (const auto& i : item)
            if (invoke(funcao, i))
                return_list.push_back(i);
        return return_list;
    }
    else if constexpr (is_same_v<RetornoFuncao, void>) {
        for (const auto& i : item)
            invoke(funcao, i);
    }
    else {
        vector<RetornoFuncao> return_list;
        for (const auto& i : item)
            return_list.push_back(invoke(funcao, i));
        return return_list;
    }
}

// ─── NPrimeiros ───────────────────────────────────────────────────────────────

class NPrimeiros {
public:
    int n;
    explicit NPrimeiros(int n) : n(n) {}
};

// operator| specialization: range | NPrimeiros  →  vector with at most n items
template<typename I>
auto operator|(const I& item, NPrimeiros np) {
    using T = decay_t<decltype(*begin(item))>;
    vector<T> result;
    result.reserve(np.n);
    for (const auto& v : item) {
        result.push_back(v);
        if (static_cast<int>(result.size()) == np.n)
            break;               // short-circuit: stop consuming the range
    }
    return result;
}

// ─── testa_se_e_primo ─────────────────────────────────────────────────────────

auto testa_se_e_primo = [](int n) -> bool {
    if (n < 2)  return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; (long long)i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
};

// ─── main ─────────────────────────────────────────────────────────────────────

int main() {
    // Prints the first 10 primes starting from 0 (Intervalo(0) = [0, ∞))
    Intervalo(0) | testa_se_e_primo
                 | NPrimeiros(10)
                 | [](auto x) { cout << x << " "; };
    cout << "\n";
    // Expected: 2 3 5 7 11 13 17 19 23 29
}