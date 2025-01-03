#include "API2024Parte2.h"
#include "APIG24.h"
#include "EstructuraGrafo24.h"
#include <time.h>
#include <string.h>
#include <limits.h>
#define TIMES 5

static void Liberar(u32 **a, int size){
    for (int i = 0; i < size; ++i) {
        
        if(a[i] != NULL) free(a[i]);
    
    }
}

static void FreeNotUse(u32 **a, int size, int save){
    for (int i = 0; i < size; i++)
    {
        if(i != save){
            free(a[i]);
            a[i] = NULL;
        }
    }
    
}
//-----------------------------------------------//


Grafo glGraph = NULL; // Grafo global

int cmp(const void *_v, const void *_w){
    u32 v = * (u32*) _v;
    u32 w = * (u32*) _w;
    Grafo g = glGraph;
    return ((Grado(v,g) < Grado(w,g)) - (Grado(v,g) > Grado(w,g)));
}


// ------------------------------------------------------- //

static void run(Grafo G, u32* ordenes[], char* name[] , int size){
    u32 ColoresMinimos[size];
    for (int i = 0; i < size; i++)
    {
        ColoresMinimos[i] = UINT_MAX;
    }
    
    u32** ColoresFilanes = malloc(size*sizeof(u32*));
    for (int i = 0; i < size; ++i)
    {
        ColoresFilanes[i] = calloc(NumeroDeVertices(G),sizeof(u32));
        u32* ord = ordenes[i];
        u32 colores = Greedy(G, ord); //correr Greedy normal
        
        if(colores == UINT_MAX){
            printf("El orden %s no es biyeccion con el Grafo G.\n",name[i]);
            continue; //Orden invalido.
        } 
        
        printf("Cantidad de colores:%u\n", colores);
        if(colores < ColoresMinimos[i]) ColoresMinimos[i] = colores;
        for (int j = 1; j <= TIMES*10; j++)
        {
            GulDukat(G, ord);
            colores = Greedy(G, ord);
            if(colores < ColoresMinimos[i]) 
            {   
                ColoresMinimos[i] = colores;
                ExtraerColores(G,ColoresFilanes[i]);
            }    
            ElimGarak(G, ord);
            colores = Greedy(G, ord);
            if(colores < ColoresMinimos[i]) 
            {   
                ColoresMinimos[i] = colores;
                ExtraerColores(G,ColoresFilanes[i]);
            }  
            printf("Orden %s : Cantidad de colores despues de iteracion (%d) es:%u\n",name[i],j,colores);
        }
    }
    u32 max_index=0 , max_value= ColoresMinimos[0];
    for (int i = 1; i < size; ++i)
    {
        if(max_value > ColoresMinimos[i])
        {
            max_value = ColoresMinimos[i];
            max_index = i;
            ImportarColores(ColoresFilanes[max_index],G);
        }
    }
    FreeNotUse(ordenes,size,max_index); // Libero la memoria de los Ordenes q no voy a utilizar.
    u32 colores;
    srand(time(NULL));
    int iGulD = 0 , iElimG = 0;
    for (int i = 1; i <= TIMES*100; i++)
    {
        if(rand()%2){
            GulDukat(G, ordenes[max_index]);
            colores = Greedy(G, ordenes[max_index]);
            iGulD++;
            printf("Orden %s :Cantidad de colores despues de GulDukat iteracion (%d) es:%u\n",name[max_index],iGulD,colores);
        }
        else{
            ElimGarak(G, ordenes[max_index]);
            colores = Greedy(G, ordenes[max_index]);
            iElimG++;
            printf("Orden %s :Cantidad de colores despues de ElimGarak iteracion (%d) es:%u\n",name[max_index],iElimG,colores);
        }
        
    }
    Liberar(ColoresFilanes,size);
    free(ColoresFilanes);
}
//----------------------------------------------------------------------------------//

int main(void){
    Grafo G = ConstruirGrafo();
    u32 n = NumeroDeVertices(G);
    u32 * orden_1 = malloc(n * (sizeof(u32)));
    u32 * orden_2 = malloc(n * (sizeof(u32)));
    u32 * orden_3 = malloc(n * (sizeof(u32)));
    u32 * orden_4 = malloc(n * (sizeof(u32)));
    u32 * orden_5 = malloc(n * (sizeof(u32)));
    u32 k = (n%2) ? n/2 : (n/2)-1;
    u32 j = (n%2) ? (n/2)+1 : n/2;  
    //----------------------------------------//
    char* Nombres[5] = {"Natural","Inverso","ParesImpares","GradosDec","GradosCrec"};
    for (u32 i = 0; i < n; i++){
        orden_1[i] = i; // Creacion Orden natural
        orden_2[i] = n-i-1; // Creacion Orden Inverso
        orden_4[i] = i;
        if(i % 2){ // Creacion Orden ParesImpares
            orden_3[j]=i;
            j++;
        }
        else{
            orden_3[k]=i;
            k--;
        }
    }
    glGraph = G; // seteo global Graph para la fucnion cmp
    qsort(orden_4,n,sizeof(u32),cmp); // Creacion GradosDec
    for (u32 i = 0; i < n; i++)
    {
        orden_5[i] = orden_4[n-i-1]; // Creacion Grados Crec
    }
    
    // Agrego los 5 ordenes en un array q paso como argumento en run.

    u32* Ordenes[5] = {orden_1,orden_2,orden_3,orden_4,orden_5};
    run(G, Ordenes, Nombres, 5);
    Liberar(Ordenes,5);
    DestruirGrafo(G);
    return 0;
}
