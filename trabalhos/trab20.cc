#include <array>
#include <memory>
#include <vector>

using namespace std;


struct Node {
    public:
        int linhas;
        int colunas;
        shared_ptr<vector<double>> data;

        Node(int l, int c) : linhas(l), colunas(c), data(make_shared<vector<double>>(l*c, 0.0)) {} // to contruindo um nó aqui

        double& at(int i, int j)       { return (*data)[i * colunas + j]; }
        double  at(int i, int j) const { return (*data)[i * colunas + j]; }
};

template <int L, int C>
class Matriz {
    public:
        Matriz() : mat_node(make_shared<Node>(L, C)) {}

        shared_ptr<Node> node() const { return mat_node; }

        struct Linha {
            double* pointer;
            double& operator[](int j) { return pointer[j]; }
        };
        struct LinhaConst {
            const double* pointer;
            const double& operator[](int j) const { return pointer[j]; }
        };

        Linha operator[](int i) { return {&mat_node->at(i, 0)}; }
        LinhaConst operator[](int i) const { return {&mat_node->at(i, 0)}; }

        constexpr int nLin() const { return L; }
        constexpr int nCol() const { return C; }

    private:
        shared_ptr<Node> mat_node;
};

class Otimizador {
    public:
        vector<shared_ptr<Node>> operandos;
        int linhas, colunas;

        Otimizador(shared_ptr<Node> a, shared_ptr<Node> b) : linhas(a->linhas), colunas(b->colunas) {
            operandos.push_back(a);
            operandos.push_back(b);
        }

        void add(shared_ptr<Node> no) {
            colunas = no->colunas;
            operandos.push_back(no);
        }

        int nLin() const { return linhas;  }
        int nCol() const { return colunas; }
};
 
template <int L, int LC, int C>
Otimizador operator * ( const Matriz<L,LC>& a, const Matriz<LC,C>& b ) {
    auto nA = a.node();
    auto nB = b.node();
    return Otimizador(nA, nB);
}

template <int LC, int C>
Otimizador operator*(Otimizador opt, const Matriz<LC,C>& b) {
    opt.add(b.node());
    return opt;
}
 
template <typename F>
class Apply {
    public:
        Apply( F f ): f(f) {}

    template <int L, int C>
    Matriz<L,C> operator()( const Matriz<L,C>& m ) const {
        Matriz<L,C> resultado;
        for (int i = 0; i < L; ++i)
            for (int j = 0; j < C; ++j)
                resultado[i][j] = f(m[i][j]);
        return resultado;
    }

    void operator()(const Otimizador& o) const {
        for(int i=0;i<o.nLin();i++)
            for(int j=0;j<o.nCol();j++)
                f(0.0);
    }

    private:
    F f;
};