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
    archivo.seekg(0, ios::beg);

    string contenido(tamanioEnBytes, '\0');
    archivo.read(&contenido[0], tamanioEnBytes);

    contenido.resize(archivo.gcount());
    contenido.erase(remove(contenido.begin(), contenido.end(), '\n'), contenido.end());

    return contenido;
}
