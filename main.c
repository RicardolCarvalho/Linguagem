/* main.c */
#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return EXIT_FAILURE;
        }
    } else {
        yyin = stdin;
    }

    if (yyparse() == 0) {
        printf("Programa analisado com sucesso!\n");
        return EXIT_SUCCESS;
    } else {
        printf("Erro de compilação!\n");
        return EXIT_FAILURE;
    }
}
