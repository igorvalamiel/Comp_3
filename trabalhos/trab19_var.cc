#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <map>
#include <functional>
#include <vector>

using namespace std;

enum Tipo { T_UNDEFINED, T_INT, T_DOUBLE, T_STR, T_CHAR, T_OBJ, T_FUNC, T_BOOL, T_ARR};

class Var;

class Undefined {
    public:
        Tipo t;
        Undefined(Tipo t = T_UNDEFINED) : t(t) {}
        virtual ~Undefined() = default;
        virtual void print(ostream& os) const {os << "undefined";}
        virtual Var Somar(const Undefined* outro) const;
        virtual Var Subtrai(const Undefined* outro) const;
        virtual Var Multiplica(const Undefined* outro) const;
        virtual Var Divide(const Undefined* outro) const;
        virtual Var Menor(const Undefined* outro) const;

        virtual double asNumber() const {return 0.0;}
        virtual string asString() const {return "undefined";}
        virtual bool asBool() const {return false;}
        virtual bool isNumber() const {return false;}

        template <class F>
        Undefined forEach(F f = T_FUNC) const;
        
        template <class F>
        Var filter(F f = T_FUNC) const;

};


class Var {
    public:
        Var() : valor( new Undefined() ) {}
        Var(int n) : valor( shared_ptr<Undefined>(new Int(n))) {}
        Var(double n) : valor( shared_ptr<Undefined>(new Double(n))) {}
        Var(const char* s) : valor(new String(string(s))) {}
        Var(string s) : valor(new String(s)) {}
        Var(char c) : valor(new Char(c)) {}
        Var(bool b) : valor(new Bool(b)) {}
        template<class F>
        Var(F f) : valor(shared_ptr<Undefined>(new Function([f](Var x) { return f(x); }))) {}

        // Classes
        class Int: public Undefined {
            public:
                int n;
                Int( int n ): Undefined(T_INT), n(n) {}
                virtual void print(ostream& os) const override {os << n;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->n + static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->n + static_cast<const Double*>(outro)->d);
                    return Var();
                }
                
