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
    unbounded_int valeur;
}variable;

typedef struct {
    variable *premier;
    variable *dernier;
}liste_variable;

void recupere_argument(int argc, char *argv[]);
static FILE *ouvrir_fichier_en_lecture(char *nom_fichier);
static FILE *ouvrir_fichier_en_ecriture(char *nom_fichier);
static variable *creer_variable();
void ajouter_variable(char *nomVar, unbounded_int unbo);
variable *rechercher_variable(char *nom_variable);
static int compare_chaine(char *nomVar);
void interprete_fichier();
static void interprete_ligne(char *char_ligne);
static void print_variable(char *char_ligne);
static void variable_assignation_ou_operations(char *char_ligne, char *varG);
static void variable_operation(int operation, char *varG, char *varD, char *char_ligne);

//void fichier_to_stdout(fichier_source); à développer plus tard
//void stdin_to_fichier(fichier_resultat); à développer plus tard


static void test_afficher_variables();
static void test_ajout_variable(char *nom, char *charUnbo, char *nomAttendu, char *charUnboAttendu);
static void test_recherche_variable(char *nom_attendu, char *char_unbo_attendu, bool resultat_recherche_attendu);

liste_variable *variables;

int main(int argc, char *argv[]) {
    variables = malloc(sizeof(liste_variable));
    if (variables == NULL) {
        perror("\ncreer_liste_variable : la création de la liste de variable a échouée\n");
        exit(1);
    }
    recupere_argument(argc, argv);
    /*test_ajout_variable("a", "123", "a", "123");
    test_ajout_variable("b", "123456", "b", "123456");
    test_afficher_variables();
    test_recherche_variable("a", "123", true);
    test_recherche_variable("b", "123456", true);
    test_recherche_variable("a", "1230", false);
    test_recherche_variable("c", "123", false);
    test_recherche_variable("d", "56789", false); 

    test_ajout_variable("abcd", "123456", "abcd", "123456");
    test_afficher_variables();
    test_recherche_variable("abcd", "123456", true);*/

    //test_ajout_variable("printab", "123", "printab", "123");
    //test_ajout_variable("abprint", "123", "printab", "123");
    //test_ajout_variable("abprintab", "123", "printab", "123");
    //test_ajout_variable("abpoprintab", "123", "abpoprintab", "123");
    //test_ajout_variable("pabprintab", "123", "printab", "123");
    //test_ajout_variable("abpoprint", "123", "printab", "123");
    //test_ajout_variable("abopri", "723", "printab", "123");


    /*
    POUR ajout_variable :
        * ne pas ajouter une variable qui CONTIENT "print" ou 0-9 FAIT
        * ne pas ajouter une variable qui EST DÉJÀ dans la liste SAUF si on fait une réassignation 
    */


    /*test_ajout_variable("ab12cd", "123456", "ab12cd", "123456");
    test_ajout_variable("ab2", "12", "ab2", "12");
    test_ajout_variable("2ab", "12", "2ab", "12");
    test_ajout_variable("a2b", "12", "a2b", "12");*/
    
    test_afficher_variables();    

    printf("****************************\n");
    printf("**********TEST OK **********\n");
    printf("****************************\n");

    fclose(fichier_source);//petit soucis "double free or corruption (!prev)"
    fclose(fichier_resultat);
    return 0;
}

void recupere_argument(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s \n", i, argv[i]);
    }
    if (argc == 1) {
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
            interprete_fichier();
            printf("fichier resultat = %s\n", argv[4]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[4]);
            return;
        }
        if (strcmp(argv[1], "-o") == 0 && strcmp(argv[3], "-i") == 0) {
            printf("fichier source = %s\n", argv[4]);
            fichier_source = ouvrir_fichier_en_lecture(argv[4]);
            interprete_fichier();
            printf("fichier resultat = %s\n", argv[2]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[2]);
            return;
        }
    }
    printf("***** KO ***** LES ARGUMENTS SONT INCORRECTS, faire ./calc_unbounded_int -i source -o resultat\n");
    exit(1);    
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

