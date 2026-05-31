#include <iostream>
#include <memory>
#include <string>
#include <map>

using namespace std;

class Undefined {
    public:
        virtual ~Undefined() = default;
        virtual void print(ostream& os) const {os << "undefined";}
};

 
class Var {
    public:
        Var() : valor( new Undefined() ) {}
        Var(int n) : valor( shared_ptr<Undefined>(new Int(n))) {}

        // Classes
        class Int: public Undefined {
            public:
                Int( int n ): n(n) {}
                virtual void print(ostream& os) const override {os << n;}
            private:
                int n;
        };

        class Double: public Undefined {
            public:
                Double( double d ): d(d) {}
                virtual void print(ostream& os) const override {os << d;}
            private:
                double d;
        };
        
        class String: public Undefined {
            public:
                String( string s ): s(s) {}
                virtual void print(ostream& os) const override {os << s;}
            private:
                string s;
        };

        /*
        class Function: public Undefined {
            public:
                Function() {}
            private:
                
        };
        */


        class Object: public Undefined {
            public:
                map<string, Var> atributos;
                Object() {}
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

        Var operator[] (string s) {
            valor = shared_ptr<Undefined>( new Object());
            return *this;
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


