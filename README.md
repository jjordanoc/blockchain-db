# Blockchain
## Algoritmos y Estructuras de Datos - 2022 - 2

[![N|Solid](https://utec.edu.pe/sites/all/themes/utec_theme/LOGO_UTEC.png)](https://utec.edu.pe/carreras/ciencia-de-la-computacion)

## Autores

|                     <a target="_blank">**Joaquín Jordán**</a>                     |                             <a target="_blank">**Renato Cernades**</a>                                                         |                       <a target="_blank">**José Chachi**</a>                        |  
|:---------------------------------------------------------------------------------:|:------------------------------------------------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|
|           ![Joaquín](https://avatars.githubusercontent.com/u/83974213)            |                                  ![Renato](https://avatars.githubusercontent.com/u/83974266)                                   |              ![José](https://avatars.githubusercontent.com/u/83974741)              |                                              
| <a href="https://github.com/jjordanoc" target="_blank">`github.com/jjordanoc`</a> |                <a href="https://github.com/RenatoCernades0107" target="_blank">`github.com/RenatoCernades`</a>                 | <a href="https://github.com/JoseChachi" target="_blank">`github.com/JoseChachi`</a> |


## Introducción

En el presente proyecto se ha implementado una estructura de datos de Blockchain. En esta se realizarán operaciones de inserción, modificación y búsqueda. Para la implementación de esta estructura, se han seleccionado las estructuras de datos óptimas con sus respectivos algoritmos asociados para manejar una complejidad computacional y espacial razonable.  

## Descripción del caso de estudio planteado por el grupo

En el presente proyecto se estudiarán las transacciones de un banco ficticio que maneja un Blockchain local con la finalidad de asegurar sus datos.

## Importancia del Blockchain en el dominio de datos elegido

El Blockchain surge en el año 2008 con el objetivo de servir como un registro de contabilidad distribuido y seguro para la criptomoneda Bitcoin. Es así que el Blockchain desde sus orígenes está pensado para servir en aplicaciones financieras.

## Estructura del blockchain

Para la implementación del Blockchain se decidió utilizar una lista circular doblemente enlazada con centinela [1]. 
Esta lista está compuesta por estructuras llamadas bloques, que son arreglos de tamaño fijo. Además, cuentan con un puntero a su propio código hash y un puntero al código hash del bloque anterior, de tal manera que, al alterarse el código hash del bloque anterior, esta actualización se ve reflejada también en el bloque actual.
Los bloques tambien cuentan con un id, un entero positivo que indica el orden cronológico del bloque creado, y un nonce. El nonce se calcula al momento de minar el bloque como el menor entero positivo que produce por lo menos cuatro ceros al inicio del hash. Esta es la técnica de _proof of work_ que se implementó en el Blockchain.
La integridad del contenido del Blockchain está garantizada por los métodos limitados que contamos para interactuar con la estructura: ``insertEntry``, ``hackEntry``, ``searchEntry``, ``isValid`` y ``validate``. Las operaciones realizadas sobre el Blockchain pueden invalidarlo; sin embargo, se puede verificar la validez del Blockchain y validarlo, minando los bloques que haga falta.


### Análisis de la complejidad de los métodos del Blockchain

- **insertEntry:** Inserta un nuevo registro al último bloque disponible en el Blockchain. Si el último bloque está lleno, se procede a crear un nuevo bloque. 
**Complejidad:** O(1), pues se emplea una estructura de tipo lista doblemente enlazada con nodo centinela, que es eficiente para la inserción al final.
- **hackEntry:** Actualiza un registro cualquiera en el Blockchain.
**Complejidad:** O(n) donde n es la longitud del Blockchain, pues se debe buscar el bloque según su id para poder actualizar uno de sus registros.
- **isValid:** Verifica que cada bloque en el Blockchain sea válido, es decir, el código hash de cada bloque debe contener por lo menos cuatro ceros al inicio.
- **Complejidad:** O(n) donde n es la longitud del Blockchain, pues en el peor de los casos (si el bloque del final es el único invalido) se debe recorrer toda la estructura.
- **validate:** Mina los bloques no válidos.
- **Complejidad:** O(k*n) donde n es la longitud del Blockchain y k es una constante aleatoria, esto se debe a que el cálculo del nonce para cada bloque tarda un tiempo aleatorio.
## Conclusiones

- La estructura de Blockchain es una buena opción para asegurar información sensible gracias al mecanismo de _proof of work_, especialmente si la cadena es de gran tamaño, pues dificulta su modificación.
- La seguridad que brinda la estructura se puede mejorar si se utiliza como un registro distribuido, pues esto dificulta aún más hacer cambios en la cadena.


## Bibliografía

- [1] T. H. Cormen, C. E. Leiserson, R. L. Rivest, C. Stein, "Introduction to Algorithms". Cambridge, Massachusetts, USA, 2009 (3er ed.) [Online]. Available: https://sd.blackball.lv/library/Introduction_to_Algorithms_Third_Edition_(2009).pdf .[Accessed: Sep 24, 2022]
- [2] OpenSSL, "Welcome to OpenSSL!", 1999. [Online]. Available: https://www.openssl.org/ .[Accessed: Oct 5, 2022]

[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=8653153&assignment_repo_type=AssignmentRepo)
