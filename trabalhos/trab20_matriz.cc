/*
 * COS625 - Tópicos Especiais em Programação - C++ (ECI)
 * Otimizador de Expressões para Matrizes
 * Padrão: C++17
 */

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace std;

// =============================================================================
// Forward declarations
// =============================================================================

template <int L, int C>
class Matriz;

// =============================================================================
// MatrizNode: nó folha — acesso elemento a elemento via função
// Armazena os dados ou referencia dados externos via shared_ptr
// =============================================================================

struct MatrizNode {
    int linhas;
    int colunas;
    // shared_ptr para os dados — garante que matrizes temporárias (como b+b)
    // sobrevivam enquanto o Otimizador existir
    shared_ptr<vector<double>> dados;

    MatrizNode(int l, int c)
        : linhas(l), colunas(c),
          dados(make_shared<vector<double>>(l * c, 0.0)) {}

    double& at(int i, int j)       { return (*dados)[i * colunas + j]; }
    double  at(int i, int j) const { return (*dados)[i * colunas + j]; }
};

// =============================================================================
// ExprNode: nó da árvore de expressão (lazy)
// =============================================================================

struct ExprNode {
    bool eh_folha = false;
    shared_ptr<MatrizNode> folha;
    shared_ptr<ExprNode>   esq;
    shared_ptr<ExprNode>   dir;
    int linhas = 0;
    int colunas = 0;

    // Folha
    explicit ExprNode(shared_ptr<MatrizNode> m)
        : eh_folha(true), folha(move(m)),
          linhas(folha->linhas), colunas(folha->colunas) {}

    // Produto
    ExprNode(shared_ptr<ExprNode> l, shared_ptr<ExprNode> r)
        : eh_folha(false), esq(move(l)), dir(move(r)),
          linhas(esq->linhas), colunas(dir->colunas) {}
};

// =============================================================================
// Algoritmo Matrix Chain Ordering — programação dinâmica O(n³)
// =============================================================================

static void coletarFolhas(const shared_ptr<ExprNode>& no,
                           vector<shared_ptr<MatrizNode>>& folhas) {
    if (no->eh_folha) folhas.push_back(no->folha);
    else {
        coletarFolhas(no->esq, folhas);
        coletarFolhas(no->dir, folhas);
    }
}

static vector<vector<int>>
calcularOrdemOtima(const vector<shared_ptr<MatrizNode>>& mats) {
    int n = (int)mats.size();
    vector<vector<long long>> custo(n, vector<long long>(n, 0LL));
    vector<vector<int>>       div  (n, vector<int>      (n, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            custo[i][j] = numeric_limits<long long>::max();
            for (int k = i; k < j; ++k) {
                long long c = custo[i][k] + custo[k+1][j]
                            + (long long)mats[i]->linhas
                            * (long long)mats[k]->colunas
                            * (long long)mats[j]->colunas;
                if (c < custo[i][j]) { custo[i][j] = c; div[i][j] = k; }
            }
        }
    }
    return div;
}

static shared_ptr<MatrizNode>
multiplicarNodes(const shared_ptr<MatrizNode>& A,
                 const shared_ptr<MatrizNode>& B) {
    int L = A->linhas, LC = A->colunas, C = B->colunas;
    auto R = make_shared<MatrizNode>(L, C);
    for (int i = 0; i < L; ++i)
        for (int k = 0; k < LC; ++k) {
            double aik = A->at(i, k);
            for (int j = 0; j < C; ++j)
                R->at(i, j) += aik * B->at(k, j);
        }
    return R;
}

static shared_ptr<MatrizNode>
executarOtimo(const vector<shared_ptr<MatrizNode>>& mats,
              const vector<vector<int>>& div, int i, int j) {
    if (i == j) return mats[i];
    int k = div[i][j];
    return multiplicarNodes(executarOtimo(mats, div, i, k),
                            executarOtimo(mats, div, k+1, j));
}

