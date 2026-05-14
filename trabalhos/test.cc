#include <type_traits>
#include <vector>
#include <iterator>
#include <functional> // <-- Essencial para o std::invoke

using namespace std;

template<typename I, typename F>
auto operator | (const I& item, F funcao) {

    using T = decay_t<decltype(*begin(item))>;
    using RetornoFuncao = invoke_result_t<F, T>;

    if constexpr (is_same_v<RetornoFuncao, bool>) {
        
        vector<T> resultado; 
        for (const auto& i : item) {
            // Usa invoke em vez de funcao(i)
            if (invoke(funcao, i)) { 
                resultado.push_back(i);
            }
        }
        return resultado;
        
    } 
    else if constexpr (is_same_v<RetornoFuncao, void>) {

        for (const auto& i : item) {
            // Usa invoke em vez de funcao(i)
            invoke(funcao, i); 
        }
        
    }
    else {
        
        vector<RetornoFuncao> resultado;
        for (const auto& i : item) {
            // Usa invoke em vez de funcao(i)
            resultado.push_back(invoke(funcao, i)); 
        }
        return resultado;
        
    }
}