#include<bits/stdc++.h>
#include <chrono>

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

void test(const string& text, int labs, int sbs, int n_veces) {
    for (int i = 0; i < n_veces; i++) {
        auto start = high_resolution_clock::now();
        vector<code> op = encoding(text, labs, sbs);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);

        size_t memoria = memoria_usada(op);

        cout << "Test " << i + 1 << ":\n";
        cout << "Encoding Time: " << duration.count() << " microseconds\n";
        cout << "Memory Used: " << memoria << " bytes\n";

        auto start_decoding = high_resolution_clock::now();
        string tex = decoding(op);
        auto stop_decoding = high_resolution_clock::now();
        auto duration_decoding = duration_cast<microseconds>(stop_decoding - start_decoding);

        cout << "Decoder Output: " << tex << endl;
        cout << "Decoding Time: " << duration_decoding.count() << " microseconds\n";
        cout << "-----------------------------------------\n";
    }
}


int main(){
	int ws, labs, sbs;
	cout<<"Ingrese el tamaño de la Ventana";
	cin>>ws;
	cout<<"Ingrese el tamaño del Buffer";
	cin>>labs;
	sbs= ws-labs;
	cin.ignore();
	string text;
	cout<<"Enter text: ";
	getline(cin,text);

	int n_veces;
    cout << "Ingrese el numero de test a realizar";
    cin >> n_veces;

    test(text, labs, sbs, n_veces);
    test(text, labs, sbs, n_veces);
    test(text, labs, sbs, n_veces);
    test(text, labs, sbs, n_veces);
    test(text, labs, sbs, n_veces);

	return 0;

}