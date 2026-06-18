#include <functional>
using namespace std;

// ─── Intervalo ────────────────────────────────────────────────────────────────
class Intervalo {
public:
    Intervalo(int a)        : begin_value(a), end_value(a-2) {}
    Intervalo(int a, int b) : begin_value(a), end_value(b)   {}

    class Iterador {
    public:
        Iterador(int v) : atual(v) {}
        int operator*()        const { return atual; }
        int operator++()             { return ++atual; }
        int operator++(int)          { return atual++; }
        bool operator!=(Iterador i)  const { return atual < i.atual; }
    private:
        int atual;
    };

    Iterador begin() const { return begin_value; }
    Iterador end()   const { return end_value;   }

private:
    int begin_value, end_value;
};

// ─── LazyFilterRange ──────────────────────────────────────────────────────────
// Wraps any range + predicate; iterates lazily skipping non-matching elements.
template<typename Range, typename F>
struct LazyFilterRange {
    const Range& src;
    F f;

    using SrcIter = decltype(::begin(declval<const Range&>()));

    struct Iter {
        SrcIter cur, last;
        F* f;

        void skip() { while (cur != last && !(*f)(*cur)) ++cur; }

        auto  operator*()  const { return *cur; }
        Iter& operator++() { ++cur; skip(); return *this; }
        bool  operator!=(const Iter& o) const { return cur != o.cur; }
    };

    Iter begin() const {
        auto it = Iter{ ::begin(src), ::end(src), const_cast<F*>(&f) };
        it.skip();
        return it;
    }
    Iter end() const { return { ::end(src), ::end(src), const_cast<F*>(&f) }; }
};

// ─── LazyMapRange ─────────────────────────────────────────────────────────────
// Wraps any range + transform; applies f lazily on dereference.
template<typename Range, typename F>
struct LazyMapRange {
    const Range& src;
    F f;

    using SrcIter = decltype(::begin(declval<const Range&>()));

    struct Iter {
        SrcIter cur;
        F* f;

        auto  operator*()  const { return (*f)(*cur); }
        Iter& operator++() { ++cur; return *this; }
        bool  operator!=(const Iter& o) const { return cur != o.cur; }
    };

    Iter begin() const { return { ::begin(src), const_cast<F*>(&f) }; }
    Iter end()   const { return { ::end(src),   const_cast<F*>(&f) }; }
};

// ─── operator|(range, F) — lazy filter or map; eager sink for void ────────────
template<typename I, typename F>
auto operator|(const I& item, F funcao) {
    using T            = decay_t<decltype(*::begin(item))>;
    using RetornoFuncao = invoke_result_t<F, T>;

    if constexpr (is_same_v<RetornoFuncao, bool>) {
        // filter → lazy
        return LazyFilterRange<I, F>{ item, funcao };
    }
    else if constexpr (is_same_v<RetornoFuncao, void>) {
        // sink → eager, no return
        for (const auto& i : item)
            invoke(funcao, i);
    }
    else {
        // transform → lazy
        return LazyMapRange<I, F>{ item, funcao };
    }
}

// ─── NPrimeiros — eager, collects first n elements from any range ─────────────
class NPrimeiros {
public:
    NPrimeiros(int n) : counter(n) {}
    int counter;
};

template<typename I>
auto operator|(const I& item, NPrimeiros np) {
    using T = decay_t<decltype(*::begin(item))>;
    vector<T> result;
    result.reserve(np.counter);
    for (const auto& v : item) {
        result.push_back(v);
        if (static_cast<int>(result.size()) == np.counter) break;
    }
    return result;
}