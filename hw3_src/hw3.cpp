#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <errno.h>
#include "Reader.h"
#include "DataSet.h"
#include "EvalResult.h"
#include "XY.h"
using namespace std;


int main(int argc, char* argv[]){
	int nFolds = 4;
	Reader r("../training_debug.csv","../testing.csv");
	DataSet training(r.trainingData);
	DataSet testing(r.testingData);
	cout<<"responses size: "<<training.responses.size()<<", num predictors: "<<training.predictors[0].size()<<endl;
	int nrows = training.predictors.size();
	int nVars = training.predictors[0].size();
	int nrowsTest = testing.predictors.size();
	int T;
	int nrowsTraining;	
	int nrowsTesting;
	
	for (int k=1;k<5;k++){//iterate over T parameters
		T = pow(3,k);
		

		vector<EvalResult> testModel=training.trainBoost(T);
		if (false){

		int* gOOS = testing.testBoost(testModel);
		for (int i=0;i<testModel.size();i++){
			cout<<"index: "<<testModel[i].index<<", accuracy: "<<testModel[i].accuracy<<", threshold: "<<testModel[i].threshold<<", sign: "<<testModel[i].sign<<endl;
		}

		double newmean=0;
		for (int i=0;i<nrowsTest;i++){
			//cout<<"predicted: "<<(gOOS[i])<<", response: "<<testing.responses[i]<<endl;
			if (gOOS[i]*testing.responses[i]==0)
			newmean+=1;
		}

		newmean/=nrowsTest;
		cout<<"total OOS accuracy: "<<newmean<<endl;

		int* gTest = training.crossValidate(T,nFolds);

		float mean=0;
		for (int i=0;i<nrows;i++){
			//cout<<"predicted: "<<""
			if (gTest[i]*training.responses[i]==1)
			mean+=1;
		}
		mean/=nrows;
		cout<<"total cross validation accuracy: "<<mean<<endl;

	
		ofstream outfile;
		outfile.open("c://users/paul/Machine Learning/h3_results.csv",ofstream::app);
			int errnum;
		      cout<<"trying to print file"<<endl;
		   	  outfile<<T<<","<< mean<<","<<newmean<<endl; 
		}
		}

}