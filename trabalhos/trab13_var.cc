#include <iostream>
#include <memory>
#include <string>
#include <map>

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
                    if (outro->t == T_BOOL) return Var(this->n < static_cast<const Bool*>(outro)->b);
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
                    if (outro->t == T_BOOL) return Var(this->d < static_cast<const Bool*>(outro)->b);
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
                    return Var();
                }
        };

        class Char: public Undefined {
            public:
                char c;
                Char( char c ): Undefined(T_CHAR), c(c) {}
                virtual void print(ostream& os) const override {os << c;}

                virtual Var Somar(const Undefined* outro) const override {
                    if (outro->t == T_CHAR) return Var(this->c + static_cast<const Char*>(outro)->c);
                    if (outro->t == T_STR) return Var(this->c + static_cast<const String*>(outro)->s);
                    if (outro->t == T_INT) return Var((int)this->c + static_cast<const Int*>(outro)->n);
                    return Var();
                }
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

        class Bool: public Undefined {
            public:
                bool b;
                Bool(bool b) : Undefined(T_BOOL), b(b) {}
                virtual void print(ostream& os) const override {os << (b ? "true" : "false");}

                virtual Var Somar(const Undefined*) const override {return Var();}

                virtual Var Subtrai(const Undefined*) const override {return Var();}

                virtual Var Multiplica(const Undefined*) const override {return Var();}

                virtual Var Divide(const Undefined*) const override {return Var();}
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

        Var& operator=(const char* s) {
            valor = shared_ptr<Undefined>(new String(s));
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

        Var operator[](string s) const {
            if (valor->t == T_OBJ) {return Var();}
            if (valor->t != T_UNDEFINED) {throw Erro("Essa variável não é um objeto");}
            return Var();
        }

        Var operator()(Var arg) {
            if (valor->t == T_FUNC) {
                Function* fPtr = static_cast<Function*>(valor.get());
                return fPtr->executar(arg);
            }
            if (valor->t != T_UNDEFINED) {throw Erro("Essa variável não pode ser usada como função");}
            return Var();
        }

        bool turn_bool() const {
            if (valor->t == T_UNDEFINED) return false;
            if (valor->t == T_INT) return static_cast<Int*>(valor.get())->n != 0;
            if (valor->t == T_DOUBLE) return static_cast<Double*>(valor.get())->d != 0.0;

            return true;
        }

        friend ostream& operator<<(ostream& os, const Var& v) {
            if (v.valor) {
                v.valor->print(os);
            } else {
                os << "null";
            }
            return os;
        }

        friend Var operator < (const Var& a, const Var& b) {
            return a.valor->Menor(b.valor.get());
        }

        //friend Var operator < ( const Var& a, const Var& b) {
        //    if (a.turn_bool() < b.turn_bool()) return Var(true);
        //    return Var(false);
        //}

        friend Var operator || ( const Var& a, const Var& b) {
            if (a.turn_bool()) return Var(true);
            if (b.turn_bool()) return Var(true);
            return Var(false);
        }
        
        friend Var operator && ( const Var& a, const Var& b) {
            //cout << a.valor->t << " , " << b.valor->t << endl;
            if (a.valor->t == T_BOOL and b.valor->t == T_BOOL) {
                if (!a.turn_bool()) return Var(false);
                if (!b.turn_bool()) return Var(false);
                return Var(true);
            }
            return Var();
        }
/*
        friend Var operator && ( const Var& a, const bool b) {
            if (!a.turn_bool()) return Var(false);
            if (!b) return Var(false);
            return Var(true);
        }

        friend Var operator && ( const bool a, const Var& b) {
            if (!a) return Var(false);
            if (!b.turn_bool()) return Var(false);
            return Var(true);
        }
*/
        friend Var operator ! ( const Var& a) {
            if (a.turn_bool()) return Var(false);
            return Var(true);
        }

        friend Var operator > (const Var& a, const Var& b) {
            if (a.valor->t == T_BOOL and b.valor->t == T_BOOL) return !(a<b);
            return Var();
        }
        friend Var operator != ( const Var& a, const Var& b ) { return (a<b) || (b<a); }
        friend Var operator == ( const Var& a, const Var& b ) { return !(a!=b); }
        friend Var operator <= ( const Var& a, const Var& b ) { return !(b<a); }
        friend Var operator >= ( const Var& a, const Var& b ) { 
            if (a.valor->t == T_BOOL and b.valor->t == T_BOOL) return !(a<b);
            return Var();
        }


        // auxiliar functions
        static Var createOBJ() {
            Var v;
            v.valor = shared_ptr<Undefined>( new Object() );
            return v;
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

