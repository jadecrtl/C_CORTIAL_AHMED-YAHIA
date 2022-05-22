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

static variable *creer_variable();
static void ajouter_variable(char *nomVar, unbounded_int unbo);
static FILE *ouvrir_fichier_en_lecture(char *nom_fichier);
static FILE *ouvrir_fichier_en_ecriture(char *nom_fichier);
variable *rechercher_variable(char *nom_variable);

void recupere_argument(int argc, char *argv[]);
static int compare_chaine(char *nomVar);
void interprete_fichier(int nbr_argument);
static void interprete_ligne(char *char_ligne, int nbr_argument);
static void print_variable(char *char_ligne, int nbr_argument);
static void variable_assignation_ou_operations(char *char_ligne, char *varG);
static void variable_operation(int operation, char *varG, char *varD, char *char_ligne);

liste_variable *variables;

int main(int argc, char *argv[]) {
    variables = malloc(sizeof(liste_variable));
    if (variables == NULL) {
        perror("\ncreer_liste_variable : la création de la liste de variable a échouée\n");
        exit(1);
    }
    recupere_argument(argc, argv);

    int fclose_resultat = fclose(fichier_resultat);//s'il n'existe pas on n'a pas de close a faire
    if (fclose_resultat == EOF) {
        perror("Erreur de fermeture fichier");
        exit(1);
    }
    return 0;
}

void recupere_argument(int argc, char *argv[]) {
    if (argc == 1) {
        fichier_source = stdin;
        fichier_resultat = stdout;
        interprete_fichier(0);
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-i") == 0) {
            fichier_source = ouvrir_fichier_en_lecture(argv[2]);
            fichier_resultat = stdout;
            interprete_fichier(1);
        }
        if (strcmp(argv[1], "-o") == 0) {
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[2]);
            fichier_source = stdin;
            interprete_fichier(2);
        }
    }
    if (argc == 5) {
        if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-o") == 0) {
            fichier_source = ouvrir_fichier_en_lecture(argv[2]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[4]);
            interprete_fichier(3);
        }
        if (strcmp(argv[1], "-o") == 0 && strcmp(argv[3], "-i") == 0) {
            fichier_source = ouvrir_fichier_en_lecture(argv[4]);
            fichier_resultat = ouvrir_fichier_en_ecriture(argv[2]);
            interprete_fichier(3);
        }
    }
}

static FILE *ouvrir_fichier_en_lecture(char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("impossible d'ouvrir le fichier source en lecture\n");
        exit(1);
    }
    return fichier;
}

