#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <initializer_list>

using namespace std;

template <typename T, typename Funct>
auto apply(const T& lista, Funct F) {
    
    using tipo = decltype(F(lista[0]));

    vector <tipo> res;

    for (auto i : lista){
        res.push_back(F(i));
    }

    return res;
}

//usando opção para "escolha" automatica do tipo
template <typename T, typename Funct>
auto apply(const initializer_list<T>& lista, Funct F) {
    
    using tipo = decltype(F(*lista.begin()));

    vector <tipo> res;

    for (auto i : lista){
        res.push_back(F(i));
    }

    return res;
}

//int main() {
//    return 0;
//}