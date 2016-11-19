#include "EvalResult.h"
#include "XY.h"
class DataSet{
public:
	vector<int> responses;
	vector<vector<float>> predictors;
	vector<int> shuffledInd;
	vector<vector<int>> trainingInd;
	vector<vector<int>> testingInd;
	
	DataSet(vector<vector<float>> rawData){
		for (int i=0;i<rawData.size();i++){
			//cout<<"rawData[i][0]: "<<rawData[i][0]<<endl;
			int newData = (int)rawData[i][0];
			if (newData==0)
				newData = -1;
			//cout<<"newData: "<<newData<<endl;
			responses.push_back(newData);
			vector<float> thisLine;
			for (int j=0;j<rawData[i].size()-1;j++){
				thisLine.push_back(rawData[i][j+1]);
			}
			predictors.push_back(thisLine);
		}
	};

	DataSet(vector<int> indices, DataSet* parent){
		for (int i=0;i<indices.size();i++){
			//cout<<"predictor size during construction: "<<parent.predictors[indices[i]].size()<<endl;
			responses.push_back((int)parent->responses[indices[i]]);
			predictors.push_back(parent->predictors[indices[i]]);
		}
		//cout<<"total size of predictors: "<<parent.predictors.size()<<endl;
	};

   	struct comp
	{
	    inline bool operator() (const XY& l, const XY& r)
	    {
	        return (l.threshold < r.threshold);
	    }
	};
	struct comp2
	{
	    inline bool operator() (const XY& l, const XY& r)
	    {
	        return (l.index< r.index);
	    }
	};
	

	vector<EvalResult> trainBoost( int T){
		int nrowsTraining = responses.size();
		vector<EvalResult> model;
		double dist[nrowsTraining];
			//cout<<"nrows: "<<nrows<<endl;
		for (int i=0;i<nrowsTraining;i++){
			//cout<<"response: "<<responses[i]<<endl;
			dist[i]=(float)1/nrowsTraining;
		}
		for (int i=0;i<T;i++){ //iteratre over base classifiers
			//cout<<"iteration: "<<i<<endl;
			EvalResult er = this->getStumps(dist);
			//cout<<"best ind: "<<er.index<<", best row ind: "<<er.rowInd<<", er.accuracy: "<<(.5+abs(.5-er.accuracy))<<", er.threshold: "<<er.threshold<<", er.sign: "<<er.sign<<endl;
			double a = .5*log((er.accuracy)/(1-er.accuracy));
			double Z = 2*pow((1-er.accuracy)*(er.accuracy),(double).5);
			double acc=0;
			double acc2=0;
			double probSum = 0;
			for (int j=0;j<nrowsTraining;j++){
				int thisH = (2*(predictors[j][er.index]<=er.threshold)-1)*er.sign;
				if (thisH==responses[j])
					acc+=er.hypothesisData[j].dist;
				if (er.hypothesisData[j].prediction!=er.hypothesisData[j].response)
					acc2+=er.hypothesisData[j].dist;
				//cout<<"dist: "<<dist[j]<<", index: "<<er.hypothesisData[j].index<<endl;
				//cout<<"a: "<<a<<", response: "<<responses[j]<<", response2: "<<er.hypothesisData[j].response<<", hypothesis: "<<(er.hypothesisData[j].prediction*er.sign)<<", hypothesis 2: "<<thisH<<endl;
				dist[j]=dist[j]*exp(-a*responses[j]*er.hypothesisData[j].prediction*er.sign)/Z;
				probSum+=dist[j];
			}
			//cout<<"real accuracy: "<<acc<<", acc2: "<<acc2 <<", probSum: "<<probSum<<endl;
			model.push_back(er);
		}
		return(model);
	};

