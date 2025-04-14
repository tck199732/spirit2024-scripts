#include "spirit.h"

std::vector<double> getOffset(const std::string &filename);
std::string getFileFromRunNumber(int idx);
void addFilesToChain(const std::vector<int> &runIds, TChain *&chain);

void CalibrateSbtTriggerTime(
	// clang-format off
	const std::vector<int> &runIds =
		{   
            // commissioning
            // 1036, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048, 
            // physics run (Normal data run)
            1160, 
            1161, 
            1162, 1163, 
            1164, 1165, 1166, 1167, 1168,
            1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 
            1179, 1180, 1183, 1184, 1185, 1186, 1188, 1189, 1190, 1191, 1192,
            1194, 1195, 
            1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242,
			1243, 1244, 1245, 1246, 1247, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1300,
		},
	const std::string &filenameOut = "sbt_timing.root"
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(runIds, chain);
	SetBranchAddress(chain);

	// std::cout << "Number of entries in the chain: " << chain->GetEntries() << std::endl;

	auto hTpcTriggerTime = new TH1D("hTpcTriggerTime", "", 100000, -300, 500);
	auto hSbtLeftTime = new TH1D("hSbtLeftTime", "", 100000, -300, 500);
	auto hSbtRightTime = new TH1D("hSbtRightTime", "", 100000, -300, 500);
	auto nEvents = chain->GetEntries();

	// Channel 0 -> TPC trigger
	// Channel 1 -> SBT left
	// Channel 8 -> HIME veto OR
	// Channel 9 -> SBT right

	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);
		for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
			auto hitModule = spirit.hime_moduleID[ihit];
			if (hitModule == 0) {
				hTpcTriggerTime->Fill(spirit.hime_tofRaw[ihit]);
			}
			if (hitModule == 1) {
				hSbtLeftTime->Fill(spirit.hime_tofRaw[ihit]);
			}
			if (hitModule == 9) {
				hSbtRightTime->Fill(spirit.hime_tofRaw[ihit]);
			}
		}
	}

	auto outFile = new TFile(filenameOut.c_str(), "recreate");
	hTpcTriggerTime->Write();
	hSbtLeftTime->Write();
	hSbtRightTime->Write();
	outFile->Close();
}

void addFilesToChain(const std::vector<int> &runIds, TChain *&chain) {
	for (auto idx : runIds) {
		auto fname = getFileFromRunNumber(idx);
		if (gSystem->AccessPathName(fname.c_str())) {
			std::cerr << "File " << fname << " does not exist, skipping ..." << std::endl;
			continue;
		}
		chain->AddFile(fname.c_str());
	}
	return;
}

std::string getFileFromRunNumber(int idx) { return Form("spirit/data%04d.root", idx); }
