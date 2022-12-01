//
// Created by renato on 11/15/22.
//

#ifndef PATRICIA_COMPACTTRIE_H
#define PATRICIA_COMPACTTRIE_H
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <forward_list>
#include "index.h"
const int SIZE_ALPHABET = 256;

/*
Requerimientos para usar la estructura:
- T debe tener por lo menos un atributo string.
- Cmp debe siempre retornar el string como referencia, este debe ser un atributo de T.
- T debe tener un constructor por defecto y otro con un parametro de tipo string.
*/

template <typename T, typename Cmp>
class CompactTrie : public Index {
    struct Node {
        T key;
        std::vector<Node*> children;
        bool endWord;
        bool isLeaf;
        int n; // numero de hijos

        Node() {
            children = std::vector<Node*>(SIZE_ALPHABET, nullptr);
            n = 0;
        }
        explicit Node(T s, bool b):key(s),endWord(b) {
            children = std::vector<Node*>(256, nullptr);
            n = 0;
        };

        void killSelf(){
            if(this != nullptr){
                for (auto& child : children) {
                    if(child != nullptr)
                        child->killSelf();
                }
                delete this;
            }
        }
        ~Node(){}
    };

    Node* root;
    Cmp get;
    int sz = 0; // La cantidad de palabras en el árbol
public:
    CompactTrie()
    {
        root = new Node(T{}, false);
        root->isLeaf = true;
        root->n = 0;
    };
    CompactTrie(Cmp& cmp):get(cmp)
    {
        root = new Node(T{}, false);
        root->isLeaf = true;
        root->n = 0;
    };
    void insert(T key)
    {
        insert(root, key);
        sz += 1;
    };
    void remove(T key)
    {
        remove(root, key);
        sz -= 1;
    };
    bool find(T key)
    {
        return find(root, key);
    };
    T searchEqual(T key)
    {
        return searchEqual(root, key);
    }
    std::forward_list<T> searchStartWith(T key)
    {
        std::forward_list<T> result;
        searchStartWith(root, result, key);
        return result;
    }
    ~CompactTrie(){
        root->killSelf();
    }

private:
    void insert(Node* node, T& key)
    {
        // Caso base: Cuando primera letra del key no está en el nodo
        Node* child = node->children[get(key)[0]];
        if(child == nullptr){
            // Se inserta la llave
            node->children[get(key)[0]] = new Node(key, true);

            node->isLeaf = false;

            node->n += 1;

            return;
        }

        // Compara con cada letra del nodo hijo con la key
        T newKey;
        get(newKey) = "";
        int i = 0;

        // Caso 1: La key es igual o más grande que la key del hijo
        if(get(key).size() >= get(child->key).size()){
            for (; i < get(child->key).size(); ++i) {

                // Salimos del bucle cuando la longitud de la key ha sido superada
                if(i >= get(key).size()) break;

                // Cuando la letra de la llave del hijo deja de ser igual a la llave, habremos encontrado la posicion de nuestra key
                if(get(child->key)[i] != get(key)[i]){

                    // Se crea un nuevo nodo con la parte que coincide de la key
                    Node* newNode = new Node(newKey, false);

                    // Se ancla al padre
                    node->children[get(newKey)[0]] = newNode;

                    // Deja de ser hoja
                    node->children[get(key)[0]]->isLeaf = false;

                    // La llave del hijo cambia a la parte que resta la su llave
                    get(child->key) = get(child->key).substr(i);

                    // El child se agrega como hijo del nuevo nodo
                    newNode->children[get(child->key)[0]] = child;

                    // Se crea un nuevo nodo con lo que resta de la key y agrega como hijo del nuevo nodo
                    T nwNd(key);
                    get(nwNd) = get(key).substr(i);

                    newNode->children[get(key)[i]] = new Node(nwNd, true);

                    // NewNode deja de ser hoja
                    newNode->isLeaf = false;

                    // Los hijos del nuevo nodo aumentan en 2
                    newNode->n += 2;

                    return;
                }
                get(newKey) += get(key)[i];
            }

            // Caso 1.1: La key es exactamente igual a la llave del nodo
            if(get(key).size() == i){

                // Hacemos endWord el nodo hijo
                child->endWord = true;
            }

            /* En este punto, la key del hijo coincide completamente con la key, pero aún faltan letras de la key.
             * Se llama recursivamente la función para insertar lo que resta de la llave.
             */
            T a(key);
            get(a) = get(key).substr(i);
            insert(child, a);
        }

            // Caso 2 Prefijo: la key es menor que la key del hijo.
        else {
            for (; i < get(key).size(); ++i) {
                if(get(child->key)[i] != get(key)[i]){

                    // Se crea un nuevo nodo con la parte que coincide de la key
                    Node* newNode = new Node(newKey, false);

                    // Se ancla al padre, borrando la conexión que tenía con el hijo
                    node->children[get(newKey)[0]] = newNode;

                    // Deja de ser hoja
                    node->children[get(key)[0]]->isLeaf = false;

                    // La llave del hijo cambia a la parte que resta la su llave
                    get(child->key) = get(child->key).substr(i);

                    // El child se agrega como hijo del nuevo nodo
                    newNode->children[get(child->key)[0]] = child;

                    // Se crea un nuevo nodo con lo que resta de la key y agrega como hijo del nuevo nodo
                    T nwNd(key);
                    get(nwNd) = get(key).substr(i);
                    newNode->children[get(key)[i]] = new Node(nwNd, true);

                    // NewNode deja de ser hoja
                    newNode->isLeaf = false;

                    // Los hijos del nuevo nodo aumentan en 2
                    newNode->n += 2;

                    return;
                }
                get(newKey) += get(key)[i];
            }

            // En este punto, la key está dentro de la key del hijo

            // Se crea un nuevo nodo con key completa
            Node* newNode = new Node(key, true);

            // Se ancla al padre, borrando la conexión que tenía con el hijo
            node->children[get(newKey)[0]] = newNode;

            // Deja de ser hoja
            node->children[get(key)[0]]->isLeaf = false;

            // La llave del hijo cambia a la parte que resta la su llave
            get(child->key) = get(child->key).substr(i);

            // El child se agrega como hijo del nuevo nodo
            newNode->children[get(child->key)[0]] = child;

            // NewNode deja de ser hoja
            newNode->isLeaf = false;

            // Los hijos del nuevo nodo aumentan en 1
            newNode->n += 1;

        }
    };

