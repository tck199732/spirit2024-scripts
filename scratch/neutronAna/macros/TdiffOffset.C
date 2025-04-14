#include "spirit.h"

const int nModules = 72;
void setChain(TChain *chain, const std::vector<int> &runIds);
std::string getFileFromRunNumber(int idx);
template <typename T> bool matchHit(const std::vector<T> &vec, T val);
unsigned int getLayer(const unsigned int &hitModule);
std::vector<double> getOffset(const std::string &filename);

void TdiffOffset(
	// clang-format off
    bool applyOffset = false,
    bool useVetoWall = true,
	const std::string &outputFilename = "tdiff.root",
	const std::string &calibFilename = "database/calibration/tdiff/tdiff_offset.json", 
	const std::vector<std::vector<unsigned int>> calibModule =
    {
        {28, 35, 42},
        {4, 11, 18},
        {28, 35, 42},
        // {24, 35, 47},
        // {48, 59, 71},
        // {24, 35, 47},

    },
    const std::vector<int> &runIds =
    {
        1148, 1150, 1151, 1152, 1153, 1154, 1155, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167,
        1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184,
        1185, 1186, 1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202,
        1203, 1204, 1205, 1206, 1207, 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1221,
        1222, 1223, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1234, 1235, 1236, 1238, 1239, 1240, 1241,
        1242, 1243, 1244, 1245, 1246, 1247, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283,
        1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 
        // remaining
        1300, 1304, 1305, 1306, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1316, 1317, 1318, 1319, 1320, 
        1322, 1324, 1325, 1326, 1327, 1331

    } // clang-format on
) {

	auto chain = new TChain("spirit", "spirit");
	setChain(chain, runIds);
	auto nEntries = chain->GetEntries();

	auto offsetVec = getOffset(calibFilename);
	std::cout << "Number of entries: " << nEntries << std::endl;

	/****************************************************************************************************************/
	auto hModuleIDVsTDiff = new TH2D("hModuleIDVsTDiff", "", 500, -25, 25, nModules, 0, nModules);
	TH2D *hModuleIDVsTDiffLayer[3];
	for (auto i = 0; i < 3; i++) {
		hModuleIDVsTDiffLayer[i] =
			new TH2D(Form("hModuleIDVsTDiffLayer%d", i), "", 500, -25, 25, nModules, 0, nModules);
	}

	TH2D *hModuleIDVsTDiffLayerGated[3];
	for (auto i = 0; i < 3; i++) {
		hModuleIDVsTDiffLayerGated[i] =
			new TH2D(Form("hModuleIDVsTDiffLayer%dGated", i), "", 500, -25, 25, nModules, 0, nModules);
	}

	/****************************************************************************************************************/
	for (auto iEvt = 0; iEvt < nEntries; iEvt++) {
		chain->GetEntry(iEvt);

		auto vetoEvent = useVetoWall ? spirit.hime_veto_multi > 0 : false;
		if (vetoEvent) {
			continue;
		}

		double sbtTime = 0.;
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			if (moduleId == 1) {
				sbtTime = spirit.hime_tofRaw[ihit];
			}
		}
		if (sbtTime == 0.) {
			continue;
		}

		std::array<bool, 3> isCalibrate = {false, false, false};
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			auto hitModule = spirit.hime_moduleID[ihit];
			for (auto layer = 0; layer < 3; layer++) {
				if (matchHit(calibModule[layer], hitModule)) {
					isCalibrate[layer] = true;
				}
			}
		}

		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			int layerId = getLayer(moduleId);

			if (layerId < 0 || layerId > 2) {
				continue;
			}

			auto tDiff = spirit.hime_tDiff[ihit];
			if (applyOffset) {
				tDiff -= offsetVec[moduleId];
			}

			if (moduleId <= 1 || moduleId >= nModules) {
				continue;
			}

			hModuleIDVsTDiff->Fill(tDiff, moduleId);
			hModuleIDVsTDiffLayer[layerId]->Fill(tDiff, moduleId);

			if (isCalibrate[layerId]) {
				hModuleIDVsTDiffLayerGated[layerId]->Fill(tDiff, moduleId);
			}
		}
	}

	auto ofile = new TFile(outputFilename.c_str(), "recreate");
	hModuleIDVsTDiff->Write();
	for (auto i = 0; i < 3; i++) {
		hModuleIDVsTDiffLayer[i]->Write();
		hModuleIDVsTDiffLayerGated[i]->Write();
	}
	ofile->Close();

	return;
}

void setChain(TChain *chain, const std::vector<int> &runIds) {
	for (auto idx : runIds) {
		auto fname = getFileFromRunNumber(idx);
		chain->AddFile(fname.c_str());
	}
	SetBranchAddress(chain); // from spirit.hh
	return;
}

std::string getFileFromRunNumber(int idx) { return Form("spirit/data%04d.root", idx); }

template <typename T> bool matchHit(const std::vector<T> &vec, T val) {
	return std::find(vec.begin(), vec.end(), val) != vec.end();
}

unsigned int getLayer(const unsigned int &hitModule) {
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

std::vector<double> getOffset(const std::string &filename) {

	if (filename == "") {
		return std::vector<double>(nModules, 0);
	}

	if (!std::filesystem::exists(filename)) {
		std::cerr << "Could not open file " << filename << std::endl;
		return std::vector<double>(nModules, 0);
	}

	std::vector<double> offset(nModules, 0);

	std::ifstream infile(filename.c_str());
	// load json file
	nlohmann::json j;
	infile >> j;

	for (auto i = 0; i < nModules; i++) {
		offset[i] = j[std::to_string(i)];
	}

	return offset;
}