#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unbounded_int.h"

void affiche(unbounded_int *unbo);
void afficheUnboPasPointeur(unbounded_int unbo);
void test_string2unbounded_int(const char *e, const char signe_voulu, const size_t len_voulu);
void test_ll2unbounded_int(long long test);
void test_unbounded_int2string(const char *c);
void test_unbounded_int_cmp_unbounded_int(const char *test_a, const char *test_b, const int resultat_voulu);
void test_unbounded_int_cmp_ll(const char *test_a, long long test_b, const int resultat_voulu);
void test_somme_2unbounded_int_positifs(const char *test_a, const char* test_b, const char* resultat);
void test_difference_2unbounded_int_positifs(const char *test_a, const char* test_b, const char* resultat);
void test_somme(const char *test_a, const char* test_b, const char* resultat);
void test_difference(const char *test_a, const char* test_b, const char* resultat);

int main(void) {
    printf("\nTest string2unbounded_int : \n\n");

    test_string2unbounded_int("1", '+', 1);
    test_string2unbounded_int("-1", '-', 1);
    test_string2unbounded_int("1234", '+', 4);
    test_string2unbounded_int("-1234", '-', 4);
    test_string2unbounded_int("-a1234", '*', 5);
    test_string2unbounded_int("-12a34", '*', 5);
    test_string2unbounded_int("-1234a", '*', 5);

    printf("\n\nTest ll2unbounded_int : \n\n");
    test_ll2unbounded_int(123456789);
    test_ll2unbounded_int(-123456789);

    printf("\n\nTest unbounded_int2string : \n\n");
    test_unbounded_int2string("5230");
    test_unbounded_int2string("-5230");
    test_unbounded_int2string("26a8");
    

    printf("\n\nTest unbounded_int_cmp_unbounded_int : \n\n");

    test_unbounded_int_cmp_unbounded_int("1234", "123", 1);
    test_unbounded_int_cmp_unbounded_int("567", "5678", -1);
    test_unbounded_int_cmp_unbounded_int("-123", "123", -1);
    test_unbounded_int_cmp_unbounded_int("123", "-123", 1);
    test_unbounded_int_cmp_unbounded_int("123", "124", -1);
    test_unbounded_int_cmp_unbounded_int("124", "123", 1);
    test_unbounded_int_cmp_unbounded_int("-123", "-124", 1);
    test_unbounded_int_cmp_unbounded_int("-124", "-123", -1);
    test_unbounded_int_cmp_unbounded_int("567", "567", 0);
    test_unbounded_int_cmp_unbounded_int("-205567", "-205567", 0);

    printf("\n\nTest unbounded_int_cmp_ll : \n\n");

    test_unbounded_int_cmp_ll("1234", 123, 1);
    test_unbounded_int_cmp_ll("567", 5678, -1);
    test_unbounded_int_cmp_ll("-123", 123, -1);
    test_unbounded_int_cmp_ll("123", -123, 1);
    test_unbounded_int_cmp_ll("123", 124, -1);
    test_unbounded_int_cmp_ll("124", 123, 1);
    test_unbounded_int_cmp_ll("-123", -124, 1);
    test_unbounded_int_cmp_ll("-124", -123, -1);
    test_unbounded_int_cmp_ll("567", 567, 0);
    test_unbounded_int_cmp_ll("-205567", -205567, 0);

    printf("\n\nTest somme\n");

    test_somme_2unbounded_int_positifs("123", "456", "579");
    test_somme_2unbounded_int_positifs("1234", "456", "1690");
    test_somme_2unbounded_int_positifs("123", "4567", "4690");
    test_somme_2unbounded_int_positifs("-678", "7865", "7187");
    test_somme_2unbounded_int_positifs("999", "-667", "332");
    test_somme_2unbounded_int_positifs("-123", "-10", "-133");
    test_somme_2unbounded_int_positifs("-678", "-786", "-1464");
    test_somme_2unbounded_int_positifs("999", "-6673", "-5674");
    test_somme_2unbounded_int_positifs("-123", "-10000", "-10123");

    printf("\n\nTest difference\n");

    test_difference_2unbounded_int_positifs("500", "456", "44");
    test_difference_2unbounded_int_positifs("1234", "456", "778");
    test_difference_2unbounded_int_positifs("6", "96", "-90");
    test_difference_2unbounded_int_positifs("-500", "-46", "-454");
    test_difference_2unbounded_int_positifs("-124", "456", "-580");
    test_difference_2unbounded_int_positifs("600", "-96", "696");
    test_difference_2unbounded_int_positifs("-600", "200", "-800");
    test_difference_2unbounded_int_positifs("-600", "-600", "0");
    test_difference_2unbounded_int_positifs("10000000", "17", "9999983");

    printf("\n\n******* TEST OK ********\n\n");
    return 0;
}

