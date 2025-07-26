#include "walkCorrection.hpp"

WalkCorrection::WalkCorrection(int funcType) : funcType(funcType) {
	switch (funcType) {
	case 0:
		walkParams = linearParams;
		break;
	case 1:
		walkParams = quadraticParams;
		break;
	case 2:
		walkParams = exponentialParams;
		break;
	default:
		throw std::invalid_argument("Invalid function type for walk correction.");
	}
}

double WalkCorrection::calcTofWalk(double tot) const {
	if (tot > totThresh) {
		tot = totThresh;
	}

	switch (funcType) {
	case 0:
		return calcTofWalkLinear(tot);
	case 1:
		return calcTofWalkQuadratic(tot);
	case 2:
		return calcTofWalkExponential(tot);
	default:
		throw std::invalid_argument("Invalid function type for walk correction.");
	}
}

double WalkCorrection::calcTofWalkLinear(double tot) const { return walkParams[0] * tot + walkParams[1]; }

double WalkCorrection::calcTofWalkQuadratic(double tot) const {
	return walkParams[0] * tot * tot + walkParams[1] * tot + walkParams[2];
}

double WalkCorrection::calcTofWalkExponential(double tot) const {
	return walkParams[0] * std::exp(-walkParams[1] * tot) + walkParams[2];
}
