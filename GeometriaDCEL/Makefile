libGeometriaDCEL.a:
	gcc -g -o libGeometriaDCEL.a -c src/geometria_dcel.c -Iinclude -I../Lista/include -I../Geometria -L../Lista -L../Geometria -lGeometria -lLista

test_dcel: libGeometriaDCEL.a
	gcc -g -o test_dcel main.c -Iinclude -I../Lista/include -I../Geometria -L. -L../Lista -L../Geometria -lGeometria -lLista -lGeometriaDCEL

clean:
	rm -f *.o *.a