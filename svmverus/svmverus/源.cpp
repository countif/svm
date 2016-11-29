#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#define FeatureNum 8
#define TotalLine 3133
#define TestLine 1044
using namespace std;
int y[5000];
double x[5000][FeatureNum];
double xpredict[5000][FeatureNum];
int  ypredict[5000];
double weight[FeatureNum];

string dict[3] = { "F", "M", "I" };
int TotalNum = 0;
int TestNum = 0;
int iteration = 1000;
double stepsize = 0.98;
double lamda = 0.000001;
double MaxValue[FeatureNum];
int a[TestLine];
int b[TestLine];
int c[TestLine];
double TotalWeight[3][FeatureNum+1];
int TestNumber[3] = { 707,669,712};

void getparameter(string path){
	ifstream in(path, ios::in);
	int index = 0;
	while (!in.eof())
	{
		in >> y[index];
		for (int i = 0; i < FeatureNum; ++i)
			in >> x[index][i];
		index++;
	}
	TotalNum = index - 1;
	for (int i = 0; i < FeatureNum; ++i)
		weight[i] = 0.0;
	in.close();
}


void MakeSimple(){

	for (int index = 0; index < FeatureNum; ++index)
	{
		MaxValue[index] = 0;
		for (int i = 0; i < TotalLine; ++i){
			if (MaxValue[index] < x[i][index])
				MaxValue[index] = x[i][index];

		}

	}
	for (int index = 0; index < FeatureNum; ++index)
	{
		for (int i = 0; i < TotalLine; ++i){
			x[index][i] /= MaxValue[index];
		}
	}


}


void MakePredictData(){
	for (int index = 0; index < FeatureNum; ++index)
	{
		for (int i = 0; i < TestLine; ++i){
			xpredict[index][i] /= MaxValue[index];
		}
	}

}
void GetPredictData(string path){
	ifstream in(path, ios::in);
	int index = 0;

	while (!in.eof()){
		in >> ypredict[index];
		for (int i = 0; i < FeatureNum; ++i)
			in >> xpredict[index][i];
		index++;
	}
	TestNum = index - 1;
	in.close();


}
void predict(int i ){

	int num = 0;
	for (int i = 0; i < TestNum; ++i)
	{
		double innerproduct = 0;
		for (int j = 0; j < FeatureNum; ++j)
			innerproduct += xpredict[i][j] * weight[j];
		if (innerproduct * ypredict[i] > 0)
			num++;
	}
	double accuracy = num;
	accuracy /= TestNumber[i];
	cout << "accuracy   " << accuracy << endl;
}


void GetWeight(int i,int j){
	int index = -1;
	if (i == 0 && j == 1)
		index = 0;
	if (i == 1 && j == 2)
		index = 2;
	if (i == 0 && j == 2)
		index = 1;

	string path = to_string(i) + to_string(j) + "weight.txt";

	ifstream in(path, ios::in);
	int wi = 0;
	while (!in.eof())
	{
		in >> TotalWeight[index][wi];
		wi++;
	}

}


void outputpredict(string path){
	ifstream in(path, ios::in);
	int index = 0;
	memset(xpredict, 0, sizeof(xpredict));
	memset(ypredict, 0, sizeof(ypredict));
	while (!in.eof()){
		string tep;
		in >> tep;
		for (int i = 0; i < FeatureNum; ++i)
			in >> xpredict[index][i];
		index++;
	}
	in.close();


	int num = 0;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	memset(c, 0, sizeof(c));

	for (int i = 0; i < TestLine; ++i)
	{
		double innerproduct = 0;
		for (int j = 0; j < FeatureNum; ++j)
			innerproduct += xpredict[i][j] * TotalWeight[0][j];
		if (innerproduct > 0)
			a[i]++;
		else
			b[i]++;
	}

	for (int i = 0; i < TestLine; ++i)
	{
		double innerproduct = 0;
		for (int j = 0; j < FeatureNum; ++j)
			innerproduct += xpredict[i][j] * TotalWeight[1][j];
		if (innerproduct > 0)
			a[i]++;
		else
			c[i]++;
	}

	for (int i = 0; i < TestLine; ++i)
	{
		double innerproduct = 0;
		for (int j = 0; j < FeatureNum; ++j)
			innerproduct += xpredict[i][j] * TotalWeight[2][j];
		if (innerproduct > 0)
			b[i]++;
		else
			c[i]++;
	}



}

void SaveWeight(string path){
	ofstream out(path);
	for (int i = 0; i < FeatureNum; ++i)
		out << weight[i] << ' ';
	out << endl;
	out.close();
	return;
}

void train(){
	for (int i = 0; i < iteration; ++i){
		if (i % 1000 == 0)
			cout << "iter" << i << endl;
		for (int indexa = 0; indexa < TotalNum; ++indexa){
			double innerproduct = 0;
			int index =rand() % TotalNum;
			for (int j = 0; j < FeatureNum; ++j)
				innerproduct += x[index][j] * weight[j];

			if (y[index] * innerproduct >1)
			{
				for (int wi = 0; wi < FeatureNum; ++wi)
					weight[wi] -= stepsize * lamda * weight[wi];
			}
			else {
				for (int wi = 0; wi < FeatureNum; ++wi)
					weight[wi] -= stepsize * (lamda * weight[wi] - x[index][wi] * y[index]);
			}

		}

	}
}
int GetMaxIndex(int i){
	if (a[i] > b[i] && a[i] > c[i])
		return 0;
	else if (b[i] > c[i] && b[i] > a[i])
		return 1;
	else if (c[i] > a[i] && c[i] > b[i])
		return 2;
	return -1;
}
void totalpredict(string path){
	int result[TestLine];
	for (int i = 0; i < TestLine; ++i){
		int index = GetMaxIndex(i);
		result[i] = index;
	}



	string originres[TestLine];
	int originI[TestLine];
	ifstream in(path, ios::in);
	int index = 0;
	for (int i = 0; i < TestLine; ++i)
		originI[i] = -1;
	while (!in.eof()){
		in >> originres[index];
		double tep;
		for (int tepi = 0; tepi < FeatureNum; ++tepi)
			in >> tep;
		for (int i = 0; i < 3; ++i)
			if (originres[index] == dict[i])
				originI[index] = i;
		index++;
	}

	int num = 0;
	for (int i = 0; i < TestLine; ++i)
		if (originI[i] == result[i])
			num++;
	double accuracy = num;
	accuracy /= TestLine;
	cout << "accuracy  " << accuracy << endl;

}

int main(){
	//getparameter("0train.txt");
	//MakeSimple();
	int cnt = 0;
	for (int i = 0; i < 3; ++i){
		for (int j = i + 1; j < 3; ++j){
			string tep = to_string(i) + to_string(j);
			getparameter(tep + "train.txt");
			MakeSimple();
			train();

			GetPredictData(tep + "test.txt");

			MakePredictData();
			predict(cnt++);

			SaveWeight(tep+"weight.txt");

		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 3; ++j)
		{
			string tep = to_string(i)+to_string(j);
			GetWeight(i, j);

		}
	}
	
	outputpredict("test.txt");
	totalpredict("test.txt");
	system("pause");
	return 0;

}