static shared_ptr<MatrizNode>
avaliarExpressao(const shared_ptr<ExprNode>& raiz) {
    vector<shared_ptr<MatrizNode>> folhas;
    coletarFolhas(raiz, folhas);
    if (folhas.size() == 1) return folhas[0];
    auto div = calcularOrdemOtima(folhas);
    return executarOtimo(folhas, div, 0, (int)folhas.size() - 1);
}

// =============================================================================
// Otimizador<Tag> — proxy lazy de expressão de multiplicação
// =============================================================================

template <typename Tag = void>
class Otimizador {
public:
    shared_ptr<ExprNode> arvore;

    explicit Otimizador(shared_ptr<ExprNode> a) : arvore(move(a)) {}

    int nLin() const { return arvore->linhas; }
    int nCol() const { return arvore->colunas; }

    // Avalia a expressão e retorna o MatrizNode resultado
    shared_ptr<MatrizNode> avaliar() const {
        return avaliarExpressao(arvore);
    }
};

// =============================================================================
// Matriz<L, C>
// =============================================================================

template <int L, int C>
class Matriz {
    static_assert(L > 0, "Linhas deve ser positivo");
    static_assert(C > 0, "Colunas deve ser positivo");

    // Dados armazenados em MatrizNode para facilitar compartilhamento
    // com o sistema de árvore (sem cópia extra ao criar folha)
    shared_ptr<MatrizNode> node_;

public:
    // --- Construtores --------------------------------------------------------

    Matriz() : node_(make_shared<MatrizNode>(L, C)) {}

    Matriz(const Matriz& o) : node_(make_shared<MatrizNode>(L, C)) {
        *node_->dados = *o.node_->dados;
    }

    Matriz& operator=(const Matriz& o) {
        if (this != &o) *node_->dados = *o.node_->dados;
        return *this;
    }

    // Construção/atribuição a partir de Otimizador (avalia lazy)
    template <typename Tag>
    Matriz(const Otimizador<Tag>& opt) : node_(make_shared<MatrizNode>(L, C)) {
        fromOtimizador(opt);
    }

    template <typename Tag>
    Matriz& operator=(const Otimizador<Tag>& opt) {
        fromOtimizador(opt);
        return *this;
    }

    // --- Dimensões -----------------------------------------------------------
    constexpr int nLin() const { return L; }
    constexpr int nCol() const { return C; }

    // --- Acesso mat[i][j] via proxy de linha ---------------------------------
    class LinhaProxy {
        double* ptr_;
    public:
        LinhaProxy(double* p) : ptr_(p) {}
        double& operator[](int j)       { return ptr_[j]; }
    };
    class LinhaProxyConst {
        const double* ptr_;
    public:
        LinhaProxyConst(const double* p) : ptr_(p) {}
        const double& operator[](int j) const { return ptr_[j]; }
    };

    LinhaProxy      operator[](int i)       { return {&(*node_->dados)[i*C]}; }
    LinhaProxyConst operator[](int i) const { return {&(*node_->dados)[i*C]}; }

    // --- Acesso ao nó interno (para criar folhas na árvore) ------------------
    shared_ptr<MatrizNode> node() const { return node_; }

private:
    template <typename Tag>
    void fromOtimizador(const Otimizador<Tag>& opt) {
        if (opt.nLin() != L || opt.nCol() != C)
            throw runtime_error("Dimensoes incompativeis ao atribuir Otimizador a Matriz");
        auto res = opt.avaliar();
        *node_->dados = *res->dados;
    }
};

// =============================================================================
// tamanho(m) — retorna L * C
// =============================================================================

template <int L, int C>
constexpr int tamanho(const Matriz<L,C>&) { return L * C; }

template <typename Tag>
int tamanho(const Otimizador<Tag>& opt) { return opt.nLin() * opt.nCol(); }

// =============================================================================
// operator* : Matriz * Matriz → Otimizador (lazy)
// Dimensões incompatíveis → erro de compilação (dedução de template falha)
// =============================================================================

