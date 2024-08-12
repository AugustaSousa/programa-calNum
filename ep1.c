#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void opcaoC();
void tabelaOctalHexa();
void converterParteInteira();
void converterParteDecimal();
void converterDecimal();

void opcaoS();
int criterioLinhas();
int criterioColunas();
void metodoGaussSeidel();

void opcaoE();
double *criarVetor(int n);
double lagrange(int grau, double *coef);
double achaX(int grau, double *coef);
double achaY(int grau, double *coef);
void inverteSinal(int n, double *v);
void derivarPolinomio(int grau, double *coef, double *derivada);
void newton(int grau, double x0, double *coeficiente, double *derivada);

int main(){
    char ch=' ';
    while(ch!='F'){
        printf("\nSelecione uma das opcoes abaixo:\n 'C' - Conversao\n 'S' - Sistema Linear\n \
'E' - Equacao Algebrica\n 'F' - Finalizar\n Opcao: ");
        scanf(" %c", &ch);
        switch (ch){
            case 'C':
                opcaoC();
                break;
            
            case 'S':
                opcaoS();
                break;
            
            case 'E':
                opcaoE();
                break;
            
            case 'F':
                printf("Finalizado\n");
                break;

            default:
                printf("Opcao invalida\n");
                break;
        }
    }
    return 0;
}

void tabelaOctalHexa(int n) {
  /* Imprime os valores de acordo com a tabela de digitos das bases binário, octal e hexadecimal*/
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
  /* Realiza as divisões da parte inteira do valor d pela base b e chama a tabela para imprimir os valores dos restos*/
  if (numero >= base) {
    converterParteInteira((numero/base), base);
  }
  int resto = numero % base;
  tabelaOctalHexa(resto);
}

void converterParteDecimal(double decimal, int base) {
    /* Realiza as multiplicações da parte à direita da vírgula até ela atingir 0.0 ou o contador aux chegar em vinte*/
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
    /* Chama a função de converter as partes inteiras e decimais de acordo com as bases*/
  int inteiro = (int)numero;
  double decimal = numero - inteiro;
  printf("Binario: ");
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

  printf("Hexadecimal: ");
  converterParteInteira(inteiro, 16);
  if (decimal > 0.0) {
    printf(".");
    converterParteDecimal(decimal, 16);
  }
  printf("\n");
}

void opcaoC() {
    /* Recebe o número decimal para ser convertido para as bases octal, binário e Hexadecimal*/
  double numero;
  printf("Digite o numero que deseja converter:\n");
  scanf("%lf", &numero);
  converterDecimal(numero);
}


int criterioLinhas(int n, double m[n][n]){
    /*  
        Essa funcao verifica se, para cada linha, o somatorio 
        dos valores absolutos dos elementos não diagonais da linha
        é menor que o valor absoluto do elemento diagonal da linha 
        Entrada: 'n' o numero de variaveis e 
                uma matriz de coeficientes m[n][n]
        Saída: retorna '1' se a matriz satisfaz o criterio das linhas,
                '0' caso a matriz não satisfaça o criterio
    */
    double sum;
    for(int i=0; i<n; i++){
        sum=0.0;
        for(int j=0; j<n; j++){
            if(i!=j){
                sum += fabs(m[i][j]);
            }
        } 
        if(fabs(m[i][i])<=sum){
            return 0;
        }  
    }
    return 1;
}

int criterioColunas(int n, double m[n][n]){
    /*  
        Essa funcao verifica se, para cada coluna, o somatorio 
        dos valores absolutos dos elementos não diagonais da coluna
        é menor que o valor absoluto do elemento diagonal da coluna 
        Entrada: 'n' o numero de variaveis e 
                uma matriz de coeficientes m[n][n]
        Saída: retorna '1' para a matriz satisfaz o criterio das linhas,
                '0' caso a matriz não satisfaça o criterio
    */
    double sum;
    for(int j=0; j<n; j++){
        sum=0.0;
        for(int i=0; i<n; i++){
            if(i!=j){
                sum += fabs(m[i][j]);
            }
        }
        if(sum-fabs(m[j][j])>0){
            return 0;
        }  
    }
    return 1;
}

