#ifndef PATRICIA_COMPACTTRIE_H
#define PATRICIA_COMPACTTRIE_H
#include <iostream>
#include <stack>
#include <string>
#include <vector>

const int SIZE_ALPHABET = 256;

class CompactTrie {
    struct Node {
        std::string key;
        std::vector<Node*> children;
        bool endWord;
        bool isLeaf;
        int n; // numero de hijos

        Node() {
            children = std::vector<Node*>(SIZE_ALPHABET, nullptr);
            n = 0;
        }
        explicit Node(std::string s, bool b):key(std::move(s)),endWord(b) {
            children = std::vector<Node*>(256, nullptr);
            n = 0;
        };

        friend std::ostream& operator<<(std::ostream& os, Node*& n) {
            if(n->endWord) os << "{" << n->key << "}";
            else os << "[" << n->key << "]";
            return os;
        }

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
    int sz = 0; // La cantidad de palabras en el árbol
public:
    CompactTrie();
    void insert(const std::string& key);
    void insert(Node* node, const std::string& key);
    void remove(const std::string& key);
    void remove(Node* node, const std::string& key);
    bool search(const std::string& key);
    bool search(Node *node, const std::string& key);
    void display(const char& spc);
    friend std::ostream& operator<<(std::ostream& os, CompactTrie& ct){
        ct.display(' ');
        return os;
    }
    void operator+=(const std::string& s){
        this->insert(s);
    }
    void operator-=(const std::string& s){
        this->remove(s);
    }
    bool operator[](const std::string& s){
        return search(s);
    }
    ~CompactTrie();
};


#endif //PATRICIA_COMPACTTRIE_H
