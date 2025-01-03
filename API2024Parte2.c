#include <stdlib.h>
#include <stdbool.h>
#include "API2024Parte2.h"
#include "APIG24.h"
#include "EstructuraGrafo24.h"
#include <string.h>
#include <limits.h>
#include <assert.h>

#define True 1u
#define False 0u

static char EsBiyeccion(Grafo G, u32* Ord){
    //Verificación del Orden: Verificamos que ORDEN induce una biyección.
    u32 n = NumeroDeVertices(G);
    u32 * check = calloc(n, sizeof(u32));
    for (u32 i = 0; i < n; i++){
        if (Ord[i] >= n){
            free(check);
            return '1';
        }
        check[Ord[i]]++;
        if (check[Ord[i]] > 1u){
            free(check);
            return '1';
        }
    }
    free(check);
    return '0';
}

static bool EsPropio(Grafo G){
    for (u32 vertice = 0u; vertice < NumeroDeVertices(G); ++vertice)
    {
        for (u32 v = 0u; v < Grado(vertice,G); ++v)
        {
            if(Color(vertice,G) == Color(Vecino(v,vertice,G),G)) {
                return False;
            }
        }

    }
    return True;
}

u32 Greedy(Grafo G,u32* Orden){ // colorea vértices de G. CORRE GREEDY.
//Input: Un grafo G y un Orden, el orden apunta a un sector de la memoria con n lugares.
//Output: Retorna el número de colores que usa. En caso de error retorna: (2**32)-1
//Complejidad -> O(m)

    if(EsBiyeccion(G,Orden) == '1') return UINT_MAX;

// Segunda parte: Algoritmo Greedy

    u32 n = NumeroDeVertices(G);
    u32 * colores = calloc(n, sizeof(u32));
    u32 * colores_aux = malloc((Delta(G) + 2) * sizeof(u32)); // array usado para guardar cuantas veces se usa un color
    AsignarColor(1, Orden[0], G);
    colores[Orden[0]] = 1;
    u32 max_color = 1;
    u32 vertice;
    for (unsigned int i = 1; i < n; i++){ //Itero sobre los vértices
        vertice = Orden[i];
        memset(colores_aux, 0x00, sizeof(u32) * (Grado(vertice, G)+2)); //Seteo los valores en 0 para cada vértice

        for(unsigned int j = 0; j < Grado(vertice, G); j++){ //Itero sobre los vecinos de cada vértice
            u32 vecino = Vecino(j, vertice, G);
            colores_aux[colores[vecino]]++; //Pongo en colores_aux el color del vecino j del vértice i;
        }
        for (unsigned int j = 1; j <= Grado(vertice, G) + 1; j++){
            if (colores_aux[j] == 0){
                colores[vertice] = j;
                AsignarColor(j, vertice, G);
                if (j > max_color){
                    max_color = j;
                }
                break;
            }
        }
    }
    free(colores_aux);
    free(colores);
    //assert(EsPropio(G)); // Dejamos comentado ya que para grafos grandes chequear aumenta mucho el tiempo (pero funciona)
    return max_color;
}

Grafo global_G;
u32* global_M;
u32* global_m;
u32* global_counts;

int cmp_div4(const void *_v, const void *_w) {
    u32 v = * (u32*) _v; //de acuerdo a dos punteros *v y *w extraigo los valores de v y w
    u32 w = * (u32*) _w;

    Grafo G = global_G;
    u32* M = global_M;

    long long value_v = M[Color(v, G)]; //extraigo los M de v y w
    long long value_w = M[Color(w, G)];

    if (value_v != value_w){
        return (value_v < value_w) - (value_v > value_w); //mejora de eficiencia (compara mayor y menor con respecto a M)   
    }
    else{
        value_v = Color(v,G);
        value_w = Color(w, G);
        return (value_v > value_w) - (value_v < value_w);
    }
}

int cmp_div2(const void *_v, const void *_w) {
    u32 v = * (u32*) _v;
    u32 w = * (u32*) _w;

    Grafo G = global_G;
    u32* M = global_M;
    u32* m = global_m;

    long long value_v = M[Color(v, G)] + m[Color(v, G)];
    long long value_w = M[Color(w, G)] + m[Color(w, G)];

    if (value_v != value_w){
        return (value_v < value_w) - (value_v > value_w); //mejora de eficiencia (compara mayor y menor con respecto a M)   
    }
    else{
        value_v = Color(v,G);
        value_w = Color(w, G);
        return (value_v > value_w) - (value_v < value_w);
    }
}

