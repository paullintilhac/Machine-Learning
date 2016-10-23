#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]){
		ifstream infile (argv[1]);
		string str;
		int id = 0;
		int count = 0;
		vector<vector<float>> svmData;
		cout<<"infile: "<<argv[1]<<endl;
		//getline(infile, str,',');
		//getline(infile, str);

		while (getline(infile, str)) {
		vector<float> thisLine;
		stringstream ss(str);

	   	float i;

	    while (ss >> i)
	    {
	        thisLine.push_back(i);

	        if (ss.peek() == ',')
	            ss.ignore();
	    }

	    svmData.push_back(thisLine);
	    //cout<<"size "<<thisLine.size()<<endl;
		}
		//cout<<"size: "<<svmData.size()<<endl;
}

