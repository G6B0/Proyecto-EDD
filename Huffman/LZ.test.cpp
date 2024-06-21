#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "leerArchivo.h"
#include <chrono>

using namespace std::chrono;
using namespace std;

typedef unsigned long long uint64;

class LempelZiv {
public:
typedef vector<pair<int, int>> CompressedData;

static CompressedData compress(const string &input) {
    unordered_map<char, set<int>> charPositions;
    CompressedData compressed;

    int inputLength = input.length();
    for (int i = 0; i < inputLength;) {
        char currentChar = input[i];
        set<int> &positions = charPositions[currentChar];

        if (positions.empty()) {
            positions.insert(i);
            compressed.emplace_back(currentChar, 0);
            i++;
            continue;
        }

        int chosenPosition = 0;
        int maxLength = 0;

        for (int position : positions) {
            int length = 1;
            while (i + length < inputLength && 
                   charPositions[input[i + length]].count(position + length)) {
                length++;
            }

            if (length > maxLength) {
                maxLength = length;
                chosenPosition = position;
            }
        }

        positions.insert(i);
        for (int j = 1; j < maxLength; ++j) {
            charPositions[input[i + j]].insert(i + j);
        }

        compressed.emplace_back(chosenPosition, maxLength);
        i += maxLength;
    }

    return compressed;
}

static string decompress(const CompressedData &compressed) {
    string decompressed;

    for (const auto &[position, length] : compressed) {
        if (length == 0) {
            decompressed += char(position);
        } else {
            decompressed += decompressed.substr(position, length);
        }
    }

    return decompressed;
}

};

void test(const string& text, int n_veces, size_t tamaño_txtoriginal, ofstream &outFile, ofstream &outFile1,  ofstream &outFile2 ) {

	if(!outFile){

		std::cerr << "No se pudo abrir el archivo 1 para escritura." << endl;

		return;

	}

    else if(!outFile1){

		std::cerr << "No se pudo abrir el archivo 2 para escritura." << endl;

		return;

	}

    for (int i = 0; i < n_veces; i++) {

    	

        auto start = high_resolution_clock::now();

        LempelZiv::CompressedData compressedData = LempelZiv::compress(text);

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);

        size_t memoria_usada_compresion = compressedData.size() * sizeof(pair<int, int>);

        outFile << i + 1 << "," << duration.count() << "," << tamaño_txtoriginal << endl;
        outFile2 << i + 1 << "," << memoria_usada_compresion << "," << tamaño_txtoriginal << endl;




        auto start_decoding = high_resolution_clock::now();

        string decompressedText = LempelZiv::decompress(compressedData);

        auto stop_decoding = high_resolution_clock::now();

        auto duration_decoding = duration_cast<microseconds>(stop_decoding - start_decoding);

        outFile1 << i + 1 << "," << duration_decoding.count() << "," << tamaño_txtoriginal << endl;

         if (text == decompressedText) {
        cout << "La descompresión ha sido exitosa." << endl;
         } else {
        cout << "La descompresión ha fallado." << endl;
         }

    }

  }

int main() {
    string rutaArchivoOriginal = "english_12MB.txt";

    size_t tamanioEnBytes = 80000; 

    string text = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);

     if (text.empty()) {
        cerr << "Error al leer el archivo o archivo vacío." << endl;
        return 1;
    }

    ofstream LZ77_Compresion("Tiempo_ejecucion_compresion_LZ.csv");
	if (!LZ77_Compresion.is_open()) {
        cerr << "No se pudo abrir el archivo Tiempo_ejecucion_compresion_LZ.csv para escritura." << endl;
        return 1;
    }
    LZ77_Compresion << "cant_experimento " << "," << "tiempo_promedio" << "," << "Tamaño Texto Original (B)" << endl;

    ofstream LZ77_Decompresion("Tiempo_ejecucion_Decompresion_LZ.csv");
	if (!LZ77_Decompresion.is_open()) {
        cerr << "No se pudo abrir el archivo Tiempo_ejecucion_Decompresion_LZ.csv para escritura." << endl;
        return 1;
    }
    LZ77_Decompresion << "cant_experimento " << "," << "tiempo_promedio" << "," << "Tamaño Texto Original (B)" << endl;

    ofstream LZ77_Espacio_Utilizado("Espacio_Utilizado_LZ.csv");
	if (!LZ77_Espacio_Utilizado.is_open()) {
        cerr << "No se pudo abrir el archivo Espacio_Utilizado_LZ.csv para escritura." << endl;
        return 1;
    }
    LZ77_Espacio_Utilizado << "cant_experimento " << "," << "Espacio Utizado en Compresion (B)" << "," << "Espacio Utilizado Original en MB" << endl;
    

    int n_veces = 1 ;
    for(int i = 0; i < 7; i++) { 
        text += text;
        size_t textoOriginalTamaño = text.length();
        test(text, n_veces, textoOriginalTamaño, LZ77_Compresion, LZ77_Decompresion, LZ77_Espacio_Utilizado);
        
    }
    return 0;
}