#ifndef EVALRESULT_H
#define EVALRESULT_H
#include "XY.h"
class EvalResult{
	public:
		int index;
		float threshold;
		double accuracy;
		int sign;
		vector<XY> hypothesisData;
		int rowInd;
		EvalResult(int i,double a,float t, int s,vector<XY> h, int r): 
		index(i),threshold(t),accuracy(a),sign(s),hypothesisData(h),rowInd(r){};
	};
#endif