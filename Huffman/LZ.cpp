#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include "leerArchivo.h"

using namespace std::chrono;

using namespace std;

struct code{
	int o, l;
	char c;
};

size_t memoria_usada(const vector<code>& v){
	return v.size()* sizeof(code);
}

// Función para convertir bytes a KB y MB
string convertir_tamaño(size_t size) {
    return to_string(size / (1e6));
}

code find(string lab, string sb, int labs, int sbs){

	code sol;
	sol.o=0;
	sol.l= 0;
	sol.c= lab[0];
	
	for(int j=sb.size()-1; j>=0; j--){
		if(lab[0]== sb[j]){
			int k=1, z= j+1, x= 0;
			while(k<labs && z<sb.size() && lab[k]== sb[z]){
				k++;
				z++;
			}
			if(k<labs && z==sb.size()){
				while(k<labs && lab[x]== lab[k]){
					x++;
					k++;
				}
			}
			code pos;
			pos.l= k;//prefix
			pos.o= sb.size()- j;//offset
			if(k==labs){
				pos.c=' ';//blank
			}else{
				pos.c= lab[k];
			}
			if(pos.l> sol.l || (pos.l== sol.l && pos.o< sol.o)){
				sol.l= pos.l;
				sol.o= pos.o;
				sol.c= pos.c;
			}
		}
	}
	return sol;
}


vector<code> encoding(string text, int labs, int sbs){
	string lab, sb="";
	int i=0; 
	vector<code> v;
	while(i<text.size()){
		lab= text.substr(i, labs);
		int sz= lab.size();
		code sol= find(lab, sb, min(labs, sz), sbs);
		v.push_back(sol);
		int ptr= sol.l+1;
		i+= ptr;
		if(sb.size()==sbs){
			if(ptr<sbs){
				sb= sb.substr(ptr)+ lab.substr(0, ptr);
			}else{
				sb= lab.substr(0, ptr);
			}
		}else{
			if(ptr+sb.size()<= sbs){
				sb= sb+ lab.substr(0, ptr);
			}else if(ptr<sbs){
				sb= sb.substr(ptr+sb.size()- sbs)+ lab.substr(0, ptr);
			}else{
				sb= lab.substr(0, ptr);
			}
		}
		if(sb.size()>sbs)
			sb= sb.substr(sb.size()- sbs);
	}
	return v;
}

string decoding(vector<code> v){
	string tex= "";
	for(int i=0; i<v.size(); i++){
		if(v[i].o==0){
			tex+= v[i].c;
		}else if(v[i].o<v[i].l){
			string temp= "";
			for(int j=0; j<v[i].l/v[i].o; j++){
				temp+= tex.substr(tex.size()-v[i].o, v[i].o);
			}
			temp+= tex.substr(tex.size()-v[i].o, v[i].l%v[i].o)+ v[i].c;
			tex+= temp;
		}else{
			tex= tex+ tex.substr(tex.size()-v[i].o, v[i].l)+ v[i].c;
		}
	}
	return tex;
}

void test(const string& text, int labs, int sbs, int n_veces, ofstream &outFile, size_t tamaño_txtoriginal) {
    vector<long long> tiemposCodificacion;
    vector<long long> tiemposDecodificacion;
    size_t memoriaUsada;
    if (!outFile) {
        std::cerr << "No se pudo abrir el archivo para escritura." << endl;
        return;
    }

    for (int i = 0; i < n_veces; i++) {
        auto start = high_resolution_clock::now();
        vector<code> op = encoding(text, labs, sbs);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        tiemposCodificacion.push_back(duration.count());

        size_t memoria = memoria_usada(op);
        memoriaUsada = memoria;

        auto start_decoding = high_resolution_clock::now();
        string tex = decoding(op);
        auto stop_decoding = high_resolution_clock::now();
        auto duration_decoding = duration_cast<microseconds>(stop_decoding - start_decoding);
        tiemposDecodificacion.push_back(duration_decoding.count());

        int LongitudMin = std::min(text.size(), tex.size());
        int LongitudMax = std::max(text.size(), tex.size());

        int contador = 0;

        for (int i = 0; i < LongitudMin; ++i) {
            if (text[i] == tex[i]) {
                contador++;
            }
        }
        double Porcentaje_Igualdad = (static_cast<double>(contador) / LongitudMax) * 100.0;
        if (Porcentaje_Igualdad < 90) {
            outFile << i + 1 << "," << "LZ77" << "," << "descomprimir" << "," << "descomprimir bajo 90 porciento de eficiencia" << endl;
            return;
        }
    }

    long long sumaCodificacion = 0;
    long long sumaDecodificacion = 0;
    for(int i=0;i<tiemposCodificacion.size();i++){
        sumaCodificacion+=tiemposCodificacion[i];
        sumaDecodificacion+=tiemposDecodificacion[i];
    }
    double promedioCodificacion = static_cast<double>(sumaCodificacion) / n_veces;
    double promedioDecodificacion = static_cast<double>(sumaDecodificacion) / n_veces;

    outFile<<n_veces<<";"<<"LZ77"<<";"<<"comprimir"<<";"<<convertir_tamaño(tamaño_txtoriginal)<<";"<<promedioCodificacion<<";"<< convertir_tamaño(memoriaUsada)<<endl;
    outFile<<n_veces<<";"<<"LZ77"<<";"<<"descomprimir"<<";"<<convertir_tamaño(tamaño_txtoriginal) <<";"<<promedioDecodificacion<<";"<< convertir_tamaño(memoriaUsada)<<endl;
}


int main(){
	int ws, labs, sbs;
	string rutaArchivoOriginal = "english_12MB.txt";
	//32768  Tamaño de la ventana de 32 KB con buenos resultados
	//16000 Tamaño de la ventana da memoria muy alta pero tiempos mas cortos
	cout<<"Ingrese el tamaño de la Ventana: ";
	cin>>ws;
	//4096 Tamaño del buffer de búsqueda de 4 KB con buenos resultados
	//16000 Tamaño del buffer da memoria muy alta pero tiempos mas cortos

	cout<<"Ingrese el tamaño del Buffer: ";
	cin>>labs;
	sbs= ws-labs;
    size_t tamanioEnBytes = 100000; // Convertir MB a bytes
    string text = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);

    
	size_t textoOriginalTmaño = text.length();
    vector<code> op = encoding(text, labs, sbs);

     // Mostrar resultados
    cout << "Texto original (EN bytes): " << text.size() << " caracteres\n";
    cout << "Texto comprimido: " << op.size()*9 << " triplets\n";


	/*ofstream LZ77("LZ77.csv");
	if (!LZ77.is_open()) {
        cerr << "No se pudo abrir el archivo codificar.csv para escritura." << endl;
        return 1;
    }

    LZ77<<"cant_experimento"<<";"<<"estructura_dato"<<";"<<"tipo_consulta"<<";"<<"tamaño_ingreasado_en_MB"<<";"<<"tiempo_promedio"<<";"<<"espacio_ocupado_en_MB"<<endl;

	vector<size_t> tamañosEnMB = {1,3,5,7,10};

	 for (size_t tamañoMB : tamañosEnMB) {
        size_t tamanioEnBytes = tamañoMB * 1e6; // Convertir MB a bytes
        string text = leerArchivo(rutaArchivoOriginal, tamanioEnBytes);
		size_t textoOriginalTmaño = text.length();

        if (text.empty()) {
            cerr << "No se pudo leer el archivo de texto." << endl;
            continue;
        }

        test(text, labs, sbs, 20, LZ77 ,textoOriginalTmaño);

    }
	LZ77.close();*/
	return 0;
}