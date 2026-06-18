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
                bool operator != ( Iterador i) const { return atual != i.atual; }

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

template<typename Range, typename F>
struct LazyFilterRange {
    Range s;
    F f;

    using sIter = decltype(::begin(declval<const Range&>()));

    struct Iterator {
        sIter atual, ultimo;
        F* f;

        void skip() { while (atual != ultimo && !(*f)(*atual)) ++atual; }

        auto  operator*()  const { return *atual; }
        Iterator& operator++() { ++atual; skip(); return *this; }
        bool  operator!=(const Iterator& o) const { return atual != o.atual; }
    };

    Iterator begin() const {
        auto it = Iterator{ ::begin(s), ::end(s), const_cast<F*>(&f) };
        it.skip();
        return it;
    }
    Iterator end() const { return { ::end(s), ::end(s), const_cast<F*>(&f) }; }
};


template<typename Range, typename F>
struct MakeLazy {
    Range s;
    F f;

    using sIter = decltype(::begin(declval<const Range&>()));

    struct Iterator {
        sIter atual;
        F* f;

        auto  operator*()  const { return (*f)(*atual); }
        Iterator& operator++() { ++atual; return *this; }
        bool  operator!=(const Iterator& o) const { return atual != o.atual; }
    };

    Iterator begin() const { return { ::begin(s), const_cast<F*>(&f) }; }
    Iterator end()   const { return { ::end(s),   const_cast<F*>(&f) }; }
};


template<typename I, typename F>
enable_if_t<is_same_v<invoke_result_t<F, decay_t<decltype(*begin(declval<const I&>()))>>, void>>
operator|(const I& item, F funcao) {
    for (const auto& i : item)
        invoke(funcao, i);
}


template<typename I, typename F>
enable_if_t<!is_same_v<invoke_result_t<F, decay_t<decltype(*begin(declval<const I&>()))>>, void>,
            conditional_t<is_same_v<invoke_result_t<F, decay_t<decltype(*begin(declval<const I&>()))>>, bool>,
                          LazyFilterRange<I,F>,
                          MakeLazy<I,F>>>
operator|(const I& item, F funcao) {
    using T = decay_t<decltype(*begin(item))>;
    using RetornoFuncao = invoke_result_t<F, T>;

    if constexpr (is_same_v<RetornoFuncao, bool>)
        return LazyFilterRange<I, F>{ item, funcao };
    else
        return MakeLazy<I, F>{ item, funcao };
}