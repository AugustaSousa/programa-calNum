#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void tabelaOctalHexa(int n) {
  if (n < 10) {
    printf("%d", n);
  }
  if (n == 10) {
    printf("A");
  }
  if (n == 11) {
    printf("B");
  }
  if (n == 12) {
    printf("C");
  }
  if (n == 13) {
    printf("D");
  }
  if (n == 14) {
    printf("E");
  }
  if (n == 15) {
    printf("F");
  }
}

void converterParteInteira(int numero, int base) {
  if (numero >= base) {
    converterParteInteira((numero/base), base);
  }
  int resto = numero % base;
  tabelaOctalHexa(resto);
}

void converterParteDecimal(double decimal, int base) {
  int aux = 0;
  while (decimal > 0.0 && aux < 20) {
    decimal *= base;
    int bit = (int)decimal;
    tabelaOctalHexa(bit);
    decimal -= bit;
    aux++;
  }
}

void converterDecimal(double numero) {
  int inteiro = (int)numero;
  double decimal = numero - inteiro;
  printf("Binário: ");
  converterParteInteira(inteiro, 2);
  if (decimal > 0.0) {
    printf(".");
    converterParteDecimal(decimal, 2);
  }
  printf("\n");

  printf("Octal: ");
  converterParteInteira(inteiro, 8);
  if (decimal > 0.0) {
    printf(".");
    converterParteDecimal(decimal, 8);
  }
  printf("\n");

  printf("Binário: ");
  converterParteInteira(inteiro, 16);
  if (decimal > 0.0) {
    printf(".");
    converterParteDecimal(decimal, 16);
  }
  printf("\n");
}

void opcaoC() {
  double numero;
  printf("Digite o número que deseja converter:\n");
  scanf("%lf", &numero);
  converterDecimal(numero);
}


