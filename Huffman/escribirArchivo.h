#pragma once

#include <fstream>
#include <string>
#include <iostream>

void escribirArchivo(const std::string& rutaArchivo, const std::string& contenido) {
    std::ofstream archivo(rutaArchivo);
    if (archivo.is_open()) {
        archivo << contenido;
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para escribir: " << rutaArchivo << std::endl;
    }
}
