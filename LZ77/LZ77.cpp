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

void test(const string& text, int labs, int sbs, int n_veces, ofstream &outFile, ofstream &outFile_1) {
	if(!outFile){
		std::cerr << "No se pudo abrir el archivo para escritura." << endl;
		return;
	}
	else if(!outFile_1){
		std::cerr << "No se pudo abrir el archivo para escritura." << endl;
		return;
	}
	

    for (int i = 0; i < n_veces; i++) {
        auto start = high_resolution_clock::now();
        vector<code> op = encoding(text, labs, sbs);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);

        size_t memoria = memoria_usada(op);

        

        outFile << "Test " << "Tiempo de Codificacion por compresion en ms " << "Memoria usada en bytes" << endl;
        outFile << i + 1 << "," << duration.count() << "," << memoria << endl;

        auto start_decoding = high_resolution_clock::now();
        string tex = decoding(op);
        auto stop_decoding = high_resolution_clock::now();
        auto duration_decoding = duration_cast<microseconds>(stop_decoding - start_decoding);

		int LongitudMin = std::min(text.size(), tex.size());
        int LongitudMax = std::max(text.size(), tex.size());

        int contador = 0;

        for (int i = 0; i < LongitudMin; ++i) {
        if (text[i] == tex[i]) {
            contador++;
            }
        }
        double Porcentaje_Igualdad = (static_cast<double>(contador) / LongitudMax) * 100.0;

        outFile_1 << "Test" << "," << "Tiempo de Decodificacion por Compresion en ms" << "," << "Porcentaje de igualdad con texto original" << endl;
        outFile_1 << i + 1 << "," << duration_decoding.count() << "," << Porcentaje_Igualdad << endl;

        
    }
}


int main(){
	int ws, labs, sbs;
	cout<<"Ingrese el tamaño de la Ventana: ";
	cin>>ws;
	cout<<"Ingrese el tamaño del Buffer: ";
	cin>>labs;
	sbs= ws-labs;
	
	//no funciona con textoOriginal.txt por que es demasiado grande
	//de momento con acrhivos de 10 mb funciona
	string text = leerArchivo("prueba.txt");
	if (text.empty()) {
        cerr << "No se pudo leer el archivo de texto." << endl;
        return 1;
    }

	ofstream Codificar("codificar.csv");
	if (!Codificar.is_open()) {
        cerr << "No se pudo abrir el archivo codificar.csv para escritura." << endl;
        return 1;
    }
	ofstream Decodificar("decodificar.csv");
	if (!Decodificar.is_open()) {
        cerr << "No se pudo abrir el archivo decodificar.csv para escritura." << endl;
        return 1;
    }

	int n_veces;
    cout << "Ingrese el numero de test a realizar";
    cin >> n_veces;

    test(text, labs, sbs, n_veces,Codificar,Decodificar);

	Codificar.close();
	Decodificar.close();

	return 0;

}