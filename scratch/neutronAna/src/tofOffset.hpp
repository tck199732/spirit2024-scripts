#ifndef TOF_OFFSET_HPP
#define TOF_OFFSET_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class TofOffset {
public:
	TofOffset(const std::string &filename, const double &sbtTime = -110.8, int nModules = 72);
	~TofOffset() = default;

	double calcTof(double tof, double sbt, int moduleId) const;
	void loadTofOffset(const std::string &filename);

private:
	int nModules;
	double sbtTimeLeft;
	std::vector<double> mOffset;
};

#endif // TOF_OFFSET_HPP