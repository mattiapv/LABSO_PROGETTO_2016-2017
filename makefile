all: progettosistemi child

progettosistemi: tree.c progettosistemi.c 
	gcc  tree.c progettosistemi.c -o progettosistemi

child: child.c 
	gcc  child.c -o child
