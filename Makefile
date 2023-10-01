Trabalho1: libRendererDCEL.a libShaders.a libGraphics.a
	$(MAKE) -C Trabalho1

libGeometria.a: libLista.a
	$(MAKE) -C Geometria

libLista.a:
	$(MAKE) -C Lista

libGeometriaDCEL.a: libLista.a libGeometria.a
	$(MAKE) -C GeometriaDCEL

libRendererDCEL.a: libLista.a libGeometria.a libGeometriaDCEL.a
	$(MAKE) -C RendererDCEL

libShaders.a:
	$(MAKE) -C Shader

libGraphics.a:
	$(MAKE) -C Graphics

clean:
	$(MAKE) clean -C Lista
	$(MAKE) clean -C Geometria
	$(MAKE) clean -C GeometriaDCEL
	$(MAKE) clean -C RendererDCEL
	$(MAKE) clean -C Shader
	$(MAKE) clean -C Graphics
	rm test_dcel -f
	