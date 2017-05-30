.PHONY: help
.PHONY: cleanO clean
.PHONY: buildDir

CC = gcc
ASSETS = ./assets
CFLAGS = -c -Wall
BUILD = ./build
SRC = ./src
BUILDDIR = build

default: help

help:
	@echo
	@echo Nicholas Grigolato 173880
	@echo Gianluca Anversa 183933
	@echo Mattia Pavan 191421
	@echo
	@echo Semplice gestore di processi.
	@echo Permette la creazione e la chiusara di processi.
	@echo
	@echo Le regole utilizzabili sono:
	@echo build: richiama prima clean e compila i sorgenti in un eseguibile
	@echo clean: pulisce i file temporanei generati e elimina la cartella build
	@echo test: richiama build ed esegue un test con vari input.
	@echo run: richiama prima build ed esegue il progetto
	@echo

compilePmanager: pmanager.o tree.o
	$(CC) $(BUILD)/pmanager.o $(BUILD)/tree.o -o $(BUILD)/pmanager

compileChild: child.o
	$(CC) $(BUILD)/child.o -o $(BUILD)/child

pmanager.o: src/pmanager.c
	$(CC) $(CFLAGS) $(SRC)/pmanager.c -o $(BUILD)/pmanager.o

tree.o: src/tree.c
	$(CC) $(CFLAGS) -o $(BUILD)/tree.o $(SRC)/tree.c

child.o: src/child.c
	$(CC) $(CFLAGS) -o $(BUILD)/child.o $(SRC)/child.c

clean:
	rm -rf $(BUILD)

cleanO:
	rm -rf $(BUILD)/*.o

buildDir:
	mkdir -p $(BUILDDIR)

build: clean buildDir compilePmanager compileChild cleanO

chmod:
	chmod +x run.sh runTest.sh

test: build chmod
	./runTest.sh

run: build chmod
	./run.sh
