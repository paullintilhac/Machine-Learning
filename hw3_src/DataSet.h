#include "EvalResult.h"
class DataSet{
public:
	vector<int> responses;
	vector<vector<float>> predictors;
	DataSet(vector<vector<float>> rawData){
		for (int i=0;i<rawData.size();i++){
			//cout<<"rawData[i][0]: "<<rawData[i][0]<<endl;
			responses.push_back((int)rawData[i][0]);
			vector<float> thisLine;
			for (int j=0;j<rawData[i].size()-1;j++){
				thisLine.push_back(rawData[i][j+1]);
			}
			predictors.push_back(thisLine);
		}

	};
	
   	struct comp
	{
	    inline bool operator() (const pair<float,float>& l, const pair<float,float>& r)
	    {
	        return (l.second < r.second);
	    }
	};
	
	EvalResult getStumps(DataSet d,float* dist){
		int nrow = d.responses.size();
		int ncol = d.predictors[0].size();
		float sum = 0;
		int responses[nrow];

		//for (int i=0;i<nrow;i++){
		//	sum+= dist[i];
		//	responses[i] = d.responses[i];
		//}
		/*if (sum!=1){
			cout<<"probabilities do not sum to 1!"<<endl;
			//exit(1);
		}*/
		int bestTotalIndex;
		float bestTotalAccuracy=0;
		float bestTotalThreshold=0;
		int bestTotalSign=0;
		int count=0;
		for (int i=0;i<ncol;i++){
			vector<pair<int,float>> responsePredictor;
			for (int j=0;j<nrow;j++){
				responsePredictor.push_back(make_pair(d.responses[j],d.predictors[i][j]));
			}
			//cout<<"column: "<<i<<", first element: "<<responsePredictor[0].second<<endl;

			sort(responsePredictor.begin(),responsePredictor.end(),comp());
			float bestThreshold = responsePredictor[0].second;
			float accuracy =weightedMean(responsePredictor,dist,0);
			float bestAccuracy = accuracy;
			int bestSign =-1;
			if (bestAccuracy>.5)
				bestSign =1;
			//cout<<"start accuracy: "<<accuracy<<endl;
			for (int j=0;j<nrow;j++){
				//this is an efficient way of calculating the accuracy 
				//for every threshold in O(n) time instead of O(n^2) time

				if (responsePredictor[j].first==1) 
				accuracy+=dist[j];
				else{
				accuracy-=dist[j];
				}
				//float accuracy2 = weightedMean(responsePredictor,dist,j+1);

				int sign=-1;
				if (accuracy>.5)
					sign = 1;
				if (abs(accuracy-.5)>abs(bestAccuracy-.5)){
					//cout<<"better accuracy"<<endl;
					bestAccuracy = accuracy;
					bestThreshold = responsePredictor[j].second;
					if (j==nrow)
						bestThreshold+=.1;
					bestSign = sign;
				}
			}
			cout<<"bestAccuracy: "<<bestAccuracy<<", bestThreshold: "<<bestThreshold<<", bestSign: "<<bestSign<<endl;

			
			if (abs(bestAccuracy-.5)>bestTotalAccuracy-.5){
				bestTotalAccuracy=bestAccuracy;
				bestTotalThreshold=bestThreshold;
				bestTotalSign = bestSign;
				bestTotalIndex = bestTotalIndex;
			}
		}
		if (bestTotalSign<0){
			bestTotalAccuracy=1-bestTotalAccuracy;
		}
		EvalResult result(bestTotalIndex,bestTotalAccuracy,bestTotalThreshold,bestTotalSign);
		return(result);
		};

		float weightedMean(vector<pair<int,float>> inVec,float* w,int index){
			float wMean = 0;
			for (int i=0;i<inVec.size();++i){
				int correct;
				//cout<<"i<index? "<<(i<index)<<", response ==1? "<<(inVec[i].first==1)<<", w[i]: "<<w[i]<<endl;
				if ((i<index&&inVec[i].first==1)||(i>=index&&inVec[i].first==0)){
					correct =1;
				} else{
					correct = 0;
				}
				wMean+=correct*w[i];
			}
			return(wMean);
		}

	
};