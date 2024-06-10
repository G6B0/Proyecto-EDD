#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <algorithm>

#include "node.h"

using namespace std;
/*Codigo sacado de:  https://gist.github.com/pepgonzalez/5360217 */
class Huffman {
private:
    node* ARBOL;
    unordered_map<string, node*> elementos;
    unordered_map<string, string> codigos;
    map<pair<string, string>, int> vertices;

    unordered_map<char, int> obtenerFrecuencias(const string& cadena) {
        unordered_map<char, int> f;
        for (char e : cadena) {
            f[e]++;
        }
        return f;
    }

    vector<node*> ordenar(const unordered_map<char, int>& frecuencias) {
        vector<node*> listaDeNodos;
        for (const auto& e : frecuencias) {
            node* nodo = new node(string(1, e.first), e.second);
            listaDeNodos.push_back(nodo);
        }
        sort(listaDeNodos.begin(), listaDeNodos.end(), [](node* a, node* b) {
            return a->frecuencia < b->frecuencia;
        });
        return listaDeNodos;
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

    unordered_map<string, string> obtenerMapaDeCodigos(const unordered_map<string, node*>& d) {
        unordered_map<string, string> nombreYCodigo;
        for (const auto& e : d) {
            node* nodo = e.second;
            nombreYCodigo[nodo->nombre] = nodo->codigo;
        }
        return nombreYCodigo;
    }

    map<pair<string, string>, int> obtenerVertices(node* nodo, node* padre) {
        if (padre != nullptr) {
            vertices[make_pair(padre->nombre, nodo->nombre)] = nodo->frecuencia;
        }
        if (nodo->izquierda != nullptr) {
            obtenerVertices(nodo->izquierda, nodo);
        }
        if (nodo->derecha != nullptr) {
            obtenerVertices(nodo->derecha, nodo);
        }
        return vertices;
    }

public:
    Huffman() : ARBOL(nullptr) {}

    ~Huffman() {
        // Destructor para liberar memoria
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
        unordered_map<char, int> frec = obtenerFrecuencias(cadena);
        vector<node*> lista = ordenar(frec);

        while (lista.size() > 1) {
            node* nuevo = generaNuevoNodo(lista[0], lista[1]);
            lista.erase(lista.begin());
            lista.erase(lista.begin());
            lista.push_back(nuevo);
            sort(lista.begin(), lista.end(), [](node* a, node* b) {
                return a->frecuencia < b->frecuencia;
            });
        }

        ARBOL = lista[0];
        generaCodigos(ARBOL);
        elementos = obtenerDiccNodos(ARBOL, elementos);
        codigos = obtenerMapaDeCodigos(elementos);
    }

    string codificar(const string& texto) {
        string huffman = "";
        for (char e : texto) {
            huffman += codigos[string(1, e)];
        }
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

    map<pair<string, string>, int> obtenerAristas() {
        return obtenerVertices(ARBOL, nullptr);
    }
};