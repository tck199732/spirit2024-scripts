#ifndef GAIN_MATCHING_HPP
#define GAIN_MATCHING_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class GainMatching {
public:
	GainMatching(
		const std::string &gainMatchingFilename = "database/calibration/tot/gain-matching-all.txt",
		const int &nModules = 72
	);
	~GainMatching();

	void loadGainMatchingParams(const std::string &filename);
	double calculateTot(int det, double tot);

private:
	unsigned int nModules;
	double refAmplitude, refOffset, refDamping;

	std::vector<double> pAmplitude, pOffset, pDamping;
	std::vector<bool> isReference;
};

#endif