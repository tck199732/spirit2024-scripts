#include "spirit.h"

const int nModules = 72;
void setChain(TChain *chain, const std::vector<int> &runIds);
std::string getFileFromRunNumber(int idx);
template <typename T> bool matchHit(const std::vector<T> &vec, T val);
unsigned int getLayer(const unsigned int &hitModule);

void CalibrateHimePos(
	const std::vector<int> &runIds =
		{
			1148, 1150, 1151, 1152, 1153, 1154, 1155, 1158, 1159, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168,
			1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186,
			1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205,
			1206, 1207, 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224,
			1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242,
			1243, 1244, 1245, 1246, 1247, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283, 1284, 1285,
			1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299,
		},
	const std::vector<std::vector<unsigned int>> calibModule =
		{
			{28, 35, 42},
			{4, 11, 18},
			{28, 35, 42},
		},
	const std::string &outputFilename = "calibrate_pos.root"
) {

	auto chain = new TChain("spirit", "spirit");
	setChain(chain, runIds);
	auto nEntries = chain->GetEntries();

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
