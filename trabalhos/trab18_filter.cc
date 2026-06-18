#include <functional>

using namespace std;


class Intervalo {
    public:
        Intervalo (int value_a): begin_value(value_a), end_value(value_a-2) {}
        Intervalo (int value_a, int value_b): begin_value(value_a), end_value(value_b) {}

        class Iterador {
            public:
                Iterador (int v) : atual(v) {}
                int operator *() const { return atual; }
                int operator ++() { return ++atual;} // prefixado ( ++a )
                int operator ++(int) { return atual++;} // posfixado ( a++ )
                bool operator != ( Iterador i) const { return atual < i.atual; }

            private:
                int atual;
        };

        Iterador begin() const {return begin_value;}

        Iterador end() const {return end_value;}

    private:
        int begin_value, end_value;
};


class NPrimeiros {
    public:
        NPrimeiros(int n) : counter(n) {}
        int counter;
};

template<typename I>
auto operator|(const I& item, NPrimeiros np) {
    using T = decay_t<decltype(*begin(item))>;
    vector<T> result;
    result.reserve(np.counter);
    for (const auto& v : item) {
        result.push_back(v);
        if (static_cast<int>(result.size()) == np.counter) break;
    }
    return result;
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