int cmp_impares(const void *_v, const void *_w) {
    u32 v = * (u32*) _v;
    u32 w = * (u32*) _w;

    Grafo G = global_G;
    u32* m = global_m;

    long long value_v = m[Color(v, G)];
    long long value_w = m[Color(w, G)];

    if (value_v != value_w){
        return (value_v < value_w) - (value_v > value_w); //mejora de eficiencia (compara mayor y menor con respecto a M)   
    }
    else{
        value_v = Color(v,G);
        value_w = Color(w, G);
        return (value_v > value_w) - (value_v < value_w);
    }
}

int cmp_count(const void *_v, const void *_w){
    u32 v = * (u32*) _v;
    u32 w = * (u32*) _w;

    Grafo G = global_G;
    u32* counts = global_counts;
    long long value_v = counts[Color(v, G)];
    long long value_w = counts[Color(w, G)];

    if (value_v != value_w){
        return (value_v > value_w) - (value_v < value_w);
    }
    else{
        value_v = Color(v, G);
        value_w = Color(w, G);
        return (value_v > value_w) - (value_v < value_w);
    }
}

char GulDukat(Grafo G,u32* Orden){
    u32 chi = Delta(G) + 1;
    u32 *m = malloc((chi+1) * sizeof(u32));
    u32 *M = malloc((chi+1) * sizeof(u32));
    for(u32 i = 0; i <= chi; i++) m[i] = Delta(G); //seteo
    for(u32 i = 0; i <= chi; i++) M[i] = 0;

    for (u32 vertice = 0; vertice < NumeroDeVertices(G); vertice++) {
        u32 x = Color(vertice, G);
        m[x] = (m[x] <= Grado(vertice, G)) ? m[x] : Grado(vertice, G); //calculo m para cada vértice
        M[x] = (M[x] >= Grado(vertice, G)) ? M[x] : Grado(vertice, G); //calculo M para cada vértice
    }

    u32 * div_4 = malloc(NumeroDeVertices(G)* sizeof(u32)); //subarrays
    u32 * div_2 = malloc(NumeroDeVertices(G)* sizeof(u32));
    u32 * impares = malloc(NumeroDeVertices(G)* sizeof(u32));
    u32 div4_size = 0u;
    u32 div2_size = 0u;
    u32 impares_size = 0u;
    for (unsigned int i = 0u; i < NumeroDeVertices(G); i++){
        u32 vertice = i;
        if (Color(vertice, G) % 4 == 0){
            div_4[div4_size] = vertice;
            div4_size++; 
        }
        else if (Color(vertice, G) % 2 == 0){
            div_2[div2_size] = vertice;
            div2_size++;
        }
        else{
            impares[impares_size] = vertice;
            impares_size++;
        }
        //recorro todo el array orden, tengo un array por cada condicion, dependiendo si cumple o no lo meto en ese array. Luego pego todo.
    }

    global_G = G;
    global_M = M;
    global_m = m;

    qsort(div_4, div4_size, sizeof(u32), cmp_div4); //qsort ordena de acuerdo a un criterio de comparación
    qsort(div_2, div2_size, sizeof(u32), cmp_div2);
    qsort(impares, impares_size, sizeof(u32), cmp_impares);

    u32 orden_size = 0;
    for (u32 i = 0; i < div4_size; i++) {
        Orden[orden_size] = div_4[i];
        orden_size++;
    }
    for (u32 i = 0; i < div2_size; i++) {
        Orden[orden_size] = div_2[i];
        orden_size++;
    }
    for (u32 i = 0; i < impares_size; i++) {
        Orden[orden_size] = impares[i];
        orden_size++;
    }

    free(div_2);
    free(div_4);
    free(impares);
    free(M);
    free(m);

    return EsBiyeccion(G,Orden);
}

char ElimGarak(Grafo G,u32* Orden){
    u32 * counts = calloc(Delta(G)+1 ,sizeof(u32));

    counts[0] = UINT_MAX;
    counts[1] = UINT_MAX - 1u;
    for (u32 i = 0; i < NumeroDeVertices(G); i++){
        if(Color(i, G) > 2u){
            counts[Color(i, G) - 1u]++;
        }
    }

    global_counts = counts;
    global_G = G;
    qsort(Orden, NumeroDeVertices(G), sizeof(u32), cmp_count); //qsort ordena de acuerdo a un criterio de comparación
    
    free(counts);

    return EsBiyeccion(G,Orden);
}
