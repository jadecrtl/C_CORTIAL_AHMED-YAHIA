#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

typedef struct chiffre{
    struct chiffre *suivant;
    char c;
    struct chiffre *precedent;
}chiffre;

typedef struct{
    char signe; /* soit ’+’ soit ’-’ */
    size_t len; /* longueur de la liste */
    chiffre *premier; /* pointeur vers le premier élément de la liste*/
    chiffre *dernier; /*pointeur vers le dernier élément de la liste*/
}unbounded_int;

/*Prend une chaîne de caractère représentant un entier et renvoie le unbounded_int correspondant à cet entier. Si la chaîne de
caractère ne représente pas un entier, on renvoie un unbounded_int avec l'attribut signe = '*'. */
unbounded_int string2unbounded_int(const char *e);

/*Prend un long long et renvoie le unbounded_int correspondant.*/
unbounded_int ll2unbounded_int(long long i);

/*Prend un unbounded_int et renvoie la chaine de caractère correspondante (contraire de string2unbounded_int()).*/
char *unbounded_int2string(unbounded_int i);

/*Retourne une des valeurs −1, 0, 1 si, respectivement, a < b, a == b, a > b (ou a, b sont
les entiers représentés par a et b).*/
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

/*Pareil que unbounded_int_cmp_unbounded_int() mais avec un long long en second argument.*/
int unbounded_int_cmp_ll(unbounded_int a, long long b);

/*Renvoie la somme de a et b en unbounded_int.*/
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

/*Renvoie la différence de a et b en unbounded_int.*/
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

/*Renvoie le produit de a et b en unbounded_int.*/
unbounded_int unbounded_int_produit( unbounded_int a, unbounded_int b);

void affiche(unbounded_int a);

#endif