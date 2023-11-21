#ifndef __RENDERER_DCEL_H
#define __RENDERER_DCEL_H
#include "geometria_dcel.h"

void DCEL_RENDERER_init();
void DCEL_RENDERER_draw();
void DCEL_RENDERER_drawPoints();
void DCEL_RENDERER_add(XDCEL_TOPOLOGY* plano);
void DCEL_RENDERER_clear();
void DCEL_RENDERER_remove(XDCEL_TOPOLOGY* plano);
void DCEL_RENDERER_update();

#endif