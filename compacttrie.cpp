//
// Created by renato on 11/15/22.
//

#include "CompactTrie.h"

CompactTrie::CompactTrie() {
    root = new Node("", false);
    root->isLeaf = true;
    root->n = 0;
}

void CompactTrie::insert(const std::string& key){
    insert(root, key);
    sz += 1;
}

void CompactTrie::insert(Node* node, const std::string& key){

    // Caso base: Cuando primera letra del key no está en el nodo
    Node* child = node->children[key[0]];
    if(child == nullptr){

        // Se inserta la llave
        node->children[key[0]] = new Node(key, true);

        node->isLeaf = false;

        node->n += 1;

        return;
    }

    // Compara con cada letra del nodo hijo con la key
    std::string newKey;
    int i = 0;

    // Caso 1: La key es igual o más grande que la key del hijo
    if(key.size() >= child->key.size()){
        for (; i < child->key.size(); ++i) {

            // Salimos del bucle cuando la longitud de la key ha sido superada
            if(i >= key.size()) break;

            // Cuando la letra de la llave del hijo deja de ser igual a la llave, habremos encontrado la posicion de nuestra key
            if(child->key[i] != key[i]){

                // Se crea un nuevo nodo con la parte que coincide de la key
                Node* newNode = new Node(newKey, false);

                // Se ancla al padre
                node->children[newKey[0]] = newNode;

                // Deja de ser hoja
                node->children[key[0]]->isLeaf = false;

                // La llave del hijo cambia a la parte que resta la su llave
                child->key = child->key.substr(i);

                // El child se agrega como hijo del nuevo nodo
                newNode->children[child->key[0]] = child;

                // Se crea un nuevo nodo con lo que resta de la key y agrega como hijo del nuevo nodo
                newNode->children[key[i]] = new Node(key.substr(i), true);

                // NewNode deja de ser hoja
                newNode->isLeaf = false;

                // Los hijos del nuevo nodo aumentan en 2
                newNode->n += 2;

                return;
            }
            newKey += key[i];
        }

        // Caso 1.1: La key es exactamente igual a la llave del nodo
        if(key.size() == i){

            // Hacemos endWord el nodo hijo
            child->endWord = true;
        }

        /* En este punto, la key del hijo coincide completamente con la key, pero aún faltan letras de la key.
         * Se llama recursivamente la función para insertar lo que resta de la llave.
         */
        insert(child, key.substr(i));
    }

    // Caso 2 Prefijo: la key es menor que la key del hijo.
    else {
        for (; i < key.size(); ++i) {
            if(child->key[i] != key[i]){

                // Se crea un nuevo nodo con la parte que coincide de la key
                Node* newNode = new Node(newKey, false);

                // Se ancla al padre, borrando la conexión que tenía con el hijo
                node->children[newKey[0]] = newNode;

                // Deja de ser hoja
                node->children[key[0]]->isLeaf = false;

                // La llave del hijo cambia a la parte que resta la su llave
                child->key = child->key.substr(i);

                // El child se agrega como hijo del nuevo nodo
                newNode->children[child->key[0]] = child;

                // Se crea un nuevo nodo con lo que resta de la key y agrega como hijo del nuevo nodo
                newNode->children[key[i]] = new Node(key.substr(i), true);

                // NewNode deja de ser hoja
                newNode->isLeaf = false;

                // Los hijos del nuevo nodo aumentan en 2
                newNode->n += 2;

                return;
            }
            newKey += key[i];
        }

        // En este punto, la key está dentro de la key del hijo

        // Se crea un nuevo nodo con key completa
        Node* newNode = new Node(key, true);

        // Se ancla al padre, borrando la conexión que tenía con el hijo
        node->children[newKey[0]] = newNode;

        // Deja de ser hoja
        node->children[key[0]]->isLeaf = false;

        // La llave del hijo cambia a la parte que resta la su llave
        child->key = child->key.substr(i);

        // El child se agrega como hijo del nuevo nodo
        newNode->children[child->key[0]] = child;

        // NewNode deja de ser hoja
        newNode->isLeaf = false;

        // Los hijos del nuevo nodo aumentan en 1
        newNode->n += 1;

    }

}

