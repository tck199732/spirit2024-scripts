#include "detectorGeometry.hpp"

DetectorGeometry::DetectorGeometry(const std::string &filename, int nLayers, int nModules) :
	nLayers(nLayers),
	nModules(nModules),
	mGen(std::random_device{}()), // Initialize random number generator with a random seed
	mUnif(0.0, 1.0) {

	barWidth = 40.0; // mm
	barDepth = 20.0; // mm

	modulePositions.resize(nModules, 0.0);
	loadModulePositions(filename);
}

void DetectorGeometry::loadModulePositions(const std::string &filename) {

	modulePositions.clear();
	modulePositions.resize(nModules, 0.0);

	std::ifstream infile(filename.c_str());
	if (!infile.is_open()) {
		std::cerr << "Could not open file " << filename << std::endl;
	}

	infile.ignore(1000, '\n');
	infile.ignore(1000, '\n');

	int id, layer;
	double pos;

	while (infile >> id >> layer >> pos) {
		modulePositions[id] = pos * 1000;
	}
	infile.close();
	return;
}

int DetectorGeometry::getLayer(const int &hitModule) const {
	if (hitModule >= 0 && hitModule <= 23) {
		return 0;
	}
	if (hitModule >= 24 && hitModule <= 47) {
		return 1;
	}
	if (hitModule >= 48 && hitModule <= 71) {
		return 2;
	}
	return -1;
}

double DetectorGeometry::getModulePosition(const int &mod) const {
	if (mod < 0 || mod >= nModules) {
		std::cerr << "Invalid module ID: " << mod << std::endl;
		return 0.0;
	}
	return modulePositions[mod];
}

double DetectorGeometry::getRandomizedPosition(const int &mod) {
	auto pos = getModulePosition(mod);
	return pos + barWidth * (mUnif(mGen) - 0.5); // Randomize position within the bar width
}