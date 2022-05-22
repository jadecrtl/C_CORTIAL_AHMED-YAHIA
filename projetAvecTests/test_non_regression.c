#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

void test_non_regression(char *prefixe, bool resultat_attendu);
static FILE *ouvrir_fichier_en_lecture(char *nom_fichier);
void compare_fichier(char *resultat, char *modele, bool resultat_attendu);

int main() {
    printf("****************************************************\n");
    printf("***** T E S T  D E  N O N  R E G R E S S I O N *****\n");
    printf("****************************************************\n");

    test_non_regression("test/testNonRegression/test01_nr", true);
    test_non_regression("test/testNonRegression/test02_nr", true);
    test_non_regression("test/testNonRegression/test03_nr", true);
    test_non_regression("test/testNonRegression/test04_nr", true);


    printf("*******************************************\n");
    printf("***** N O N  R E G R E S S I O N  O K *****\n");
    printf("*******************************************\n");

    return 0;
}

static FILE *ouvrir_fichier_en_lecture(char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier != NULL) {
        printf("%s ouvert en lecture\n", nom_fichier);
    }
    else {
        printf("impossible d'ouvrir <%s> en lecture\n", nom_fichier);
        exit(1);
    }
    return fichier;
}

void test_non_regression(char *prefixe, bool resultat_attendu) {
    char *input = malloc(sizeof(*input)*5000);
    assert(input != NULL);
    sprintf(input, "%s_input.txt", prefixe);
    //printf(input);
    //printf("\n");

    char *resultat = malloc(sizeof(*resultat)*5000);
    assert(resultat != NULL);
    sprintf(resultat, "%s_resultat.txt", prefixe);
    //printf(resultat);
    //printf("\n");

    char *modele = malloc(sizeof(*modele)*5000);
    assert(modele != NULL);
    sprintf(modele, "%s_modele.txt", prefixe);
    //printf(modele);
    //printf("\n");
    
    char *lancement = malloc(sizeof(*lancement)*5000000);
    assert(lancement != NULL);
    sprintf(lancement, "./calc_unbounded_int -i %s -o %s", input, resultat);
    //printf(lancement);
    printf("\n");

    int res_main;
    res_main = system(lancement);
    assert(res_main == 0);
    compare_fichier(resultat, modele, resultat_attendu);
}

void compare_fichier(char *resultat, char *modele, bool resultat_attendu) {
    bool resultat_compare = true;
    FILE *fichier_resultat = ouvrir_fichier_en_lecture(resultat);
    FILE *fichier_modele = ouvrir_fichier_en_lecture(modele);
    char r, m;
    int position = 0;

    while(true) {
        r = fgetc(fichier_resultat);
        m = fgetc(fichier_modele);
        if(r == '\n' && m == EOF){
            continue;
        }
        if (r == EOF && m == EOF) {
            break;
        }
        if (r != m) {
            //printf("       compare_fichier : changement a %d : %c different de modele (%c)\n", position, r, m);
            resultat_compare = false;
            break;
        }
        position++;
    }
    fclose(fichier_resultat);
    fclose(fichier_modele);
    if(resultat_attendu != resultat_compare) {
        perror("Les fichiers ne sont pas identiques");
        exit(1);
    }
    printf("Les fichiers <%s> et <%s> sont identiques\n", resultat, modele);
}