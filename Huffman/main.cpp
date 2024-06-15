#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include "Huffman.h"
#include "leerArchivo.h"

using namespace std;
using namespace std::chrono;

bool verificarCodificacion(Huffman& huffman, const string& textoOriginal) {
    string textoCodificado = huffman.codificar(textoOriginal);
    string textoDecodificado = huffman.decodificar(textoCodificado);
    return textoOriginal == textoDecodificado;
}

int main() {
    string rutaArchivoOriginal = "english_12MB.txt";
    string nombreArchivoCSV = "resultados.csv";
    vector<size_t> tamañosEnMB = {1, 3, 5, 7, 10};
    int numRepeticiones = 20;

    // Escribir la cabecera en el archivo CSV
    ofstream archivoCSV(nombreArchivoCSV);
    if (!archivoCSV.is_open()) {
        cerr << "No se pudo abrir el archivo CSV para escribir la cabecera." << endl;
        return 1;
    }
    archivoCSV << "cant_experimento;estructura_dato;tipo_consulta;tamaño_en_MB;tiempo_promedio\n";
    archivoCSV.close();

    for (size_t tamañoMB : tamañosEnMB) {
        size_t tamanioEnBytes = tamañoMB * 1024 * 1024; // Convertir MB a bytes
        string textoOriginal = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);

        if (textoOriginal.empty()) {
            cerr << "Error al leer el archivo para el tamaño de " << tamañoMB << " MB." << endl;
            continue;
        }

        Huffman huffman;
        huffman.construirArbol(textoOriginal);

        vector<long long> tiemposCodificacion;
        vector<long long> tiemposDecodificacion;

        for (int i = 0; i < numRepeticiones; i++) {
            auto inicioCodificacion = high_resolution_clock::now();
            string textoCodificado = huffman.codificar(textoOriginal);
            auto finCodificacion = high_resolution_clock::now();

            auto inicioDecodificacion = high_resolution_clock::now();
            string textoDecodificado = huffman.decodificar(textoCodificado);
            auto finDecodificacion = high_resolution_clock::now();

            auto duracionCodificacion = duration_cast<microseconds>(finCodificacion - inicioCodificacion).count();
            auto duracionDecodificacion = duration_cast<microseconds>(finDecodificacion - inicioDecodificacion).count();

            tiemposCodificacion.push_back(duracionCodificacion);
            tiemposDecodificacion.push_back(duracionDecodificacion);

            if (!verificarCodificacion(huffman, textoOriginal)) {
                cout << "La codificación y decodificación fallaron en la repetición " << i + 1 << " para " << tamañoMB << " MB." << endl;
            }
        }

        long long sumaCodificacion = 0;
        long long sumaDecodificacion = 0;

        for (int i = 0; i < numRepeticiones; i++) {
            sumaCodificacion += tiemposCodificacion[i];
            sumaDecodificacion += tiemposDecodificacion[i];
        }

        double promedioCodificacion = static_cast<double>(sumaCodificacion) / numRepeticiones;
        double promedioDecodificacion = static_cast<double>(sumaDecodificacion) / numRepeticiones;
        
        // Escribir los resultados en el archivo CSV
        archivoCSV.open(nombreArchivoCSV, ios::app);
        if (!archivoCSV.is_open()) {
            cerr << "No se pudo abrir el archivo CSV para escribir los resultados." << endl;
            return 1;
        }
        archivoCSV<<numRepeticiones<<";"<<"Huffman"<<";"<<"codificacion"<<";"<<tamañoMB<<";"<< promedioCodificacion <<endl;
        archivoCSV<<numRepeticiones<<";"<<"Huffman"<<";"<<"decodificacion"<<";"<<tamañoMB<<";"<<promedioDecodificacion <<endl;;
        archivoCSV.close();
    }

    return 0;
}
