#include <stdio.h>

int yyparse(void);

int main() {
    if (yyparse() == 0) {
        printf("Programa analisado com sucesso!\n");
    } else {
        printf("Erro de compilação!\n");
    }
    return 0;
}
