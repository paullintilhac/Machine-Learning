#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "Reader.h"
#include "DataSet.h"
#include "EvalResult.h"
using namespace std;


int main(int argc, char* argv[]){

	Reader r("training.csv","testing.csv");
	DataSet training(r.trainingData);
	DataSet testing(r.testingData);
	cout<<"responses size: "<<training.responses.size()<<", num predictors: "<<training.predictors[0].size()<<endl;

	int nrows = training.responses.size();
	int nVars = training.predictors[0].size();
	
	float dist[nrows];
	cout<<"nrows: "<<nrows<<endl;
	for (int i=0;i<nrows;i++){
		dist[i]=(float)1/nrows;
	}
	int T = 10;
	float g[nrows];
	float classifier[nrows];
	for (int i=0;i<T;i++){
		EvalResult er = training.getStumps(training,dist);
		cout<<"best ind: "<<er.index<<", er.accuracy: "<<er.accuracy<<", er.threshold: "<<er.threshold<<", er.sign: "<<er.sign<<endl;
		
		float a = .5*log((er.accuracy)/(1-er.accuracy));
		float Z = 2*pow((1-er.accuracy)*(er.accuracy),.5);
		for (int j=0;j<nrows;j++){
			float thisH;
			if (er.sign==1){
				thisH = (training.predictors[j][er.index]<=er.threshold);
			} else{
				thisH = (training.predictors[j][er.index]>er.threshold);
			}
			g[j]+=thisH*a;
			dist[j]=dist[j]*exp(-a*training.responses[j]*thisH)/Z;
		}
	}
	for (int i=0;i<nrows;i++){
		if (g[i]>0){
			classifier[i] =1;
		}
		else{
			classifier[i]=0;
		} 
	}


}