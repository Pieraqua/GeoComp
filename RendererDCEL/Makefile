INCLUDES=-I. -I../include -I../Lista/include -I../GeometriaDCEL/include -I../Geometria -I./include -I../Shader/include
LIBS_DIR=-L../Lista -L../GeometriaDCEL  -L../Geometria -L../Shader
LIBS=-lLista -lGeometriaDCEL -lGeometria -lShaders -lglfw3 -ldl -lglfw3 -lX11 -lpthread -lXrandr -lXi

libRendererDCEL.a:
	gcc -g -o rendererDCEL.o -c rendererDCEL.c ${INCLUDES} ${LIBS_DIR} ${LIBS}  
	ar rcs libRendererDCEL.a rendererDCEL.o

clean:
	rm -f *.a *.o