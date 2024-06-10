#include <iostream>
#include "Huffman.h"
#include "leerArchivo.h"
#include "escribirArchivo.h"

using namespace std;


bool verificarCodificacion(Huffman& huffman, string& textoOriginal) {
    string textoCodificado = huffman.codificar(textoOriginal);
    string textoDecodificado = huffman.decodificar(textoCodificado);
    return textoOriginal == textoDecodificado;
}

int main() {
    string rutaArchivoOriginal = "english.50MB.txt";
    string textoOriginal = leerArchivo(rutaArchivoOriginal);

    Huffman huffman;
    huffman.construirArbol(textoOriginal);

    string textoCodificado = huffman.codificar(textoOriginal);
    
    string textoDecodificado = huffman.decodificar(textoCodificado);

    escribirArchivo("textoOriginal.txt", textoOriginal);
    escribirArchivo("textoCodificado.txt", textoCodificado);
    escribirArchivo("textoDecodificado.txt", textoDecodificado);

    if (verificarCodificacion(huffman, textoOriginal)) {
        std::cout << "La codificación y decodificación son correctas." << std::endl;
    } else {
        std::cout << "La codificación y decodificación fallaron." << std::endl;
    }

    return 0;
}