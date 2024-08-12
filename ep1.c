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