static FILE *ouvrir_fichier_en_ecriture(char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier != NULL) {
        printf("%s ouvert en écriture\n", nom_fichier);//IMPOSSIBLE SI ON A PAS LES DROITS D'ECRITURE
    }//POSSIBLE DE L'OUVRIR EN ECRITURE MEME SI ON ECRIT ACTUELLEMENT DEDANS
    else {
        printf("impossible d'ouvrir <%s> en écriture\n", nom_fichier);
    }
    return fichier;
}

static variable *creer_variable() {
    variable* var = malloc(sizeof(variable));
    if (var == NULL) {
        perror("\ncreer_variable : La création de la variable a échouée\n");
        exit(1);
    }
    return var;
}

void ajouter_variable(char *nomVar, unbounded_int unbo) {
    char *i = nomVar;
    if(compare_chaine(i) == 0){
        printf("Le nom de <%s> ne doit pas contenir de print\n", nomVar);
        exit(1);
    }
    while (*i != '\0') {
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
    printf("variable <%s> ajoutée (%s)\n", nomVar, unbounded_int2string(unbo));
}

static int compare_chaine(char *nomVar) {
    char *mot_print = "print";
    int cpt = 0;
    while(*nomVar != '\0'){
        if(strlen(nomVar) < 5){
            return 1;
        }
        if(*nomVar == 'p'){
            for(int i = 0; i < 5; i++) {
                if (mot_print[i] == nomVar[i]) {
                    cpt++;
                }
            }
            if(cpt == 5){
                return 0;
            }
            cpt = 0;
        }
        nomVar++;
    }
    return 1;
}

variable *rechercher_variable(char *nom_variable) {
    variable *tmp = variables->premier;
    if(tmp == NULL){
        return NULL;
    }
    if (strcmp(tmp->nom, nom_variable) == 0) {
        return tmp;
    }
    while(tmp != NULL) {
        if (strcmp(tmp->nom, nom_variable) == 0) {
            return tmp;
        }
        tmp = tmp->suivant;
    }
    printf("**** ERREUR **** : la variable <%s> n'existe pas.\n", nom_variable);
    return NULL;//FAIRE ATTENTION
}

static void test_afficher_variables() {
    variable *tmp = creer_variable();
    tmp = variables->premier;
    int i = 0;
    printf("%d : [%s = %s]\n", i, tmp->nom, unbounded_int2string(tmp->valeur));
    while(tmp->suivant != NULL) {
        tmp = tmp->suivant;
        i++;
        printf("%d : [%s = %s]\n", i, tmp->nom, unbounded_int2string(tmp->valeur));
    }
}

static void test_ajout_variable(char *nom, char *charUnbo, char *nomAttendu, char *charUnboAttendu) {
    unbounded_int ubi = string2unbounded_int(charUnbo);
    ajouter_variable(nom, ubi);
    printf("OK test_ajout_variable : %s = %s\n", nom, charUnbo);
}

static void test_recherche_variable(char *nom_attendu, char *char_unbo_attendu, bool resultat_recherche_attendu) {
    variable *varRecherchee = rechercher_variable(nom_attendu);
    bool resultat_recherche = true;
    if (varRecherchee == NULL) {
        //printf("**** KO **** test_recherche_variable non trouvée : %s = %s\n", nom_attendu, char_unbo_attendu);
        resultat_recherche = false;        
    }
    else {
    //variable trouvée mais pas la bonne valeur pour l'unbounded_int
        if (strcmp(char_unbo_attendu, unbounded_int2string(varRecherchee->valeur)) != 0) {
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

void interprete_fichier(){
    char *stock_ligne = malloc(sizeof(char));
    while(fgets(stock_ligne, 1024, fichier_source) != NULL){
        interprete_ligne(stock_ligne);
    }
    if(ferror(fichier_source)){
        perror("Erreur fgets !");
        exit(1);
    }
    free(stock_ligne);
}

static void interprete_ligne(char *char_ligne){
    /*ALGO
    Tant que je suis pas arrivé à la fin de la ligne
        SI j'ai des espaces 
            j'avance dans la ligne
            je continue
        Dès que je rencontre un caractère j'incrémente varG
        SI je rencontre un espace ou un caractère =
            SI varG == print
                fonction auxiliaire qui gère le print
            SINON c'est une variable
                fonction qui gère les variables
    */
    char *varG = malloc(sizeof(char));//Partie de la ligne qui contient soit print soit une variable
    if(varG == NULL){
        perror("malloc error !\n");
        exit(1);
    }
    char *tmp = varG;
    while (*char_ligne != '\0' && *char_ligne != '=') {
        if (*char_ligne == ' ') {
            if (strcmp(tmp, "print") == 0) {
                print_variable(char_ligne);
                return;
            }
            char_ligne++;
            continue;
        }
        // ajout du caractère actuel de la ligne dans varG
        *varG = *char_ligne;
        varG++;
        char_ligne++;
    }
    *varG = '\0';
    varG = tmp;//Permet de pointer sur le premier caractère de varG pour l'avoir en entier    
    if (strlen(char_ligne) == 0 && strlen(varG) != 0) {
        printf("On ne peut pas écrire une variable seule!!!\n");
        return;
    }
    if (strlen(char_ligne) == 0 && strlen(varG) == 0) {
        printf("ligne vide\n");
    }
    char_ligne++;
    variable_assignation_ou_operations(char_ligne, varG);
    return;
}

static void print_variable(char *char_ligne) {
    char *varD = malloc(sizeof(char));//Partie de la ligne qui contient la variable
    if(varD == NULL){
        perror("malloc error !\n");
        exit(1);
    }
    char *tmp = varD;
    while (*char_ligne != '\0' && *char_ligne != '\n') {
        if (*char_ligne == ' ') {
            char_ligne++;
            continue;
        }
        if (*char_ligne == '=' || *char_ligne == '*' || *char_ligne == '+' || *char_ligne == '-') {
            printf("ERREUR pas d'assignation ou d'opérations avec print!!!!\n");
            return;
        }
        *varD = *char_ligne;
        varD++;
        char_ligne++;
    }
    *varD = '\0';
    varD = tmp;
    if (compare_chaine(tmp) == 0) {
        printf("La variable <%s> n'a pas le droit de contenir print\n", varD);
        return;
    }
    if (strlen(varD) == 0) {
        printf("Il n'y a pas de variable!\n");
        return;
    }
    if (rechercher_variable(varD) == NULL) {
        printf("%s = 0\n", varD);
    }
    else {
        variable *res = rechercher_variable(varD);
        char *unbo = unbounded_int2string(res->valeur);
        printf("%s = %s\n", res->nom, unbo);
    }
}

static void variable_assignation_ou_operations(char *char_ligne, char *varG) {
    char *varD = malloc(sizeof(char));//Partie de la ligne qui contient la variable
    if(varD == NULL){
        perror("malloc error !\n");
        exit(1);
    }
    char *pointeur = varD;
    int i = 0;
    int cpt_espace = 0;
    while (*char_ligne != '\0' && *char_ligne != '\n') {
        if (*char_ligne == ' ') {
            char_ligne++;
            cpt_espace++;
            continue;
        }
        if (*char_ligne == '+' && strlen(pointeur) == 0) {
            i = 1;
            char_ligne++;
            cpt_espace = 0;
            continue;
        }
        if (strlen(pointeur) > 0 && cpt_espace > 0) {
            *varD = '\0';
            if(*char_ligne == '+'){
                char_ligne++;
                variable_operation(1,varG,pointeur,char_ligne);
                return;
            }
            if(*char_ligne == '-'){
                char_ligne++;
                variable_operation(2,varG,pointeur,char_ligne);
                return;
            }
            if(*char_ligne == '*'){
                char_ligne++;
                variable_operation(3,varG,pointeur,char_ligne);
                return;
            }
            printf("Entier ou variable mal indenté !\n");
            return;
        }
        *varD = *char_ligne;
        varD++;
        char_ligne++;
        cpt_espace = 0;
    }
    *varD = '\0';
    varD = pointeur;
    variable *var = rechercher_variable(varG);
    unbounded_int unbo = string2unbounded_int(varD);
    if(unbo.signe == '*'){
        variable *tmp = rechercher_variable(varD);
        if (tmp == NULL || i == 1) {
            printf("ERREUR : la variable n'existe pas");
            return;
        } else {
            var->valeur = tmp->valeur;
        }
    } else {
        if (var == NULL) {
            ajouter_variable(varG, unbo);
        } else {
            var->valeur = unbo;
        }
    }
}

static void variable_operation(int operation, char *varG, char *varD, char *char_ligne){
    char *op = malloc(sizeof(char));//Partie de la ligne qui contient la variable
    int cpt_espace = 0;
    if(op == NULL){
        perror("malloc error !\n");
        exit(1);
    }
    char *pointeur = op;
    if(*char_ligne != ' '){
        printf("Mauvaise indentation d'operation !\n");
        return;
    }
    while (*char_ligne != '\0' && *char_ligne != '\n') {
        if (*char_ligne == ' ') {
            char_ligne++;
            cpt_espace++;
            continue;
        }
        if(strlen(pointeur) > 0 && cpt_espace > 0){
            printf("Entier ou variable mal indenté !\n");
            return;
        }
        *op = *char_ligne;
        op++;
        char_ligne++;
    }
    *op = '\0';
    op = pointeur;
    if(strlen(op) == 0){
        printf("Opération impossible !\n");
        return;
    }
    variable *var = rechercher_variable(varG);
    unbounded_int unbo1 = string2unbounded_int(varD);
    unbounded_int unbo2 = string2unbounded_int(op);
    if(unbo1.signe == '*' && unbo2.signe == '*'){
        variable *tmp1 = rechercher_variable(varD);
        variable *tmp2 = rechercher_variable(op);
        if (tmp1 == NULL || tmp2 == NULL) {
            printf("ERREUR : la variable n'existe pas");
            return;
        } else {
            switch (operation){
                case 1:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_somme(tmp1->valeur,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_somme(tmp1->valeur,tmp2->valeur);
                    }
                    break;
                case 2:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_difference(tmp1->valeur,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_difference(tmp1->valeur,tmp2->valeur);
                    }
                    break;
                case 3:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_produit(tmp1->valeur,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_produit(tmp1->valeur,tmp2->valeur);
                    }
                    break;
            }
        }
    }
    if(unbo1.signe != '*' && unbo2.signe != '*'){
        switch (operation){
                case 1:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_somme(unbo1,unbo2));
                    } else {
                        var->valeur = unbounded_int_somme(unbo1,unbo2);
                    }
                    break;
                case 2:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_difference(unbo1,unbo2));
                    } else {
                        var->valeur = unbounded_int_difference(unbo1,unbo2);
                    }
                    break;
                case 3:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_produit(unbo1,unbo2));
                    } else {
                        var->valeur = unbounded_int_produit(unbo1,unbo2);
                    }
                    break;
            }
    }
    if(unbo1.signe == '*' && unbo2.signe != '*'){
        variable *tmp1 = rechercher_variable(varD);
        if (tmp1 == NULL) {
            printf("ERREUR : la variable n'existe pas");
            return;
        } else {
            switch (operation){
                case 1:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_somme(tmp1->valeur,unbo2));
                    } else {
                        var->valeur = unbounded_int_somme(tmp1->valeur,unbo2);
                    }
                    break;
                case 2:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_difference(tmp1->valeur,unbo2));
                    } else {
                        var->valeur = unbounded_int_difference(tmp1->valeur,unbo2);
                    }
                    break;
                case 3:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_produit(tmp1->valeur,unbo2));
                    } else {
                        var->valeur = unbounded_int_produit(tmp1->valeur,unbo2);
                    }
                    break;
            }
        }
    }
    if(unbo1.signe != '*' && unbo2.signe == '*'){
        variable *tmp2 = rechercher_variable(op);
        if (tmp2 == NULL) {
            printf("ERREUR : la variable n'existe pas");
            return;
        } else {
            switch (operation){
                case 1:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_somme(unbo1,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_somme(unbo1,tmp2->valeur);
                    }
                    break;
                case 2:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_difference(unbo1,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_difference(unbo1,tmp2->valeur);
                    }
                    break;
                case 3:
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_produit(unbo1,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_produit(unbo1,tmp2->valeur);
                    }
                    break;
            }
        }
    }
}