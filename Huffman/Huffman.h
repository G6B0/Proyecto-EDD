#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <algorithm>
#include <bitset>
#include <stdexcept>

#include "compareNode.h"
#include "node.h"

using namespace std;
/*Codigo sacado de:  https://gist.github.com/pepgonzalez/5360217 */

/**
 * Clase Huffman tendrá un node que sera el arbol Huffman construido
 * una tabla hash de elementos, guardara el nombre asociado a cada nodo
 * codigos, tendra cada secuencia de bits de cada caracter
 */
class Huffman {
private:
    node* ARBOL;
    unordered_map<string, node*> elementos;
    unordered_map<string, string> codigos;
    size_t tamañoCodificado;

    /**Almacena la frecuencia de cada caracter de un string ingresado y los guarda en una
     * tabla hash
     */
    unordered_map<char, int> obtenerFrecuencias(const string& cadena) {
        unordered_map<char, int> f;
        for (char e : cadena) {
            f[e]++;
        }
        return f;
    }
    /**Combina un nuevo nodo, sumando sus frecuencias, combinando sus nombres(esto util para diferencia de hoja y no hoja)
     * ademas de fija el codigo para el nodo izquierdo que seria 0 y derecho 0(por definicion)
     */
    node* generaNuevoNodo(node* n1, node* n2) {
        string nombre = n1->nombre + n2->nombre;
        int suma = n1->frecuencia + n2->frecuencia;
        n1->codigo = '0';
        n2->codigo = '1';
        node* nuevo = new node(nombre, suma, n1, n2);
        return nuevo;
    }
    /*Genera los codigos de cada nodo recursivamente
    esto se hace concatenando el nodo padre con su hijo(0 o 1) */
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
 * guarda cada nodo del arbor en una tabla hash, con clave el nombre del nodo
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

    /**
     * Construye el arbol Huffman con ayuda de una cola de prioridad
     * Genera los codigos correspondientes y los almacena en una tabla hash
     */
void construirArbol(const string& cadena) {
    unordered_map<char, int> frec = obtenerFrecuencias(cadena); // O(length(cadena))

    priority_queue<node*, vector<node*>, CompareNode> pq; //cola de prioridad de mayor prioridad la menores frecuencia

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

/*Convierte a enteros de 32 bits sin signo, se divide la secuendia de bits codificada
en fragmentos de 32 de largo, si es menor se rellena con 0´s */

vector<uint32_t> codificarAEnteros(const string& bitString) {
        vector<uint32_t> enteros;

        for (size_t i = 0; i < bitString.size(); i += 32) {
            string fragmento = bitString.substr(i, 32);
            while (fragmento.length() < 32) {
                fragmento += '0';
            }
            uint32_t entero = bitset<32>(fragmento).to_ulong();
            enteros.push_back(entero);
        }

        return enteros;
    }
    /**
     * decodifica los enteros del vector a secuencia de bits en sring de longitudBits
     * para luego descomprimirlo y devolverlo al string original
     */
    string decodificarDesdeEnteros(const vector<uint32_t>& enteros, size_t longitudBits) {
        string bitString = "";
        for (uint32_t entero : enteros) {
            bitString += bitset<32>(entero).to_string();
        }

        bitString = bitString.substr(0, longitudBits);
        return descomprime(ARBOL, bitString);
    }
    /**
     * Descomprime el arbol de huffman en forma de posicion(cada hoja se representa por el nombre.size()=1)
     * cuando se llega a una hoja se agrega el nombre, pues la secuencia de bits representa el camino unico hacia esa hoja
     */
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
/**
 * Primero construye el arbol huffman para luego codificar
 * Codifica el string ingresado en la secuencia de bits 
 * de secuencia de bits enteros de 32 bits almacenados en un vector
 */
    vector<uint32_t> codificar(const string& texto) {
        construirArbol(texto);
        string huffman = "";
        for (char e : texto) {
            huffman += codigos[string(1, e)];
        }
        this->tamañoCodificado=huffman.length();
        return codificarAEnteros(huffman);
    }

    /**Decoficar el vector de enteros a la secuencia de bits correspondiente al string original
     * Luego descomprime y lo transforma el string original 
     */

    string decodificar(vector<uint32_t> codificadoEnEnteros) {
        return decodificarDesdeEnteros(codificadoEnEnteros,tamañoCodificado);
    }

};