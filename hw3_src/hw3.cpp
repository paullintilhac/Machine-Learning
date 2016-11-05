#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Reader.h"
#include "DataSet.h"
using namespace std;


int main(int argc, char* argv[]){

	Reader r("training.csv","testing.csv");
	DataSet training(r.trainingData);
	DataSet testing(r.testingData);
	cout<<"responses size: "<<training.responses.size()<<", num predictors: "<<training.predictors[0].size()<<endl;

	int nrows = training.responses.size();
	int nVars = training.predictors[0].size();
	int dist[nrows];

	for (int i=0;i<nrows;i++){
		dist[i]=1/nrows;
	}
	int T = 5;
	for (int i=0;i<T;i++){

	}


}