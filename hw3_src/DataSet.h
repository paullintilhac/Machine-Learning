
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
	}
	
   	struct comp
	{
	    inline bool operator() (const pair<float,float>& l, const pair<float,float>& r)
	    {
	        return (l.second < r.second);
	    }
	};
	getStumps(DataSet d,float* dist){
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
		float bestAccuracies[nrow];
		float bestThresholds[nrow];
		int bestSigns[nrow];
		cout<<"hello"<<endl;
		for (int i=0;i<ncol;i++){
			vector<pair<float,float>> responsePredictor;
			for (int j=0;j<nrow;j++){
				responsePredictor.push_back(make_pair(d.responses[j],d.predictors[i][j]));
			}
			sort(responsePredictor.begin(),responsePredictor.end(),comp());
			for (int j=0;j<nrow;j++){
				cout<<responsePredictor[j].first<<" "<<responsePredictor[j].second<<endl;
			}
		}

	}
};