static FILE *ouvrir_fichier_en_ecriture(char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        perror("Impossible d'ouvrir le fichier resultat en ecriture.\n");
        exit(1);
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

static void ajouter_variable(char *nomVar, unbounded_int unbo) {
    char *i = nomVar;
    if(compare_chaine(i) == 0){
        exit(1);
    }
    while (*i != '\0') {
        if (isdigit(*i)){
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
    return NULL;
}

void interprete_fichier(int nbr_argument){
    char *stock_ligne = malloc(sizeof(char));
    switch (nbr_argument){
        case 0 : // cas stdin et stdout
            fichier_source = fopen("tmp_scanf.txt","w+"); //on stocke dans un fichier ce qu'on ecrit dans le flux d'entree stdin pour apres avec le case 1 l'interpreter
            char *tmp = malloc(sizeof(char));
            if (tmp == NULL) {
                perror("malloc erreur!!\n");
                exit(1);
            }
            while(fgets(tmp,1024,stdin) != NULL){
                fputs(tmp, fichier_source);
            }
            fseek(fichier_source,0,SEEK_SET); // on se replace au debut du fichier
            interprete_fichier(1);
            break;
        case 1 : // cas fichier entree et stdout
            while(fgets(stock_ligne, 1024, fichier_source) != NULL){
                interprete_ligne(stock_ligne, nbr_argument);
            }
            if(ferror(fichier_source)){
                perror("Erreur fgets !");
                exit(1);
            }
            free(stock_ligne);
            break;
        case 2 : // cas stdin et fichier resultat 
            fichier_source = fopen("tmp_scanf.txt","w+");
            char *tmp_fichier_resultat = malloc(sizeof(char));
            if (tmp_fichier_resultat == NULL) {
                perror("malloc erreur!!\n");
                exit(1);
            }
            while(fgets(tmp_fichier_resultat,1024,stdin) != NULL){
                fputs(tmp_fichier_resultat, fichier_source);
            }
            fseek(fichier_source,0,SEEK_SET);
            interprete_fichier(3);
            break;
        case 3 : // cas fichier entree et fichier resultat
            while(fgets(stock_ligne, 1024, fichier_source) != NULL){
                interprete_ligne(stock_ligne, nbr_argument);
            }
            if(ferror(fichier_source)){
                perror("Erreur fgets !");
                exit(1);
            }
            free(stock_ligne);
            break;
    }
}

static void interprete_ligne(char *char_ligne, int nbr_argument){
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
    char *tmp = varG; // pointeur char tmp qui pointe sur varG
    int cpt_espace = 0;
    while (*char_ligne != '\0' && *char_ligne != '=') {
        if (*char_ligne == ' ') {
            if (strcmp(tmp, "print") == 0) { // On check si on doit print une variable
                print_variable(char_ligne, nbr_argument);
                return;
            }
            cpt_espace++;
            char_ligne++;
            continue;
        }
        if(strlen(tmp) > 0 && cpt_espace > 0){ //si la variable contient des espaces on ne la cree pas
            return;
        }
        // ajout du caractère actuel de la ligne dans varG
        *varG = *char_ligne;
        varG++;
        char_ligne++;
    }
    *varG = '\0'; 
    varG = tmp;//Permet de pointer sur le premier caractère de varG pour l'avoir en entier car tmp pointe vers le premier element
    if (strlen(char_ligne) == 0 && strlen(varG) != 0) { // cas ou il y a juste un print ou une variable
        return;
    }
    char_ligne++;
    variable_assignation_ou_operations(char_ligne, varG);
}

static void print_variable(char *char_ligne, int nbr_argument) {
    char *varD = malloc(sizeof(char));//Partie de la ligne qui contient la variable
    if(varD == NULL){
        perror("malloc error !\n");
        exit(1);
    }
    char *tmp = varD;
    int cpt_espace = 0;
    while (*char_ligne != '\0' && *char_ligne != '\n') {
        if (*char_ligne == ' ') {
            char_ligne++;
            cpt_espace++;
            continue;
        }
        if (*char_ligne == '=' || *char_ligne == '*' || *char_ligne == '+' || *char_ligne == '-') { //Si il y a une operation dans la variable qui est cense etre affiche par le print alors on return
            return;
        }
        if(strlen(tmp) > 0 && cpt_espace > 1){ //Si la variable qu'on veut afficher contient au moins un espace on return
            return;
        }
        *varD = *char_ligne;
        varD++;
        char_ligne++;
    }
    *varD = '\0';
    varD = tmp;
    if (compare_chaine(tmp) == 0) { // Pas le droit de print une variable qui contient print
        return;
    }
    if (strlen(varD) == 0) { // Pas de variable
        return;
    }
    if (rechercher_variable(varD) == NULL) {
        if (nbr_argument == 1) {
            printf("%s = 0\n", varD);
        }
        if (nbr_argument == 3) {
            char *res = malloc (sizeof(char));
            strcat(strcpy(res, varD), " = 0\n");   
            fputs(res, fichier_resultat);
        }
    }
    else {
        variable *res = rechercher_variable(varD);
        char *unbo = unbounded_int2string(res->valeur);
        if (nbr_argument == 0 || nbr_argument == 1) {
            printf("%s = %s\n", res->nom, unbo);
        }
        if (nbr_argument == 2 || nbr_argument == 3) {
            char *resultat = malloc (sizeof(char));
            strcat(strcpy(resultat, varD), " = ");   
            strcat(resultat, unbo); 
            strcat(resultat, "\n");   
            fputs(resultat, fichier_resultat);
        }
    }
}

static void variable_assignation_ou_operations(char *char_ligne, char *varG) {
    char *varD = malloc(sizeof(char)); //Partie de la ligne qui contient la variable
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
        if (strlen(pointeur) == 0 && *char_ligne == '+') { //ex : a = +123
            cpt_espace = 0;
            i = 1;
            char_ligne++;
            continue;
        }
        if (strlen(pointeur) > 0 && cpt_espace > 0) { // une variable puis plusieurs espaces
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
            return;
        }

        *varD = *char_ligne;
        varD++;
        char_ligne++;
        cpt_espace = 0;
    }
    *varD = '\0';
    varD = pointeur;
    if (strlen(varD) == 0 || strcmp(varD,"-") == 0) { //cas ou varD est vide ou il y a juste '-' dans varD
        return;
    }
    variable *var = rechercher_variable(varG);
    unbounded_int unbo = string2unbounded_int(varD);
    if(unbo.signe == '*'){ //varD est donc possiblement une variable
        variable *tmp = rechercher_variable(varD);
        if (tmp == NULL || i == 1) {
            //printf("ERREUR : la variable n'existe pas");
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
    if(*char_ligne != ' '){ //il doit y avoir minimum un espace apres l'operateur
        return;
    }
    while (*char_ligne != '\0' && *char_ligne != '\n') {
        if (*char_ligne == ' ') {
            char_ligne++;
            cpt_espace++;
            continue;
        }
        if(strlen(pointeur) > 0 && cpt_espace > 0){ 
            return;
        }
        if(*char_ligne == '+' || *char_ligne == '-' || *char_ligne == '*'){
            return;
        }
        *op = *char_ligne;
        op++;
        char_ligne++;
        cpt_espace = 0;
    }
    *op = '\0';
    op = pointeur;
    if(strlen(op) == 0){
        return;
    }
    variable *var = rechercher_variable(varG);
    unbounded_int unbo1 = string2unbounded_int(varD); // unbounded_int ou int avant l'operateur
    unbounded_int unbo2 = string2unbounded_int(op); // unbounded_int ou int apres l'operateur
    if(unbo1.signe == '*' && unbo2.signe == '*'){ // S'ils sont tout les 2 des variables
        variable *tmp1 = rechercher_variable(varD);
        variable *tmp2 = rechercher_variable(op);
        if (tmp1 == NULL || tmp2 == NULL) {
            //printf("ERREUR : la variable n'existe pas");
            return;
        } else {
            switch (operation){
                case 1: // somme
                    if(var == NULL){ // Soit on cree une variable avec la somme de unbo1 et unbo2
                        ajouter_variable(varG,unbounded_int_somme(tmp1->valeur,tmp2->valeur));
                    } else { // Soit on modifie la variable avec la somme de unbo1 et unbo2
                        var->valeur = unbounded_int_somme(tmp1->valeur,tmp2->valeur);
                    }
                    break;
                case 2: // soustraction
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_difference(tmp1->valeur,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_difference(tmp1->valeur,tmp2->valeur);
                    }
                    break;
                case 3: // produit
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_produit(tmp1->valeur,tmp2->valeur));
                    } else {
                        var->valeur = unbounded_int_produit(tmp1->valeur,tmp2->valeur);
                    }
                    break;
            }
        }
    }
    if(unbo1.signe != '*' && unbo2.signe != '*'){ // Ce sont tout les 2 des unbounded_int
        switch (operation){
                case 1: // somme
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_somme(unbo1,unbo2));
                    } else {
                        var->valeur = unbounded_int_somme(unbo1,unbo2);
                    }
                    break;
                case 2: // soustraction
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_difference(unbo1,unbo2));
                    } else {
                        var->valeur = unbounded_int_difference(unbo1,unbo2);
                    }
                    break;
                case 3: // produit
                    if(var == NULL){
                        ajouter_variable(varG,unbounded_int_produit(unbo1,unbo2));
                    } else {
                        var->valeur = unbounded_int_produit(unbo1,unbo2);
                    }
                    break;
            }
    }
    if(unbo1.signe == '*' && unbo2.signe != '*'){ // unbo1 est une variable et unbo2 un unbounded_int
        variable *tmp1 = rechercher_variable(varD);
        if (tmp1 == NULL) { // si la variable n'existe pas
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
    if(unbo1.signe != '*' && unbo2.signe == '*'){ // unbo1 est un unbounded_int et unbo2 une variable
        variable *tmp2 = rechercher_variable(op);
        if (tmp2 == NULL) {
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