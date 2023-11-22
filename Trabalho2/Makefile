LIBS_INCLUDE_DIR=
INCLUDES=-I./include
LIBS_DIR=
LIBS=-lglfw3 -ldl -lX11 -lpthread -lXrandr -lXi -lm -lfreeimage

Trabalho2:
	gcc -g -o Trabalho2 main.c ui.c glad.c clickpolygons.c geometria_dcel.c geometria.c graphics.c lista.c shaders.c rendererDCEL.c circle.c ${INCLUDES} ${LIBS_DIR} ${LIBS}  

clean:
	rm -f *.a *.o Trabalho2