#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#define FeatureNum 8
#define TotalLine 3133
#define TestLine 1044
using namespace std;
int y[TotalLine];
double x[TotalLine][FeatureNum];
double xpredict[TestLine][FeatureNum];
int  ypredict[TestLine];
double weight[FeatureNum];

string dict[3] = { "F", "M", "I" };
int TotalNum = 0;
int iteration = 100000;
double stepsize = 2;
double lamda = 0.000000001;
double MaxValue[FeatureNum];
int Result[TestLine];
void getparameter(string path){
	ifstream in(path,ios::in);
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


/*
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
		

}*/
/*
void MakePredictData(){
	for (int index = 0; index < FeatureNum; ++index)
	{
		for (int i = 0; i < TestLine; ++i){
			xpredict[index][i] /= MaxValue[index];
		}
	}

}*/

void GetPredictData(string path){
	ifstream in(path, ios::in);
	int index = 0;

	while (!in.eof()){
		in >> ypredict[index];
		for (int i = 0; i < FeatureNum; ++i)
			in >> xpredict[index][i];
		index++;
	}
	in.close();


}
void predict(){

	int num = 0;
	for (int i = 0; i < TestLine; ++i)
	{
		double innerproduct = 0;
		for (int j = 0; j < FeatureNum; ++j)
			innerproduct += xpredict[i][j] * weight[j];
		if (innerproduct * ypredict[i] > 0)
			num++;
	}
	double accuracy = num;
	accuracy /= TestLine;
	cout << "accuracy   " << accuracy << endl;
}
void outputpredict(string path,string outputpath){
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
	ofstream out(outputpath);
	for (int i = 0; i < TestLine; ++i)
	{
		double innerproduct = 0;
		for (int j = 0; j < FeatureNum; ++j)
			innerproduct += xpredict[i][j] * weight[j];
		out << innerproduct << endl;
	}
	out.close();
}

void SaveWeight(string path){
	ofstream out(path);
	for (int i = 0; i < FeatureNum; ++i)
		out << weight[i] << ' ';
	out << endl;
	return;
}

void train(){
	for (int i = 0; i < iteration; ++i){
		if (i% 1000 == 0)
		cout << "iter" << i << endl;
		for (int indexa = 0; indexa < TotalNum; ++indexa){
			double innerproduct = 0;
			int index = rand() % TotalNum;
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

void GetOrigin(){
	ifstream in("input.txt");
	
	for (int i = 0; i < TestLine; ++i){
		string tep;
		in >> tep;
		if (tep == "F")
			xpredict[i][0] = -1;
		else if (tep == "M")
			xpredict[i][0] = 0;
		else
			xpredict[i][0] = 1;

		for (int j = 1; j < FeatureNum; ++j)
			in >> xpredict[i][j];
		int age;
		in >> age;
		if (age >= 0 && age <= 8)
			Result[i] = 0;
		else if (age == 9 || age == 10)
			Result[i] = 1;
		else
			Result[i] = 2;
	}




}
int result[TestLine];
void GetResult(){
	double f[TestLine][3];

	for (int i = 0; i < 3; ++i)
	{
		string tep = to_string(i);
		tep += "res.txt";
		ifstream in(tep, ios::in);
		int index = 0;
		while (!in.eof())
		{
			in >> f[index++][i];
		}
		in.close();
	}
	int resultindex = 0;
	for (int i = 0; i < TestLine; ++i)
	{
		int tempindex = 0;
		double MaxValue = f[i][0];
		for (int x = 0; x < 3; ++x)
		{
			if (f[i][x] > MaxValue)
			{

				MaxValue = f[i][x];
				tempindex = x;
			}

		}

		result[resultindex++] = tempindex;
	}

}



void totalpredict(string path){


	int num = 0;
	for (int i = 0; i < TestLine; ++i)
		if (Result[i] == result[i])
			num++;
	double accuracy = num;
	accuracy /= TestLine;
	cout << "accuracy  " << accuracy << endl;

}
int main(){
	//getparameter("0train.txt");
	//MakeSimple();
	for (int i = 0; i < 3; ++i){
		string tep = to_string(i);
		getparameter("train"+tep+".txt");
		//MakeSimple();
		train();
		GetPredictData( "test"+tep+".txt");
		//MakePredictData();
		predict();

		outputpredict("test.txt",tep+"res.txt");
		SaveWeight("weight"+tep+".txt");
	}
	GetOrigin();
	GetResult();
	totalpredict("test.txt");
	system("pause");
	return 0;

}