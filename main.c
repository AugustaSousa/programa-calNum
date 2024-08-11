#include <stdio.h>
#include "opcaoS.c"
#include "opcaoC.c"

int main(){
    char ch=' ';
    while(ch!='F'){
        printf("Selecione uma das opcoes abaixo:\n \
'C' - Conversao\n 'S' - Sistema Linear\n \
'E' - Equacao Algebrica\n 'F' - Finalizar\n \
Opcao: ");
        scanf(" %c", &ch);
        switch (ch){
            case 'C':
                opcaoC();
                break;
            
            case 'S':
                opcaoS();
                break;
            
            case 'E':
                printf("Escolheu E\n");
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