template <int L, int LC, int C>
auto operator*(const Matriz<L,LC>& a, const Matriz<LC,C>& b) {
    auto exprA = make_shared<ExprNode>(a.node());
    auto exprB = make_shared<ExprNode>(b.node());
    auto raiz  = make_shared<ExprNode>(exprA, exprB);
    return Otimizador<>(raiz);
}

// Otimizador * Matriz
template <typename Tag, int LC, int C>
auto operator*(const Otimizador<Tag>& opt, const Matriz<LC,C>& b) {
    if (opt.nCol() != LC)
        throw runtime_error("Dimensoes incompativeis em multiplicacao");
    auto exprB = make_shared<ExprNode>(b.node());
    auto raiz  = make_shared<ExprNode>(opt.arvore, exprB);
    return Otimizador<>(raiz);
}

// Matriz * Otimizador
template <int L, int LC, typename Tag>
auto operator*(const Matriz<L,LC>& a, const Otimizador<Tag>& opt) {
    if (opt.nLin() != LC)
        throw runtime_error("Dimensoes incompativeis em multiplicacao");
    auto exprA = make_shared<ExprNode>(a.node());
    auto raiz  = make_shared<ExprNode>(exprA, opt.arvore);
    return Otimizador<>(raiz);
}

// Otimizador * Otimizador
template <typename T1, typename T2>
auto operator*(const Otimizador<T1>& lhs, const Otimizador<T2>& rhs) {
    if (lhs.nCol() != rhs.nLin())
        throw runtime_error("Dimensoes incompativeis em multiplicacao");
    auto raiz = make_shared<ExprNode>(lhs.arvore, rhs.arvore);
    return Otimizador<>(raiz);
}

// =============================================================================
// operator+ : sempre avalia imediatamente, retorna Matriz<L,C>
// (adição não tem "chain ordering"; resultado é usado como operando em *)
// =============================================================================

// Matriz + Matriz
template <int L, int C>
Matriz<L,C> operator+(const Matriz<L,C>& a, const Matriz<L,C>& b) {
    Matriz<L,C> r;
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < C; ++j)
            r[i][j] = a[i][j] + b[i][j];
    return r;
}

// Otimizador + Otimizador → avalia ambos, soma
template <typename T1, typename T2>
auto operator+(const Otimizador<T1>& lhs, const Otimizador<T2>& rhs) {
    if (lhs.nLin() != rhs.nLin() || lhs.nCol() != rhs.nCol())
        throw runtime_error("Dimensoes incompativeis em adicao");
    // Avalia ambos os lados
    auto resL = lhs.avaliar();
    auto resR = rhs.avaliar();
    int L = lhs.nLin(), C = lhs.nCol();
    // Cria nó resultado
    auto node = make_shared<MatrizNode>(L, C);
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < C; ++j)
            node->at(i,j) = resL->at(i,j) + resR->at(i,j);
    // Retorna como Otimizador folha para poder ser usado em expressões mistas
    auto expr = make_shared<ExprNode>(node);
    return Otimizador<>(expr);
}

// Matriz + Otimizador
template <int L, int C, typename Tag>
Matriz<L,C> operator+(const Matriz<L,C>& a, const Otimizador<Tag>& opt) {
    if (opt.nLin() != L || opt.nCol() != C)
        throw runtime_error("Dimensoes incompativeis em adicao");
    auto res = opt.avaliar();
    Matriz<L,C> r;
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < C; ++j)
            r[i][j] = a[i][j] + res->at(i,j);
    return r;
}

// Otimizador + Matriz
template <typename Tag, int L, int C>
Matriz<L,C> operator+(const Otimizador<Tag>& opt, const Matriz<L,C>& b) {
    if (opt.nLin() != L || opt.nCol() != C)
        throw runtime_error("Dimensoes incompativeis em adicao");
    auto res = opt.avaliar();
    Matriz<L,C> r;
    for (int i = 0; i < L; ++i)
        for (int j = 0; j < C; ++j)
            r[i][j] = res->at(i,j) + b[i][j];
    return r;
}

