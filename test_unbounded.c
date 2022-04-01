#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unbounded_int.h"

void affiche(unbounded_int *unbo);
void test_string2unbounded_int(const char *e, const char signe_voulu, const size_t len_voulu);
void test_ll2unbounded_int(long long test);
void test_unbounded_int2string(unbounded_int unbo);
void afficheUnboPasPointeur(unbounded_int unbo);


int main(void) {
    printf("\nTest string2unbounded_int : \n\n");
    test_string2unbounded_int("1", '+', 1);
    test_string2unbounded_int("-1", '-', 1);
    test_string2unbounded_int("1234", '+', 4);
    test_string2unbounded_int("-1234", '-', 4);
    test_string2unbounded_int("-a1234", '*', 5);
    test_string2unbounded_int("-12a34", '*', 5);
    test_string2unbounded_int("-1234a", '*', 5);

    printf("\nTest ll2unbounded_int : \n\n");
    test_ll2unbounded_int(123456789);
    test_ll2unbounded_int(-123456789);

    printf("\nTest ll2unbounded_int : \n\n");
    test_unbounded_int2string(ll2unbounded_int(123456789));
    printf("\n\n******* TEST OK ********\n\n");
    return 0;
}

void affiche(unbounded_int *unbo) {
    printf("Voici le signe de unbounded_int : %c\n", unbo->signe);
    printf("Voici la taille de unbounded_int : %ld\n", unbo->len);
    printf("Voici le premier chiffre de unbounded_int : %c\n", unbo->premier->c);
    printf("Voici le dernier chiffre de unbounded_int : %c\n", unbo->dernier->c);
    chiffre *chi = malloc(sizeof(chiffre));
    if(chi == NULL) {
        perror("\naffiche : La création du chiffre a échouée\n");
        exit(1);
    }
    printf("%c", unbo->signe);
    chi = unbo->premier;
    while(chi != NULL) {
        printf("%c", chi->c);
        chi = chi->suivant;
    }
    printf("\n");
}

void afficheUnboPasPointeur(unbounded_int unbo) {
    printf("Voici le signe de unbounded_int : %c\n", unbo.signe);
    printf("Voici la taille de unbounded_int : %ld\n", unbo.len);
    printf("Voici le premier chiffre de unbounded_int : %c\n", unbo.premier->c);
    printf("Voici le dernier chiffre de unbounded_int : %c\n", unbo.dernier->c);
    chiffre *chi = malloc(sizeof(chiffre));
    if(chi == NULL) {
        perror("\naffiche : La création du chiffre a échouée\n");
        exit(1);
    }
    printf("%c", unbo.signe);
    chi = unbo.premier;
    while(chi != NULL) {
        printf("%c", chi->c);
        chi = chi->suivant;
    }
    printf("\n");
}

void test_string2unbounded_int(const char *test, const char signe_voulu, const size_t len_voulu) {
    unbounded_int* ubi = malloc(sizeof(unbounded_int));
    if (ubi == NULL) {
        perror("\ntest_string2unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *ubi = string2unbounded_int(test);
    if (ubi->signe != signe_voulu || ubi->len != len_voulu) {
        printf("\n** KO ** Echec du test_string2unbounded_int: %s, %c, %ld\n", test, signe_voulu, len_voulu);
        free(ubi);
        exit(1);
    }
    affiche(ubi);
    printf("OK test_string2unbounded_int: %s, %c, %ld\n", test, signe_voulu, len_voulu);
    free(ubi);
}

void test_ll2unbounded_int(long long test) {
    unbounded_int* ubi = malloc(sizeof(unbounded_int));
    if (ubi == NULL) {
        perror("\ntest_string2unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    printf("Début test_ll2unbounded_int\n");
    *ubi = ll2unbounded_int(test);
    affiche(ubi);
    printf("OK test_ll2unbounded_int\n");
    free(ubi);
}

void test_unbounded_int2string(unbounded_int unbo) {
    char *e = unbounded_int2string(unbo);
    afficheUnboPasPointeur(unbo);
    puts(e);
}