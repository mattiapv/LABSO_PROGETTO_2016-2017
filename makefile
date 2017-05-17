all: pmanager child

pmanager: tree.c pmanager.c
	gcc  tree.c pmanager.c -o pmanager

child: child.c
	gcc  child.c -o child
