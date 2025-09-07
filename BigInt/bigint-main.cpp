#include <iostream>
#include "bigint.h"

using namespace std;

int main()
{
  BigInt I1, I2, Result;
  char opcao=' ';

  cout << "CALCULADORA DE INTEIROS SEM LIMITE\n";
  do
  {
    // Solicita a opcao
    do
    {
      cout << endl;

      cout << "I1:    " << I1 << endl;
      cout << "I2:    " << I2 << endl;
      if (opcao=='+' || opcao=='-' || opcao=='*' ||
          opcao=='/' || opcao=='%' || opcao=='!')
      {
        cout << "     " << opcao << " =====\n";
      }
      cout << "Result: " << Result << endl;

      cout << "Digite sua opcao:\n";
      cout << "< - Entrar um novo numero\n";
      cout << "+ - Somar os numeros\n";
      cout << "- - Subtrair os numeros\n";
      cout << "* - Multiplicar os numeros\n";
      cout << "/ - Dividir os numeros (retornar quociente)\n";
      cout << "% - Dividir os numeros (retornar resto)\n";
      cout << "! - Calcular o fatorial do ultimo numero (Result)\n";
      cout << "i - Inverter o sinal do ultimo numero (Result)\n";
      cout << "t - Trocar os numeros (I1->I2 I2->Result Result->I1)\n";
      cout << "q - Terminar\n";
      cin >> opcao;
      opcao = tolower(opcao);
    }
    while (opcao!='<' && opcao!='+' && opcao!='-' &&
           opcao!='*' && opcao!='/' && opcao!='%' &&
           opcao!='!' && opcao!='i' && opcao!='t' && opcao!='q');

    // Rotaciona os numeros no buffer
    if (opcao!='i' && opcao!='q') // Nao rotaciona nada se for i ou q
    {
      if (opcao=='t') // Rotaciona no sentido inverso
      {
        BigInt prov(move(Result));
        Result = move(I2);
        I2 = move(I1);
        I1 = move(prov);
      }
      else // Todas as opcoes menos i,t,q: rotaciona normalmente
      {
        I1 = move(I2);
        I2 = move(Result);
        // Novo resultado entra em result
      }
    }

    // Executa a opcao escolhida
    switch(opcao)
    {
    case '<':
      cout << "Valor: ";
      cin >> Result;
      break;
    case '+':
      Result = I1+I2;
      break;
    case '-':
      Result = I1-I2;
      break;
    case '*':
      Result = I1*I2;
      break;
    case '/':
      Result = I1/I2;
      break;
    case '%':
      Result = I1%I2;
      break;
    case '!':
      Result = !I2;
      break;
    case 'i':
      Result = -Result;
      break;
    case 't':
    case 'q':
      break;
    default:
      cerr << "Opcao desconhecida\n";
      Result=BigInt();
      break;
    }
  }
  while (opcao != 'q');

  return 0;
}
