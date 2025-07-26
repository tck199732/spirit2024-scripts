
#include "tofOffset.hpp"

TofOffset::TofOffset(const std::string &filename, const double &sbtTimeLeft, int nModules) :
	nModules(nModules),
	sbtTimeLeft(sbtTimeLeft) {
	mOffset.resize(nModules, 0.0);
	loadTofOffset(filename);
}

double TofOffset::calcTof(double tof, double sbt, int moduleId) const {
	if (moduleId < 0 || moduleId >= static_cast<int>(mOffset.size())) {
		throw std::out_of_range("Module ID out of range");
	}

	// match offset to the start detector
	auto tof_shifted = tof - (sbtTimeLeft - sbt);
	tof_shifted -= mOffset[moduleId];

	// some constant offset applied in Zibi's analysis
	// to offset to 0, should not matter
	// tof-offset calibration only aligns prompt gamma
	tof_shifted += (11.8 + 2. * 4.8 / 0.299792458);
	return tof_shifted;
}

void TofOffset::loadTofOffset(const std::string &filename) {

	mOffset.resize(nModules, 0);

	if (filename == "") {
		return;
	}

	if (!std::filesystem::exists(filename)) {
		std::cerr << "Could not open file " << filename << std::endl;
		return;
	}

	std::string extension = filename.substr(filename.find_last_of(".") + 1);

	if (extension == "json") {
		std::ifstream infile(filename.c_str());
		// load json file
		nlohmann::json j;
		infile >> j;

		for (auto i = 0; i < nModules; i++) {
			mOffset[i] = j[std::to_string(i)];
		}

	} else if (extension == "txt") {
		std::ifstream infile(filename.c_str());
		int id;
		double off;
		while (infile >> id >> off) {
			mOffset[id] = off;
		}
	}
	return;
}