                virtual Var Subtrai(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->n - static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->n - static_cast<const Double*>(outro)->d);
                    return Var();
                }

                virtual Var Multiplica(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->n * static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->n * static_cast<const Double*>(outro)->d);
                    return Var();
                }

                virtual Var Divide(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->n / static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->n / static_cast<const Double*>(outro)->d);
                    return Var();
                }

                virtual Var Menor(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->n < static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->n < static_cast<const Double*>(outro)->d);
                    if (outro->t == T_CHAR) return Var(this->n < static_cast<const Char*>(outro)->c);
                    return Var();
                }

                virtual double asNumber() const override { return static_cast<double>(n); }

                virtual string asString() const override { return to_string(n); }

                virtual bool asBool() const override { return (bool)n; }

                virtual bool isNumber() const override { return true; }
        };

        class Double: public Undefined {
            public:
                double d;
                Double( double d ): Undefined(T_DOUBLE), d(d) {}
                virtual void print(ostream& os) const override {os << d;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->d + static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->d + static_cast<const Double*>(outro)->d);
                    return Var();
                }
                
                virtual Var Subtrai(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->d - static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->d - static_cast<const Double*>(outro)->d);
                    return Var();
                }

                virtual Var Multiplica(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->d * static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->d * static_cast<const Double*>(outro)->d);
                    return Var();
                }

                virtual Var Divide(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->d / static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->d / static_cast<const Double*>(outro)->d);
                    return Var();
                }

                virtual Var Menor(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->d < static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->d < static_cast<const Double*>(outro)->d);
                    if (outro->t == T_CHAR) return Var(this->d < static_cast<const Char*>(outro)->c);
                    return Var();
                }

                virtual double asNumber() const override { return d; }

                virtual string asString() const override { 
                    ostringstream ss;
                    ss << d;
                    return ss.str();
                }

                virtual bool asBool() const override { return (bool)d; }

                virtual bool isNumber() const override { return true; }
        };
        
        class String: public Undefined {
            public:
                string s;
                String( string s ): Undefined(T_STR), s(s) {}
                virtual void print(ostream& os) const override {os << s;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_STR) return Var(this->s + static_cast<const String*>(outro)->s);
                    if (outro->t == T_CHAR) return Var(this->s + static_cast<const Char*>(outro)->c);
                    return Var();
                }

                virtual Var Menor(const Undefined* outro) const override {
                    if (outro->t == T_STR) return Var(this->s < static_cast<const String*>(outro)->s);
                    return Var();
                }

                virtual string asString() const override { return s; }

                virtual bool asBool() const override { return !s.empty(); }

                // OBS: usei IA nessa aqui 
                virtual bool isNumber() const override {
                    size_t first = s.find_first_not_of(" \t\n\r");
                    if (first == string::npos) return true; // String vazia "" ou só espaços vira 0 (isNumber = true)

                    size_t last = s.find_last_not_of(" \t\n\r");
                    string trimmed = s.substr(first, last - first + 1);

                    char* endptr;
                    strtod(trimmed.c_str(), &endptr);
                    return *endptr == '\0'; // Se processou tudo, é um número válido
                }
        };

        class Char: public Undefined {
            public:
                char c;
                Char( char c ): Undefined(T_CHAR), c(c) {}
                virtual void print(ostream& os) const override {os << c;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_CHAR) return Var(string(1, this->c) + static_cast<const Char*>(outro)->c);
                    if (outro->t == T_STR) return Var(string(1, this->c) + static_cast<const String*>(outro)->s);
                    if (outro->t == T_INT) return Var((int)this->c + static_cast<const Int*>(outro)->n);
                    return Var();
                }

                virtual Var Menor(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var((int)this->c < static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var((double)this->c < static_cast<const Double*>(outro)->d);
                    if (outro->t == T_CHAR) return Var(this->c < static_cast<const Char*>(outro)->c);
                    if (outro->t == T_STR) return Var(string(1, this->c) < static_cast<const String*>(outro)->s);
                    return Var();
                }

                virtual double asNumber() const override { return static_cast<double>(c); }

                virtual string asString() const override { return string(1, c); }

                virtual bool isNumber() const override { return c >= '0' && c <= '9'; }
                virtual bool asBool() const override { return true; }
        };

        class Function : public Undefined {
            public:
                function<Var(Var)> func; 

                Function(function<Var(Var)> f): Undefined(T_FUNC), func(f) {}
                
                virtual void print(ostream& os) const override { os << "function"; }
                
                Var executar(Var arg) const { return func(arg); }
        };


        class Object: public Undefined {
            public:
                map<string, Var> atributos;
                Object(): Undefined(T_OBJ) {}
                virtual void print(ostream& os) const override {
                    os << "{ ";
                    for (auto it = atributos.begin(); it != atributos.end(); ++it) {
                        os << "\"" << it->first << "\": " << it->second;
                        if (next(it) != atributos.end()) os << ", ";
                    }
                    os << " }";
                }

                virtual string asString() const override { return "object"; }

                virtual bool asBool() const override { return true; }
        };

        class Array: public Object {
            public:
                vector<Var> lista;
                Array(): Object() {
                    this->t=T_ARR;

                    for (auto i : this->atributos) { lista.push_back(i.second); }
                }
                int size = lista.size();

                Undefined forEach(Function F) {
                    for (int i=0; i < this->size; i++) {
                        this->lista[i] = F.executar(lista[i]);
                    }
                    return Undefined();
                }

                Var filter(Function F) {
                    Var ret = Var::createARR();
                    Array* arrPtr = static_cast<Array*>(ret.valor.get());
                    
                    for (size_t i = 0; i < this->lista.size(); i++) {
                        if (F.executar(lista[i]).asBool()) arrPtr->lista.push_back(lista[i]);
                    }
                    return ret;
                }


        };

        class Bool: public Undefined {
            public:
                bool b;
                Bool(bool b) : Undefined(T_BOOL), b(b) {}
                virtual void print(ostream& os) const override {os << (b ? "true" : "false");}

                virtual Var Somar(const Undefined*) const override {return Var();}

                virtual Var Subtrai(const Undefined*) const override {return Var();}

                virtual Var Multiplica(const Undefined*) const override {return Var();}

                virtual Var Divide(const Undefined*) const override {return Var();}

                virtual Var Menor(const Undefined* outro) const override {
                    if (outro->t == T_BOOL) return Var(this->b < static_cast<const Bool*>(outro)->b);
                    return Var();
                }

                virtual double asNumber() const override { return static_cast<double>(b); }

                virtual string asString() const override { 
                    if (b) return "true";
                    else if (!b) return "false";
                    else return to_string(b);
                }

                virtual bool asBool() const override { return b; }

                virtual bool isNumber() const override { return true; }
        };

        class Erro {
            public:
                Erro( string msg ): msg(msg) {}
                
                string operator()() const { return msg; }
            private:
                string msg;
        };

        
        // Conversão e Verificação de tipo
        bool isNumber() const {
            if (this->valor) return this->valor->isNumber();
            return false;
        }

        bool isString() {
            return this->type() == "string";
        }

        bool isBool() {
            return this->type() == "bool";
        }

        bool isObject() {
            return this->type() == "object" || this->type() == "array";
        }
        
        bool isUndefined() {
            return this->type() == "undefined";
        }

        double asNumber() const {
            if (this->valor) return this->valor->asNumber();
            return 0.0;
        }

        string asString() const {
            if (this->valor) return this->valor->asString();
            return "null";
        }

        bool asBool() const {
            if (this->valor) return this->valor->asBool();
            return false;
        }

        operator int() const { return this->asNumber(); }
        
        operator double() const { return this->asNumber(); }
        
        operator bool() const { return this->asBool(); }
        
        operator std::string() const { return this->asString(); }

        
        // Operators
        Var& operator = (int n) {
            valor = shared_ptr<Undefined>( new Int( n ) );
            return *this;
        }

        Var& operator = (double d) {
            valor = shared_ptr<Undefined>( new Double( d ) );
            return *this;
        }

        Var& operator = (string s) {
            valor = shared_ptr<Undefined>( new String( s ) );
            return *this;
        }

        Var& operator = (const char* s) {
            valor = shared_ptr<Undefined>(new String(s));
            return *this;
        }

        Var& operator = (char c) {
            valor = shared_ptr<Undefined>(new Char(c));
            return *this;
        }

        template<class F>
        Var& operator=(F f) {
            valor = shared_ptr<Undefined>(new Function([f](Var x) { return f(x); }));
            return *this;
        }

        friend Var operator+(const Var& a, const Var& b) {
            return a.valor->Somar(b.valor.get());
        }

        friend Var operator-(const Var& a, const Var& b) {
            return a.valor->Subtrai(b.valor.get());
        }

        friend Var operator*(const Var& a, const Var& b) {
            return a.valor->Multiplica(b.valor.get());
        }

        friend Var operator/(const Var& a, const Var& b) {
            return a.valor->Divide(b.valor.get());
        }


        // lendo OBJ+ARR
        Var operator[](const Var& v) const {
            if (v.type() == "string") {
                if (valor->t == T_OBJ) {
                    const Object* objPtr = static_cast<const Object*>(valor.get());
                    
                    string s = v.valor->asString();
                    auto it = objPtr->atributos.find(s);
                    if (it != objPtr->atributos.end()) { return (it->second); }
                    return Var();
                }
                throw Erro("Essa variável não é um objeto");
            } else if (v.type() == "int") {
                if (valor->t == T_ARR) {
                    const Array* arrPtr = static_cast<const Array*>(valor.get());
                    int n = v.valor->asNumber();
                    if (n < 0 || n >= (int)arrPtr->lista.size()) return Var();
                    return arrPtr->lista[n];  // return the actual element
                }
                throw Erro("Essa variável não é um objeto");
            }
        }

        // escrevendo OBJ+ARR
        Var& operator[](const Var& v) {
            if (valor->t == T_ARR) {
                Array* arrPtr = static_cast<Array*>(valor.get());
                
                if ((v.type() == "int" || v.type() == "double") && v.asNumber() >= 0) {
                    int n = static_cast<int>(v.asNumber());                    
                    if (n >= (int)arrPtr->lista.size()) { arrPtr->lista.resize(n + 1); }
                    return arrPtr->lista[n];
                } else {
                    string s = v.asString();
                    return arrPtr->atributos[s];
                }
            } 
            else if (valor->t == T_OBJ) {
                Object* objPtr = static_cast<Object*>(valor.get());
                string s = v.asString();
                return objPtr->atributos[s];
            }
            throw Erro("Essa variável não é um objeto");
        }

        Var& operator[](const char* s) { return operator[](Var(s)); }
        Var operator[](const char* s) const { return operator[](Var(s)); }

        Var& operator[](const string& s) { return operator[](Var(s)); }
        Var operator[](const string& s) const { return operator[](Var(s)); }

        Var& operator[](int n) { return operator[](Var(n)); }
        Var operator[](int n) const { return operator[](Var(n)); }

        Var operator()(Var arg) const {
            auto f = std::dynamic_pointer_cast<Function>(valor);
            if (f) {
                return f->func(arg);
            }
            return Var();
        }


        friend ostream& operator<<(ostream& os, const Var& v) {
            if (v.valor->t == T_OBJ) {
                os << v.type();
                return os;
            } else if (v.valor) {
                v.valor->print(os);
            } 
            else {
                os << "null";
            }
            return os;
        }

        friend Var operator < (const Var& a, const Var& b) {
            return a.valor->Menor(b.valor.get());
        }

        friend Var operator || ( const Var& a, const Var& b) {
            if (a.valor->t == T_BOOL && b.valor->t == T_BOOL) {
                bool va = static_cast<const Bool*>(a.valor.get())->b;
                bool vb = static_cast<const Bool*>(b.valor.get())->b;
                return Var(va || vb);
            }
            return Var();
        }
        
        friend Var operator && ( const Var& a, const Var& b) {
            if (a.valor->t == T_BOOL && b.valor->t == T_BOOL) {
                bool va = static_cast<const Bool*>(a.valor.get())->b;
                bool vb = static_cast<const Bool*>(b.valor.get())->b;
                return Var(va && vb);
            }
            return Var();
        }

        friend Var operator ! ( const Var& a) {
            return Var(!a.asBool());
        }

        friend Var operator > (const Var& a, const Var& b) { return b<a; }
        friend Var operator != ( const Var& a, const Var& b ) { return (a<b) || (b<a); }

        friend Var operator==(const Var& a, const Var& b) {
            if (a.valor->t != b.valor->t){
                if ((a.valor->t == T_CHAR && b.valor->t == T_STR) || (b.valor->t == T_CHAR && a.valor->t == T_STR)) {
                    return Var(a.asString() == b.asString());
                } else return Var(false);
            }

            switch(a.valor->t) {
                case T_INT:
                    return Var(
                        static_cast<const Int*>(a.valor.get())->n ==
                        static_cast<const Int*>(b.valor.get())->n
                    );

                case T_DOUBLE:
                    return Var(
                        static_cast<const Double*>(a.valor.get())->d ==
                        static_cast<const Double*>(b.valor.get())->d
                    );

                case T_CHAR:
                    return Var(
                        static_cast<const Char*>(a.valor.get())->c ==
                        static_cast<const Char*>(b.valor.get())->c
                    );

                case T_STR:
                    return Var(
                        static_cast<const String*>(a.valor.get())->s ==
                        static_cast<const String*>(b.valor.get())->s
                    );

                case T_BOOL:
                    return Var(
                        static_cast<const Bool*>(a.valor.get())->b ==
                        static_cast<const Bool*>(b.valor.get())->b
                    );

                default:
                    return Var(false);
            }
        }

        friend Var operator <= ( const Var& a, const Var& b ) { return !(b<a); }
        friend Var operator >= ( const Var& a, const Var& b ) { return !(a<b); }

        // mudando operadores para receber var e int
        friend Var operator < (const Var& a, const int& b) { return a.asNumber() < b; }
        friend Var operator < (const int& a, const Var& b) { return a < b.asNumber(); }

        friend Var operator > (const Var& a, const int& b) { return b < a.asNumber(); }
        friend Var operator > (const int& a, const Var& b) { return b.asNumber() < a; }

        friend Var operator+(const Var& a, const int& b) { return a.asNumber() + b; }
        friend Var operator+(const int& a, const Var& b) { return a + b.asNumber(); }

        friend Var operator-(const Var& a, const int& b) { return a.asNumber() - b; }
        friend Var operator-(const int& a, const Var& b) { return a - b.asNumber(); }

        friend Var operator*(const Var& a, const int& b) { return a.asNumber() * b; }
        friend Var operator*(const int& a, const Var& b) { return a * b.asNumber(); }

        friend Var operator/(const Var& a, const int& b) { return a.asNumber() / b; }
        friend Var operator/(const int& a, const Var& b) { return a / b.asNumber(); }

        friend Var operator == ( const Var& a, const int& b ) { return !(a.asNumber()!=b); }
        friend Var operator == ( const int& a, const Var& b ) { return !(a!=b.asNumber()); }

        // auxiliar functions
        static Var createOBJ() {
            Var v;
            v.valor = shared_ptr<Undefined>( new Object() );
            return v;
        }

        static Var createARR() {
            Var v;
            v.valor = shared_ptr<Undefined>( new Array() );
            return v;
        }

        template <class F>
        Undefined  forEach(F f) const {
            if (valor && valor->t == T_ARR) {
                Array* arrPtr = static_cast<Array*>(valor.get());
                for (auto& i : arrPtr->lista) f(i);
            }
            return Undefined();
        }

        template <class F>
        Var filter(F f) {
            Var ret = Var::createARR();
            if (valor && valor->t == T_ARR) {
                Array* arrPtrOriginal = static_cast<Array*>(valor.get());
                Array* arrPtrNovo = static_cast<Array*>(ret.valor.get());
                
                for (auto& i : arrPtrOriginal->lista) {
                    if (f(i))arrPtrNovo->lista.push_back(i);
                }
            }
            return ret;
        }   

        Var indexOf(const Var& valor_buscado) const {
            if (valor && valor->t == T_ARR) {
                const Array* arrPtr = static_cast<const Array*>(valor.get());
                
                for (int i = 0; i < (int)arrPtr->lista.size(); i++) {
                    if ((arrPtr->lista[i] == valor_buscado).asBool()) return Var(i);
                }
            }
            return Var(-1);
        }

        Var indexOf(const char* s) const { return indexOf(Var(s)); }
        Var indexOf(const string& s) const { return indexOf(Var(s)); }
        Var indexOf(int n) const { return indexOf(Var(n)); }
        Var indexOf(char c) const { return indexOf(Var(c)); }

        // só pra me ajudar no debug (acabou q vou usar kakakakka)
        string type() const {
            vector <string> types =  {"undefined", "int", "double", "string", "char", "object", "function", "bool", "array"};
            return types[valor->t];
        }


    private:
        shared_ptr<Undefined> valor;
};

Var Undefined::Somar(const Undefined* outro) const {return Var();}

Var Undefined::Subtrai(const Undefined* outro) const {return Var();}

Var Undefined::Multiplica(const Undefined* outro) const {return Var();}

Var Undefined::Divide(const Undefined* outro) const {return Var();}

Var Undefined::Menor(const Undefined* outro) const {return Var();}

Var newObject() {
    Var obj;
    return obj.createOBJ();
}

Var newArray() {
    Var arr;
    return arr.createARR();
}
