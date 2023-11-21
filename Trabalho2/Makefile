LIBS_INCLUDE_DIR=
INCLUDES=-I./include
LIBS_DIR=
LIBS=-lglfw3 -ldl -lX11 -lpthread -lXrandr -lXi -lm -lfreeimage

Trabalho1:
	gcc -g -o Trabalho1 main.c ui.c glad.c clickpolygons.c geometria_dcel.c geometria.c graphics.c lista.c shaders.c rendererDCEL.c ${INCLUDES} ${LIBS_DIR} ${LIBS}  

clean:
	rm -f *.a *.o Trabalho1