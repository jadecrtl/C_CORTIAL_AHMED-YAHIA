#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unbounded_int.h"

void affiche(unbounded_int unbo);
void test_string2unbounded_int(const char *e, const char signe_voulu, const size_t len_voulu);
void test_ll2unbounded_int(long long test, const char *resultat_attendu);
void test_unbounded_int2string(const char *c);
void test_unbounded_int_cmp_unbounded_int(const char *test_a, const char *test_b, const int resultat_voulu);
void test_unbounded_int_cmp_ll(const char *test_a, long long test_b, const int resultat_voulu);
void test_somme(const char *test_a, const char* test_b, const char* resultat);
void test_difference(const char *test_a, const char* test_b, const char* resultat);
void test_somme(const char *test_a, const char* test_b, const char* resultat);
void test_difference(const char *test_a, const char* test_b, const char* resultat);
void test_produit(const char *test_a, const char* test_b, const char* resultat);

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
    test_ll2unbounded_int(123456789, "123456789");
    test_ll2unbounded_int(-123456789, "-123456789");
    test_ll2unbounded_int(1234567898765432, "1234567898765432");

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

    printf("\n\nTest somme : \n\n");

    test_somme("123", "456", "579");
    test_somme("1234", "456", "1690");
    test_somme("123", "4567", "4690");
    test_somme("-678", "7865", "7187");
    test_somme("999", "-667", "332");
    test_somme("-123", "-10", "-133");
    test_somme("-678", "-786", "-1464");
    test_somme("999", "-6673", "-5674");
    test_somme("-123", "-10000", "-10123");

    printf("\n\nTest difference : \n\n");

    test_difference("500", "456", "44");
    test_difference("1234", "456", "778");
    test_difference("1234", "1256", "-22");
    test_difference("6", "96", "-90");
    test_difference("-500", "-46", "-454");
    test_difference("-124", "456", "-580");
    test_difference("600", "-106", "706");
    test_difference("-600", "200", "-800");
    test_difference("-600", "-600", "0");
    test_difference("10000000", "17", "9999983");
    test_difference("34", "-456", "490");

    printf("\n\nTest produit : \n\n");

    test_produit("32","10","320");
    test_produit("12","5","60");
    test_produit("-6","-30","180");
    test_produit("-32","10","-320");
    test_produit("32","-10","-320");
    test_produit("1","1","1");
    test_produit("5","5","25");
    test_produit("565","498","281370");
    test_produit("565","98","55370");
    test_produit("565","980","553700");
    test_produit("5","350","1750");
    test_produit("5605","-9884","-55399820");
    test_produit("-5","-350","1750");
    test_produit("5","3050","15250");
    test_produit("0","350","0");
    test_produit("5","6","30");
    test_produit("6","5","30");
    test_produit("6","500","3000");
    test_produit("60","5","300");
    test_produit("500","6","3000");

    printf("\n\n******* TEST OK ********\n\n");
    return 0;
}

void affiche(unbounded_int unbo) {
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
    free(chi);
}

void test_string2unbounded_int(const char *test, const char signe_voulu, const size_t len_voulu) {
    unbounded_int ubi = string2unbounded_int(test);
    affiche(ubi);
    if (ubi.signe != signe_voulu) {
        printf("\n** MAUVAIS RESULTAT ** test_string2unbounded_int\n");
        exit(1);
    }
    if (*test == '-' || *test == '+') {
        test++;
    }
    while (*test != '\0' && ubi.premier != NULL) {
        if(*test != ubi.premier->c){
            printf("\n** MAUVAIS RESULTAT ** test_string2unbounded_int\n");
            exit(1);
        }
        test++;
        ubi.premier = ubi.premier->suivant;
    }
    printf("\nOK test_string2unbounded_int: %s, %c, %ld\n\n", test, signe_voulu, len_voulu);
}

void test_ll2unbounded_int(long long test, const char *resultat_attendu) {
    unbounded_int ubi = ll2unbounded_int(test);
    affiche(ubi);
    char *resultat = unbounded_int2string(ubi);
    if (strcmp(resultat, resultat_attendu) != 0) {
        printf("\n** PAS IDENTIQUE ** Echec du test_ll2unbounded_int: %s, %s\n", resultat, resultat_attendu);
        exit(1);

    }
    printf("\nOK test_ll2unbounded_int <%lld>\n\n",test);
}

void test_unbounded_int2string(const char *resultat) {
    unbounded_int u = string2unbounded_int(resultat);
    char *res = unbounded_int2string(u);
    puts(res); //puts(e) écrit la chaine de caractère du tableau de char e
    puts(resultat);
    while(*res != '\0' && *resultat != '\0'){
        if(*res != *resultat){
            printf("\n** MAUVAIS RESULTAT ** test_unbounded_int2string\n");
            exit(1);
        }
        res++;
        resultat++;
    }
    printf("\nOK test_unbounded_int2string\n\n");
}

