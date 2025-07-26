#include "gainMatching.hpp"

GainMatching::GainMatching(const std::string &gainMatchingFilename, const int &nModules) : nModules(nModules) {

	refAmplitude = 0.0;
	refDamping = 0.0;
	refOffset = 0.0;

	pAmplitude.resize(nModules, 0.0);
	pDamping.resize(nModules, 0.0);
	pOffset.resize(nModules, 0.0);
	isReference.resize(nModules, false);

	loadGainMatchingParams(gainMatchingFilename);

	if (pAmplitude.empty() || pDamping.empty() || pOffset.empty() || isReference.empty()) {
		std::cerr << "Error: Gain matching parameters not loaded correctly." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	if (refAmplitude == 0.0 || refDamping == 0.0 || refOffset == 0.0) {
		std::cerr << "Error: Reference gain matching parameters not set." << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GainMatching::~GainMatching() {
	// Destructor implementation (if needed)
}

void GainMatching::loadGainMatchingParams(const std::string &filename) {

	std::ifstream infile(filename.c_str());
	if (!infile.is_open()) {
		std::cerr << "Could not open file " << filename << std::endl;
		return;
	}

	infile.ignore(1000, '\n'); // skip header line
	int moduleId, isRef;
	double v1, v2, v3;

	while (infile >> moduleId >> v1 >> v2 >> v3 >> isRef) {
		if (moduleId < 0 || moduleId >= static_cast<int>(nModules)) {
			std::exit(EXIT_FAILURE); // Invalid module ID
		}
		pAmplitude[moduleId] = v1;
		pOffset[moduleId] = v2;
		pDamping[moduleId] = v3;
		isReference[moduleId] = isRef;

		if (isRef) {
			refAmplitude = v1;
			refOffset = v2;
			refDamping = v3;
		}
	}
	infile.close();
}

double GainMatching::calculateTot(int det, double tot) {
	if (isReference[det]) {
		return tot; // Return original ToT for reference detectors
	}

	auto amp = pAmplitude[det];
	auto damp = pDamping[det];
	auto offset = pOffset[det];

	auto tof = amp * std::exp(damp * tot) + offset;
	auto tot_new = std::log((tof - refOffset) / refAmplitude) / refDamping;

	return tot_new; // Return the adjusted ToT
}