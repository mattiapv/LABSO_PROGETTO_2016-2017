.PHONY: help
.PHONY: clean
.PHONY: buildDir
.PHONY: run

CC = gcc
ASSETS = ./assets
CFLAGS = -c -Wall
BUILD = ./build
SRC = ./src
BUILDDIR = build

default: help

help:
	@echo
	@echo Mattia Pavan 191421
	@echo Nicholas Grigolato 173880
	@echo Gianluca Anversa 183933
	@echo
	@echo Semplice gestore di processi.
	@echo Permette la creazione e la chiusara di processi.
	@echo
	@echo Le regole utilizzabili sono:
	@echo compile: compila i sorgenti in un eseguibile
	@echo clean: pulisce i file temporanei generati
	@echo build: richiama compile e clean in questo ordine
	@echo test: richiama build ed esegue 10 test con vari input creando i file di output nella cartella assets
	@echo run: esegue il progetto

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
	rm -rf $(BUILD)/*.o $(ASSETS)/output.*.txt $(ASSETS)/.*~ $(ASSETS)/*~ $(BUILD)/.*~ $(BUILD)/*~ $(SRC)/.*~ $(SRC)/*~

buildDir:
	mkdir -p $(BUILDDIR)

build: buildDir compilePmanager compileChild clean

run: build
	./run.sh

test: build test01

test01:
	./build/pmanager ./build/test.txt
