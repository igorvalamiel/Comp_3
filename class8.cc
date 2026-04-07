#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int main() {
    vector<int> v = { 5, 3, 8, 1, 9, 2 };

    // Inserção
    v.push_back(7);

    // Ordenação
    sort( v.begin(), v.end() );

    // Range-for
    for ( auto x : v )
        cout << x << " ";  // 1 2 3 5 7 8 9
    cout << '\n';

    // Busca binária (requer vetor ordenado)
    if ( binary_search( v.begin(), v.end(), 8 ) )
        cout << "encontrou 8\n";

    // count_if com lambda
    auto pares = count_if( v.begin(), v.end(),
                            [](int x){ return x % 2 == 0; });
    cout << "pares: " << pares << '\n';  // 2

    // Remoção de elemento no meio
    v.erase( v.begin() + 2 );  // remove índice 2 (o valor 3)

    v = { 10, 20, 30, 40, 50 };

    // ── Iteração manual ───────────────────────────────────────────────
    for ( auto it = v.begin(); it != v.end(); ++it )
    {cout << *it << " ";}    // 10 20 30 40 50
    cout << endl;

    // ── Range-for — açúcar sintático para o loop acima ────────────────
    for ( auto x : v )
    {cout << x << " ";}        // idêntico ao loop acima
    cout << endl;

    // ── Iteração reversa ───────────────────────────────────────────────
    for ( auto it = v.rbegin(); it != v.rend(); ++it )
    {cout << *it << " ";}    // percorre de trás para frente
    cout << endl;

    // ── Algoritmos STL ────────────────────────────────────────────────
    // A grande maioria recebe um par (begin, end) como intervalo
    sort     ( v.begin(), v.end() );
    reverse  ( v.begin(), v.end() );
    for_each ( v.begin(), v.end(), []( int x ){ cout << x << " "; } );
    count_if ( v.begin(), v.end(), []( int x ){ return x > 20; } );

    // ── Operações com posição ─────────────────────────────────────────
    v.insert( v.begin(),      99 );  // insere 99 no início
    v.insert( v.begin() + 2, 55 );  // insere 55 na posição 2
    v.erase( v.begin() );          // remove o primeiro
    v.erase( v.end() - 1 );       // remove o último

    // ── Subintervalo — begin/end não precisam ser do vetor inteiro ─────
    sort( v.begin() + 1, v.end() - 1 );  // ordena apenas do índice 1 ao penúltimo
    cout << endl;

    return 0;
}