    void remove(Node* node, T& key)
    {
        Node* child = node->children[get(key)[0]];

        if(child == nullptr) throw std::runtime_error("No se encontró la palabra");

        /* CASO BASE */
        if(get(child->key) == get(key)){

            if(!child->endWord) throw std::runtime_error("No se encontró la palabra");

            child->endWord = false;

            if(child->isLeaf){
                delete child;
                node->children[get(key)[0]] = nullptr;
                node->n -= 1;
                if(node->n == 0) node->isLeaf = true;
            }
        }

        else {
            /* RECURSIVIDAD */
            // Caso 1: la key del hijo es igual más grande en longitud que la key
            int i = 0;
            if(get(key).size() >= get(child->key).size()){
                for (; i < get(child->key).size(); ++i) {
                    if(get(child->key)[i] != get(key)[i]){
                        throw std::runtime_error("No se encontró la palabra");
                    }
                }

                /* La key del hijo coincide totalmente con la key a remover, por ello, se llama recursivamente
                 * para seguir buscando la llave
                 */
                T a(key);
                get(a) = get(key).substr(i);
                remove(child, a);
            }

                // Caso 2: la longitud de la key es menor que la del nodo hijo
            else {
                throw std::runtime_error("No se encontró la palabra");
            }
        }

        /* REGRESO DE LA RECURSIVIDAD */
        // Caso 1: Si el hijo tiene un solo hijo, se combina con ese hijo en caso pueda
        if(!child->endWord && child->n == 1){
            Node* tmpChild = child;
            Node* tmpFather = node;

            // Se busca el hijo del nodo hijo
            int j = 0;
            for (; j < SIZE_ALPHABET; ++j) {
                if(child->children[j] != nullptr) break;
            }

            T keyAcum(key);
            get(keyAcum) = "";
            // Mientras no encontremos un endWord, buscamos hasta el último nodo que se puede compactar
            while (!tmpChild->endWord){

                // Si nuestro tmp tiene 2 hijos o más, entonces paramos
                if(tmpChild->n > 1) break;

                get(keyAcum) += get(tmpChild->key);

                // Se va al siguiente hijo y padre
                tmpFather = tmpChild;

                // Se busca el siguiente hijo del nodo hijo temporal
                int k = 0;
                for (; k < SIZE_ALPHABET; ++k) {
                    if(tmpChild->children[k] != nullptr) break;
                }

                tmpChild = tmpChild->children[k];
            }
            get(keyAcum) += get(tmpChild->key);

            // Desconectamos el temporal con su padre
            tmpFather->children[get(tmpChild->key)[0]] = nullptr;

            // Eliminamos el child
            delete child;

            // Al padre se le cambia el hijo por el nodo temporal
            node->children[get(key)[0]] = tmpChild;

            // Se le cambia la key al nodo temporal por la nueva llave combinada
            get(tmpChild->key) = get(keyAcum);

            return;
        }

        // Caso 2: El hijo no tiene hijos
        if(child->n == 0){
            // Si no es un fin de palabra
            if(!child->endWord){
                delete child;
                node->children[get(key)[0]] = nullptr;
            }
            return;
        }
    };

