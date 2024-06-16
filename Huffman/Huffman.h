#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <algorithm>

#include "compareNode.h"
#include "node.h"

using namespace std;
/*Codigo sacado de:  https://gist.github.com/pepgonzalez/5360217 */
class Huffman {
private:
    node* ARBOL;
    unordered_map<string, node*> elementos;
    unordered_map<string, string> codigos;
    size_t tamañoCodificado;

    unordered_map<char, int> obtenerFrecuencias(const string& cadena) {
        unordered_map<char, int> f;
        for (char e : cadena) {
            f[e]++;
        }
        return f;
    }

    node* generaNuevoNodo(node* n1, node* n2) {
        string nombre = n1->nombre + n2->nombre;
        int suma = n1->frecuencia + n2->frecuencia;
        n1->codigo = '0';
        n2->codigo = '1';
        node* nuevo = new node(nombre, suma, n1, n2);
        return nuevo;
    }

    void generaCodigos(node* r) {
        if (r->izquierda != nullptr) {
            r->izquierda->codigo = r->codigo + r->izquierda->codigo;
            generaCodigos(r->izquierda);
        }
        if (r->derecha != nullptr) {
            r->derecha->codigo = r->codigo + r->derecha->codigo;
            generaCodigos(r->derecha);
        }
    }
/**
 * guarda cada nodo del arbor en una tabla hash 
 */
    unordered_map<string, node*> obtenerDiccNodos(node* r, unordered_map<string, node*>& elementos) {
        if (elementos.find(r->nombre) == elementos.end()) {
            elementos[r->nombre] = r;
        }
        if (r->izquierda != nullptr) {
            obtenerDiccNodos(r->izquierda, elementos);
        }
        if (r->derecha != nullptr) {
            obtenerDiccNodos(r->derecha, elementos);
        }
        return elementos;
    }
/**
 * Convierte el diccionario de nodos en un diccionario donde las 
 * claves son los nombres de los nodos y los valores son sus códigos.
 */
    unordered_map<string, string> obtenerMapaDeCodigos(const unordered_map<string, node*>& d) {
        unordered_map<string, string> nombreYCodigo;
        for (const auto& e : d) {
            node* nodo = e.second;
            nombreYCodigo[nodo->nombre] = nodo->codigo;
        }
        return nombreYCodigo;
    }
public:
    Huffman() : ARBOL(nullptr) {}

    ~Huffman() {
        limpiarArbol(ARBOL);
    }

    void limpiarArbol(node* nodo) {
        if (nodo != nullptr) {
            limpiarArbol(nodo->izquierda);
            limpiarArbol(nodo->derecha);
            delete nodo;
        }
    }

    void construirArbol(const string& cadena) {
    unordered_map<char, int> frec = obtenerFrecuencias(cadena); // O(length(cadena))

    priority_queue<node*, vector<node*>, CompareNode> pq; //minHeap por cantidad de frecuencia de caracter

    for (const auto& pair : frec) { // O(cantidad de caracteres distintos)
        node* nodo = new node(string(1, pair.first), pair.second);
        pq.push(nodo); // O(log k) donde k es el número de elementos en la cola de prioridad
    }

    while (pq.size() > 1) { // O(cantidad de caracteres distintos* logk)
        node* n1 = pq.top(); // O(1)
        pq.pop(); // O(log k)

        node* n2 = pq.top(); // O(1)
        pq.pop(); // O(log k)

        node* nuevo = generaNuevoNodo(n1, n2); // O(1)
        pq.push(nuevo); // O(log k)
    }

    ARBOL = pq.top(); // O(1)
    pq.pop(); // O(log k)

    generaCodigos(ARBOL); // O(cantidad de nodos)
    elementos = obtenerDiccNodos(ARBOL, elementos); //O(cantidad de nodos)
    codigos = obtenerMapaDeCodigos(elementos); // O(size(elementos))
}

    string codificar(const string& texto) {
        string huffman = "";
        for (char e : texto) {
            huffman += codigos[string(1, e)];
        }
        this->tamañoCodificado=huffman.length();
        return huffman;
    }

    string decodificar(const string& codigo) {
        return descomprime(ARBOL, codigo);
    }

    string descomprime(node* arbol, const string& codigo) {
    node* original = arbol;
    string msj = "";

    for (char c : codigo) {
        if (c == '0') {
            if (arbol->izquierda != nullptr && arbol->izquierda->nombre.size() == 1) {
                msj += arbol->izquierda->nombre;
                arbol = original;
            } else {
                arbol = arbol->izquierda;
            }
        } else {
            if (arbol->derecha != nullptr && arbol->derecha->nombre.size() == 1) {
                msj += arbol->derecha->nombre;
                arbol = original;
            } else {
                arbol = arbol->derecha;
            }
        }
    }

    return msj;
}
    double tamañoCodificadoEnMB(){
        return (double)(tamañoCodificado/8/(1e6));
    }

};