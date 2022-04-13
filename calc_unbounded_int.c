#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>


#include "unbounded_int.h"

FILE *fichier_source;
FILE *fichier_resultat;

typedef struct variable{
    char *nom;
    struct variable *suivant;
    unbounded_int *valeur;
}variable;

typedef struct {
    variable *premier;
    variable *dernier;
}liste_variable;

void recupere_argument(int argc, char *argv[]);
FILE *ouvrir_fichier_en_lecture(char *nom_fichier);
FILE *ouvrir_fichier_en_ecriture(char *nom_fichier);
variable *creer_variable();
void ajouter_variable(char *nomVar, unbounded_int *unbo);
variable *rechercher_variable(char *nom_variable);
void compare_chaine(char *nomVar);
void stdin_et_stdout_aucun_argument();
//void fichier_to_stdout(fichier_source); à développer plus tard
//void stdin_to_fichier(fichier_resultat); à développer plus tard

void test_afficher_variables();
void test_ajout_variable(char *nom, char *charUnbo, char *nomAttendu, char *charUnboAttendu);
void test_recherche_variable(char *nom_attendu, char *char_unbo_attendu, bool resultat_recherche_attendu);

liste_variable *variables;

int main(int argc, char *argv[]) {
    recupere_argument(argc, argv);
    variables = malloc(sizeof(liste_variable));
    if (variables == NULL) {
        perror("\ncreer_liste_variable : la création de la liste de variable a échouée\n");
        exit(1);
    }
    test_ajout_variable("a", "123", "a", "123");
    test_ajout_variable("b", "123456", "b", "123456");
    test_afficher_variables();
    test_recherche_variable("a", "123", true);
    test_recherche_variable("b", "123456", true);
    test_recherche_variable("a", "1230", false);
    test_recherche_variable("c", "123", false);
    test_recherche_variable("d", "56789", false); 

    test_ajout_variable("abcd", "123456", "abcd", "123456");
    test_afficher_variables();
    test_recherche_variable("abcd", "123456", true);

    //test_ajout_variable("printab", "123", "printab", "123");
    //test_ajout_variable("abprint", "123", "printab", "123");
    //test_ajout_variable("abprintab", "123", "printab", "123");
    //test_ajout_variable("abpoprintab", "123", "printab", "123");
    //test_ajout_variable("pabprintab", "123", "printab", "123");
    //test_ajout_variable("abpoprint", "123", "printab", "123");
    //test_ajout_variable("abopri", "723", "printab", "123");


    /*
    POUR ajout_variable :
        * ne pas ajouter une variable qui CONTIENT "print" ou 0-9 FAIT
        * ne pas ajouter une variable qui EST DÉJÀ dans la liste SAUF si on fait une réassignation 
    */


    //test_ajout_variable("ab12cd", "123456", "ab12cd", "123456");
    //test_ajout_variable("ab2", "12", "ab2", "12");
    //test_ajout_variable("2ab", "12", "2ab", "12");
    test_ajout_variable("a2b", "12", "a2b", "12");

    printf("****************************\n");
    printf("**********TEST OK **********\n");
    printf("****************************\n");

    fclose(fichier_source);
    fclose(fichier_resultat);
    return 0;
}

void stdin_et_stdout_aucun_argument() {
    
}

void recupere_argument(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s \n", i, argv[i]);
    }
    if (argc == 1) {
        stdin_et_stdout_aucun_argument();
        return;
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-i") == 0) {
            //fichier_to_stdout(fichier_source); à développer plus tard
            printf("fichier source = %s\n", argv[2]);
            fichier_source = ouvrir_fichier_en_lecture(argv[2]);
            return;
        }
        if (strcmp(argv[1], "-o") == 0) {
            //stdin_to_fichier(fichier_resultat); à développer plus tard
            printf("fichier resultat = %s\n", argv[2]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[2]);
            return;
        }
    }
    if (argc == 5) {
        if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0) {
            printf("fichier source = %s\n", argv[2]);
            fichier_source = ouvrir_fichier_en_lecture(argv[2]);
            printf("fichier resultat = %s\n", argv[4]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[4]);
            return;
        }
        if (strcmp(argv[1], "-o") == 0 && strcmp(argv[3], "-i") == 0) {
            printf("fichier source = %s\n", argv[4]);
            fichier_source = ouvrir_fichier_en_lecture(argv[4]);
            printf("fichier resultat = %s\n", argv[2]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[2]);
            return;
        }
    }
    printf("***** KO ***** LES ARGUMENTS SONT INCORRECTS, faire ./calc_unbounded_int -i source -o resultat\n");
    exit(1);    
}

