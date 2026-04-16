#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <initializer_list>

using namespace std;

auto apply(auto lista, auto F) {
    
    auto Func(auto L) -> decltype(F);

    return Func(lista);
}