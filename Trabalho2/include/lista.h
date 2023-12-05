#ifndef LISTA_H
#define LISTA_H

#define LISTA_VER 100

typedef enum{
    SUCCESS,
    FAIL
}enStatus;

typedef struct LISTA_DUPLA{
    struct LISTA_DUPLA* anterior;
    struct LISTA_DUPLA* proximo;
    void* item;
} XLISTA_DUPLA;

typedef struct LISTA_SIMPLES{
    struct LISTA_SIMPLES* proximo;
    void* item;
}XLISTA_SIMPLES;

typedef struct LISTA_DUPLA_IT
{
    XLISTA_DUPLA* atual;
}XLISTA_DUPLA_IT;

typedef struct LISTA_SIMPLES_IT
{
    XLISTA_SIMPLES* atual;
}XLISTA_SIMPLES_IT;

void createListaDupla(XLISTA_DUPLA* lista);
void clearListaDupla(XLISTA_DUPLA* lista);
void addListaDupla(XLISTA_DUPLA* lista, void* item);
enStatus removeListaDupla(XLISTA_DUPLA* lista, void* item);
XLISTA_DUPLA_IT getIteratorLD(XLISTA_DUPLA* lista);
void* getItemItLD(XLISTA_DUPLA_IT* it); 
void* getItemItLD_rev(XLISTA_DUPLA_IT* it);

void createListaSimples(XLISTA_SIMPLES* lista);
void clearListaSimples(XLISTA_SIMPLES* lista);
void addListaSimples(XLISTA_SIMPLES* lista, void* item);
enStatus removeListaSimples(XLISTA_SIMPLES* lista, void* item);
XLISTA_SIMPLES_IT getIteratorLS(XLISTA_SIMPLES* lista);
void* getItemItLS(XLISTA_SIMPLES_IT* it);


#endif // LISTA_H
