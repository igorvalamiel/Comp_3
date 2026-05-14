#include <iostream>
#include <type_traits>
#include <cmath>

using namespace std;

class Var {
    public:
        double e(double v) const {return v;}
        double dx(double v) const {return 1;}
};

Var x;

class Cte {
    public:
        Cte(double v) : valor(v) {}
        double e(double v) const {return valor;}
        double dx(double v) const {return 0;}
    private:
        double valor;
};

// ----------------------------------------------------------------------------
// Operação ADICAO +

template <typename F, typename G>
class Adicao {
    public:
        Adicao(const F& f, const G& g): f(f), g(g) {}

        double e(double v) const {return f.e(v) + g.e(v);}
        double dx(double v) const {return f.dx(v) + g.dx(v);}
    
    private:
        F f;
        G g;
};

template <typename F, typename G>
auto operator + (const F& f, const G& g) {
    return Adicao<F, G>{f, g};
}
template <typename F>
auto operator + (const F& f, double g) {
    return Adicao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator + (double f, const G& g) {
    return Adicao<Cte, G>{Cte(f), g};
}
template <typename F>
auto operator + (const F& f, int g) {
    return Adicao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator + (int f, const G& g) {
    return Adicao<Cte, G>{Cte(f), g};
}

// ----------------------------------------------------------------------------
// Operação SUBTRACAO -
template <typename F, typename G>
class Subtracao {
    public:
        Subtracao(const F& f, const G& g): f(f), g(g) {}

        double e(double v) const {return f.e(v) - g.e(v);}
        double dx(double v) const {return f.dx(v) - g.dx(v);}
    
    private:
        F f;
        G g;
};

template <typename F, typename G>
auto operator - (const F& f, const G& g) {
    return Subtracao<F, G>{f, g};
}
template <typename F>
auto operator - (const F& f, double g) {
    return Subtracao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator - (double f, const G& g) {
    return Subtracao<Cte, G>{Cte(f), g};
}
template <typename F>
auto operator - (const F& f, int g) {
    return Subtracao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator - (int f, const G& g) {
    return Subtracao<Cte, G>{Cte(f), g};
}

// ----------------------------------------------------------------------------
// Operação MULTIPLICACAO *
template <typename F, typename G>
class Multiplicacao {
    public:
        Multiplicacao(const F& f, const G& g): f(f), g(g) {}

        double e(double v) const {return f.e(v) * g.e(v);}
        double dx(double v) const {return (f.dx(v) * g.e(v)) + (f.e(v) * g.dx(v));}
    
    private:
        F f;
        G g;
};

template <typename F, typename G>
auto operator * (const F& f, const G& g) {
    return Multiplicacao<F, G>{f, g};
}
template <typename F>
auto operator * (const F& f, double g) {
    return Multiplicacao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator * (double f, const G& g) {
    return Multiplicacao<Cte, G>{Cte(f), g};
}
template <typename F>
auto operator * (const F& f, int g) {
    return Multiplicacao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator * (int f, const G& g) {
    return Multiplicacao<Cte, G>{Cte(f), g};
}

// ----------------------------------------------------------------------------
// Operação DIVISAO /
template <typename F, typename G>
class Divisao {
    public:
        Divisao (const F& f, const G& g): f(f), g(g) {}

        double e(double v) const {return f.e(v) / g.e(v);}
        double dx(double v) const {
            auto dividendo = (f.dx(v) * g.e(v)) - (f.e(v) * g.dx(v));
            auto divisor = g.e(v) * g.e(v);
            return dividendo/divisor;
        }
    
    private:
        F f;
        G g;
};

template <typename F, typename G>
auto operator / (const F& f, const G& g) {
    return Divisao<F, G>{f, g};
}
template <typename F>
auto operator / (const F& f, double g) {
    return Divisao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator / (double f, const G& g) {
    return Divisao<Cte, G>{Cte(f), g};
}
template <typename F>
auto operator / (const F& f, int g) {
    return Divisao<F, Cte>{f, Cte(g)};
}
template <typename G>
auto operator / (int f, const G& g) {
    return Divisao<Cte, G>{Cte(f), g};
}

// ----------------------------------------------------------------------------
// Operação POTENCIACAO ->*
template <typename F>
class Potenciacao {
public:
    Potenciacao(const F& f, int g): f(f), g(g) {}

    double e(double v) const {
        double result = 1;

        for (int i = 0; i < g; i++) {result *= f.e(v);}

        return result;
    }

    double dx(double v) const {
        if (g == 0) return 0;

        return (g * Potenciacao<F>(f, g - 1).e(v) * f.dx(v));
    }

private:
    F f;
    int g;
};

template <typename F, typename G>
auto operator ->* (const F& f, G g) {
    static_assert(is_same_v<G, int>, "O expoente deve ser um numero inteiro!");
    return Potenciacao<F>{f, g};
}

// ----------------------------------------------------------------------------
// Operação EXP exp(f)
template <typename F>
class Exp {
    public:
        Exp (const F& f): f(f) {}

        double e(double v) const {return std::exp(f.e(v));}
        double dx(double v) const {return std::exp(f.e(v)) * f.dx(v);}
    
    private:
        F f;
};

template <typename F>
auto exp(const F& f) {
    return Exp{f};
}
// ----------------------------------------------------------------------------
// Operação LOG log(f)
template <typename F>
class Log {
    public:
        Log (const F& f): f(f) {}

        double e(double v) const {return std::log(f.e(v));}
        double dx(double v) const {return f.dx(v) / f.e(v);}
    
    private:
        F f;
};

template <typename F>
auto log(const F& f) {
    return Log{f};
}
// ----------------------------------------------------------------------------
// Operação SIN sin(f)
template <typename F>
class Sin {
    public:
        Sin (const F& f): f(f) {}

        double e(double v) const {return std::sin(f.e(v));}
        double dx(double v) const {return std::cos(f.e(v)) * f.dx(v);}
    
    private:
        F f;
};

template <typename F>
auto sin(const F& f) {
    return Sin{f};
}
// ----------------------------------------------------------------------------
// Operação COS cos(f)
template <typename F>
class Cos {
    public:
        Cos (const F& f): f(f) {}

        double e(double v) const {return std::cos(f.e(v));}
        double dx(double v) const {return -1 * std::sin(f.e(v)) * f.dx(v);}
    
    private:
        F f;
};

template <typename F>
auto cos(const F& f) {
    return Cos{f};
}

// ----------------------------------------------------------------------------
int main(){
    double v = 0.1;
    auto f = 1 / (1 + exp( -2*( x - 1 )->*4 ) );
    cout << "f(" << v << ") = " << f.e(v) << "\n f'(" << v << ") = " << f.dx(v) << endl;

    return 0;
}