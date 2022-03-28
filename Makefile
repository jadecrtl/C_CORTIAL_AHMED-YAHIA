#url du lien web pour faire des makefile : https://gl.developpez.com/tutoriel/outil/makefile/
COMPILATEUR=gcc
CFLAGS=-Wall -g -pedantic

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

#cible: dependances
#	commandes

all: arithmetique/src/operation

arithmetique/src/operation: operation.o testOperation.o
	$(COMPILATEUR) -o $@ $^

testOperation.o: operation.h

%.o: %.c
	$(COMPILATEUR) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf main

integral: clean all
