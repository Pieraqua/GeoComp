#ifndef RENDERER_H
#define RENDERER_H
#include "geometria.h"

/** @brief Renderiza os poligonos incluidos */
void render();

/** @brief Limpa a lista de poligonos */
void clear();

/** @brief Adiciona um poligono a lista */
int addPolygon(XPOLIGONO poligono);

/** @brief Remove um poligono da lista */
int removePolygon(XPOLIGONO poligono);

#endif // RENDERER_H
