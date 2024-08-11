#include <stdio.h>
#include <math.h>

int criterioLinhas();
int criterioColunas();
void metodoGaussSeidel();

int main() {

    FILE *f;
    char str[200];
    printf("Informe o nome do arquivo que contem o SL: \n");
    scanf("%s", str);

    f = fopen(str, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int n = getc(f)-'0';
    int m_coef[n][n];
    int v_ind[n];

    for(int i=0;i<n;i++){
        for(int j=0; j<n;j++){
            fscanf(f,"%i", &m_coef[i][j]);
        }
        fscanf(f,"%i", &v_ind[i]);
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
    
    return 0;
}
void metodoGaussSeidel(int n, int m_coef[n][n], int v_ind[n]){
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
            printf("%.10f\n", fabs(val-sol[i]));
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
    }
}

int criterioLinhas(int n, int m[n][n]){
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
        sum=0;
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

int criterioColunas(int n, int m[n][n]){
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
        sum=0;
        for(int i=0; i<n; i++){
            if(i!=j){
                sum += fabs(m[i][j]);
            }
        } 
        if(fabs(m[j][j])<=sum){
            return 0;
        }  
    }
    return 1;
}