//CREATEAD BY ANDIKA RAHIM D

#include <bits/stdc++.h>
#define mp make_pair 

using namespace std;


//menghitung miu detak jantung
/*index 0 = dibawah normal
  index  1 = normal
  index 2 = diatas normal*/
vector<double> MiuDetakJantung(double x){
	vector<double> miu(3);
	//dibawah normal
	if (x <= 60) miu[0]=1;
	else if (x <= 96) miu[0] = (96-x)/36.0;
	else miu[0] = 0;

	//normal
	if (x < 60 || x > 192) miu[1]=0;
	else if (60 <= x && x < 96) miu[1] = (x-60)/36.0;
	else if (96 <= x && x <= 170) miu[1] = 1;
	else miu[1] = (192-x)/22.0;

	//diatas normal
	if (x <= 96) miu[2]=0;
	else if (x <= 192) miu[2] = (x-96)/96.0;
	else miu[2] = 1;

	return miu;
}

//menghitung miu suhu badan
/*index 0 = rendah
  index  1 = normal
  index 2 = tinggi*/
vector<double> MiuSuhuBadan(double x){
	vector<double> miu(3);
	//rendah
	if (x <= 30) miu[0]=1;
	else if (x <= 36.5) miu[0] = (36.5-x)/6.5;
	else miu[0] = 0;

	//normal
	if (x <= 30 || x > 40) miu[1]=0;
	else if (30 <= x && x < 36.5) miu[1] = (x-30)/6.5;
	else if (36.5 <= x && x <= 37.5) miu[1] = 1;
	else miu[1] = (40-x)/2.5;

	//tinggi
	if (x < 36.5) miu[2]=0;
	else if (x <= 37.5) miu[2] = (x-36.5)/1.0;
	else miu[0] = 1;
	return miu;
}

//kaidah aturan.
//Detak Jantung
//0 : dibawah normal
//1 : normal
//2 : diatas normal

//Suhu Badan
//0 : rendah
//1 : normal
//2 : tinggi

//status kesehatan
//0 kurang sehat
//1 sehat


//set kaidah aturan
//contoh maps : (0,1) berarti detak jantung dibawah normal dan suhu badan normal
//maka nilai mapsnya adalah 1 yaitu sehat (status[(0,1)]=1)
map<pair<int,int> , int>  status;
void setKaidahAturan() {
	status[mp(0,0)] = 0;
	status[mp(0,1)] = 1;
	status[mp(0,2)] = 0;
	status[mp(1,0)] = 0;
	status[mp(1,1)] = 1;
	status[mp(1,2)] = 0;
	status[mp(2,0)] = 0;
	status[mp(2,1)] = 1;
	status[mp(2,2)] = 0;
}

//mengembalikan nilai dari alfa predikat dan value dari 
//nilai defuzifer dari alfa predikat tersebut.
//contoh : (0,1) -> 1. miu jatung dibawah normal 0.2 dan miu suhu normal=1
//maka alfa predikat menggunakan t-norm adalah miu status kesehatan(sehat) bernilai 0.2
//dalam rentang 0-100, o.2 akan bernilai  74.
//maka fungsi ini akan mengembalikan sepasang bilangan (0.2,74)
pair<double,double> defuzifikasi(int i, int j, double miudetakjantung, double miusuhubadan) {
	double alfaPredikat = min(miudetakjantung,miusuhubadan); //t-norm

	//kurang sehat
	if (status[mp(i,j)]==0) {
		if (alfaPredikat==1) {
			return (mp(alfaPredikat,25));
		}
		else if (alfaPredikat==0){
			return (mp(alfaPredikat,90));
		}
		else {
			return (mp(alfaPredikat,80-alfaPredikat*30));
		}
	}

	//sehat
	else {
		if (alfaPredikat==0) {
			return (mp(alfaPredikat,25));
		}
		else if (alfaPredikat==1){
			return (mp(alfaPredikat,90));
		}
		else {
			return (mp(alfaPredikat,alfaPredikat*30+50));
		}
	}
}

int main() {
	cout<<"SISTEM HEALTH CHECKING"<<endl;
	cout<<"######################"<<endl;
	cout<<endl;
	
	//input suhu Badan dan detak jantung
	double suhuBadan, detakJantung;
	cout<<"Masukkan suhu badan : ";
	cin >> suhuBadan;
	cout<<"Masukkan detakJantung : ";
	cin >> detakJantung;

	setKaidahAturan();

	vector<double> miusuhubadan = MiuSuhuBadan(suhuBadan); //hitung miu
	vector<double> miudetakjantung = MiuDetakJantung(detakJantung); //hitung miu

	vector<pair<double,double> > statusKesehatan; //vector untuk menyimpan alfa predikat dan value setelah didefuzifikasi
	double sumAlfaPredikat = 0; // penyebut dari formula tsukamoto
	double ValueStatusKesehatan = 0; //pembilang dari formula tsukamoto

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			pair<double,double> ValueDefuzzy = defuzifikasi(i,j,miudetakjantung[i],miusuhubadan[j]);

			//tsukamoto
			sumAlfaPredikat += ValueDefuzzy.first; //first adalah alfapredikat
			ValueStatusKesehatan += ValueDefuzzy.first*ValueDefuzzy.second; // second adalah nilai defuzzy
		}
	}
	
	//Tsukamoto lanjutan : rata rata terbobot
	ValueStatusKesehatan /= sumAlfaPredikat;

	cout<<"Tingkat Kesehatan anda adalah "<<ValueStatusKesehatan<<endl;
}