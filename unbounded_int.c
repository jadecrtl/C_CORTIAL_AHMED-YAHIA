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

unbounded_int string2unbounded_int(const char *e) {
    unbounded_int *unbo = creer_unbounded_int();
    if (*e == '-') {
        unbo->signe = '-';
        *e++;
    }
    else {
        unbo->signe = '+';
    }
    return *unbo;
}

static unbounded_int *creer_unbounded_int() {
    unbounded_int* unbo = malloc(sizeof(unbounded_int));
    if(unbo == NULL) {
        perror("\ncreer_unbounded_int : La création de l'unbounded_int a échouée\n");
        free(unbo);
        exit(1);
    }
    return unbo;
}