LIBS_INCLUDE_DIR=../include
INCLUDES=-I. -I./include -I${LIBS_INCLUDE_DIR}
LIBS_DIR=
LIBS=-lglfw3 -ldl -lglfw3 -lX11 -lpthread -lXrandr -lXi

libShaders.a:
	gcc -g -o shaders.o -c shaders.c ${INCLUDES} ${LIBS_DIR} ${LIBS}  
	ar rcs libShaders.a shaders.o

clean:
	rm -f *.a *.o