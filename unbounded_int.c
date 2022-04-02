#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "unbounded_int.h"


static unbounded_int *creer_unbounded_int();
static chiffre *creer_chiffre();
static void ajouter_chiffre_a_la_fin(const char e, unbounded_int *unbo);
static void ajouter_chiffre_au_debut(const char e, unbounded_int *unbo);

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

static void ajouter_chiffre_au_debut(const char e, unbounded_int *unbo) {
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
        //sinon insertion en première position
        unbo->premier->precedent = chi;
        chi->suivant = unbo->premier;
        unbo->premier = chi;
        chi->precedent = NULL;
    }
}

/*Prend une chaîne de caractère représentant un entier et renvoie le unbounded_int correspondant à cet entier. Si la chaîne de
caractère ne représente pas un entier, on renvoie un unbounded_int avec l'attribut signe = '*'. */
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
    while (*e != '\0') {
        ajouter_chiffre_a_la_fin(*e, unbo);
        unbo->len++;
        e++;
    }
    return *unbo;
}

/*Prend un long long et renvoie le unbounded_int correspondant.*/
unbounded_int ll2unbounded_int(long long i) {
    int j = i;
    unbounded_int *unbo = creer_unbounded_int();
    if (i < 0) {
        unbo->signe = '-';
        j = -j;
    }
    else {
        unbo->signe = '+';
    }
    while (j > 0) {
        char tmp = (char) ((j % 10) + '0');
        ajouter_chiffre_au_debut(tmp, unbo);
        j = j / 10;
        unbo->len++;
    }
    return *unbo;
}

/*Prend un unbounded_int et renvoie la chaine de caractère correspondante (contraire de string2unbounded_int()).*/
char *unbounded_int2string(unbounded_int i) {
    char *e = malloc(sizeof(char));
    if(e == NULL) {
        perror("\ncreer_chiffre : La création du chiffre a échouée\n");
        exit(1);
    }
    int j = 0;
    while(i.premier != NULL) {
        e[j] = i.premier->c;
        j++;
        i.premier = i.premier->suivant;
    }
    return e;
}


/*Retourne une des valeurs −1, 0, 1 si, respectivement, a < b, a == b, a > b (ou a, b sont
les entiers représentés par a et b).*/
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
    if (a.signe == '*' || b.signe == '*') {
        return 99;
    }
    if (a.signe == '-' && b.signe == '+') {
        return -1;
    }
    if (a.signe == '+' && b.signe == '-') {
        return 1;
    }
    if (a.signe == '+' && b.signe == '+') {
        if (a.len > b.len) {
            return 1;
        }
        if (a.len < b.len) {
            return -1;
        }
        if (a.len == b.len) {
            int tmp = strcmp(unbounded_int2string(a), unbounded_int2string(b));
            if (tmp == 0) {
                return 0;
            }
            if (tmp > 0) {
                return 1;
            }
            if (tmp < 0) {
                return -1;
            }
        }
    }
    if (a.signe == '-' && b.signe == '-') {
        if (a.len > b.len) {
            return -1;
        }
        if (a.len < b.len) {
            return 1;
        }
        if (a.len == b.len) {
            int tmp = strcmp(unbounded_int2string(a), unbounded_int2string(b));
            if (tmp == 0) {
                return 0;
            }
            if (tmp > 0) {
                return -1;
            }
            if (tmp < 0) {
                return 1;
            }
        }
    }
    return -99;
}

/*Pareil que unbounded_int_cmp_unbounded_int() mais avec un long long en second argument.*/
int unbounded_int_cmp_ll(unbounded_int a, long long b) {
    unbounded_int unbo_b = ll2unbounded_int(b);
    unbounded_int_cmp_unbounded_int(a, unbo_b);
    return -99;
}