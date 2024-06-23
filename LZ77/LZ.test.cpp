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

class LempelZiv {
public:
    // Definición del tipo CompressedData como un vector de pares (posición, longitud)
    typedef vector<pair<int, int>> CompressedData;

    /** Se inicia un diccinario que almacenara las posiciones de cada caracter, leyendo cada caracter del texto a comprimir}
      * Despues por cada caracter actual se busca la subcadena mas larga que ya ha aparecido antereriormente en la cadena original
      * esto se hace verificando las posiciones del diccinario
      * En caso de que se encuentre una subcadena se almacena se referencia la posicion actual y su respectiva longitud 
      * Sino se encuentra ninguna coincidencia se almacena solo el caracter individual
       */
    static CompressedData compress(const string &input) {
        unordered_map<char, set<int>> charPositions; 
        CompressedData compressed; // Vector para almacenar los datos comprimidos

        int inputLength = input.length(); // Longitud de la cadena de entrada
        for (int i = 0; i < inputLength;) {
            char currentChar = input[i]; // Carácter actual en la posición i
            set<int> &positions = charPositions[currentChar]; // Conjunto de posiciones del carácter actual

            // Si no hay posiciones registradas para el carácter actual
            if (positions.empty()) {
                positions.insert(i); // Añadir la posición actual al conjunto
                compressed.emplace_back(currentChar, 0); // Añadir el carácter actual con longitud 0 al vector comprimido
                i++; // Avanzar a la siguiente posición
                continue;
            }

            int chosenPosition = 0; // Posición elegida para la referencia
            int maxLength = 0; // Longitud máxima encontrada

            // Iterar sobre las posiciones registradas del carácter actual
            for (int position : positions) {
                int length = 1; // Longitud inicial
                // Mientras no se salga del rango y los caracteres coincidan
                while (i + length < inputLength && 
                       charPositions[input[i + length]].count(position + length)) {
                    length++; // Incrementar la longitud
                }

                // Si la longitud encontrada es mayor que la máxima actual
                if (length > maxLength) {
                    maxLength = length; // Actualizar la longitud máxima
                    chosenPosition = position; // Actualizar la posición elegida
                }
            }

            positions.insert(i); // Añadir la posición actual al conjunto
            // Añadir las posiciones de los caracteres siguientes si es necesario
            for (int j = 1; j < maxLength; ++j) {
                charPositions[input[i + j]].insert(i + j);
            }

            compressed.emplace_back(chosenPosition, maxLength); // Añadir la referencia y la longitud al vector comprimido
            i += maxLength; // Avanzar la posición actual según la longitud encontrada
        }

        return compressed; // Retornar el vector de datos comprimidos
    }

    /**
     * Se descomprime reeconstruyendo con las subcadenas referencias o con el caracter individual(caso largo==0)
     */
    static string decompress(const CompressedData &compressed) {
        string decompressed; // Cadena para almacenar los datos descomprimidos

        // Iterar sobre los pares (posición, longitud) en el vector comprimido
        for (const auto &[position, length] : compressed) {
            if (length == 0) {
                decompressed += char(position); // Si la longitud es 0, añadir el carácter directamente
            } else {
                decompressed += decompressed.substr(position, length); // Si no, añadir la subcadena referenciada
            }
        }

        return decompressed; // Retornar la cadena descomprimida
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
  /**Los experimentos se explican en el informe */

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