void test_unbounded_int_cmp_unbounded_int(const char *test_a, const char *test_b, const int resultat_voulu) {
    unbounded_int unbo_a = string2unbounded_int(test_a);
    affiche(unbo_a);

    unbounded_int unbo_b = string2unbounded_int(test_b);
    affiche(unbo_b);

    int resultat = unbounded_int_cmp_unbounded_int(unbo_a, unbo_b);
    if (resultat != resultat_voulu) {
        printf("\n** PAS IDENTIQUE ** Echec du test_unbounded_int_cmp_unbounded_int: %s, %s, %d\n\n", test_a, test_b, resultat_voulu);
        exit(1);
    }
    if (resultat == 1) {
        printf("OK test_unbounded_int_cmp_unbounded_int: %s > %s car resultat = %d\n\n", test_a, test_b, resultat_voulu);
    }
    if (resultat == 0) {
        printf("OK test_unbounded_int_cmp_unbounded_int: %s = %s car resultat = %d\n\n", test_a, test_b, resultat_voulu);
    }
    if (resultat == -1) {
        printf("OK test_unbounded_int_cmp_unbounded_int: %s < %s car resultat = %d\n\n", test_a, test_b, resultat_voulu);
    }
}

void test_unbounded_int_cmp_ll(const char *test_a, long long test_b, const int resultat_voulu) {
    unbounded_int unbo_a = string2unbounded_int(test_a);
    unbounded_int unbo_b = ll2unbounded_int(test_b);
    int resultat = unbounded_int_cmp_unbounded_int(unbo_a, unbo_b);
    if (resultat != resultat_voulu) {
        printf("\n** PAS IDENTIQUE ** Echec du test_unbounded_int_cmp_ll: %s, %lld, %d\n", test_a, test_b, resultat_voulu);
        exit(1);
    }
    if (resultat == 1) {
        printf("OK test_unbounded_int_ll: %s > %lld car resultat = %d\n\n", test_a, test_b, resultat_voulu);
    }
    if (resultat == 0) {
        printf("OK test_unbounded_int_ll: %s = %lld car resultat = %d\n\n", test_a, test_b, resultat_voulu);
    }
    if (resultat == -1) {
        printf("OK test_unbounded_int_ll: %s < %lld car resultat = %d\n\n", test_a, test_b, resultat_voulu);
    }
}

void test_somme(const char *test_a, const char* test_b, const char* resultat){
    unbounded_int unbo_a = string2unbounded_int(test_a);
    printf("Premier unbounded_int : ");
    affiche(unbo_a);

    unbounded_int unbo_b = string2unbounded_int(test_b);
    printf("Second unbounded_int : ");
    affiche(unbo_b);
    
    unbounded_int unbo_c = unbounded_int_somme(unbo_a,unbo_b);
    printf("resultat de la somme : ");
    affiche(unbo_c);

    char* c = unbounded_int2string(unbo_c);
    while(*c != '\0' && *resultat != '\0'){
        if(*c != *resultat){
            printf("\n** FAUX ** test_somme\n");
            exit(1);
        }
        c++;
        resultat++;
    }
    printf("\nOK test_somme\n\n");
}

void test_difference(const char *test_a, const char* test_b, const char* resultat){
    unbounded_int unbo_a = string2unbounded_int(test_a);
    printf("Premier unbounded_int : ");
    affiche(unbo_a);

    unbounded_int unbo_b = string2unbounded_int(test_b);
    printf("Second unbounded_int : ");
    affiche(unbo_b);
    
    unbounded_int unbo_c = unbounded_int_difference(unbo_a,unbo_b);
    printf("resultat de la difference : ");
    affiche(unbo_c);

    char* c = unbounded_int2string(unbo_c);
    while(*c != '\0' && *resultat != '\0'){
        if(*c != *resultat){
            printf("\n** FAUX ** test_difference\n");
            exit(1);
        }
        c++;
        resultat++;
    }
    printf("\nOK test_difference\n\n");
}

void test_produit(const char *test_a, const char* test_b, const char* resultat){
    unbounded_int unbo_a = string2unbounded_int(test_a);
    printf("Premier unbounded_int : ");
    affiche(unbo_a);

    unbounded_int unbo_b = string2unbounded_int(test_b);
    printf("Second unbounded_int : ");
    affiche(unbo_b);
    
    unbounded_int unbo_c = unbounded_int_produit(unbo_a,unbo_b);
    printf("resultat du produit : ");
    affiche(unbo_c);

    char* c = unbounded_int2string(unbo_c);
    while(*c != '\0' && *resultat != '\0'){
        if(*c != *resultat){
            printf("\n** FAUX ** test_produit\n");
            exit(1);
        }
        c++;
        resultat++;
    }
    printf("\nOK test__produit\n\n");
}


