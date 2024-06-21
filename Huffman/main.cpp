#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include "Huffman.h"
#include "leerArchivo.h"

using namespace std;
using namespace std::chrono;

bool verificarCodificacion(Huffman& huffman, const string& textoOriginal) {
    vector<uint32_t> textoCodificado = huffman.codificar(textoOriginal);
    string textoDecodificado = huffman.decodificar(textoCodificado);
    return textoOriginal == textoDecodificado;
}
    /*string rutaArchivoOriginal = "english_12MB.txt";
    string nombreArchivoCSV = "resultados.csv";
    vector<size_t> tamañosEnMB = {1, 3, 5, 7, 10};
    int numRepeticiones = 20;
    size_t tamanioEnBytes = 10 * 1e6;

    try {
        // Leer el archivo
        string textoOriginal = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);
        size_t textoOriginalTamaño = textoOriginal.length();

        Huffman huffman;


        vector<uint32_t> codificado = huffman.codificar(textoOriginal);


        cout << "Espacio del string original: " << textoOriginalTamaño << " bytes" << endl;
        cout << "Espacio usado codificado" << codificado.size()*sizeof(uint32_t) << " bytes" << endl;


        string textoDecodificado = huffman.decodificar(codificado);
        size_t tamañoOriginalBytes = textoOriginalTamaño * sizeof(char);
        size_t tamañoCodificadoBytes = codificado.size()*sizeof(uint32_t);


        double reducciónPorcentaje = ((double)(tamañoOriginalBytes - tamañoCodificadoBytes) / tamañoOriginalBytes) * 100;


        if (textoOriginal == textoDecodificado) {
            cout << "La decodificación fue exitosa, se redujo un: "<< reducciónPorcentaje<<"%"<<endl;
        } else {
            cout << "La decodificación falló." << endl;
        }
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }*/

int main() {

    string rutaArchivoOriginal = "english_12MB.txt";
    size_t tamanioEnBytes = 80000; 
    string nombreArchivoCSV = "resultados.csv";
    string textoOriginal = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);

    std::ofstream csvFile(nombreArchivoCSV);
    if (!csvFile.is_open()) {
        cerr << "No se pudo abrir el archivo CSV para escribir la cabecera." << endl;
        return 1;
    }
    csvFile << "cant_experimento;estructura_dato;tipo_consulta;tamaño_ingreasado_en_MB;tiempo_promedio;espacio_salida_en_MB"<<std::endl;

    for(int i=0;i<8;i++) {
        size_t textoOriginalTamaño = textoOriginal.length();
        cout << "Tamaño del texto original: " << textoOriginalTamaño << " caracteres" << endl;

        if (textoOriginal.empty()) {
            cerr << "Error al leer el archivo para el tamaño de " << textoOriginalTamaño << " MB." << endl;
            continue;
        }

        vector<long long> tiemposCodificacion;
        vector<long long> tiemposDecodificacion;
        long long sumaCodificacion = 0;
        long long sumaDecodificacion = 0;
        size_t sizeCodificado;
        size_t sizeDecodificado;

        for (int i = 0; i < 20; i++) {
            Huffman huffman;
            auto inicioCodificacion = high_resolution_clock::now();
            vector<uint32_t> codificacion = huffman.codificar(textoOriginal);
            auto finCodificacion = high_resolution_clock::now();

            sizeCodificado= codificacion.size()*sizeof(uint32_t);

            auto inicioDecodificacion = high_resolution_clock::now();
            string textoDecodificado = huffman.decodificar(codificacion);
            auto finDecodificacion = high_resolution_clock::now();

            sizeDecodificado= textoDecodificado.length();

            auto duracionCodificacion = duration_cast<microseconds>(finCodificacion - inicioCodificacion).count();
            auto duracionDecodificacion = duration_cast<microseconds>(finDecodificacion - inicioDecodificacion).count();

            tiemposCodificacion.push_back(duracionCodificacion);
            tiemposDecodificacion.push_back(duracionDecodificacion);

            if (!verificarCodificacion(huffman, textoOriginal)) {
                cout << "La codificación y decodificación fallaron en la repetición " << i + 1 << " para " << textoOriginalTamaño << " MB." << endl;
            }
        }
        

        for(long long tiempo:tiemposCodificacion){
            sumaCodificacion+=tiempo;
            
        }
        for(long long tiempo:tiemposDecodificacion){
            sumaDecodificacion+=tiempo;
        }

        double promedioCodificacion = static_cast<double>(sumaCodificacion) / 20;
        double promedioDecodificacion = static_cast<double>(sumaDecodificacion) / 20;

        // Calcular el tamaño codificado en MB
        double espacioCodificadoMB = static_cast<double>(sizeCodificado) / 1e6;
        double espacioOriginalMB = static_cast<double>(textoOriginalTamaño) / 1e6;
        double espacioDecodificado = static_cast<double>(sizeDecodificado) / 1e6;

        csvFile<<20<<";"<<"Huffman"<<";"<<"codificacion"<<";"<<espacioOriginalMB<<";"<<promedioCodificacion<<";"<<espacioCodificadoMB <<std::endl;

        csvFile<<20<<";"<<"Huffman"<<";"<<"decodificacion"<<";"<< espacioCodificadoMB<<";"<<promedioDecodificacion<<";"<< espacioDecodificado<<std::endl;

        textoOriginal += textoOriginal; 
    }
csvFile.close();   

    return 0;
}
