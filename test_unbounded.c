#include <stdlib.h>
#include <stdio.h>

#include "unbounded_int.h"

void affiche(unbounded_int a);


int main(void) {
    printf("Hello\n");
//    char a[] = {'-','1','2','3','4','5'};
    char b[] = {'1','2','3','4','5'};
//    affiche(string2unbounded_int(a));
    affiche(string2unbounded_int(b));
    return 0;
}

void affiche(unbounded_int a) {
    printf("Voici le signe de unbounded_int : %c\n", a.signe);
    printf("Voici la taille de unbounded_int : %ld\n", a.len);
    printf("Voici le premier chiffre de unbounded_int : %c\n", a.premier->c);
    printf("Voici le dernier chiffre de unbounded_int : %c\n", a.dernier->c);
}
