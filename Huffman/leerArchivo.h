#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string leerArchivo(const string& rutaArchivo, size_t tamanioEnBytes) {
    ifstream archivo(rutaArchivo, ios::in | ios::binary);
    if (!archivo) {
        cerr << "No se pudo abrir el archivo: " << rutaArchivo << endl;
        return "";
    }

    // Asegurarse de que el archivo se posiciona al inicio
    archivo.seekg(0, ios::beg);

    // Leer los primeros tamanioEnBytes del archivo
    string contenido(tamanioEnBytes, '\0');
    archivo.read(&contenido[0], tamanioEnBytes);

    // Ajustar el tamaño de la cadena leída si se leyeron menos bytes
    contenido.resize(archivo.gcount());

    // Quitar los caracteres de nueva línea
    contenido.erase(remove(contenido.begin(), contenido.end(), '\n'), contenido.end());

    return contenido;
}