void affiche(unbounded_int *unbo) {
    //printf("Voici le signe de unbounded_int : %c\n", unbo->signe);
    //printf("Voici la taille de unbounded_int : %ld\n", unbo->len);
    //printf("Voici le premier chiffre de unbounded_int : %c\n", unbo->premier->c);
    //printf("Voici le dernier chiffre de unbounded_int : %c\n\n", unbo->dernier->c);
    chiffre *chi = malloc(sizeof(chiffre));
    if(chi == NULL) {
        perror("\naffiche : La création du chiffre a échouée\n");
        exit(1);
    }
    printf("unbounded_int affichage en entier : ");
    printf("%c", unbo->signe);
    chi = unbo->premier;
    while(chi != NULL) {
        printf("%c", chi->c);
        chi = chi->suivant;
    }
    printf("\n");
    //free(chi);
}

void afficheUnboPasPointeur(unbounded_int unbo) {
    printf("Voici le signe de unbounded_int : %c\n", unbo.signe);
    printf("Voici la taille de unbounded_int : %ld\n", unbo.len);
    printf("Voici le premier chiffre de unbounded_int : %c\n", unbo.premier->c);
    printf("Voici le dernier chiffre de unbounded_int : %c\n\n", unbo.dernier->c);
    chiffre *chi = malloc(sizeof(chiffre));
    if(chi == NULL) {
        perror("\naffiche : La création du chiffre a échouée\n");
        exit(1);
    }
    printf("unbounded_int affichage en entier : ");
    printf("%c", unbo.signe);
    chi = unbo.premier;
    while(chi != NULL) {
        printf("%c", chi->c);
        chi = chi->suivant;
    }
    printf("\n");
    //free(chi);
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
    printf("\nOK test_string2unbounded_int: %s, %c, %ld\n\n", test, signe_voulu, len_voulu);
    //free(ubi);
}