void CompactTrie::remove(const std::string& key) {
    remove(root, key);
}

void CompactTrie::remove(Node *node, const std::string &key) {
    Node* child = node->children[key[0]];

    if(child == nullptr) throw std::runtime_error("No se encontró la palabra");

    /* CASO BASE */
    if(child->key == key){

       if(!child->endWord) throw std::runtime_error("No se encontró la palabra");

       child->endWord = false;

       if(child->isLeaf){
           delete child;
           node->children[key[0]] = nullptr;
           node->n -= 1;
           if(node->n == 0) node->isLeaf = true;
       }
    }

    else {
        /* RECURSIVIDAD */
        // Caso 1: la key del hijo es igual más grande en longitud que la key
        int i = 0;
        if(key.size() >= child->key.size()){
            for (; i < child->key.size(); ++i) {
                if(child->key[i] != key[i]){
                    throw std::runtime_error("No se encontró la palabra");
                }
            }

            /* La key del hijo coincide totalmente con la key a remover, por ello, se llama recursivamente
             * para seguir buscando la llave
             */
            remove(child, key.substr(i));
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

        std::string keyAcum;
        // Mientras no encontremos un endWord, buscamos hasta el último nodo que se puede compactar
        while (!tmpChild->endWord){

            // Si nuestro tmp tiene 2 hijos o más, entonces paramos
            if(tmpChild->n > 1) break;

            keyAcum += tmpChild->key;

            // Se va al siguiente hijo y padre
            tmpFather = tmpChild;

            // Se busca el siguiente hijo del nodo hijo temporal
            int k = 0;
            for (; k < SIZE_ALPHABET; ++k) {
                if(tmpChild->children[k] != nullptr) break;
            }

            tmpChild = tmpChild->children[k];
        }
        keyAcum += tmpChild->key;

        // Desconectamos el temporal con su padre
        tmpFather->children[tmpChild->key[0]] = nullptr;

        // Eliminamos el child
        delete child;

        // Al padre se le cambia el hijo por el nodo temporal
        node->children[key[0]] = tmpChild;

        // Se le cambia la key al nodo temporal por la nueva llave combinada
        tmpChild->key = keyAcum;

        return;
    }

    // Caso 2: El hijo no tiene hijos
    if(child->n == 0){
        // Si no es un fin de palabra
        if(!child->endWord){
            delete child;
            node->children[key[0]] = nullptr;
        }
        return;
    }

}

bool CompactTrie::search(const std::string& key) {
    return search(root, key);
}

bool CompactTrie::search(Node *node, const std::string& key) {
    Node* child = node->children[key[0]];

    if(child == nullptr) return false;

    /* CASO BASE */
    if(child->key == key){
        return child->endWord;
    }

    else {
        /* RECURSIVIDAD */
        int i = 0;
        if(key.size() >= child->key.size()){
            for (; i < child->key.size(); ++i) {
                if(child->key[i] != key[i]){
                    return false;
                }
            }
            return search(child, key.substr(i));
        }
        else {
            return false;
        }
    }
}

void CompactTrie::display(const char& spc) {
    if(root == nullptr) throw std::runtime_error("No existen elementos a mostrar");
    std::stack<std::pair<int, Node*>> nodes;
    nodes.push(std::make_pair(0, root));
    while (!nodes.empty()) {
        std::pair<int, Node*> pair = nodes.top();
        nodes.pop();
        int level = pair.first;
        Node* node = pair.second;
        std::cout << level << std::boolalpha << node->isLeaf << "~" << node->n << std::string(level * 2, spc) << node << std::endl;
        for (int i = 0; i < SIZE_ALPHABET; ++i) {
            if(node->children[i] != nullptr)
                nodes.push(std::make_pair(level+1, node->children[i]));
        }
    }
}

CompactTrie::~CompactTrie() {
    root->killSelf();
}


