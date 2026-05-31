#include <iostream>
#include <memory>
#include <string>
#include <map>

using namespace std;

enum Tipo { T_UNDEFINED, T_INT, T_DOUBLE, T_STR, T_OBJ, T_FUNC };

class Var;

class Undefined {
    public:
        Tipo t;
        Undefined(Tipo t = T_UNDEFINED) : t(t) {}
        virtual ~Undefined() = default;
        virtual void print(ostream& os) const {os << "undefined";}
        virtual Var Somar(const Undefined* outro) const;
};

 
class Var {
    public:
        // inicializadores
        Var() : valor( new Undefined() ) {}
        Var(int n) : valor( shared_ptr<Undefined>(new Int(n))) {}
        Var(double n) : valor( shared_ptr<Undefined>(new Double(n))) {}
        Var(const char* s) : valor(new String(string(s))) {}
        Var(string s) : valor(new String(s)) {}
        Var(bool b) : valor(new Int(b ? 1 : 0)) {} 
        Var(char c) : valor(new Int((int)c)) {}

        // Classes
        class Int: public Undefined {
            public:
                int n;
                Int( int n ): Undefined(T_INT), n(n) {}
                virtual void print(ostream& os) const override {os << n;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->n + static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->n + static_cast<const Double*>(outro)->d);
                    throw Var::Erro("Não sei somar isso");
                }
        };

        class Double: public Undefined {
            public:
                double d;
                Double( double d ): Undefined(T_DOUBLE), d(d) {}
                virtual void print(ostream& os) const override {os << d;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_INT) return Var(this->d + static_cast<const Int*>(outro)->n);
                    if (outro->t == T_DOUBLE) return Var(this->d + static_cast<const Double*>(outro)->d);
                    throw Var::Erro("Não sei somar isso");
                }

        };
        
        class String: public Undefined {
            public:
                string s;
                String( string s ): Undefined(T_STR), s(s) {}
                virtual void print(ostream& os) const override {os << s;}
        };

        class Function : public Undefined {
            public:
                function<Var(Var)> func;
                Function(function<Var(Var)> f): Undefined(T_FUNC), func(f) {}
                virtual void print(ostream& os) const override { os << "[Function]"; }
                Var executar(Var arg) { return func(arg); }
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

        class Erro {
            public:
                Erro( string msg ): msg(msg) {}
                
                string operator()() const { return msg; }
            private:
                string msg;
        };

        // Operators
        
        Var operator = (int n) {
            valor = shared_ptr<Undefined>( new Int( n ) );
            return *this;
        }

        Var operator = (double d) {
            valor = shared_ptr<Undefined>( new Double( d ) );
            return *this;
        }

        Var operator = (string s) {
            valor = shared_ptr<Undefined>( new String( s ) );
            return *this;
        }

        Var operator+(const Var& outro) const {
            return this->valor->Somar(outro.valor.get());
        }

        //Var operator > ( const Var& a, const Var& b ) { return b<a; }
        //Var operator != ( const Var& a, const Var& b ) { return (a<b) || (b<a); }
        //Var operator == ( const Var& a, const Var& b ) { return !(a!=b); }
        //Var operator <= ( const Var& a, const Var& b ) { return !(b<a); }
        //Var operator >= ( const Var& a, const Var& b ) { return !(a<b); }

        Var operator[](string s) const {
            if (valor->t == T_OBJ) {return Var();}
            if (valor->t != T_UNDEFINED) {throw Var::Erro("Essa variável não é um objeto");}
            return Var();
        }

        Var operator()(Var arg) {
            if (valor->t == T_FUNC) {
                Function* fPtr = static_cast<Function*>(valor.get());
                return fPtr->executar(arg);
            }
            if (valor->t != T_UNDEFINED) {throw Var::Erro("Essa variável não pode ser usada como função");}
            return Var();
        }

        friend ostream& operator<<(ostream& os, const Var& v) {
            if (v.valor) {
                v.valor->print(os);
            } else {
                os << "null";
            }
            return os;
        }


        // auxiliar functions
        static Var createOBJ() {
            Var v;
            v.valor = shared_ptr<Undefined>( new Object() );
            return v;
        }

        shared_ptr<Undefined> valor;
};

Var newObject() {
    Var obj;
    return obj.createOBJ();
}