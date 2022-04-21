#url du lien web pour faire des makefile : https://gl.developpez.com/tutoriel/outil/makefile/
COMPILATEUR=gcc
CFLAGS=-Wall -g -pedantic
EXEC=test_unbounded calc_unbounded_int test_non_regression

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

#Makefile permet également de créer des règles génériques (par exemple construire un .o à partir d'un .c) qui se verront appelées par défaut.
#Une telle règle se présente sous la forme suivante : 
#%.o: %.c
#	commandes

#cible: dependance
#	commandes

all : $(EXEC)

calc_unbounded_int: calc_unbounded_int.o unbounded_int.o
	$(COMPILATEUR) -o $@ $^

test_unbounded: unbounded_int.o test_unbounded.o
	$(COMPILATEUR) -o $@ $^

test_non_regression: test_non_regression.o
	$(COMPILATEUR) -o $@ $^

%.o: %.c
	$(COMPILATEUR) $(CFLAGS) -o $@ -c $< 

clean :
	rm -f *.o
	rm -f test_unbounded
	rm -f calc_unbounded_int
	rm -f test_non_regression