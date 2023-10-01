libLista.a:
	+$(MAKE) -C src
	ar rcs libLista.a src/libLista.o

testeLista: libLista.a
	gcc -g main.c -o testeLista -I./include -L. -lLista

clean:
	rm -f src/*.o *.a