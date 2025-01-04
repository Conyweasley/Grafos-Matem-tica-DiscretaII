# Graph-Algorithms-in-C

El objetivo de este proyecto es hacer un programa que coloree un grafo tratando de obtener un coloreo con la menor cantidad de colores posibles siguiendo la teoria de grafos.
El proyecto se dividirá en dos etapas. En la primera simplemente especificamos funciones que permitan leer los datos del grafo
que vamos a colorear, guardarlos en alguna estructura adecuada, y otras funciones que nos permitan acceder a esos
datos, como el número de vertices y lados, los grados de cada vertice, y cuales son los vecinos de un vértice. En la segunda etapa se realiza el respectivo coloreo de grafos usando Greedy corriendo con distintos ordenes. Para estos ordenes se crean distintas funciones (GulDukat y ElimGarak) que ordenan los vértices siguiendo distintas propiedades.

### Testeo
Deben testear la funcionalidad de cada una de las funciones que programan. Hay una lista de grafos de ejemplos para testear la funciones sobre ellos.

# Primera Parte
## Estructura del Grafo
GrafoSt es la estructura que contiene toda la información sobre el grafo necesaria para correr las funciones pedidas. 
Contiene:
- Número de vértices y lados del grafo
- Grados y colores de todos los vértices
- Delta del grafo (el mayor grado)
- Quienes son los vecinos de cada vértice

Los grafos que se carguen serán no dirigidos.

Se utiliza el tipo de dato u32 para especificar un entero de 32 bits sin signo.

## Funciones de Construcción/Destrucción
**Grafo ConstruirGrafo();**

 La función aloca memoria, inicializa lo que haya que inicializar de una estructura GrafoSt, lee un grafo desde
standard input en el formato indicado en la última sección, lo carga en la estructura, colorea todos los vertices
con el color 0, y devuelve un puntero a la estructura. 

**Restricción de complejidad**: La función es O(m). esta función NO PUEDE ser O(n^2) (y MENOS puede ser O(mn)) pues en los grafos de
testeo habrá grafos con millones de vértices, y un grafo asi con un algoritmo O(n2 ) no terminará en un tiempo razonable.
En cuanto a m, puede estar en el orden de millones tambien, y puede ser m = O(n2 ), pero sólo para n del orden de miles, mientras que cuando n sea del orden de millones, m no será O(n2 ) sino O(n), pues como dijimos arriba esta función no puede tener complejidad menor a O(m) y un m de pej miles de millones haria que demorara mucho.

**DestruirGrafo();** 

Destruye G y libera la memoria alocada. Esta función tambien tiene una complejidad razonable, no hay razón para que sea mayor a O(m) e incluso puede ser menor, pero O(m) es aceptable.

## Funciones para extraer información
Las funciones detalladas en esta sección y las que siguen deben ser todas O(1):
* NúmeroDeVertices
* NúmeroDeLados
* Delta
* Grado
* Color
* Vecino
* AsignarColor
* ExtraerColores
* ImportarColores

Que hace cada una de estas funciones está especificado en APIG24.h
# Segunda Parte
Estas funciones están pensadas para correr con cualquier implementacion de la primera parte que cumpla las especificaciones. Por lo tanto las funciones definidas aca usan las funciones definidas en la parte 1 PERO NO la estructura interna del grafo.

## Greedy: Coloreo

_u32 Greedy(Grafo G,u32* Orden);_

Esta función asume que Orden apunta a un sector de memoria con n lugares. 
Pero, lo primero que hace esta función es verificar que Orden provee efectivamente un orden de los elementos
{0, 1, ..., n − 1}. Es decir, que Orden induce una biyección en {0, 1, ..., n − 1}.
Esta función colorea los vértices de G de la forma descripta abajo.
Retorna el número de colores que usa, salvo que haya algún error, (pej al alocar memoria temporal o si Orden no induce
una biyección) en cuyo caso retorna 232 − 1.
Corre greedy en G comenzando con el color 1, iterando sobre los vértices siguiendo el orden dado en el array apuntado
por Orden, asumiendo que Orden[i]=k significa que el vértice k será el vértice procesado en el i-esimo lugar.

Esta función es O(m).

## Funciones para crear ordenes

Estas funciones asumen que el grafo G tiene un coloreo propio con colores {1, 2, .., r} para algún r.

Tambien asumen que Orden apunta a una region de memoria con n lugares, donde n es el número de vertices de G.
Lo que hacen estas funciones es ordenar los vértices, llenando el array Orden en la forma indicada en cada uno de los
casos.
Si todo anduvo bien devuelven el char 0, si no el char 1.
La forma de ordenar los vértices en ambos casos es ordenar vértices por bloques de colores, como en el VIT.
Es decir, ambas funciones lo que harán es ordenar los vértices poniendo primero todos los vértices que tengan un color
x1 , luego todos los vértices que tengan un color x2 , etc.
La diferencia entre ambas funciones es quienes son los colores x1 , x2 , ... etc. asi que en cada caso especificaremos esto.
Ambas funciones deberian ser, idealmente, O(n), pero O(nlog n) es aceptable.

## Guldukat
Prototipo de función:
_char GulDukat(Grafo G,u32* Orden);_

La forma de llenar Orden, como se dijo, es poner todos los vértices de color x1 primero, luego los de color x2 , etc, donde
x1 , x2 , ... vienen dados de la siguiente manera:
asumiendo que r es la cantidad de colores usados para colorear G, definimos las siguientes funciones de {1, ..., r} a ZZ,
usando las funciones de la parte 1:

$$

m(x) = \min\{ \text{Grado}(i, G) : \text{Color}(i, G) = x \}
\\
M(x) = \max\{ \text{Grado}(i, G) : \text{Color}(i, G) = x \}
\

