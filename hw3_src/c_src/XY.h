#ifndef XY_H
#define XY_H
class XY{
	public:
		int response;
		float threshold;
		int index;
		int prediction;
		double dist;
		XY(int r,float t,float i, float p,double d): response(r),threshold(t),index(i),prediction(p),dist(d){};
};
#endif