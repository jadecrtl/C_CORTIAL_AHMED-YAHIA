#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unbounded_int.h"

FILE *fichier_source;
FILE *fichier_resultat;

typedef struct variable{
    char *nom;
    struct variable *suivant;
    unbounded_int valeur;
    struct variable *precedent;
}variable;

typedef struct {
    size_t len;
    variable *premier;
    variable *dernier;
}liste_variable;

void recupere_argument(int argc, char *argv[]);
FILE *ouvrir_fichier_en_lecture(char *nom_fichier);
FILE *ouvrir_fichier_en_ecriture(char *nom_fichier);

int main(int argc, char *argv[]) {
    printf("hello world calc_unbounded_int\n");
    recupere_argument(argc, argv);
    fclose(fichier_source);
    fclose(fichier_resultat);
    return 0;
}

void recupere_argument(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s \n", i, argv[i]);
    }
    if (argc == 1) {
        printf("faire stdin ou stdout\n");
        exit(1);
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-i") == 0) {
            printf("faire stdout\n");
            printf("fichier source = %s\n", argv[2]);
            fichier_source = ouvrir_fichier_en_lecture(argv[2]);
            return;
        }
        if (strcmp(argv[1], "-o") == 0) {
            printf("faire stdin\n");
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
