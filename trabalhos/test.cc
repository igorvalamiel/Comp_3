#include <iostream>
#include <type_traits>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

// ----------------------------------------------------------------------------
// Infraestrutura de simplificação / impressão
//
// Cada nó passa a devolver um "Term": a string já simplificada, a sua
// precedência (para saber quando precisa de parênteses) e se ela representa
// literalmente 0 ou 1 (para eliminar termos neutros como 0+x, 1*x, x^0...).
// ----------------------------------------------------------------------------

constexpr int PREC_ATOM = 100; // x, constantes, sin(...), cos(...), exp(...), log(...)
constexpr int PREC_POW  = 30;  // ^
constexpr int PREC_MUL  = 20;  // * /
constexpr int PREC_ADD  = 10;  // + -

struct Term {
    string s;
    int prec;
    bool is_zero;
    bool is_one;

    Term(string s, int prec, bool is_zero = false, bool is_one = false)
        : s(move(s)), prec(prec), is_zero(is_zero), is_one(is_one) {}

    operator string() const { return s; }
};

ostream& operator<<(ostream& os, const Term& t) { return os << t.s; }

// parêntese se a precedência do filho for MENOR que a do pai
// (uso em lados comutativos/associativos: + com +, * com *, etc.)
static string p_if(const Term& t, int parent_prec) {
    return (t.prec < parent_prec) ? ("(" + t.s + ")") : t.s;
}
// parêntese se a precedência do filho for MENOR OU IGUAL à do pai
// (uso no lado direito de operações não-comutativas: a-(b...), a/(b...), base de potência)
static string p_if_eq(const Term& t, int parent_prec) {
    return (t.prec <= parent_prec) ? ("(" + t.s + ")") : t.s;
}

static string num_to_str(double v) {
    stringstream ss;
    ss << v;
    return ss.str();
}

static Term make_add(const Term& a, const Term& b) {
    if (a.is_zero) return b;                 // 0+g = g
    if (b.is_zero) return a;                 // f+0 = f
    return Term(p_if(a, PREC_ADD) + "+" + p_if(b, PREC_ADD), PREC_ADD);
}

static Term make_sub(const Term& a, const Term& b) {
    if (b.is_zero) return a;                                  // f-0 = f
    if (a.is_zero) return Term("-" + p_if_eq(b, PREC_MUL), PREC_ADD); // 0-g = -g
    return Term(p_if(a, PREC_ADD) + "-" + p_if_eq(b, PREC_ADD), PREC_ADD);
}

static Term make_mul(const Term& a, const Term& b) {
    if (a.is_zero || b.is_zero) return Term("0", PREC_ATOM, true); // 0*g = f*0 = 0
    if (a.is_one) return b;                                        // 1*g = g
    if (b.is_one) return a;                                        // f*1 = f
    return Term(p_if(a, PREC_MUL) + "*" + p_if(b, PREC_MUL), PREC_MUL);
}

static Term make_div(const Term& a, const Term& b) {
    if (a.is_zero) return Term("0", PREC_ATOM, true); // 0/g = 0
    if (b.is_one) return a;                            // f/1 = f
    return Term(p_if(a, PREC_MUL) + "/" + p_if_eq(b, PREC_MUL), PREC_MUL);
}

static Term make_pow(const Term& base, int n) {
    if (n == 0) return Term("1", PREC_ATOM, false, true); // f^0 = 1
    if (n == 1) return base;                                // f^1 = f
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
        Term dx_str() const {
            return make_add(make_mul(f.dx_str(), g.str()), make_mul(f.str(), g.dx_str()));
        }
    
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
        return make_mul(make_mul(coef, lower_pow), f.dx_str());
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
            return make_mul(expf, f.dx_str());
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
        Term dx_str() const {return make_div(f.dx_str(), f.str());}
    
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
            Term neg_sinf("-sin(" + f.str().s + ")", PREC_ADD);
            return make_mul(neg_sinf, f.dx_str());
        }
    
    private:
        F f;
};

template <typename F>
auto cos(const F& f) {
    return Cos{f};
}

// ----------------------------------------------------------------------------
int main(){
    auto f = 3.0 + x + x;
    cout << "f(x) = " << f.str() << "\n f'(x) = " << f.dx_str() << endl;

    return 0;
}