#include <iostream>
#include <type_traits>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

// "Tabela" das predências
constexpr int PREC_ATOM = 100; // x, constantes, sin(...), cos(...), exp(...), log(...)
constexpr int PREC_POW  = 30;  // ^
constexpr int PREC_MUL  = 20;  // * /
constexpr int PREC_ADD  = 10;  // + -

struct Term {
    string s;
    int prec;
    bool is_zero;
    bool is_one;

    Term(string s, int prec, bool is_zero = false, bool is_one = false) : s(move(s)), prec(prec), is_zero(is_zero), is_one(is_one) {}

    operator string() const { return s; }
};

ostream& operator<<(ostream& os, const Term& t) { return os << t.s; }

// funções auxiliares para a simplificação

static string num_to_str(double v) {
    stringstream ss;
    ss << v;
    return ss.str();
}

static string p_if(const Term& t, int parent_prec) {
    if (t.prec < parent_prec) return "(" + t.s + ")";
    else return t.s;
}

static string p_if_eq(const Term& t, int parent_prec) {
    if (t.prec <= parent_prec) return "(" + t.s + ")";
    else return t.s;
}

// x + 0 = x    |   0 + x = x
static Term make_add(const Term& a, const Term& b) {
    if (a.is_zero) return b;
    if (b.is_zero) return a;
    return Term(p_if(a, PREC_ADD) + "+" + p_if(b, PREC_ADD), PREC_ADD);
}

// x - 0 = x    |   0 - x = -x
static Term make_sub(const Term& a, const Term& b) {
    if (b.is_zero) return a;
    if (a.is_zero) return Term("-" + p_if_eq(b, PREC_ADD), PREC_ADD);
    return Term(p_if(a, PREC_ADD) + "-" + p_if_eq(b, PREC_ADD), PREC_ADD);
}

// 0 * x = 0    |   1 * x = x
static Term make_mul(const Term& a, const Term& b) {
    if (a.is_zero || b.is_zero) return Term("0", PREC_ATOM, true);
    if (a.is_one) return b;                                       
    if (b.is_one) return a;                                       
    return Term(p_if(a, PREC_MUL) + "*" + p_if(b, PREC_MUL), PREC_MUL);
}

// 0 / g = 0    |   f / 1 = f
static Term make_div(const Term& a, const Term& b) {
    if (a.is_zero) return Term("0", PREC_ATOM, true);
    if (b.is_one) return a;                          
    return Term(p_if(a, PREC_MUL) + "/" + p_if_eq(b, PREC_MUL), PREC_MUL);
}

// f ^ 0 = 1    |   f ^ 1 = f
static Term make_pow(const Term& base, int n) {
    if (n == 0) return Term("1", PREC_ATOM, false, true);
    if (n == 1) return base;                             
    stringstream ss; ss << n;
    return Term(p_if_eq(base, PREC_POW) + "^" + ss.str(), PREC_POW);
}

// ----------------------------------------------------------------------------

class Var {
    public:
        double e(double v) const {return v;}
        double dx(double v) const {return 1;}
        Term str() const {return Term("x", PREC_ATOM);}
        Term dx_str() const {return Term("1", PREC_ATOM, false, true);}
};

Var x;

class Cte {
    public:
        Cte(double v) : valor(v) {}
        double e(double v) const {return valor;}
        double dx(double v) const {return 0;}
        Term str() const {return Term(num_to_str(valor), PREC_ATOM, valor == 0, valor == 1);}
        Term dx_str() const {return Term("0", PREC_ATOM, true);}
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
        Term str() const {return make_add(f.str(), g.str());}
        Term dx_str() const {return make_add(f.dx_str(), g.dx_str());}
    
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
        Term str() const {return make_sub(f.str(), g.str());}
        Term dx_str() const {return make_sub(f.dx_str(), g.dx_str());}
    
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
        Term str() const {return make_mul(f.str(), g.str());}
        Term dx_str() const {return make_add(make_mul(f.dx_str(), g.str()), make_mul(f.str(), g.dx_str()));}
    
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
        Term str() const {return make_div(f.str(), g.str());}
        Term dx_str() const {
            Term num = make_sub(make_mul(f.dx_str(), g.str()), make_mul(f.str(), g.dx_str()));
            Term den = make_pow(g.str(), 2);
            return make_div(num, den);
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

    Term str() const {return make_pow(f.str(), g);}

    Term dx_str() const {
        if (g == 0) return Term("0", PREC_ATOM, true);
        stringstream ss; ss << g;
        Term coef(ss.str(), PREC_ATOM, false, g == 1);
        Term lower_pow = make_pow(f.str(), g - 1);
        // Alterado: (coef * f.dx_str()) * lower_pow para obter a ordem correta
        return make_mul(make_mul(coef, f.dx_str()), lower_pow);
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
        Term str() const {return Term("exp(" + f.str().s + ")", PREC_ATOM);}
        Term dx_str() const {
            Term expf("exp(" + f.str().s + ")", PREC_ATOM);
            // Alterado: f.dx_str() * expf para obter a ordem correta
            return make_mul(f.dx_str(), expf);
        }
    
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
        Term str() const {return Term("log(" + f.str().s + ")", PREC_ATOM);}
        Term dx_str() const {
            // Alterado: (1/f) * f' em vez de f'/f
            Term one_over_f = make_div(Term("1", PREC_ATOM, false, true), f.str());
            return make_mul(one_over_f, f.dx_str());
        }
    
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
        Term str() const {return Term("sin(" + f.str().s + ")", PREC_ATOM);}
        Term dx_str() const {
            Term cosf("cos(" + f.str().s + ")", PREC_ATOM);
            return make_mul(cosf, f.dx_str());
        }
    
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
        Term str() const {return Term("cos(" + f.str().s + ")", PREC_ATOM);}
        Term dx_str() const {
            // Alterado: precedência PREC_MUL para evitar parênteses desnecessários
            Term neg_sinf("-sin(" + f.str().s + ")", PREC_MUL);
            return make_mul(neg_sinf, f.dx_str());
        }
    
    private:
        F f;
};

template <typename F>
auto cos(const F& f) {
    return Cos{f};
}