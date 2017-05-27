.PHONY: help
all: pmanager child

pmanager: tree.c pmanager.c
	gcc  tree.c pmanager.c -o pmanager

child: child.c
	gcc  child.c -o child

help:
	@echo
	@echo Mattia Pavan 191421
	@echo Nicholas Grigolato 173880
	@echo Gianluca Anversa 183933
	@echo
	@echo Semplice gestore di processi.
	@echo Permette la creazione e la chiusara di processi.
	@echo