$$


Entonces debe poner primero todos los colores que sean divisibles por 4, ordenados entre si de acuerdo con M (x) (de
mayor a menor), luego todos los colores pares no divisibles por 4, ordenados entre si de acuerdo con M (x) + m(x) (de mayor
a menor) y finalmente todos los colores impares, ordenados entre si de acuerdo con m(x). (de mayor a menor).
(motivación para esta función: vertices con alto grado en general tienen mayores problemas para ser coloreados. Asi que
poner primero los vertices de mayor grado deberia dar un buen coloreo....pero eso no necesariamente será un ordenamiento
por bloque de colores. Asi que debemos ordenar los colores, de acuerdo con los grados de los vertices de ese color. Podriamos
tomar el promedio de los grados, o el mayor grado, o el menor grado...aca hicimos una mezcla entre el mayor y el menor
grado, con un test de divisibilidad para que sea facil chequear la funcion en el caso de un grafo regular por ejemplo y ademas
para mezclar los colores).

## ElimGarak()
Prototipo de función:
_char ElimGarak(Grafo G,u32* Orden);_


La forma de llenar Orden como se dijo, es poner todos los vértices de color x1 primero, luego los de color x2 , etc, donde
x1 , x2 , ... vienen dados de la siguiente manera, asumiendo que hay r colores:
xr = 1.
xr−1 = 2.
x1 es el color que tiene la menor cantidad de vértices de ese color, excluyendo a los colores 1 y 2.
x2 es el color que tiene la menor cantidad de vértices de ese color, excluyendo a los colores 1,2 y x1 .
etc.
(motivación para esta función: poniendo los colores 1 y 2 al ultimo, nos aseguramos que los vertices que menos problemas
tuvieron para ser coloreados sean coloreados al final. Y los demas colores los ordenamos por cardinalidad, asumiendo que los
colores que menos vertices tienen son los mas problematicos, y poniendolos al principio quizas podamos bajar la cantidad de
colores).

## Main
Main para testear las funciones.
El main empieza corriendo Greedy en 5 ordenes distintos, (especificados mas abajo) imprimiendo la
cantidad de colores que se obtiene, y para cada uno de esos ordenes iniciales, luego se hace una iteracion en donde hacen lo
siguiente:
1. Corre GulDukat y a continuacio
n Greedy con ese orden
2. Luego corre ElimGarak y Greedy con ese orden.
Se iteran estos dos pasos un total de 50 veces, imprimiendo la cantidad de colores obtenidos luego de cada Greedy. (es decir,
que para cada uno de los ordenes iniciales, haran un total de 100 reordenamientos y coloreos con Greedy, con lo cual haran
en total 500 reordenamientos mas Greedys, mas 5 Greedys iniciales).
Para cada una de esas iteraciones se guardan la cantidad de colores que usaron en esa iteración, mas el ultimo coloreo
que se usó (el cual se guarda en algun array apropiado, usando ExtraerColores)
Luego, comparando las 5 corridas, se toma el coloreo que les dio la menor cantidad de colores, se recolorea los vertices de G
con ese coloreo (usando ImportarColores) y luego hace 500 veces un reordenamiento GulDukat o ElimGarak seguido de un
Greedy, impriendo la cantidad de colores obtenida.
El programa elije al azar, con una probabilidad de 50%, cual de los dos ordenes se elije en cada una de las 500
iteraciones.
Al final de todo se habrá hecho 1000 reordenes por bloque de colores, más sus correspondientes Greedys, mas algunos
Greedys extras.
Los ordenes iniciales son:
1) el orden natural 0,1,...,n-1 de los vertices,
2) el orden n-1,...,2,1,0
3) el orden poniendo primero todos los pares en orden decreciente y luego todos los impares, en orden creciente.
4) ordenar los vértices de acuerdo con su grado, comenzando por el mayor grado y terminando con el menor grado.
y 5) algun orden extra, elegido arbitrariamente.


## Velocidad
El código es razonablemente rápido. Se puso una restricción de 15 minutos máximos para correr los grafos de gran tamaño la cantidad de veces escrita arriba.

## Restricciones generales
1. No se puede incluir NINGUNA libreria externa que no sea una de las básicas de C. (eg, stdlib.h, stdio.h,
strings.h, stdbool.h, assert.h etc, si, pero otras no. Especificamente, glibc NO).
2. El código debe ser razonablemente portable, aunque no es probable que testemos sobre Apple, y en general
testearemos con Linux, quizas podamos testearlo desde Windows.
3. No pueden usar archivos llamados aux.c o aux.h
4. No pueden usar getline.
5. El uso de macros esta permitido pero como siempre, sean cuidadosos si los usan.

## Archivos
* APIG24.h: Declaración de las funciones de la parte 1
* API2024Parte2.h: Declaración de las funciones de la parte 2
* EstructuraGrafo24.h: Archivo que contiene la estructura del grafo en particular, es decir, como son guardados los vértices, las aristas, etc.
* /Grafos_Prueba -> Carpeta que contiene distintos grafos de distintos tamaños para realizar las pruebas.

## Compilación
Se compila usando usando gcc, -Wall, -Wextra, -O3, -std=c99 . Tambien haremos -I al directorio
donde pondremos los .h
Esas flags seran usadas para testear la velocidad, pero para testear grafos chicos podemos agregar otras flags.
Por ejemplo, podemos usar -DNDEBUG.
Tambien compilaremos, para testear grafos chicos, con flags que nos permitan ver si hay buffer overflows, shadow variables o comportamientos indefinidos, en particular con -fsanitize=address,undefined. El programa compila correctamente con esta flag aunque para mayor velocidad se recomienda no usarla ya que provoca una gran demora en grafos muy grandes.


