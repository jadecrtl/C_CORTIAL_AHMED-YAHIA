#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

void test_non_regression(char *prefixe, bool resultat_attendu);

int main() {
    printf("****************************************************\n");
    printf("***** T E S T  D E  N O N  R E G R E S S I O N *****\n");
    printf("****************************************************\n");

    test_non_regression("test/testNonRegression/test01_nr", true);




    printf("*******************************************\n");
    printf("***** N O N  R E G R E S S I O N  O K *****\n");
    printf("*******************************************\n");

    return 0;
}

void test_non_regression(char *prefixe, bool resultat_attendu) {
    char *input = malloc(sizeof(*input)*50);
    assert(input != NULL);
    sprintf(input, "%s_input.txt", prefixe);
    //printf(input);
    //printf("\n");

    char *resultat = malloc(sizeof(*resultat)*50);
    assert(resultat != NULL);
    sprintf(resultat, "%s_resultat.txt", prefixe);
    //printf(resultat);
    //printf("\n");

    char *modele = malloc(sizeof(*modele)*50);
    assert(modele != NULL);
    sprintf(modele, "%s_modele.txt", prefixe);
    //printf(modele);
    //printf("\n");
    
    char *lancement = malloc(sizeof(*lancement)*50);
    assert(lancement != NULL);
    sprintf(lancement, "./calc_unbounded_int -i %s -o %s", input, resultat);
    //printf(lancement);
    //printf("\n");

    int res_main;
    res_main = system(lancement);
    assert(res_main == 0);
}