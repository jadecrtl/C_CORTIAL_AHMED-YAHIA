#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "unbounded_int.h"

/*Prend une chaîne de caractère représentant un entier et renvoie le unbounded_int correspondant à cet entier. Si la chaîne de
caractère ne représente pas un entier, on renvoie un unbounded_int avec l'attribut signe = '*'. */

static unbounded_int *creer_unbounded_int();
static chiffre *creer_chiffre();
static void ajouter_chiffre_a_la_fin(const char e, unbounded_int *unbo);

//static char *long2char(long long b);

unbounded_int string2unbounded_int(const char *e) {
    /*ALGORITHME
    ABCDEFGHIJ
    1234478909

    creer 1(nouveau) > A (precedent = NULL, suivant = NULL)
    modifier UNBI premier = nouveau, dernier = nouveau
    
    UNBI premier = (A), dernier = (A)
    A precedent = NULL
    A suivant = NULL

    creer 2(nouveau) > B (precedent = dernier(A), suivant = NULL)
    modifier dernier (A) suivant = nouveau (B)
    modifier UNBI dernier = nouveau

    UNBI premier = (A), dernier = (B)
    A precedent = NULL
    A suivant = B
    B precedent = A
    B suivant = NULL

    creer 3(nouveau) > C (precedent = dernier(B), suivant = NULL)
    modifier dernier (B) suivant = nouveau (C)
    modifier UNBI dernier = nouveau (C)

    UNBI premier = (A), dernier = (C)
    A precedent = NULL
    A suivant = B
    B precedent = A
    B suivant = C
    C precedent = B
    C suivant = NULL

    */
    unbounded_int *unbo = creer_unbounded_int();
    if (*e == '-') {
        unbo->signe = '-';
        e++;
    }
    else {
        unbo->signe = '+';
    }
    chiffre *chi = creer_chiffre();
    while (*e != '\0') {
        ajouter_chiffre_a_la_fin(*e, unbo);
        unbo->len++;
        e++;
    }
    free(chi);
    return *unbo;
}

static unbounded_int *creer_unbounded_int() {
    unbounded_int* unbo = malloc(sizeof(unbounded_int));
    if(unbo == NULL) {
        perror("\ncreer_unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    unbo->len = 0;
    return unbo;
}

static chiffre *creer_chiffre() {
    chiffre* chi = malloc(sizeof(chiffre));
    if(chi == NULL) {
        perror("\ncreer_chiffre : La création du chiffre a échouée\n");
        exit(1);
    }
    return chi;
}

static void ajouter_chiffre_a_la_fin(const char e, unbounded_int *unbo) {
    if (!isdigit(e)) {
        unbo->signe = '*';
        printf("\najouter_chiffre_a_la_fin : Cette chaîne contient un non numérique.\n");
    }
    chiffre *chi = creer_chiffre();
    chi->c = e;
    if (unbo->premier == NULL && unbo->dernier == NULL) {
        //premier élément de la liste
        chi->precedent = NULL;
        chi->suivant = NULL;
        unbo->premier = chi;
        unbo->dernier = chi;
    }
    else {
        //sinon insertion en dernière position
        chi->precedent = unbo->dernier;
        chi->suivant = NULL;
        unbo->dernier->suivant = chi;
        unbo->dernier = chi;
    }
}



/*
static char *long2char(long long b){
    char *c = malloc(sizeof(char));
    assert(c != NULL);
    return c;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){
    size_t length = sizeof(b)/sizeof(long long);
    if((a.signe == '-' && b >= 0) || (a.len < length)){
        return -1;
    }
    if((a.signe == '+' && b < 0) || (a.len > length)){
        return 1;
    }
    long long tmp = b/pow(10,length-1);
    while(a.premier != NULL){
        if(a.premier->c-'0' < tmp){
            return -1;
        }
        if(a.premier->c-'0' > tmp){
            return 1;
        }
        a.premier = a.premier->suivant;
        tmp = tmp/pow(10,length-1);
    }
    return 0;
}
*/
