#include <iostream>
#include <memory>
#include <string>
#include <map>

using namespace std;

enum Tipo { T_UNDEFINED, T_INT, T_DOUBLE, T_STR, T_OBJ, T_FUNC };

class Undefined {
    public:
        Undefined(Tipo t = T_UNDEFINED) : t(t) {}
        virtual ~Undefined() = default;
        virtual void print(ostream& os) const {os << "undefined";}
    private:
        Tipo t;
};

 
class Var {
    public:
        Var() : valor( new Undefined() ) {}
        Var(int n) : valor( shared_ptr<Undefined>(new Int(n))) {}

        // Classes
        class Int: public Undefined {
            public:
                Int( int n ): Undefined(T_INT), n(n) {}
                virtual void print(ostream& os) const override {os << n;}
            private:
                int n;
        };

        class Double: public Undefined {
            public:
                Double( double d ): Undefined(T_DOUBLE), d(d) {}
                virtual void print(ostream& os) const override {os << d;}
            private:
                double d;
        };
        
        class String: public Undefined {
            public:
                String( string s ): Undefined(T_STR), s(s) {}
                virtual void print(ostream& os) const override {os << s;}
            private:
                string s;
        };

        class Function : public Undefined {
            public:                
                Function(function<Var(Var)> f): Undefined(T_FUNC), func(f) {}
                
                virtual void print(ostream& os) const override { os << "[Function]"; }
                
                Var executar(Var arg) { return func(arg); }

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

        //Var operator > ( const Var& a, const Var& b ) { return b<a; }
        //Var operator != ( const Var& a, const Var& b ) { return (a<b) || (b<a); }
        //Var operator == ( const Var& a, const Var& b ) { return !(a!=b); }
        //Var operator <= ( const Var& a, const Var& b ) { return !(b<a); }
        //Var operator >= ( const Var& a, const Var& b ) { return !(a<b); }

        Var operator[](string s) const {
            return Var();
        }

        Var operator()(Var arg) {
            Function* fPtr = static_cast<Function*>(valor.get());
            return fPtr->executar(arg);
        }

        friend ostream& operator<<(ostream& os, const Var& v) {
            if (v.valor) {
                v.valor->print(os);
            } else {
                os << "null";
            }
            return os;
        }


        class Erro {
            public:
                Erro( string msg ): msg(msg) {}
                
                string operator()() const { return msg; }
            private:
                string msg;
        };
    private:
        shared_ptr<Undefined> valor;
};


