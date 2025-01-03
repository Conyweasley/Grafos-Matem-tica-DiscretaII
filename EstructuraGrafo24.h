#ifndef ESTRUCTURAGRAFO24_H
#define ESTRUCTURAGRAFO24_H

typedef unsigned int u32;

typedef u32 color;

struct Vertice{
    u32 grado;
    u32 *vecinos;
    color color;
};

typedef struct _GrafoSt {
    u32 vertices;
    u32 lados;
    struct Vertice *vertices_array;
    u32 delta;
} GrafoSt;

#endif