	vector<int> testBoost(vector<EvalResult> model){
		//cout<<"model size: "<<model.size()<<endl;
		int nrowsTesting = responses.size();
		//cout<<"nrowsTesting: "<<nrowsTesting<<endl;
		vector<double> g;
		vector<int> intG;
		for (int i=0;i<nrowsTesting;i++)
			g.push_back(0);
		for (int i=0;i<model.size();i++){
			double a = .5*log((model[i].accuracy)/(1-model[i].accuracy));
			//cout<<"a: "<<a<<endl;
			for (int j=0; j<nrowsTesting;j++){
				int thisH;				
				thisH = (2*(predictors[j][model[i].index]<=model[i].threshold)-1)*model[i].sign;
				
				g[j]+=thisH*a;
				
			}
		}
		for (int i=0;i<nrowsTesting;i++){
			if (g[i]>0)
				intG.push_back(1);
			else
				intG.push_back(-1);
		}
		return(intG);
	};

	EvalResult getStumps(double* dist){
		int nrow = responses.size();
		int ncol = predictors[0].size();
		double sum = 0;
		//cout<<"best index List size: "<<d->bestIndexList.size()<<endl;
		//cout<<"nrow in training: "<<nrow<<endl;
		for (int i=0;i<nrow;i++){
			//cout<<"dist: "<<dist[i]<<endl;
			sum+= dist[i];
		}
		if (sum!=1){
			//cout<<"probabilities do not sum to 1! "<<sum<<endl;
			//exit(1);
		}
		int bestTotalIndex;
		double bestTotalAccuracy=.5;
		float bestTotalThreshold=0;
		int bestTotalSign=0;
		int bestTotalRowInd=0;
		int count=0;
		vector<XY> totalBestResponsePredictor;
		vector<XY> bestResponsePredictor;
		vector<XY> responsePredictor;
		for (int i=0;i<ncol;i++){
			//cout<<"hello0"<<endl;

			bool repeatCol=false;
			//cout<<"bestIndexList: ";
			
			for (int j=0;j<nrow;j++){
				XY xy(responses[j],predictors[j][i],j,-1,dist[j]);
				responsePredictor.push_back(xy);
			}

			//cout<<"column: "<<i<<", first element: "<<responsePredictor[0].second<<endl;

			sort(responsePredictor.begin(),responsePredictor.end(),comp());
			
			float bestThreshold = responsePredictor[0].threshold;
			//cout<<"hello1"<<endl;
			double accuracy =0;
			for (int j=0;j<nrow;j++){
				//cout<<"accuracy: "<<accuracy<<", response: "<<responsePredictor[j].response<<", prediction: "<<responsePredictor[j].prediction<<endl; 
				if (responsePredictor[j].response==-1)
					accuracy+=responsePredictor[j].dist;
			}
			//cout<<"initial accuracy: "<<accuracy<<endl;
			double bestAccuracy = accuracy;
			int bestSign =-1;
			int bestRowInd=0;
			if (bestAccuracy>.5)
				bestSign =1;
			
			for (int j=0;j<nrow;j++){

				responsePredictor[j].prediction = 1;
				//this is an efficient way of calculating the accuracy 
				//for each threshold in O(nrow) time instead of O(nrow^2) time
				//cout<<"dist: "<<responsePredictor[j].dist<<endl;
				if (responsePredictor[j].response==1){
				accuracy+=responsePredictor[j].dist;
				}
				else{
				accuracy-=responsePredictor[j].dist;
				}
				

				int sign=-1;
				if (accuracy>.5)
					sign = 1;
				if (j<nrow-1){
					if (responsePredictor[j].threshold==responsePredictor[j+1].threshold){
						continue;
					}
				}
				if (abs(accuracy-.5)>=abs(bestAccuracy-.5)){
					//cout<<"better accuracy"<<endl;
					bestAccuracy = accuracy;
					bestThreshold = responsePredictor[j].threshold;
					bestSign = sign;
					bestRowInd = j;
					bestResponsePredictor = responsePredictor;
				}
			}
			//cout<<"bestAccuracy: "<<(.5+abs(bestAccuracy-.5))<<", best total accuracy: "<<(.5+abs(.5-bestTotalAccuracy))<<", bestThreshold: "<<bestThreshold<<", bestSign: "<<bestSign<<endl;

			
			if (abs(bestAccuracy-.5)>=abs(bestTotalAccuracy-.5)){
				//cout<<"found new best: "<<i<<endl;
				bestTotalAccuracy=bestAccuracy;
				bestTotalThreshold=bestThreshold;
				bestTotalSign = bestSign;
				bestTotalIndex = i;
				totalBestResponsePredictor = bestResponsePredictor;
				bestTotalRowInd=bestRowInd;
			}
			responsePredictor.clear();


		}
		if (bestTotalSign<0){
			bestTotalAccuracy=1-bestTotalAccuracy;
		}
		

		sort(totalBestResponsePredictor.begin(),totalBestResponsePredictor.end(),comp2());
		
		//double acc = 0;
		//for (int i=0;i<nrow;i++){
		//	cout<<"index after sorting: "<<totalBestResponsePredictor[i].index<<endl;
		//	if (totalBestResponsePredictor[i].response==totalBestResponsePredictor[i].prediction)
		//		acc++;
		//}
		//acc/=nrow;
		//cout<<"accuracy in stubs: "<<acc<<endl;


		//cout<<"totalBestIndex: "<<bestTotalIndex<<endl;
		EvalResult result(bestTotalIndex,
			bestTotalAccuracy,
			bestTotalThreshold,
			bestTotalSign,
			totalBestResponsePredictor,
			bestTotalRowInd);
		return(result);
		};

	
	void createFolds(int k){
		int nrows = this->responses.size();
		//cout<<"nrows: "<<nrows<<endl;
		for (int i=0;i<nrows;i++){
			this->shuffledInd.push_back(i);
		}

		random_shuffle(this->shuffledInd.begin(),this->shuffledInd.end());

		int foldSize = nrows/k;
		//cout<<"fold size: "<<foldSize<<endl;
		vector<int> thisFold;
		for (int i=0;i<nrows;i++){
			int foldIndex = i/foldSize;
			//cout<<"shuffledInd[i]: "<<shuffledInd[i]<<endl;
			if (i%foldSize==0&&i>0){
				//cout<<"thisFold.size(): "<<thisFold.size()<<endl;
				sort(thisFold.begin(),thisFold.end());
				testingInd.push_back(thisFold);	
				//cout<<"new fold: "<<foldIndex<<endl;					
				//this->trainingInd[fodlIndex].push_back
				thisFold.clear();
			}
			//cout<<shuffledInd[i]<<" "<<endl;
			thisFold.push_back(shuffledInd[i]);	
			//cout<<"shuffled ind: "<<this->shuffledInd[i]<<endl;
		}
		vector<int> thisRemainder;

		for (int i=0;i<testingInd.size();i++){
			//cout<<"training fold "<<i<<", testing size: "<<testingInd[i].size()<<endl;
			for (int j=0;j<shuffledInd.size();j++){
				bool contained = false;
				for (int k=0;k<testingInd[i].size();k++){
					if (j==testingInd[i][k])
					contained = true;
				}
				if (!contained){
					thisRemainder.push_back(j);
					//cout<<j<<" "<<endl;
				}
			}
			trainingInd.push_back(thisRemainder);
			//cout<<"training size: "<<thisRemainder.size()<<", testing size: "<<testingInd[i].size()<<endl;
			thisRemainder.clear();
		}
		shuffledInd.clear();

	};

	vector<int> crossValidate(int T, int nFolds){
		int nrows = responses.size();
		vector<int> gTest;
		for (int i=0;i<nrows;i++){
			gTest.push_back(-1);
		}
		this->createFolds(nFolds);
		for (int l=0;l<nFolds;l++){ //do cross-validation
			cout<<"fold: "<<l<<endl;
			//cout<<"training ind size: "<<training.trainingInd[l].size()<<endl;
			//cout<<"training.size: "<<trainingInd[l].size()<<", testing size: "<<testingInd[l].size()<<endl;
			DataSet trainingSet(trainingInd[l],this);
			DataSet testingSet(testingInd[l],this);
			int nrowsTraining = trainingSet.responses.size();
									
			vector<EvalResult> model =trainingSet.trainBoost(T);
			//cout<<"model size: "<<model.size()<<endl;
			vector<int> gTemp =testingSet.testBoost(model);
			for (int i=0;i<testingInd[l].size();i++){
				gTest[testingInd[l][i]] = gTemp[i];
			}

		}
		return(gTest);

		};
};