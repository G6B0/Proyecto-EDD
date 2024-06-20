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

// Función para escribir un archivo de texto
void escribirArchivo(const string& rutaArchivo, const string& contenido) {
    ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo para escribir.");
    }
    archivo << contenido;
    archivo.close();
}

// Función para escribir un archivo de texto con vector de enteros
void escribirArchivoEnteros(const string& rutaArchivo, const vector<uint32_t>& enteros) {
    ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo para escribir.");
    }
    for (uint32_t entero : enteros) {
        archivo << entero;
    }
    archivo.close();
}


int main() {
    string rutaArchivoOriginal = "english_12MB.txt";
    string nombreArchivoCSV = "resultados.csv";
    vector<size_t> tamañosEnMB = {1, 3, 5, 7, 10};
    int numRepeticiones = 20;
    size_t tamanioEnBytes = 10 * 1e6;

    try {
        // Leer el archivo
        string textoOriginal = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);
        size_t textoOriginalTamaño = textoOriginal.length();

        Huffman huffman;
        huffman.construirArbol(textoOriginal);


        string codificado = huffman.codificar(textoOriginal);


        cout << "Espacio del string original: " << textoOriginalTamaño << " bytes" << endl;
        cout << "Espacio de codificacion de bits en string: " << codificado.length() << " bytes" << endl;


        vector<uint32_t> enteros = huffman.codificarAEnteros(codificado);


        cout << "Espacio usado por vector de enteros: " << enteros.size()*sizeof(uint32_t) << " bytes" << endl;


        string textoDecodificado = huffman.decodificarDesdeEnteros(enteros, codificado.length());
        size_t tamañoOriginalBytes = textoOriginalTamaño * sizeof(char);
        size_t tamañoCodificadoBytes = enteros.size() * sizeof(uint32_t);


        double reducciónPorcentaje = ((double)(tamañoOriginalBytes - tamañoCodificadoBytes) / tamañoOriginalBytes) * 100;


        if (textoOriginal == textoDecodificado) {
            cout << "La decodificación fue exitosa, se redujo un: "<< reducciónPorcentaje<<"%"<<endl;
        } else {
            cout << "La decodificación falló." << endl;
        }

        // Escribir archivos de salida
        escribirArchivo("texto_original.txt", textoOriginal);
        escribirArchivo("texto_codificado_bits.txt", codificado);
        escribirArchivoEnteros("vector_enteros.txt", enteros);

    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    /*// Escribir la cabecera en el archivo CSV
    ofstream archivoCSV(nombreArchivoCSV);
    if (!archivoCSV.is_open()) {
        cerr << "No se pudo abrir el archivo CSV para escribir la cabecera." << endl;
        return 1;
    }
    archivoCSV << "cant_experimento;estructura_dato;tipo_consulta;tamaño_ingreasado_en_MB;tiempo_promedio;espacio_ocupado_en_MB\n";
    archivoCSV.close();

    for (size_t tamañoMB : tamañosEnMB) {
        size_t tamanioEnBytes = tamañoMB * 1e6; // Convertir MB a bytes
        string textoOriginal = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);
        size_t textoOriginalTamaño = textoOriginal.length();
        cout << "Tamaño del texto original: " << textoOriginalTamaño << " caracteres" << endl;

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
            vector<uint32_t> enteros = huffman.codificarAEnteros(textoCodificado);
            auto finCodificacion = high_resolution_clock::now();

            auto inicioDecodificacion = high_resolution_clock::now();
            string textoDecodificado = huffman.decodificarDesdeEnteros(enteros, textoCodificado.length());
            auto finDecodificacion = high_resolution_clock::now();

            auto duracionCodificacion = duration_cast<microseconds>(finCodificacion - inicioCodificacion).count();
            auto duracionDecodificacion = duration_cast<microseconds>(finDecodificacion - inicioDecodificacion).count();

            tiemposCodificacion.push_back(duracionCodificacion);
            tiemposDecodificacion.push_back(duracionDecodificacion);

            if (!verificarCodificacion(huffman, textoOriginal)) {
                cout << "La codificación y decodificación fallaron en la repetición " << i + 1 << " para " << tamañoMB << " MB." << endl;
            }
        }

        long long sumaCodificacion = accumulate(tiemposCodificacion.begin(), tiemposCodificacion.end(), 0LL);
        long long sumaDecodificacion = accumulate(tiemposDecodificacion.begin(), tiemposDecodificacion.end(), 0LL);

        double promedioCodificacion = static_cast<double>(sumaCodificacion) / numRepeticiones;
        double promedioDecodificacion = static_cast<double>(sumaDecodificacion) / numRepeticiones;

        // Calcular el tamaño original en bytes y el tamaño del vector de enteros en bytes
        size_t tamañoOriginalBytes = textoOriginalTamaño * sizeof(char);
        size_t tamañoCodificadoBytes = enteros.size() * sizeof(uint32_t);

        // Calcular el tamaño codificado en MB
        double espacioCodificadoMB = static_cast<double>(tamañoCodificadoBytes) / 1e6;
        double espacioOriginalMB = static_cast<double>(textoOriginalTamaño) / 1e6;

        // Calcular el porcentaje de reducción
        double porcentajeReduccion = 100.0 * (1.0 - static_cast<double>(tamañoCodificadoBytes) / tamañoOriginalBytes);

        // Escribir resultados en el archivo CSV
        archivoCSV.open(nombreArchivoCSV, ios::app);
        if (!archivoCSV.is_open()) {
            cerr << "No se pudo abrir el archivo CSV para escribir los resultados." << endl;
            return 1;
        }
        archivoCSV << numRepeticiones << ";" << "Huffman" << ";" << "codificacion" << ";" << espacioOriginalMB << ";" << promedioCodificacion << ";" << espacioCodificadoMB << "\n";
        archivoCSV << numRepeticiones << ";" << "Huffman" << ";" << "decodificacion" << ";" << espacioOriginalMB << ";" << promedioDecodificacion << ";" << espacioCodificadoMB << "\n";
        archivoCSV << "Reducción en porcentaje: " << porcentajeReduccion << "%\n";
        archivoCSV.close();

        // Escribir archivos de salida
        escribirArchivo("texto_original_" + to_string(tamañoMB) + "MB.txt", textoOriginal);
        escribirArchivo("texto_codificado_bits_" + to_string(tamañoMB) + "MB.txt", huffman.codificar(textoOriginal));
        escribirArchivoEnteros("vector_enteros_" + to_string(tamañoMB) + "MB.txt", huffman.codificarAEnteros(huffman.codificar(textoOriginal)));
    }*/

    return 0;
}
