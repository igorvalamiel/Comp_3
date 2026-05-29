#include <iostream>
#include <algorithm>
#include <array>

using namespace std;

// 1. Declaramos a classe base com um tipo padrão (I = void) para o segundo parâmetro
template <typename F, typename I = void>
class Bind {
    public:
        Bind(F f, I i) : Func(f), item(i) {}

        template <typename... Args>
        auto operator()(Args... args) {
            return Func(item, args...);
        }
        
    private:
        F Func;
        I item;
};

// 2. Especialização da classe para quando o segundo tipo for 'void' (apenas a função)
template <typename F>
class Bind<F, void> { // Indicamos aqui que esta é a versão específica para 'void'
    public:
        Bind(F f) : Func(f) {}

        template <typename... Args>
        auto operator()(Args... args) {
            return Func(args...);
        }
        
    private:
        F Func;
        // Removido o 'I item' porque aqui o tipo 'I' seria void (e não podemos ter variáveis void)
};

// 3. Suas funções auxiliares agora funcionam perfeitamente!
template <typename F, typename I>
Bind<F, I> bind(const F Func, const I item) {
    return Bind<F, I>(Func, item);
}

template <typename F>
Bind<F, void> bind(F Func) { // Especificamos 'void' aqui para casar com a especialização
    return Bind<F, void>(Func);
}