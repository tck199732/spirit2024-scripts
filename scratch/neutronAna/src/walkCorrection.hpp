#ifndef WALK_CORRECTION_H
#define WALK_CORRECTION_H

#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

class WalkCorrection {
public:
	WalkCorrection(int funcType = 2);
	~WalkCorrection() = default;

	double calcTofWalk(double tot) const;
	double calcTofWalkLinear(double tot) const;
	double calcTofWalkQuadratic(double tot) const;
	double calcTofWalkExponential(double tot) const;

	void setFuncType(int type) { funcType = type; }
	void setTotThresh(double thresh) { totThresh = thresh; }
	int getFuncType() const { return funcType; }
	double getTotThresh() const { return totThresh; }
	std::vector<double> getWalkParams() const { return walkParams; }

private:
	int funcType; // 0: linear, 1: quadratic, 2: exponential
	double totThresh = 22.13;

	std::vector<double> walkParams;
	std::vector<double> linearParams = {-0.20, -6.389};
	std::vector<double> quadraticParams = {0.0042, -0.305, -5.863};
	std::vector<double> exponentialParams = {7.8, 0.043, -13.549};
};

#endif // WALK_CORRECTION_H