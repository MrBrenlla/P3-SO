all : lista array
	gcc -o shell shell.c list.c arraylist.c -Wall

lista: list.c
	gcc -c -o list list.c -Wall

array: arraylist.c
	gcc -c -o arraylist arraylist.c -Wall
