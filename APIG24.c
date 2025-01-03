#include "APIG24.h"
#include <stdbool.h>

/* Añade el vecino a el arreglo de vecinos del vertices, actualiza el grado del vertice y cambia el Delta del grafo si es necesario*/
static void AñadirVecinoAVertice(Grafo G, u32 vertice, u32 vecino) {
    G->vertices_array[vertice].vecinos[G->vertices_array[vertice].grado] = vecino;
    G->vertices_array[vertice].grado++;
    if (G->delta<G->vertices_array[vertice].grado) {
        G->delta = G->vertices_array[vertice].grado;
    }
    if (G->vertices_array[vertice].grado>=10) {
        G->vertices_array[vertice].vecinos = realloc(G->vertices_array[vertice].vecinos, (G->vertices_array[vertice].grado+1)*sizeof(u32));
        if (G->vertices_array[vertice].vecinos == NULL) {
            DestruirGrafo(G);
        }
    }
}

Grafo ConstruirGrafo() {
    u32 resultado;
    Grafo G = malloc(sizeof(GrafoSt));
    if (G==NULL) {
        return NULL;
    }
    while (true) {
        resultado = scanf(" p edge %u %u ", &G->vertices, &G->lados);
        if (resultado==2) {
            break;
        } else {
            scanf("%*c");
        }
    }
    G->delta = 0;
    G->vertices_array = calloc(sizeof(struct Vertice), G->vertices);
    if (G->vertices_array==NULL) {
        free(G);
        return NULL;
    }
    for (u32 i=0; i<G->vertices; i++) {
        
        G->vertices_array[i].vecinos = calloc(sizeof(u32), 100);
        if (G->vertices_array[i].vecinos==NULL) {
            DestruirGrafo(G);
            return NULL;
        }
        G->vertices_array[i].grado = 0;
        G->vertices_array[i].color = 0;
    }
    u32 v,w, grado;
    for (u32 i=0; i<G->lados; i++) {
        resultado = scanf(" e %u %u ", &v, &w);
        if (resultado!=2) {
            DestruirGrafo(G);
            return NULL;
        }
        AñadirVecinoAVertice(G,v,w);
        if (G==NULL) {
            return NULL;
        }
        AñadirVecinoAVertice(G,w,v);
        if (G==NULL) {
            return NULL;
        }
    }
    for (u32 i=0; i<G->vertices; i++) {
        G->vertices_array[i].vecinos = realloc(G->vertices_array[i].vecinos, G->vertices_array[i].grado*sizeof(u32));
        if (G->vertices_array[i].vecinos == NULL) {
            DestruirGrafo(G);
            return NULL;
        }
    }
    return G;
};

void DestruirGrafo(Grafo G) {
    for (u32 i=0; i<G->vertices; i++) {
        free(G->vertices_array[i].vecinos);
    }
    free(G->vertices_array);
    free(G);
};


u32 NumeroDeVertices(Grafo G) {
    return G->vertices;
};

u32 NumeroDeLados(Grafo G) {
    return G->lados;
};

u32 Delta(Grafo G) {
    return G->delta;
};


u32 Grado(u32 i, Grafo G) {
    if (i>=G->vertices) {
        return 0;
    }
    return G->vertices_array[i].grado;
};

color Color(u32 i, Grafo G) {
    if (i>=G->vertices) {
        return __UINT32_MAX__;
    }
    return G->vertices_array[i].color;
};

u32 Vecino(u32 j, u32 i, Grafo G) {
    if (i>=G->vertices || j>=G->vertices_array[i].grado) {
        return __UINT32_MAX__;
    }
    return G->vertices_array[i].vecinos[j];
}


void AsignarColor(color x,u32 i,Grafo  G) {
    if (i>=G->vertices) {
        return;
    }
    G->vertices_array[i].color = x;
}
void ExtraerColores(Grafo G,color* Color) {
    for (u32 i=0; i<G->vertices; i++) {
        Color[i] = G->vertices_array[i].color;
    }
}
void ImportarColores(color* Color,Grafo  G) {
    for (u32 i=0; i<G->vertices; i++) {
        G->vertices_array[i].color = Color[i];
    }
}
