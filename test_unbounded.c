#include <stdlib.h>
#include <stdio.h>

#include "unbounded_int.h"

void affiche(unbounded_int a);
void test_string2unbounded_int(const char *e, const char signe_voulu, const size_t len_voulu);

int main(void) {
    test_string2unbounded_int("1", '+', 1);
    test_string2unbounded_int("-1", '-', 1);
    test_string2unbounded_int("1234", '+', 4);
    test_string2unbounded_int("-1234", '-', 4);
    return 0;
}

void affiche(unbounded_int a) {
    printf("Voici le signe de unbounded_int : %c\n", a.signe);
    printf("Voici la taille de unbounded_int : %ld\n", a.len);
    printf("Voici le premier chiffre de unbounded_int : %c\n", a.premier->c);
    printf("Voici le dernier chiffre de unbounded_int : %c\n", a.dernier->c);
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
    printf("OK test_string2unbounded_int: %s, %c, %ld\n", test, signe_voulu, len_voulu);
    free(ubi);
}
