#ifndef API2024PARTE2_H
#define API2024PARTE2_H

#include "EstructuraGrafo24.h"
#include "APIG24.h"
u32 Greedy(Grafo G,u32* Orden); // colorea vértices de G. CORRE GREEDY.
//Input: Un grafo G y un Orden, el orden apunta a un sector de la memoria con n lugares.
//Output: Retorna el número de colores que usa. En caso de error retorna: (2**32)-1
//Complejidad -> O(m)

char GulDukat(Grafo G,u32* Orden); 
//

char ElimGarak(Grafo G,u32* Orden);


#endif