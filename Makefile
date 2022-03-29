#url du lien web pour faire des makefile : https://gl.developpez.com/tutoriel/outil/makefile/
COMPILATEUR=gcc
CFLAGS=-Wall -g -pedantic

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

all : unbounded_int test_unbounded

test_unbounded.o: unbounded_int.h

unbounded_int: unbounded_int.c
	$(COMPILATEUR) $(CFLAGS) $< -o $@

test_unbounded: test_unbounded.c
	$(COMPILATEUR) $(CFLAGS) $< -o $@

clean :
	rm -f *.o
	rm -f test_unbounded
	rm -f unbounded_int

go :
	./unbounded_int
	./test_unbounded