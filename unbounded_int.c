#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "unbounded_int.h"

/*Prend une chaîne de caractère représentant un entier et renvoie le unbounded_int correspondant à cet entier. Si la chaîne de
caractère ne représente pas un entier, on renvoie un unbounded_int avec l'attribut signe = '*'. */
unbounded_int string2unbounded_int(const char *e) {
    unbounded_int* unbo = malloc(sizeof(unbo));
    assert(unbo != NULL);
    if (e == '-') {
        unbo->signe = '-';
        e++;
    }
    else {
        unbo->signe = '+';
    }
    chiffre* nouveau = malloc(sizeof(chiffre));
    assert(nouveau != NULL);
    while(e != '\0') {
        
    }
    return *unbo;
}