FILE *ouvrir_fichier_en_lecture(char *nom_fichier) {
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

FILE *ouvrir_fichier_en_ecriture(char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier != NULL) {
        printf("%s ouvert en écriture\n", nom_fichier);//IMPOSSIBLE SI ON A PAS LES DROITS D'ECRITURE
    }//POSSIBLE DE L'OUVRIR EN ECRITURE MEME SI ON ECRIT ACTUELLEMENT DEDANS
    else {
        printf("impossible d'ouvrir <%s> en écriture\n", nom_fichier);
    }
    return fichier;
}

variable *creer_variable() {
    variable* var = malloc(sizeof(variable));
    if (var == NULL) {
        perror("\ncreer_variable : La création de la variable a échouée\n");
        exit(1);
    }
    return var;
}

void ajouter_variable(char *nomVar, unbounded_int *unbo) {
    char *i = nomVar;
    while (*i != '\0') {
        if (*i == 'p') {
            compare_chaine(i);
        }
        if (isdigit(*i)){
            printf("Le nom de <%s> ne doit pas contenir de chiffre\n", nomVar);
            exit(1);
        }
        i++;
    }
    variable *var = creer_variable();
    var->nom = nomVar;
    var->valeur = unbo;
    if (variables->premier == NULL && variables->dernier == NULL) {
        variables->premier = var;
        variables->dernier = var;
    }
    else {
        variables->dernier->suivant = var;
        variables->dernier = var;
    }
    var->suivant = NULL;
    printf("variable <%s> ajoutée (%s)\n", nomVar, unbounded_int2string(*unbo));
}

void compare_chaine(char *nomVar) {
    char *mot_print = "print";
    for(int i = 0; i < 5; i++) {
        if (mot_print[i] != nomVar[i]) {
            return;
        }
    }
    printf("<%s> : Le mot print ne peut pas être implémenté dans le nom d'une variable\n", nomVar);
    exit(1);
}

variable *rechercher_variable(char *nom_variable) {
    variable *tmp = creer_variable();
    tmp = variables->premier;
    if (strcmp(tmp->nom, nom_variable) == 0) {
        return tmp;
    }
    while(tmp->suivant != NULL) {
        tmp = tmp->suivant;
        if (strcmp(tmp->nom, nom_variable) == 0) {
            return tmp;
        }
    }
    printf("**** ERREUR **** : la variable <%s> n'existe pas.\n", nom_variable);
    return NULL;//FAIRE ATTENTION
}

void test_afficher_variables() {
    variable *tmp = creer_variable();
    tmp = variables->premier;
    int i = 0;
    printf("%d : [%s = %s]\n", i, tmp->nom, unbounded_int2string(*tmp->valeur));
    while(tmp->suivant != NULL) {
        tmp = tmp->suivant;
        i++;
        printf("%d : [%s = %s]\n", i, tmp->nom, unbounded_int2string(*tmp->valeur));
    }
}

void test_ajout_variable(char *nom, char *charUnbo, char *nomAttendu, char *charUnboAttendu) {
    unbounded_int* ubi = malloc(sizeof(unbounded_int));
    if (ubi == NULL) {
        perror("\ntest_string2unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *ubi = string2unbounded_int(charUnbo);
    ajouter_variable(nom, ubi);
    printf("OK test_ajout_variable : %s = %s\n", nom, charUnbo);
}

void test_recherche_variable(char *nom_attendu, char *char_unbo_attendu, bool resultat_recherche_attendu) {
    variable *varRecherchee = rechercher_variable(nom_attendu);
    bool resultat_recherche = true;
    if (varRecherchee == NULL) {
        //printf("**** KO **** test_recherche_variable non trouvée : %s = %s\n", nom_attendu, char_unbo_attendu);
        resultat_recherche = false;        
    }
    else {
    //variable trouvée mais pas la bonne valeur pour l'unbounded_int
        if (strcmp(char_unbo_attendu, unbounded_int2string(*varRecherchee->valeur)) != 0) {
            //printf("**** KO **** test_ajout_variable : la valeur de cette var <%s> ne correspond pas à (%s)\n",nom_attendu, char_unbo_attendu);
            //exit(1);
            resultat_recherche = false;
        }
    }
    if (resultat_recherche == resultat_recherche_attendu) {
        //printf("OK test_recherche_variable : %s = %s\n", nom_attendu, char_unbo_attendu);
    }
    else {
        //printf("**** KO **** test_recherche_variable : %s = %s\n", nom_attendu, char_unbo_attendu);
        exit(1);
    }
}

