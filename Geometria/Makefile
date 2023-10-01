libGeometria.a:
	gcc -g -o geometria.o -c geometria.c -I. -I../Lista/include -L../Lista -lLista
	ar rcs libGeometria.a geometria.o

testeGeometria:
	gcc -g -o testeGeometria -c main.c -I. -L. -L../Lista -lLista -lGeometria

clean:
	rm -f *.a *.o