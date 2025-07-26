#ifndef POSITION_CALIBRATION_H
#define POSITION_CALIBRATION_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class PositionCalibration {
public:
	PositionCalibration(
		const std::string &tdiffOffsetFile = "database/calibration/tdiff-offset.json", const int &nModules = 72
	);
	~PositionCalibration();

	void loadPositionCalibration(const std::string &filename);

	double calculatePosition(int moduleId, double tDiff);

private:
	unsigned int nModules;
	std::vector<double> effVelocities;	 // Effective velocity for each module
	std::vector<double> positionOffsets; // Position offsets for each module
};
#endif // POSITION_CALIBRATION_H