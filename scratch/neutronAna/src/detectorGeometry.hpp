#ifndef DETECTOR_GEOMETRY_H
#define DETECTOR_GEOMETRY_H

#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class DetectorGeometry {
public:
	DetectorGeometry(const std::string &filename, int nLayers = 3, int nModules = 72);
	~DetectorGeometry() = default;

	void loadModulePositions(const std::string &filename);
	int getLayer(const int &mod) const;
	double getModulePosition(const int &mod) const;
	double getRandomizedPosition(const int &mod);

	double getBarWidth() const { return barWidth; }
	double getBarDepth() const { return barDepth; }
	double getNLayers() const { return nLayers; }
	double getNModules() const { return nModules; }

private:
	int nLayers, nModules;
	double barWidth; // mm
	double barDepth; // mm

	std::mt19937 mGen;
	std::uniform_real_distribution<> mUnif;
	std::vector<double> modulePositions; //  center positions of each module in mm
};

#endif // DETECTOR_GEOMETRY_H