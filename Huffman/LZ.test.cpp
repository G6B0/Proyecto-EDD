#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "leerArchivo.h"

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

int main() {
string rutaArchivoOriginal = "english_12MB.txt";

size_t tamanioEnBytes = 100000; 

string text = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);

for(int i=0; i < 10 ;i++){
text=text+text;
}
if (text.empty()) {
    cerr << "Error al leer el archivo o archivo vacío." << endl;
    return 1;
}

size_t textoOriginalTamaño = text.length();

LempelZiv::CompressedData compressedData = LempelZiv::compress(text);


cout << "Texto original (caracteres): " << textoOriginalTamaño << endl;
cout << "Texto comprimido: " << compressedData.size() * sizeof(pair<int, int>) << " bytes" << endl;

string decompressedText = LempelZiv::decompress(compressedData);
cout << "Texto descomprimido (caracteres): " << decompressedText.size() << endl;


if (text == decompressedText) {
    cout << "La descompresión ha sido exitosa." << endl;
} else {
    cout << "La descompresión ha fallado." << endl;
}

return 0;
}