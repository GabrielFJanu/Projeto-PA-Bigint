#include <cmath>
#include "bigint.h"

using namespace std;

//construtor default
BigInt::BigInt(): neg(false), nDig(1),d(new int8_t[this->size()]){
    d[0] = 0;
}
//destrutor
BigInt::~BigInt(){
    delete[] d;
}
//construtor por copia
BigInt::BigInt(const BigInt& BI): neg(BI.isNeg()), nDig(BI.size()),d(new int8_t[this->size()]){
    for(int i = 0; i < this->size(); ++i){d[i] = BI.d[i];}
}
//construtor por movimento
BigInt::BigInt(BigInt&& BI) noexcept: neg(BI.isNeg()), nDig(BI.size()), d(BI.d){
    BI.d = nullptr;
}
//operador de atribuicao por copia
BigInt& BigInt::operator=(const BigInt& BI){
    if(this != &BI){
        delete[] d;
        neg = BI.isNeg();
        nDig = BI.size();
        d = new int8_t[this->size()];
        for(int i = 0; i <this->size(); ++i){
            d[i] = BI.d[i];
        }
    }
    return *this;
}
//operador atribuicao por movimento
BigInt& BigInt::operator=(BigInt&& BI) noexcept{
    delete[] d;
    neg = BI.isNeg();
    nDig = BI.size();
    d = BI.d;
    BI.d = nullptr;
    return *this;
}
//construtor especifico/conversor de long long int para BigInt
BigInt::BigInt(long long int LLI): neg(false),nDig(0),d(nullptr){
    if(LLI==0){
        //zerando
        *this = BigInt();
        return;
    }
    neg = (LLI<0);
    nDig = 1+int(log10(abs(static_cast<double>(LLI))));
    d = new int8_t[this->size()];
    for(int i = 0; i < this->size(); ++i){
        d[i] = abs(LLI%10);
        LLI/=10;
    }
}
//correct
void BigInt::correct(){
    if(this->size() <= 0 || d == nullptr){
        *this = BigInt();
        return;
    }
    int newNDig(this->size());
    while(newNDig>1 && d[newNDig-1]==0){
        --newNDig;
    }
    if(newNDig!=this->size()){
        int8_t* prov = new int8_t[newNDig];
        for(int i = 0; i < newNDig; ++i){
            prov[i] = d[i];
        }
        delete[] d;
        d = prov;
        nDig = newNDig;
    }
    if(this->isZero()){neg = false;}
}
//construtor especifico que cria um BigInt a partir de uma string
BigInt::BigInt(string S):neg(false),nDig(0),d(nullptr){
    if(S.size()==0){
        //zerando
        *this = BigInt();
        cerr << endl << "ERRO: string invalida, entao BigInt foi inicializado como 0" << endl;
        return;
    }
    int ini(0);
    if(S[0]=='+' or S[0]=='-'){
        if(S.size()==1){
            //zerando
            *this = BigInt();
            cerr << endl << "ERRO: string invalida, entao BigInt foi inicializado como 0" << endl;
            return;
        }
        ini = 1;
        neg = (S[0]=='-');
    }
    nDig = S.size() - ini;
    d = new int8_t[this->size()];
    for(int i = 0; i < this->size(); ++i){
        char C = S[S.size()-1-i];
        if(!(isdigit(C))){
            //zerando
            *this = BigInt();
            cerr << endl << "ERRO: string invalida, entao BigInt foi inicializado como 0" << endl;
            return;
        }
        d[i] = static_cast<int8_t>(C - '0');
    }
    this->correct();
}
//operator[]
int BigInt::operator[](int i) const{
    if(i<0 || i>=this->size()){return 0;}
    return static_cast<int>(d[i]);
}
//metodo toInt()
long long int BigInt::toInt() const{
    long long int C = 0;
    for(int i = this->size()-1; i >= 0; --i){
        C *=10;
        C += d[i];
        if(C<0){
            cerr << endl << "ERRO: o valor do BigInt excedeu os limites de um long long int, entao long long int foi inicializado como 0" << endl;
            C = 0;
            return C;
        }
    }
    if(this->isNeg()){C = -C;}
    return C;
}
//funcoes classicas com paramentros stream e BigInt
ostream& operator<<(ostream& O, const BigInt& BI){
    if(BI.isNeg()){O << '-';}
    for(int i = BI.size()-1; i >=0; --i){
        O << BI[i];
    }
    return O;
}
istream& operator>>(istream& I, BigInt& BI){
    delete[] BI.d;
    BI.d = nullptr;
    BI.nDig = 0;
    BI.neg = false;
    I >> ws;
    char C = I.peek();
    if(C=='+' || C=='-'){
        I.get();
        if(C=='-'){BI.neg = true;}
        C = I.peek();
    }
    while(isdigit(C)){
        C = I.get();
        int8_t* newD = new int8_t[BI.size()+1];
        for(int i = 0; i < BI.size(); ++i){newD[i+1] = BI.d[i];}
        newD[0] = static_cast<int8_t>(C-'0');
        delete[] BI.d;
        BI.d = newD;
        ++BI.nDig;
        C = I.peek();
    }
    BI.correct();
    return I;
}
//increment
void BigInt::increment(){
    int k = 0;
    ++d[k];
    while(k<this->size()-1 && d[k]>9){
        d[k] = 0;
        ++k;
        ++d[k];
    }
    if(k==this->size()-1 && d[k]>9){
        int8_t* newD = new int8_t[this->size()+1];
        for(int i = 0; i<this->size()-1; ++i){newD[i] = d[i];}
        newD[this->size()-1] = 0;
        newD[this->size()] = 1;
        delete[] d;
        d = newD;
        ++nDig;
    }
}
//decrement
void BigInt::decrement(){
    int k = 0;
    --d[k];
    while(k < this->size()-1 && d[k]<0){
        d[k] = 9;
        ++k;
        --d[k];
    }
    if(k==this->size()-1 && d[k]<=0){
        if(this->size()>1){
            int8_t* newD = new int8_t[this->size()-1];
            for(int i = 0; i<this->size()-1; ++i){newD[i]=d[i];}
            delete[] d;
            d = newD;
            --nDig;
        }else{
            if(this->isNeg()){
                neg = false;
            }else if(d[0]<0){
                d[0] = -d[0];
                neg = true;
            }
        }
    }
}
//incremento pre-fixado
BigInt& BigInt::operator++(){
    if(!this->isNeg()){
        this->increment();
    }else{
        this->decrement();
    }
    return *this;
}
//decremento pre-fixado
BigInt& BigInt::operator--(){
    if(this->isNeg()){
        this->increment();
    }else{
        this->decrement();
    }
    return *this;
}
//incremento pos-fixado
BigInt BigInt::operator++(int){
    BigInt copia(*this);
    ++(*this);
    return copia;
}
//decremento pos-fixado
BigInt BigInt::operator--(int){
    BigInt copia(*this);
    --(*this);
    return copia;
}
//funcao classica abs()
BigInt abs(BigInt BI){
    BI.neg = false;
    return BI;
}
//deslocamento a esquerda
BigInt BigInt::operator<<(int N) const{
    if(N<=0 || this->isZero()){return *this;}
    BigInt C;
    C.neg = this->isNeg();
    C.nDig = this->size()+N;
    delete[] C.d;
    C.d = new int8_t[C.size()];
    for(int i = 0; i < C.size(); ++i){
        if(i<N){
            C.d[i]=0;
        }else{
            C.d[i]=d[i-N];
        }
    }
    return C;
}
//deslocamento a direita
BigInt BigInt::operator>>(int N) const{
    if(N<=0 || this->isZero()){return *this;}
    if(N>=this->size()){return BigInt();}
    BigInt C;
    C.neg = this->isNeg();
    C.nDig = this->size()-N;
    delete[] C.d;
    C.d = new int8_t[C.size()];
    for(int i = 0; i<C.size(); ++i){C.d[i]=d[i+N];}
    return C;
}
//operator==
bool BigInt::operator==(const BigInt& BI) const{
    if(BI.isNeg()!=this->isNeg() || BI.size()!=this->size()){return false;}
    for(int i = 0; i<this->size(); ++i){
        if(BI.d[i]!=d[i]){return false;}
    }
    return true;
}
//operator<
bool BigInt::operator<(const BigInt& BI) const{
    if(this->isNeg()!=BI.isNeg()){return this->isNeg();}
    if(this->isNeg()){return abs(BI)<abs(*this);}
    if(this->size() < BI.size()){return true;}
    if(this->size() > BI.size()){return false;}
    for(int i = this->size()-1; i >=0; --i){
        if(d[i] < BI.d[i]){return true;}
        if(d[i] > BI.d[i]){return false;}
    }
    return false;
}
//operator- unario
BigInt BigInt::operator-() const{
    if(this->isZero()){return *this;}
    BigInt copia(*this);
    copia.neg = !this->isNeg();
    return copia;
}
//operator+ binario
BigInt BigInt::operator+(const BigInt& BI) const{
    BigInt C;
    int carry;
    if(!this->isNeg() && !BI.isNeg()){
        C.nDig = 1 + max(this->size(),BI.size());
        delete[] C.d;
        C.d = new int8_t[C.size()];
        carry=0;
        for(int i =0; i<C.size(); ++i){
            C.d[i] = (*this)[i]+BI[i]+carry;
            if(C.d[i]>9){
                C.d[i] -= 10;
                carry = 1;
            }else{
                carry = 0;
            }
        }
        C.correct();
    }else if(this->isNeg() && BI.isNeg()){
        C = -(abs(*this)+abs(BI));
    }else if(!this->isNeg() && BI.isNeg()){
        if(abs(*this)>=abs(BI)){
            C.nDig = this->size();
            delete[] C.d;
            C.d = new int8_t[C.size()];
            carry = 0;
            for(int i = 0; i<C.size(); ++i){
                C.d[i] = (*this)[i]-BI[i]-carry;
                if(C.d[i]<0){
                    C.d[i]+=10;
                    carry = 1;
                }else{
                    carry = 0;
                }
            }
            C.correct();
        }else{
            C = -(abs(BI)+(-(*this)));
        }
    }else{
        if(abs(*this)>=abs(BI)){
            C = -(abs(*this)+(-BI));
        }else{
            C = BI+(*this);
        }
    }
    return C;
}
//operator* binario
BigInt BigInt::operator*(const BigInt& BI) const{
    if(this->isZero() || BI.isZero()){return BigInt();}
    BigInt C;
    C.neg = (this->isNeg()!=BI.isNeg());
    C.nDig = this->size() + BI.size();
    delete[] C.d;
    C.d = new int8_t[C.size()];
    for(int k = 0; k < C.size(); ++k){
        C.d[k] = 0;
    }
    int carry;
    for(int i = 0; i<this->size(); ++i){
        for(int j = 0; j<BI.size(); ++j){
            int k = i+j;
            C.d[k]+= d[i]*BI.d[j];
            while(C.d[k]>9){
                carry = C.d[k]/10;
                C.d[k] = C.d[k]%10;
                ++k;
                C.d[k] += carry;
            }
        }
    }
    C.correct();
    return C;
}
//metodo division
void BigInt::division(const BigInt& D, BigInt& Q, BigInt& R) const{
    if(this->isZero() || D.isZero()){
        if(D.isZero()){
            cerr << endl << "ERRO: divisor nulo, entao quociente e resto foram inicializados como 0" << endl;
        }
        Q=R=BigInt();
        return;
    }
    if(abs(*this)<abs(D)){
        Q=BigInt();
        R=*this;
        return;
    }
    Q = BigInt();
    int nComp = D.size();
    R = (abs(*this)>>(this->size()-nComp));
    if(R < abs(D)){
        ++nComp;
        R = (abs(*this)>>(this->size()-nComp));
    }
    int8_t div =0;
    while(R>=abs(D)){
        R=R-abs(D);
        ++div;
    }
    Q.d[0] = div;
    while(nComp < this->size()){
        ++nComp;
        R= R<<1;
        R.d[0] = d[this->size()-nComp];
        div = 0;
        while(R>=abs(D)){
            R=R-abs(D);
            ++div;
        }
        Q = Q<<1;
        Q.d[0] = div;
    }
    Q.neg = (this->isNeg()!=D.isNeg());
    if(!R.isZero()){R.neg = this->isNeg();}
}
//operator/ binario
BigInt BigInt::operator/(const BigInt& BI) const{
    BigInt Q, R;
    this->division(BI,Q,R);
    return Q;
}
//operator% binario
BigInt BigInt::operator%(const BigInt& BI) const{
    BigInt Q, R;
    this->division(BI,Q,R);
    return R;
}
//operator! unario
BigInt BigInt::operator!() const{
    if(this->isNeg()){
        cerr << endl << "ERRO: numero negativo, entao BigInt foi inicializado como 0" << endl;
        return BigInt();
    }
    if(this->isZero() || *this==BigInt(1)){
        return BigInt(1);
    }
    return (*this)*(!(*this-BigInt(1)));
}
