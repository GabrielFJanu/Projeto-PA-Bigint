#ifndef _BIGINT_H_
#define _BIGINT_H_

#include <cstdint>
#include <string>
#include <iostream>

class BigInt {
private:
    bool neg;
    int nDig;
    std::int8_t* d;
    //correct
    void correct();
    //increment
    void increment();
    //decrement
    void decrement();
    //metodo division
    void division(const BigInt& D, BigInt& Q, BigInt& R) const;
public:
    //construtor default
    BigInt();
    //destrutor
    ~BigInt();
    //construtor por copia
    BigInt(const BigInt& BI);
    //construtor por movimento
    BigInt(BigInt&& BI) noexcept;
    //operador de atribuicao por copia
    BigInt& operator=(const BigInt& BI);
    //atribuicao por movimento
    BigInt& operator=(BigInt&& BI) noexcept;
    //construtor especifico/conversor de long long int para BigInt
    BigInt(long long int LLI);
    //construtor especifico que cria um BigInt a partir de uma string
    explicit BigInt(std::string S);
    //metodo isNeg()
    inline bool isNeg() const{return neg;}
    //metodo size()
    inline int size() const{return nDig;}
    //operator[]
    int operator[](int i) const;
    //metodo toInt()
    long long int toInt() const;
    //funcoes classicas com paramentros stream e BigInt
    friend std::ostream& operator<<(std::ostream& X, const BigInt& BI);
    friend std::istream& operator>>(std::istream& X, BigInt& BI);
    //incremento pre-fixado
    BigInt& operator++();
    //decremento pre-fixado
    BigInt& operator--();
    //incremento pos-fixado
    BigInt operator++(int);
    //decremento pos-fixado
    BigInt operator--(int);
    //funcao classica abs()
    friend BigInt abs(BigInt BI);
    //isZero
    inline bool isZero() const{return (nDig==1 && d[0]==0);}
    //deslocamento a esquerda
    BigInt operator<<(int N) const;
    //deslocamento a direita
    BigInt operator>>(int N) const;
    //operator==
    bool operator==(const BigInt& BI) const;
    //operator!=
    inline bool operator!=(const BigInt& BI) const{return !(*this==BI);}
    //operator<
    bool operator<(const BigInt& BI) const;
    //operator<=
    inline bool operator<=(const BigInt& BI) const{return (*this<BI || *this==BI);}
    //operator>
    inline bool operator>(const BigInt& BI) const{return BI<*this;}
    //operator>=
    inline bool operator>=(const BigInt& BI) const{return !(*this<BI);}
    //operator- unario
    BigInt operator-() const;
    //operator+ binario
    BigInt operator+(const BigInt& BI) const;
    //operator- binario
    inline BigInt operator-(const BigInt& BI) const{return (*this)+(-BI);}
    //operator* binario
    BigInt operator*(const BigInt& BI) const;
    //operator/ binario
    BigInt operator/(const BigInt& BI) const;
    //operator% binario
    BigInt operator%(const BigInt& BI) const;
    //operator! unario
    BigInt operator!() const;
};

#endif //#ifndef _BIGINT_H_
