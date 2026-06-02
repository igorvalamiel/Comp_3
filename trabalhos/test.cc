// 1. VERSÃO PARA LEITURA (Chamada quando o objeto for const)
Var operator[](string s) const {
    if (valor->t == T_OBJ) {
        const Object* objPtr = static_cast<const Object*>(valor.get());
        
        // Em funções const, não podemos usar atributos[s] porque ele cria a chave se não existir.
        // Usamos o .find() para apenas procurar sem alterar o mapa.
        auto it = objPtr->atributos.find(s);
        if (it != objPtr->atributos.end()) {
            return it->second; // Retorna o valor encontrado (cópia)
        }
        return Var(); // Se a chave não existir, retorna Undefined (estilo JavaScript)
    }
    
    if (valor->t != T_UNDEFINED) {
        throw Erro("Essa variável não é um objeto");
    }
    return Var();
}

// 2. VERSÃO PARA ESCRITA (Chamada quando o objeto NÃO for const)
Var& operator[](string s) {
    if (valor->t == T_UNDEFINED) {
        valor = shared_ptr<Undefined>(new Object());
    }

    if (valor->t == T_OBJ) {
        Object* objPtr = static_cast<Object*>(valor.get());
        return objPtr->atributos[s];
    }

    throw Erro("Essa variável não é um objeto");
}