    bool find(Node *node, T& key)
    {
        Node* child = node->children[get(key)[0]];

        if(child == nullptr) return false;

        /* CASO BASE */
        if(get(child->key) == get(key)){
            return child->endWord;
        }

        else {
            /* RECURSIVIDAD */
            int i = 0;
            if(get(key).size() >= get(child->key).size()){
                for (; i < get(child->key).size(); ++i) {
                    if(get(child->key)[i] != get(key)[i]){
                        return false;
                    }
                }
                T a(key);
                get(a) = get(key).substr(i);
                return find(child, a);
            }
            else {
                return false;
            }
        }
    };

    T searchEqual(Node *node, T& key)
    {
        Node* child = node->children[get(key)[0]];

        if(child == nullptr) return T{};

        /* CASO BASE */
        if(get(child->key) == get(key)){
            if(child->endWord)
                return child->key;
            else
                return T{};
        }

        else {
            /* RECURSIVIDAD */
            int i = 0;
            if(get(key).size() >= get(child->key).size()){
                for (; i < get(child->key).size(); ++i) {
                    if(get(child->key)[i] != get(key)[i]){
                        return T{};
                    }
                }
                T a(key);
                get(a) = get(key).substr(i);
                return searchEqual(child, a);
            }
            else {
                return T{};
            }
        }
    }

    void addChildren(Node* node, std::forward_list<T>& result){
        result.push_front(node->key);
        for (int i = 0; i < node->children.size(); ++i) {
            if(node->children[i] != nullptr){
                addChildren(node->children[i], result);
            }
        }
    }

    void searchStartWith(Node *node, std::forward_list<T>& result, T& key)
    {
        Node* child = node->children[get(key)[0]];

        if(child == nullptr) return;

        /* CASO BASE */
        if(get(child->key) == get(key)){
            // Agregamos todos los hijos del nodo hijo
            addChildren(child, result);
            return;
        }

        else {
            /* RECURSIVIDAD */
            int i = 0;
            if(get(key).size() >= get(child->key).size()){
                for (; i < get(child->key).size(); ++i) {
                    if(get(child->key)[i] != get(key)[i]){
                        return;
                    }
                }
                T a(key);
                get(a) = get(key).substr(i);
                searchStartWith(child, result, a);
                return;
            }
            else {
                for (; i < get(key).size(); ++i) {
                    if(get(child->key)[i] != get(key)[i]){
                        return;
                    }
                }
                // Agregamos todos los hijos del nodo hijo
                addChildren(child, result);
                return;
            }
        }
    }
};


#endif //PATRICIA_COMPACTTRIE_H
