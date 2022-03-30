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
//static char *long2char(long long b);

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
