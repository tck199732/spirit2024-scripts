#include "spirit.h"

const int nModules = 72;
unsigned int getLayer(const unsigned int &hitModule);
std::vector<double> getModulePositions(const std::string &filename);
void getPositionCalibration(const std::string &filename, std::vector<double> &veff, std::vector<double> &offset);

void TdiffOffset(
	// clang-format off
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

    },
    const std::string &outputFilename = "tdiff.root",
    bool applyOffset = false,
    bool useVetoWall = false,
    const std::string &tdiffOffsetFile = "database/calibration/tdiff/position-calib-muon.json"

	// clang-format on
) {

	auto chain = getSpiritChain(runIds, "spirit");
	auto nEntries = chain->GetEntries();

	// load tdiff offset and velocity
	std::vector<double> veff(nModules, 0.0);
	std::vector<double> posOffset(nModules, 0.0);
	getPositionCalibration(tdiffOffsetFile, veff, posOffset);

	std::cout << "Number of entries: " << nEntries << std::endl;

	/****************************************************************************************************************/

	TH2D *hModuleIDVsTDiff[nModules];
	TH2D *hGatedModuleIDVsTDiff[nModules];

	for (auto iMod = 0; iMod < nModules; iMod++) {
		hModuleIDVsTDiff[iMod] =
			new TH2D(Form("hModuleIDVsTDiffModule%d", iMod), "", 500, -25, 25, nModules, 0, nModules);
		hGatedModuleIDVsTDiff[iMod] =
			new TH2D(Form("hGatedModuleIDVsTDiffModule%d", iMod), "", 500, -25, 25, nModules, 0, nModules);
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

		std::vector<bool> isHit(nModules, false);
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			isHit[moduleId] = true;
		}

		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			int moduleId = spirit.hime_moduleID[ihit];
			int layerId = getLayer(moduleId);

			if (layerId < 0 || layerId > 2) {
				continue;
			}

			auto tDiff = spirit.hime_tDiff[ihit];
			if (applyOffset) {
				auto offset = -posOffset[moduleId] / (0.5 * veff[moduleId]);
				tDiff -= offset;
			}

			if (moduleId <= 1 || moduleId >= nModules) {
				continue;
			}

			int gateLayer = (layerId == 0 || layerId == 2) ? 1 : 0;
			for (auto i = gateLayer * 24; i < (gateLayer + 1) * 24; i++) {
				if (isHit[i]) {
					hModuleIDVsTDiff[moduleId]->Fill(tDiff, i);
					hGatedModuleIDVsTDiff[i]->Fill(tDiff, moduleId);
				}
			}
		}
	}

	auto ofile = new TFile(outputFilename.c_str(), "recreate");
	for (auto i = 0; i < nModules; i++) {
		hModuleIDVsTDiff[i]->Write();
		hGatedModuleIDVsTDiff[i]->Write();
	}
	ofile->Close();

	return;
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

void getPositionCalibration(const std::string &filename, std::vector<double> &veff, std::vector<double> &offset) {
	std::ifstream infile(filename.c_str());

	assert(veff.size() == nModules);
	assert(offset.size() == nModules);

	for (int i = 0; i < nModules; i++) {
		veff[i] = 0.0;
		offset[i] = 0.0;
	}

	nlohmann::json j;
	infile >> j;
	infile.close();

	for (int i = 0; i < nModules; i++) {
		if (j.contains(std::to_string(i))) {
			if (!j[std::to_string(i)].contains("v") || !j[std::to_string(i)].contains("offset")) {
				continue; // skip if v or offset is not present
			}
			veff[i] = j[std::to_string(i)]["v"].get<double>();
			offset[i] = j[std::to_string(i)]["offset"].get<double>();
		} else {
			std::cerr << "Module " << i << " not found in calibration file." << std::endl;
		}
	}
}