// =============================================================================
// apply(f) — função livre que retorna functor Apply<F>
// =============================================================================

template <typename F>
class Apply {
public:
    explicit Apply(F f) : f_(move(f)) {}

    template <int L, int C>
    Matriz<L,C> operator()(const Matriz<L,C>& m) const {
        Matriz<L,C> r;
        for (int i = 0; i < L; ++i)
            for (int j = 0; j < C; ++j)
                r[i][j] = f_(m[i][j]);
        return r;
    }

private:
    F f_;
};

// Função livre apply() — constrói o Apply<F> por dedução de tipo (C++17)
template <typename F>
auto apply(F f) { return Apply<F>(move(f)); }

// =============================================================================
// TESTES
// =============================================================================

int main() {
    // -----------------------------------------------------------------------
    // Teste 1
    // -----------------------------------------------------------------------
    {
        int i = 0;
        auto f0    = apply( [&i]( double x ) { return (double)(i++); } );
        auto print = apply( []( double x ) { cout << x << " "; return x; } );
        auto f2    = apply( []( double x ) { return x*x; } );
        Matriz<4,3> m;
        m = f0( m );
        print( m );
        cout << endl;
        m = f2( m );
        print( m );
        cout << endl;
        cout << tamanho( m ) << endl;
    }

    // -----------------------------------------------------------------------
    // Teste 2
    // -----------------------------------------------------------------------
    {
        Matriz<100,1> a;
        Matriz<1,100> b;
        auto x = a * b;
        auto y = b * a;
        cout << tamanho( x ) << endl;
        cout << tamanho( y ) << endl;
    }

    // -----------------------------------------------------------------------
    // Teste 3
    // -----------------------------------------------------------------------
    {
        Matriz<17,93> a, b;
        auto x = a + b;
        auto y = a + b + a + b;
        cout << tamanho( x ) << endl;
        cout << tamanho( y ) << endl;
    }

    // -----------------------------------------------------------------------
    // Teste 4
    // -----------------------------------------------------------------------
    {
        Matriz<1000,1> a;
        Matriz<1,1000> b;
        Matriz<1000,1000> c;
        int tempo = 0, lapso1, lapso2;
        tempo = clock();
        Matriz<1000,1000> x1 = a * b;
        Matriz<1000,1000> x2 = x1 * c;
        cout << tamanho(x1) << endl;
        cout << tamanho(x2) << endl;
        lapso1 = clock() - tempo;
        tempo = clock();
        Matriz<1000,1000> y = a * b * c;
        lapso2 = clock() - tempo;
        cout << tamanho(y) << endl;
        if( lapso1/lapso2 > 10 )
            cout << "Otimizou" << endl;
        else
            cout << "Não otimizou" << endl;
    }

    // -----------------------------------------------------------------------
    // Teste 5
    // -----------------------------------------------------------------------
    {
        Matriz<997,1> a;
        Matriz<1,997> b;
        Matriz<997,997> c;
        int tempo = 0, lapso1, lapso2;
        tempo = clock();
        Matriz<997,997> x1 = a * b;
        Matriz<997,997> x2 = x1 * c;
        Matriz<997,997> t1 = a * (b + b);
        Matriz<997,997> x3 = x2 + t1 * c;
        cout << tamanho(x1) << endl;
        cout << tamanho(x2) << endl;
        cout << tamanho(t1) << endl;
        cout << tamanho(x3) << endl;
        lapso1 = clock() - tempo;
        tempo = clock();
        Matriz<997,997> y = a * b * c + a * (b + b) * c;
        lapso2 = clock() - tempo;
        cout << tamanho(y) << endl;
        if( lapso1/lapso2 > 10 )
            cout << "Otimizou" << endl;
        else
            cout << "Não otimizou" << endl;
    }

    return 0;
}