void test_ll2unbounded_int(long long test) {
    unbounded_int* ubi = malloc(sizeof(unbounded_int));
    if (ubi == NULL) {
        perror("\ntest_string2unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    //printf("Début test_ll2unbounded_int\n");
    *ubi = ll2unbounded_int(test);
    affiche(ubi);
    printf("\nOK test_ll2unbounded_int\n\n");
    //free(ubi);
}

void test_unbounded_int2string(const char *resultat) {
    unbounded_int *u = malloc(sizeof(unbounded_int));
    if (u == NULL) {
        perror("\ntest_unbounded_int2string : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *u = string2unbounded_int(resultat);
    char *res = unbounded_int2string(*u);
    puts(res); //puts(e) écrit la chaine de caractère du tableau de char e
    puts(resultat);
    while(*res != '\0' && *resultat != '\0'){
        if(*res != *resultat){
            printf("\n** KO ** test_string2unbounded_int\n");
            return;
        }
        res++;
        resultat++;
    }
    printf("\nOK test_string2unbounded_int\n\n");
}

void test_unbounded_int_cmp_unbounded_int(const char *test_a, const char *test_b, const int resultat_voulu) {
    unbounded_int *unbo_a = malloc(sizeof(unbounded_int));
    if (unbo_a == NULL) {
        perror("\ntest_string2unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *unbo_a = string2unbounded_int(test_a);
    affiche(unbo_a);

    unbounded_int *unbo_b = malloc(sizeof(unbounded_int));
    if (unbo_b == NULL) {
        perror("\ntest_string2unbounded_int : La création de l'unbounded_int a échouée\n");
        exit(1);
    }

    *unbo_b = string2unbounded_int(test_b);
    affiche(unbo_b);

    int resultat = unbounded_int_cmp_unbounded_int(*unbo_a, *unbo_b);
    if (resultat != resultat_voulu) {
        printf("\n** KO ** Echec du test_unbounded_int_cmp_unbounded_int: %s, %s, %d\n", test_a, test_b, resultat_voulu);
        //free(unbo_a);
        //free(unbo_b);
        exit(1);
    }
    //printf("Resultat de la comparaison entre les 2 unbo : %d", resultat);
    printf("OK test_unbounded_int_cmp_unbounded_int: %s, %s, %d\n", test_a, test_b, resultat_voulu);
    //free(unbo_a);
    //free(unbo_b);
}

void test_unbounded_int_cmp_ll(const char *test_a, long long test_b, const int resultat_voulu) {
    unbounded_int *unbo_a = malloc(sizeof(unbounded_int));
    if (unbo_a == NULL) {
        perror("\ntest_unbounded_int_cmp_ll : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    unbounded_int* unbo_b = malloc(sizeof(unbounded_int));
    if (unbo_b == NULL) {
        perror("\ntest_unbounded_int_cmp_ll : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *unbo_a = string2unbounded_int(test_a);
    *unbo_b = ll2unbounded_int(test_b);
    int resultat = unbounded_int_cmp_unbounded_int(*unbo_a, *unbo_b);
    if (resultat != resultat_voulu) {
        printf("\n** KO ** Echec du test_unbounded_int_cmp_ll: %s, %lld, %d\n", test_a, test_b, resultat_voulu);
        //free(unbo_a);
        //free(unbo_b);
        exit(1);
    }
    //printf("Resultat de la comparaison entre les 2 unbo : %d", resultat);
    printf("OK test_unbounded_int_cmp_ll: %s, %lld, %d\n", test_a, test_b, resultat_voulu);
    //free(unbo_a);
    //free(unbo_b);
}

void test_somme_2unbounded_int_positifs(const char *test_a, const char* test_b, const char* resultat){
    unbounded_int *unbo_a = malloc(sizeof(unbounded_int));
    if (unbo_a == NULL) {
        perror("\ntest_somme_2unbounded_int_positifs : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *unbo_a = string2unbounded_int(test_a);
    affiche(unbo_a);

    unbounded_int *unbo_b = malloc(sizeof(unbounded_int));
    if (unbo_b == NULL) {
        perror("\ntest_somme_2unbounded_int_positifs : La création de l'unbounded_int a échouée\n");
        exit(1);
    }

    *unbo_b = string2unbounded_int(test_b);
    affiche(unbo_b);
    
    unbounded_int *unbo_c = malloc(sizeof(unbounded_int));
    if (unbo_c == NULL) {
        perror("\ntest_somme_2unbounded_int_positifs : La création de l'unbounded_int a échouée\n");
        exit(1);
    }

    *unbo_c = unbounded_int_somme(*unbo_a,*unbo_b);
    affiche(unbo_c);
    printf("\ntaille du resultat : %ld\n",unbo_c->len);
    printf("\nsigne du resultat : %c\n",unbo_c->signe);

    char* c = unbounded_int2string(*unbo_c);
    while(*c != '\0' && *resultat != '\0'){
        if(*c != *resultat){
            printf("\n** KO ** test_somme_2unbounded_int_positifs\n");
            return;
        }
        c++;
        resultat++;
    }
    printf("\nOK test_somme_2unbounded_int_positifs\n\n");
}

void test_difference_2unbounded_int_positifs(const char *test_a, const char* test_b, const char* resultat){
    unbounded_int *unbo_a = malloc(sizeof(unbounded_int));
    if (unbo_a == NULL) {
        perror("\ntest_difference_2unbounded_int_positifs : La création de l'unbounded_int a échouée\n");
        exit(1);
    }
    *unbo_a = string2unbounded_int(test_a);
    affiche(unbo_a);

    unbounded_int *unbo_b = malloc(sizeof(unbounded_int));
    if (unbo_b == NULL) {
        perror("\ntest_difference_2unbounded_int_positifs : La création de l'unbounded_int a échouée\n");
        exit(1);
    }

    *unbo_b = string2unbounded_int(test_b);
    affiche(unbo_b);
    
    unbounded_int *unbo_c = malloc(sizeof(unbounded_int));
    if (unbo_c == NULL) {
        perror("\ntest_difference_2unbounded_int_positifs : La création de l'unbounded_int a échouée\n");
        exit(1);
    }

    *unbo_c = unbounded_int_difference(*unbo_a,*unbo_b);
    affiche(unbo_c);
    printf("\ntaille du resultat : %ld\n",unbo_c->len);
    printf("\nsigne du resultat : %c\n",unbo_c->signe);

    char* c = unbounded_int2string(*unbo_c);
    while(*c != '\0' && *resultat != '\0'){
        if(*c != *resultat){
            printf("\n** KO ** test_difference_2unbounded_int_positifs\n");
            return;
        }
        c++;
        resultat++;
    }
    printf("\nOK test_difference_2unbounded_int_positifs\n\n");
}


