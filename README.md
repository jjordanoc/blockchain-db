# Block Chain
## Proyecto Computacional
### Algoritmos y Estructuras de Datos - 2022 - 2

[![N|Solid](https://utec.edu.pe/sites/all/themes/utec_theme/LOGO_UTEC.png)](https://utec.edu.pe/carreras/ciencia-de-la-computacion)

|            Participante             |  Papel  |
|:-----------------------------------:|:-------:|
|  Joaquín Francisco Jordán O'Connor  | Backend |
|    José Rafael Chachi Rodriguez     | Backend |
|    Renato Aurelio Cernades Ames     | Backend |

## Introducción

En el presente proyecto se ha implementado una estructura de datos denominada Blockchain. En esta se realizarán operaciones de inserción, modificación y búsqueda. Se tratará de seleccionar las mejores estrcuturas de datos con sus respectivos algoritmos para manejar una complejidad computacional y espacial razonable.  

## Motivación e importancia del proyecto

Un Blockchain es una estructura que permite el guardado de datos de una forma segura. Su implementación consiste, básicamente, en un conjunto de distintos bloques en donde cada bloque está referenciado por el siguiente como si fuese una lista.
Un bloque está conformado los datos que es un número máximo de registros y por un código hash que permite el guardado seguro. Si se intentase modificar los datos dentro del bloque, se procederá a generar un nuevo código hash lo que genera un retraso y así se pueden detectar fácilmente intentos de modificación.
Se decidió estudiar más a fondo esta estructura de datos segura y ver que tan bien puede a ser el factor de confiabilidad que brinda el Blockchain, es por ello que se recurre a la construcción desde el inicio utilizando como ayuda un código de encriptación tal como [2].

## Implementación del proyecto

Para la implementación del Blockchain se decidió utilizar la estructura de datos **lista circular doblemente enlazada con nodo centila** [1]. 
La lista circular doblemente enlazada con nodo centila es una construcción de nodos que apuntan a un siguiente nodo o uno anterior. Con esta construcción del nodo se logró juntar los bloques para poder formar nuestro Blockchain.

Se sabe por construcción del Blockchain que este siempre tiene un bloque inicial con el número máximo de registros que el bloque soporta, a este bloque se le denominará bloque génesis. Es por ello
que se aprovechará la construcción que posee la lista circular doblemente enlazada con su nodo centila. Este nodo centila nos servirá para definir el bloque génesis el cual no tiene registro disponibles para modificar o buscar.


### Métodos disponibles del Blockchain

- **push( ):** Se insertará un nuevo registro al último bloque disponible que el Blockchain tenga. Si el último bloque está con el número máximo de registros disponibles por bloque, se procede a crear un nuevo bloque. 
**Complejidad:** Se aprovechó el nodo centila de la lista circular doblementa enlazada para poder acceder al nodo previo que este tiene. Por definición de lista circular en [1], el primer nodo siempre apunturá hacia el último nodo. Complejidad de O(1).

## Conclusiones

- Manejar un repositorio como github ayuda a trabajar de forma organizada.
- La definición de un bloque mediante un código hash es seguro hasta cierto punto. Para poder mejorar esta seguridad se puede modificar el código hash para que cumpla ciertos requerimientos como un cantidad de 0 fijos al inicio. De esta forma se hashea reiteradas veces.

## Autores

|                     <a target="_blank">**Joaquín Jordán**</a>                     |                             <a target="_blank">**Renato Cernades**</a>                                                         |                       <a target="_blank">**José Chachi**</a>                        |  
|:---------------------------------------------------------------------------------:|:------------------------------------------------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|
|           ![Joaquín](https://avatars.githubusercontent.com/u/83974213)            |                                  ![Renato](https://avatars.githubusercontent.com/u/83974266)                                   |              ![José](https://avatars.githubusercontent.com/u/83974741)              |                                              
| <a href="https://github.com/jjordanoc" target="_blank">`github.com/jjordanoc`</a> |                <a href="https://github.com/RenatoCernades0107" target="_blank">`github.com/RenatoCernades`</a>                 | <a href="https://github.com/JoseChachi" target="_blank">`github.com/JoseChachi`</a> |


## Bibliografía

- [1] T. H. Cormen, C. E. Leiserson, R. L. Rivest, C. Stein, "Introduction to Algorithms". Cambridge, Massachusetts, USA, 2009 (3er ed.) [Online]. Available: https://sd.blackball.lv/library/Introduction_to_Algorithms_Third_Edition_(2009).pdf .[Accessed: Sep 24, 2022]
- [2] OpenSSL, "Welcome to OpenSSL!", 1999. [Online]. Available: https://www.openssl.org/ .[Accessed: Oct 5, 2022]

[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=8653153&assignment_repo_type=AssignmentRepo)
