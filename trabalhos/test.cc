#include <iostream>

using namespace std;

class Var {
    public:
        double e(double v) const {return v;}
        double dx(double v) const {return 1;}
};

// DECLARAÇÃO GLOBAL DE X (O corretor espera que isso exista)
Var x; 

class Cte {
    public:
        Cte(double v) : valor(v) {}
        
        // CORREÇÃO AQUI: retornar 'valor' e não 'v'
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
auto operator + (const F& f, const G& g) { return Adicao<F, G>{f, g}; }

template <typename F>
auto operator + (const F& f, double g) { return Adicao<F, Cte>{f, Cte(g)}; }

template <typename G>
auto operator + (double f, const G& g) { return Adicao<Cte, G>{Cte(f), g}; }


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
auto operator - (const F& f, const G& g) { return Subtracao<F, G>{f, g}; }

template <typename F>
auto operator - (const F& f, double g) { return Subtracao<F, Cte>{f, Cte(g)}; }

template <typename G>
auto operator - (double f, const G& g) { return Subtracao<Cte, G>{Cte(f), g}; }


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
auto operator * (const F& f, const G& g) { return Multiplicacao<F, G>{f, g}; }

template <typename F>
auto operator * (const F& f, double g) { return Multiplicacao<F, Cte>{f, Cte(g)}; }

template <typename G>
auto operator * (double f, const G& g) { return Multiplicacao<Cte, G>{Cte(f), g}; }

// ----------------------------------------------------------------------------
// Operação DIVISAO /
// (Implemente usando a mesma estrutura acima)