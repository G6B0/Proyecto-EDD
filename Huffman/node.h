#pragma once
#include <string>

using namespace std;

class node {
public:
    string nombre;        // nombre del string 
    int frecuencia;      // frecuencia
    node* izquierda;     // puntero al nodo hijo izquierdo
    node* derecha;       // puntero al nodo hijo derecho
    string codigo;       // codigo de huffman asociado al nodo

    // Constructor
    node(string nombre, int frecuencia, node* izquierda = nullptr, node* derecha = nullptr) {
        this->nombre = nombre;
        this->frecuencia = frecuencia;
        this->izquierda = izquierda;
        this->derecha = derecha;
        this->codigo = "";
    }

};