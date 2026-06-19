#include <iostream>
#include <memory>
#include <string>
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

                virtual string asString() const override { return to_string(d); }

                virtual bool asBool() const override { return (bool)d; }
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

                virtual bool asBool() const override { return !s.empty(); }
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

                virtual string asString() const override { return to_string(c); }

                virtual bool asBool() const override { return (bool)c; }
        };

        class Function : public Undefined {
            public:                
                Function(function<Var(Var)> f): Undefined(T_FUNC), func(f) {}
                
                virtual void print(ostream& os) const override { os << "function"; }
                
                Var executar(Var arg) const { return func(arg); }

            private:
                function<Var(Var)> func;   
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
        };

        class Array: public Object {
            public:
                vector<Var> lista;
                Array(): Object() {
                    this->t=T_ARR;

                    for (auto i : this->atributos) { lista.push_back(i.second); }
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
        };

        class Erro {
            public:
                Erro( string msg ): msg(msg) {}
                
                string operator()() const { return msg; }
            private:
                string msg;
        };

        
        // Conversão e Verificação de tipo
        bool isNumber() {
            return this->type() == "int" || this->type() == "double" || this->type() == "bool";
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
        /*
        Var operator[](Var v) const {
            if (v.type() == "string") {
                if (valor->t == T_OBJ) {
                    const Object* objPtr = static_cast<const Object*>(valor.get());
                    
                    string s = v.valor;
                    auto it = objPtr->atributos.find(s);
                    if (it != objPtr->atributos.end()) { return it->second; }
                    return Var();
                }
                throw Erro("Essa variável não é um objeto");
            } else if (v.type() == "int") {
                if (valor->t == T_ARR) {
                const Array* arrPtr = static_cast<const Array*>(valor.get());
                
                int n = v.valor;
                auto it = arrPtr->lista[n];
                if (n > (int)arrPtr->lista.size()) { return arrPtr->lista[n].type(); }
                return Var();
            }
            throw Erro("Essa variável não é um objeto");
            }
            throw Erro("Essa variável não é um objeto");
        }
        */
/*
        // lendo OBJ
        Var operator[](string s) const {
            if (valor->t == T_OBJ) {
                const Object* objPtr = static_cast<const Object*>(valor.get());
                
                auto it = objPtr->atributos.find(s);
                if (it != objPtr->atributos.end()) { return it->second; }
                return Var();
            }
            throw Erro("Essa variável não é um objeto");
        }

        // escrevendo OBJ
        Var& operator[](string s) {
            if (valor->t == T_OBJ) {
                Object* objPtr = static_cast<Object*>(valor.get());
                return objPtr->atributos[s];
            }
            throw Erro("Essa variável não é um objeto");
        }

        // lendo ARR7
        Var operator[](int n) const {
            if (valor->t == T_ARR) {
                const Array* arrPtr = static_cast<const Array*>(valor.get());
                
                auto it = arrPtr->lista[n];
                if (n > (int)arrPtr->lista.size()) { return arrPtr->lista[n].type(); }
                return Var();
            }
            throw Erro("Essa variável não é um objeto");
        }

        // Escrevendo ARR
        Var& operator[](int n) {
            if (valor->t == T_ARR) {
                Array* arrPtr = static_cast<Array*>(valor.get());
                return arrPtr->lista[n];
            }
            throw Erro("Essa variável não é um objeto");
        }

        Var operator()(Var arg) const {
            if (valor->t == T_FUNC) {
                const Function* fPtr = static_cast<const Function*>(valor.get());
                return fPtr->executar(arg);
            }
            throw Erro("Essa variável não pode ser usada como função");
        }
*/


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
            if (a.valor->t == T_UNDEFINED) return true;
            return !Var().asBool(); 
        }

        friend Var operator > (const Var& a, const Var& b) { return b<a; }
        friend Var operator != ( const Var& a, const Var& b ) { return (a<b) || (b<a); }
        friend Var operator == ( const Var& a, const Var& b ) { return !(a!=b); }
        friend Var operator <= ( const Var& a, const Var& b ) { return !(b<a); }
        friend Var operator >= ( const Var& a, const Var& b ) { return !(a<b); }


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
