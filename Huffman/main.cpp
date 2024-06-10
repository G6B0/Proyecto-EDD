#include "Huffman.h"

#include <iostream>
#include <fstream>

int main() {
    Huffman huffman;
    string cadena = "tangananica-tanganana";

    huffman.construirArbol(cadena);

    string codificado = huffman.codificar(cadena);
    cout << "Codigo Huffman: " << codificado << endl;

    string decodificado = huffman.decodificar(codificado);
    cout << "Mensaje original: " << decodificado << endl;

    return 0;
}