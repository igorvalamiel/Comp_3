#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <initializer_list>

using namespace std;

template <typename T>
auto apply(vector<T> lista, auto F) {
    
    auto tipo = decltype(F(lista[0]));
    
    cout << tipo << endl;

    vector <tipo> res;
    for (auto i : lista){
        res.push_back(F(i))
    }

    return res;
}

//int main() {
//    return 0;
//}