LIBS_INCLUDE_DIR=../include
INCLUDES=-I. -I./include -I../Shader/include -I${LIBS_INCLUDE_DIR}
LIBS_DIR=-L../Shader
LIBS=-lShaders -lglfw3 -ldl -lglfw3 -lX11 -lpthread -lXrandr -lXi

libGraphics.a:
	gcc -g -o graphics.o -c graphics.c ${INCLUDES} ${LIBS_DIR} ${LIBS}  
	ar rcs libGraphics.a graphics.o

clean:
	rm -f *.a *.o