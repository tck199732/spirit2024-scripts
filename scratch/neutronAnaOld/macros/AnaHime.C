#include "spirit.h"
#include <random>
#include <vector>

std::vector<double> getOffset(const std::string &filename);
std::string getFileFromRunNumber(int idx);
void addFilesToChain(const std::vector<int> &runIds, TChain *&chain);
int getLayer(int hitModule);
int getCenterModule(int layer);
double getDistanceInMeter(int layer);

const int nModules = 72;
const double barWidthInMeter = 0.04; // 40 mm
const double barGapInMeter = 0.004;	 // 4 mm
const double C_LIGHT = 0.299792458;	 // m/ns

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> unif(0.0, 1.0);

void AnaHime(
	// clang-format off

	const std::vector<int> &runIds =
		{// commissioning
		 1036, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048,
		 // noraml
		 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177,
		 1178, 1179, 1180, 1183, 1184, 1185, 1186, 1188, 1189, 1190, 1191, 1192, 1194, 1195,
		 // low-gain, removed 1 shadow bar
		 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 1232,
		 1233, 1234, 1235, 1236, 1239, 1240,
		 // noraml, removed 1 shadow bar
		 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1248, 1249, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 1270, 1271,
		 1272, 1273, 1274, 1275, 1276, 1277, 1278, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287,
		 // noraml gain, removed 1 shadow bar, kyoto threshold 1
		 1258, 1259, 1260, 1261, 1262, 1263, 1264,
		 // normal gain, removed 1 shadow bar, kyoto threshold 2
		 1265, 1266,
		 // normal gain, removed 1 shadow bar, kyoto threshold 3
		 1267, 1268, 1269,
		},
	const std::string &filenameOut = "results-kin/hime-hit-pattern.root",
	const std::string &posOffsetFile = "database/pos_offsets_marco.txt",
	const std::string &timeOffsetFile = "database/time_offsets_6.24.txt",
	const std::string &velocityFile = "database/velocity_marco.txt",

	bool isApplyingVeto = true, bool isApplyingTimeOffset = true, bool isApplyingPosOffset = true
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(runIds, chain);
	SetBranchAddress(chain);

	auto posOffset = isApplyingPosOffset ? getOffset(posOffsetFile) : std::vector<double>(nModules, 0); // marco = cm
	auto timeOffset = isApplyingTimeOffset ? getOffset(timeOffsetFile) : std::vector<double>(nModules, 0);
	auto calibVelocity = getOffset(velocityFile); // marco = cm/ns
	/*************************************************************************************************************************************
	 */

	auto hHitPatternFull = new TH2D("hHitPatternFull", "", 1000, -1000, 1000, 1000, -1000, 1000);
	std::array<TH2D *, 3> hHitPatternLayer;
	for (auto l = 0; l < 3; l++) {
		hHitPatternLayer[l] = new TH2D(Form("hHitPatternLayer%d", l), "", 1000, -1000, 1000, 1000, -1000, 1000);
	}

	auto hTotVsTof = new TH2D("hTotVsTof", "", 500, -200, 500, 300, 0, 75);
	std::array<TH2D *, 3> hTotVsTofLayer;
	for (auto l = 0; l < 3; l++) {
		hTotVsTofLayer[l] = new TH2D(Form("hTotVsTofLayer%d", l), "", 500, -200, 500, 300, 0, 75);
	}

	auto hTotVsTofLayer1_VL0 = new TH2D("hTotVsTofLayer2_VL1", "", 500, -200, 500, 300, 0, 75);
	auto hTotVsTofLayer2_VL1 = new TH2D("hTotVsTofLayer2_VL2", "", 500, -200, 500, 300, 0, 75);

	/*************************************************************************************************************************************
	 */
	auto barWidth = barWidthInMeter * 1000; // mm
	auto barGap = barGapInMeter * 1000;		// mm

	auto nEvents = chain->GetEntries();
	std::cout << "Number of events: " << nEvents << std::endl;
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		chain->GetEvent(iEvt);

		auto isVetoed = isApplyingVeto && spirit.hime_veto_multi == 0;
		bool isLayer0Hit = false;
		bool isLayer1Hit = false;
		for (auto h = 0; h < spirit.hime_nHits; h++) {
			auto hitModule = spirit.hime_moduleID[h];
			if (getLayer(hitModule) == 0) {
				isLayer0Hit = true;
			}
			if (getLayer(hitModule) == 1) {
				isLayer1Hit = true;
			}
		}

		for (auto h = 0; h < spirit.hime_nHits; h++) {

			auto hitModule = spirit.hime_moduleID[h];

			// Channel 0 -> TPC trigger
			// Channel 1 -> SBT left
			// Channel 8 -> HIME veto OR
			// Channel 9 -> SBT right

			if (hitModule == 0 || hitModule == 1 || hitModule == 8 || hitModule == 9) {
				continue;
			}

			auto tDiff = spirit.hime_tDiff[h] + posOffset[hitModule] / calibVelocity[hitModule];
			auto barPos = calibVelocity[hitModule] * tDiff; // position along the bar
			barPos *= 10;									// cm -> mm

			auto layer = getLayer(hitModule);
			if (layer == -1) {
				std::cerr << "Invalid module ID: " << hitModule << std::endl;
				continue;
			}

			auto distance = getDistanceInMeter(layer); // m
			auto centerModule = getCenterModule(layer);
			double deltaModule = centerModule - static_cast<int>(hitModule); // hitModule is unsigned int
			double xHit, yHit;
			if (layer == 0 || layer == 2) {
				xHit = barPos;
				yHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else if (layer == 1) {
				// std::swap(xHit, yHit);
				yHit = barPos;
				xHit = 0.5 * barGap + deltaModule * (barWidth + barGap) + barWidth * unif(gen);
			} else {
				std::cerr << "Should never happen, layer = " << layer << std::endl;
				std::exit(1);
			}

			auto rHit = std::sqrt(xHit * xHit + yHit * yHit) * 1e-3; // mm -> m
			auto distanceHit = std::sqrt(distance * distance + rHit * rHit);
			auto tof = spirit.hime_tofRaw[h] - timeOffset[hitModule] - (distanceHit - distance) / C_LIGHT;
			auto tot = std::sqrt(spirit.hime_tot0[h] * spirit.hime_tot1[h]);

			if (isVetoed) {
				hHitPatternFull->Fill(xHit, yHit);
				hHitPatternLayer[layer]->Fill(xHit, yHit);
				hTotVsTof->Fill(tof, tot);
				hTotVsTofLayer[layer]->Fill(tof, tot);

				if (layer == 1 && isLayer0Hit) {
					hTotVsTofLayer1_VL0->Fill(tof, tot);
				}
				if (layer == 2 && isLayer1Hit) {
					hTotVsTofLayer2_VL1->Fill(tof, tot);
				}
			}
		}
	}

	// normalization
	hHitPatternFull->Scale(1.0 / nEvents);
	hTotVsTof->Scale(1.0 / nEvents);
	for (auto l = 0; l < 3; l++) {
		hHitPatternLayer[l]->Scale(1.0 / nEvents);
		hTotVsTofLayer[l]->Scale(1.0 / nEvents);
	}
	hTotVsTofLayer1_VL0->Scale(1.0 / nEvents);
	hTotVsTofLayer2_VL1->Scale(1.0 / nEvents);

	auto fileOut = new TFile(filenameOut.c_str(), "RECREATE");
	hHitPatternFull->Write();
	hTotVsTof->Write();
	for (auto l = 0; l < 3; l++) {
		hHitPatternLayer[l]->Write();
		hTotVsTofLayer[l]->Write();
	}
	hTotVsTofLayer1_VL0->Write();
	hTotVsTofLayer2_VL1->Write();
	fileOut->Close();

	// auto canvas = new TCanvas("canvas", "canvas", 800, 600);
	// hTotVsTof->GetYaxis()->SetRangeUser(0, 50);
	// hTotVsTof->Draw("COLZ");
	// canvas->SaveAs("hTotVsTof.png");

	// auto canvas = new TCanvas("canvas", "canvas", 800, 600);
	// canvas->Divide(3, 1);
	// for (int i = 0; i < 3; i++) {
	// 	canvas->cd(i + 1);
	// 	hTotVsTofLayer[i]->Draw("COLZ");
	// }
	// canvas->SaveAs("hTotVsTofLayer.png");

	// delete canvas;
	// canvas = new TCanvas("canvas", "canvas", 800, 600);
	// canvas->Divide(3, 1);
	// for (int i = 0; i < 3; i++) {
	// 	canvas->cd(i + 1);
	// 	hTotVsTofLayer[i]->ProjectionY()->Draw("COLZ");
	// }
	// canvas->SaveAs("hTotLayer.png");

	return;
}

std::vector<double> getOffset(const std::string &filename) {
	std::vector<double> offset(nModules, 0);
	std::ifstream infile(filename.c_str());
	if (!infile.is_open()) {
		std::cerr << "Could not open file " << filename << std::endl;
		return offset;
	}

	std::string line;
	while (std::getline(infile, line)) {
		if (line[0] == '#') {
			continue;
		}
		std::stringstream ss(line);
		int detid;
		ss >> detid >> offset[detid];
	}
	return offset;
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

int getLayer(int hitModule) {
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

int getCenterModule(int layer) {
	switch (layer) {
	case 0:
		return 11;
	case 1:
		return 35;
	case 2:
		return 59;
	default:
		return -1;
	}
	return -1;
}

double getDistanceInMeter(int layer) {
	// all in meter
	if (layer == 0) {
		return 4.8;
	}
	if (layer == 1) {
		return 4.8 + 0.06;
	}
	if (layer == 2) {
		return 4.8 + 0.06 + 0.06;
	}
	return -DBL_MAX;
}