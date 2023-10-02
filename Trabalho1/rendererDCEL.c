#include <glad/glad.h>
#include "shaders.h"
#include "rendererDCEL.h"
#include "lista.h"
#include <stdlib.h>

struct RendererDCEL{
    XLISTA_SIMPLES lista_topologia;
    float vertice_info[5000];
    int index;
};

struct RendererDCEL renderer;
XSHADER shader_renderer_dcel;
int VAO, VBO;

void DCEL_RENDERER_init()
{
    createListaSimples(&(renderer.lista_topologia));
    renderer.index = 0;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    initShader("../resource/shaders/vertexShader2D.vs", "../resource/shaders/fragmentShader2D.fs", &shader_renderer_dcel);
    useShader(shader_renderer_dcel);
}

void DCEL_RENDERER_draw()
{
    glBindVertexArray(VAO);
    useShader(shader_renderer_dcel);
    glDrawArrays(GL_LINES, 0, renderer.index/5);
}

void DCEL_RENDERER_add(XDCEL_TOPOLOGY* plano)
{
    // Adiciona a topologia a lista do renderer
    addListaSimples(&(renderer.lista_topologia), (void*)plano);
    DCEL_RENDERER_update();
}

void DCEL_RENDERER_remove(XDCEL_TOPOLOGY* plano)
{
    removeListaSimples(&(renderer.lista_topologia), (void*) plano);
}

void DCEL_RENDERER_update()
{
    // Para cada face da topologia, adicionar todas as arestas a lista do renderer
    XLISTA_SIMPLES_IT it;
    it = getIteratorLS(&(renderer.lista_topologia));
    XLISTA_SIMPLES_IT it_faces;
    XDCEL_TOPOLOGY* item = (XDCEL_TOPOLOGY*)getItemItLS(&it);

    renderer.index = 0;

    XDCEL_FACE *faceAtual = NULL;
    XDCEL_HALF_EDGE *primeira = NULL, *atual = NULL;

    // Para cada topologia do renderer,
    while(item != NULL && renderer.index < 4995)
    {
        it_faces = getIteratorLS(&(item->faces));
        faceAtual = getItemItLS(&it_faces);
        // Para cada face da lista de faces,
        while(faceAtual != NULL)
        {
            primeira = faceAtual->halfedge;
            if (primeira != NULL)
            {
                renderer.vertice_info[renderer.index++] = primeira->origin->v.x;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.y;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.R;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.G;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.B;
            
                atual = primeira->next;
                while(atual != primeira && atual != NULL && renderer.index < 4995)
                {
                    renderer.vertice_info[renderer.index++] = atual->origin->v.x;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.y;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.R;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.G;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.B;


                    renderer.vertice_info[renderer.index++] = atual->origin->v.x;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.y;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.R;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.G;
                    renderer.vertice_info[renderer.index++] = atual->origin->v.B;

                    atual = atual->next;

                }
                renderer.vertice_info[renderer.index++] = primeira->origin->v.x;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.y;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.R;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.G;
                renderer.vertice_info[renderer.index++] = primeira->origin->v.B;
            }

            faceAtual = getItemItLS(&it_faces);
        }

        item = getItemItLS(&it);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*renderer.index, renderer.vertice_info, GL_STATIC_DRAW);
}

void DCEL_RENDERER_clear()
{
    clearListaSimples(&(renderer.lista_topologia));
    renderer.index = 0;
}