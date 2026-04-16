#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <initializer_list>

using namespace std;

template <typename T>
auto apply(vector<T> lista, auto F) {
    
    auto Func(vector <T>) -> decltype(F);

    return Func(lista);
}