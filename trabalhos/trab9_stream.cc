#include <type_traits>
#include <vector>
#include <iterator>
#include <functional>

using namespace std;

template<typename I, typename F>
auto operator | (const I& item, F funcao) {

    using T = decay_t<decltype(*begin(item))>;
    using RetornoFuncao = invoke_result_t<F, T>;

    if constexpr (is_same_v<RetornoFuncao, bool>) {

        vector<T> return_list;
        for (const auto& i : item) {
            if (invoke(funcao, i)) {
                return_list.push_back(i);
            }
        }
        return return_list;
    } 
    else if constexpr (is_same_v<RetornoFuncao, void>) {

        for (const auto& i : item) {
            invoke(funcao, i);
        }
    }
    else {

        vector<RetornoFuncao> return_list;
        for (const auto& i : item) {
            return_list.push_back(invoke(funcao, i));
        }
        return return_list;
    }
}
