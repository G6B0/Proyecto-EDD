#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

std::string leerArchivo(const std::string& rutaArchivo) {
    ifstream archivo(rutaArchivo);
    stringstream buffer;
    buffer << archivo.rdbuf();

    string contenido(buffer.str());
    contenido.erase(remove(contenido.begin(), contenido.end(), '\n'), contenido.end());

    return contenido;
}
