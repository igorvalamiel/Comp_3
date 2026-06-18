#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <functional>
#include <vector>

using namespace std;

enum Tipo { T_UNDEFINED, T_INT, T_DOUBLE, T_STR, T_CHAR, T_OBJ, T_FUNC, T_BOOL };

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
        };

        class Erro {
            public:
                Erro( string msg ): msg(msg) {}
                
                string operator()() const { return msg; }
            private:
                string msg;
        };

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

        // lendo
        Var operator[](string s) const {
            if (valor->t == T_OBJ) {
                const Object* objPtr = static_cast<const Object*>(valor.get());
                
                auto it = objPtr->atributos.find(s);
                if (it != objPtr->atributos.end()) { return it->second; }
                return Var();
            }
            throw Erro("Essa variável não é um objeto");
        }

        // escrevendo
        Var& operator[](string s) {
            if (valor->t == T_OBJ) {
                Object* objPtr = static_cast<Object*>(valor.get());
                return objPtr->atributos[s];
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
            if (a.valor->t == T_BOOL) {
                return Var(!static_cast<const Bool*>(a.valor.get())->b);
            }
            return Var(); 
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

        // só pra me ajudar no debug (acabou q vou usar kakakakka)
        string type() const {
            vector <string> types =  {"undefined", "int", "double", "string", "char", "object", "function", "bool"};
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