void opcaoS() {
    FILE *f;
    char str[200];
    printf("Informe o nome do arquivo que contem o SL: \n");
    scanf("%s", str);

    f = fopen(str, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int n = getc(f)-'0';
    double m_coef[n][n];
    double v_ind[n];

    for(int i=0;i<n;i++){
        for(int j=0; j<n;j++){
            fscanf(f,"%lf", &m_coef[i][j]);
        }
        fscanf(f,"%lf", &v_ind[i]);
    }
    fclose(f);

    int condl = criterioLinhas(n,m_coef);
    int condc = criterioColunas(n,m_coef);
    if(!condl || !condc){
        if(!condl){
            printf("O sistema linear nao satisfaz o criterio das linhas\n");
        }
        if(!condc){
            printf("O sistema linear nao satisfaz o criterio das colunas\n");
        }
    }
    else{
        metodoGaussSeidel(n, m_coef, v_ind);
    }

}

void metodoGaussSeidel(int n, double m_coef[n][n], double v_ind[n]){
    /*  
        Essa funcao calcula uma solucao aproximada a partir do 
        Metodo de Gauss-Seidel e para quando atingi 1000 iteracoes
        ou a variacao no valor das variaveis em umas das iteracoes 
        tiver sido menor que 10^{-8} 

        Entrada: 'n' o numero de variaveis,
                uma matriz de coeficientes m_coef[n][n] e
                um vetor de termos independentes v_ind[n]
        Saída: exibe a solução aproximada encontrada e 
                o número de iteracoes feitas
    */
    
    double sol[n];
    //zerando a solucao aproximada das variaveis
    for(int i=0;i<n;i++){
        sol[i]=0.0;
    }

    int ni=0;
    while(ni<1000){
        ni++; //numero de iteracoes
        int cond=0;
        for(int i=0; i<n; i++){
        double val=0.0;
        double sum=0.0;
            for(int j=0; j<n; j++){
                if(i!=j){
                    sum += (double)m_coef[i][j]*sol[j];
                }
            }
            val = (v_ind[i]-sum)/m_coef[i][i];
            //contar quantas variaveis tiveram
            //uma variação menor que 10^{-8}
            if(fabs(val-sol[i])<0.00000001){
                cond++;
            }
            sol[i]=val;
        }
        if(cond==n){break;}
    }
    printf("Numero de iteracoes: %i\n", ni);
    for(int i=0;i<n;i++){
        printf("%.4f ", sol[i]);
    } printf("\n");
}

void opcaoE() {
  /* Opção E -
  Recebe um polinômio de grau.
  Usando o Teorema de Lagrange, o programa deverá calcular e exibir os
  intervalos onde se encontram as raízes reais negativas e as raízes reais
  positivas. Use o Método de Newton para determinar uma aproximação para a raiz
  no final.
  */
  int grau = 0;
  double *coef;
  double *coefDeriv;
  double x0;

  printf("\nDigite o grau do polinomio desejado: ");
  scanf("%d", &grau);

  coef = criarVetor(grau + 1);
  printf("\nDigite os coeficientes do polinomio(an > 0 && a0 != 0):\n");
  for (int i = 0; i < grau + 1; ++i) {
    double valor;
    scanf("%lf", &valor);
    coef[i] = valor;
  }

  x0 = lagrange(grau, coef);

  coefDeriv = criarVetor(grau + 1);
  derivarPolinomio(grau, coef, coefDeriv);

  newton(grau, x0, coef, coefDeriv);
}

double lagrange(int grau, double *coef) {
  /* Calculando os limites inferiores e superiores das raízes reais positivas e negativas. */

  int i;
  double grau_double = grau, X[4], Y[4], an[4], limi[4];
  double *coefInve, *coefExpoInve, *coefInveExpo;
  // equação com os indices invertidos
  coefInve = criarVetor(grau + 1);
  // equação com os expoentes impares invertidos
  coefExpoInve = criarVetor(grau + 1);
  // equação invertida com os expoentes impares invertidos
  coefInveExpo = criarVetor(grau + 1);

  // obter a equação com os indices invertidos
  for (i = 0; i <= grau; i++) {
    coefInve[i] = coef[grau - i];
  }
  printf("\nCoeficientes invertidos:\n");
  for (i = 0; i <= grau; i++) {
    printf("%10.2lf ", coefInve[i]);
  }

  // obter a equação com os expoentes impares invertidos
  for (i = 0; i <= grau; i++) {
    coefExpoInve[i] = coef[i];
    if (i % 2 != 0 && i != 0) {
      coefExpoInve[i] = -coefExpoInve[i];
    }
  }
  printf("\nSinais dos coeficientes com expoentes impares invertidos:\n");
  for (i = 0; i <= grau; i++) {
    printf("%10.2lf ", coefExpoInve[i]);
  }

  // obtém a equação invertida com os expoentes impares invertidos
  for (i = 0; i <= grau; i++) {
    coefInveExpo[i] = coefExpoInve[grau - i];
  }
  printf(
      "\nCoeficientes invertidos e sinais dos expoentes impares invertidos:\n");
  for (i = 0; i <= grau; i++) {
    printf("%10.2lf ", coefInveExpo[i]);
  }

  // calculando o limite positivo
  //  limite superior
  X[0] = achaX(grau, coef);
  Y[0] = achaY(grau, coef);
  an[0] = coef[0];
  limi[0] = 1.0 + pow(Y[0] / an[0], 1.0 / (grau_double - X[0]));
  // limite inferior
  X[1] = achaX(grau, coefInve);
  Y[1] = achaY(grau, coefInve);
  an[1] = coefInve[0];
  limi[1] = 1.0 / (1 + pow(Y[1] / an[1], 1.0 / (grau_double - X[1])));

  // calculando limite negativo
  //  limite inferior
  X[2] = achaX(grau, coefExpoInve);
  Y[2] = achaY(grau, coefExpoInve);
  an[2] = coefExpoInve[0];
  limi[2] = -1.0 * (1 + pow(Y[2] / an[2], 1.0 / (grau_double - X[2])));
  // limite superior
  X[3] = achaX(grau, coefInveExpo);
  Y[3] = achaY(grau, coefInveExpo);
  an[3] = coefInveExpo[0];
  limi[3] = -1.0 / (1 + pow(Y[3] / an[3], 1.0 / (grau_double - X[3])));

  printf("\nLimite Positivo:\n %3.4lf <= X+ <= %3.4lf \n", limi[1], limi[0]);
  printf("\nLimite Negativo:\n %3.4lf <= X- <= %3.4lf \n", limi[2], limi[3]);

  return limi[0];
}

double achaX(int grau, double *coef) {
  /*achaX - Retorna o maior indice dos coeficientes negativos*/
  int i;
  double aux = 0;
  for (i = 0; i < grau; i++) {
    if (coef[i] < 0) {
      if (aux < grau - i) {
        aux = grau - i;
      }
    }
  }
  return aux;
}

double achaY(int grau, double *coef) {
  /*achaY - Retorna o modulo do menor indice dos coeficientes negativos*/
  int i;
  double aux1 = 0;
  double modulo = -1;
  for (i = 0; i <= grau; i++) {
    if (coef[i] < 0) {
      if (aux1 > coef[i]) {
        aux1 = coef[i];
      }
    }
  }
  return aux1 * modulo;
}

void inverteSinal(int n, double *v) {
  /*caso an < 0, modificar os vetores do coeficiente*/
  int j;
  for (j = n; j >= 0; j--) {
    v[j] = -v[j];
  }
}

void derivarPolinomio(int grau, double *coef, double *deri) {

  for (int i = 0; i < grau + 1; ++i) {
    deri[i] = coef[i] * (grau - i);
    scanf("%lf", &deri[i]);
  }
}

void newton(int grau, double x0, double *coef, double *deri) {

  int aux = 0;
  double x = x0;
  double *px, *dPx;
  double respx, resdpx;
  double x1, erro = 1;

  px = criarVetor(grau + 1);
  dPx = criarVetor(grau);

  for (int i = 0; i < grau + 1; ++i) {
    px[i] = coef[i];
  }

  for (int i = 0; i < grau; ++i) {
    dPx[i] = deri[i];
  }
  printf("\nOs coeficientes da derivada do polinomio:");
  for (int i = 0; i < grau; ++i) {
    printf("%10.4lf", dPx[i]);
  }

  while (aux < 100 && erro > 0.00000001) {

    respx = 0;
    for (int i = 0; i < grau + 1; ++i) {
      respx += px[i] * pow(x, grau - i);
    }

    resdpx = 0;
    for (int i = 0; i < grau; ++i) {
      resdpx += dPx[i] * pow(x, grau - 1 - i);
    }
    x1 = x - (respx / resdpx);
    erro = fabs(x - x1);
    x = x1;
    aux++;
  }

  printf("\nRaiz achada: %10.4lf", x1);
  printf("\nNumero de iteracoes (Metodo de Newton): %d\n", aux);
}

double *criarVetor(int num) {
  /* Cria um vetor de doubles com tamanho l se houver memória suficiente. Caso
   contrário, retorna um ponteiro nulo.*/

  double *vetor;
  vetor = malloc(sizeof(double) * num);
  if (vetor == NULL) {
    printf("erro ao criar o vetor!");
    return NULL;
  }
  return vetor;
}