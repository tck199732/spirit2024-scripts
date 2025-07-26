#include "positionCalibration.hpp"

PositionCalibration::PositionCalibration(const std::string &tdiffOffsetFile, const int &nModules) : nModules(nModules) {
	effVelocities.resize(nModules, 0.0);
	positionOffsets.resize(nModules, 0.0);
	loadPositionCalibration(tdiffOffsetFile);
}

PositionCalibration::~PositionCalibration() {
	// Destructor implementation (if needed)
}

void PositionCalibration::loadPositionCalibration(const std::string &filename) {

	std::ifstream infile(filename.c_str());

	for (int i = 0; i < static_cast<int>(nModules); i++) {
		effVelocities[i] = 0.0;
		positionOffsets[i] = 0.0;
	}

	nlohmann::json j;
	infile >> j;
	infile.close();

	for (int i = 0; i < static_cast<int>(nModules); i++) {
		if (j.contains(std::to_string(i))) {
			if (!j[std::to_string(i)].contains("v") || !j[std::to_string(i)].contains("offset")) {
				continue; // skip if v or offset is not present
			}
			effVelocities[i] = j[std::to_string(i)]["v"].get<double>();
			positionOffsets[i] = j[std::to_string(i)]["offset"].get<double>();
		} else {
			std::cerr << "Module " << i << " not found in calibration file." << std::endl;
		}
	}
}

double PositionCalibration::calculatePosition(int moduleId, double tDiff) {
	if (moduleId < 0 || moduleId >= static_cast<int>(nModules)) {
		std::cerr << "Invalid module ID: " << moduleId << std::endl;
		return 0.0;
	}
	auto veff = effVelocities[moduleId];
	auto offset = positionOffsets[moduleId];
	return 0.5 * veff * tDiff + offset;
}