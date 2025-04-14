#include "spirit.h"
#include <vector>

std::vector<double> getOffset(const std::string &filename);
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);

const int nModules = 72;
void draw_HIME_experiment(
	// clang-format off
	int mTotMin = 0,		 // min threshold for ToT
	int mTotMax = 1000,		 // max threshold for ToT
	bool mVetoWallMult0 = 1, // if 1 then mutl 0 in veto is required
	int calibrate_tof = 0, 
    bool calibrate_walk = 0,
	// if >0 then tof offsets are ignored and new offsets are calculated (using landau or gaus fit)
	const std::vector<std::string> &files =
		{
            "spirit/data1148.root",
   //         "spirit/data1149.root",
            "spirit/data1150.root",
            "spirit/data1151.root",
            "spirit/data1152.root",
            "spirit/data1153.root",
            "spirit/data1154.root",
            "spirit/data1155.root",
            "spirit/data1158.root",
            "spirit/data1159.root",
            "spirit/data1160.root",
			"spirit/data1161.root",
			"spirit/data1162.root",
			"spirit/data1163.root",
			"spirit/data1164.root",
			"spirit/data1165.root",
			"spirit/data1166.root",
			"spirit/data1167.root",
			"spirit/data1168.root",
			"spirit/data1169.root",
			"spirit/data1170.root",
			"spirit/data1171.root",
			"spirit/data1172.root",
			"spirit/data1173.root",
			"spirit/data1174.root",
			"spirit/data1175.root",
			"spirit/data1176.root",
			"spirit/data1177.root",
			"spirit/data1178.root",
			"spirit/data1179.root",
			"spirit/data1180.root",
			"spirit/data1181.root",
			"spirit/data1182.root",
			"spirit/data1183.root",
			"spirit/data1184.root",
			"spirit/data1185.root",
			"spirit/data1186.root",
	//		"spirit/data1187.root",
			"spirit/data1188.root",
			"spirit/data1189.root",
			"spirit/data1190.root",
			"spirit/data1191.root",
			"spirit/data1192.root",
			"spirit/data1193.root",
			"spirit/data1194.root",
			"spirit/data1195.root",
			"spirit/data1196.root",
			"spirit/data1197.root",
			"spirit/data1198.root",
			"spirit/data1199.root",
			"spirit/data1200.root",
			"spirit/data1201.root",
			"spirit/data1202.root",
			"spirit/data1203.root",
			"spirit/data1204.root",
			"spirit/data1205.root",
			"spirit/data1206.root",
			"spirit/data1207.root",
			"spirit/data1208.root",
//			"spirit/data1209.root",
			"spirit/data1210.root",
			"spirit/data1211.root",
			"spirit/data1212.root",
			"spirit/data1213.root",
			"spirit/data1214.root",
			"spirit/data1215.root",
			"spirit/data1216.root",
			"spirit/data1217.root",
			"spirit/data1218.root",
			"spirit/data1219.root",
			// "spirit/data1220.root",
			"spirit/data1221.root",
			"spirit/data1222.root",
			"spirit/data1223.root",
			// "spirit/data1224.root",
			"spirit/data1225.root",
			"spirit/data1226.root",
			"spirit/data1227.root",
			"spirit/data1228.root",
			"spirit/data1229.root",
			"spirit/data1230.root",
			"spirit/data1231.root",
			"spirit/data1232.root",
			// "spirit/data1233.root",
			"spirit/data1234.root",
			"spirit/data1235.root", 
			"spirit/data1236.root",
			// "spirit/data1237.root",
			"spirit/data1238.root",
			"spirit/data1239.root",
			"spirit/data1240.root",
			"spirit/data1241.root",
			"spirit/data1242.root",
			"spirit/data1243.root",
			"spirit/data1244.root",
			"spirit/data1245.root", 
			"spirit/data1246.root",
			"spirit/data1247.root",
	/*		"spirit/data1248.root",
			"spirit/data1249.root",
			"spirit/data1250.root",
			"spirit/data1251.root",
			"spirit/data1252.root",
			"spirit/data1253.root",
			"spirit/data1254.root",
			"spirit/data1255.root", 
			"spirit/data1256.root",
			"spirit/data1257.root",
			"spirit/data1258.root",
		"spirit/data1259.root",
			"spirit/data1260.root",
			"spirit/data1261.root",
			"spirit/data1262.root",
			"spirit/data1263.root",
			"spirit/data1264.root",
			"spirit/data1265.root", 
			"spirit/data1266.root",
			"spirit/data1267.root",
				*/

			"spirit/data1268.root",
			"spirit/data1269.root",
//			"spirit/data1270.root",
//			"spirit/data1271.root",
//			"spirit/data1272.root",
			"spirit/data1273.root",
			"spirit/data1274.root",
			"spirit/data1275.root", 
			"spirit/data1276.root",
			"spirit/data1277.root",
//			"spirit/data1278.root",
			"spirit/data1279.root",
			"spirit/data1280.root",
			"spirit/data1241.root",
			"spirit/data1282.root",
			"spirit/data1283.root",
			"spirit/data1284.root",
			"spirit/data1285.root", 
			"spirit/data1286.root",
			"spirit/data1287.root",
			"spirit/data1288.root",
			"spirit/data1289.root",
			"spirit/data1290.root",
			"spirit/data1291.root",
			"spirit/data1292.root",
			"spirit/data1293.root",
			"spirit/data1294.root",
			"spirit/data1295.root", 
			"spirit/data1296.root",
			"spirit/data1297.root",
			"spirit/data1298.root",
			"spirit/data1299.root",
      	}, 
	std::string filenameOut = "hime_anal_experiment", 
	//const std::string &posOffsetFile = "database/pos_offsets_marco.txt",
    const std::string& posOffsetFile = "database/pos_offsets_experiment.txt",
	const std::string &timeOffsetFile = "database/time_offsets_experiment_gaus.txt",
	const std::string &velocityFile = "database/velocity_marco.txt", bool debug = 0
	// clang-format on
) {
	filenameOut += "_TOTmin";
	filenameOut += std::to_string(mTotMin);
	filenameOut += "_TOTmax";
	filenameOut += std::to_string(mTotMax);
	if (mVetoWallMult0)
		filenameOut += "_VetoMult0";
	else
		filenameOut += "_NoVeto";
	if (calibrate_tof) {

		filenameOut += "_OffsetCalibration";
		if (calibrate_tof == 1)
			filenameOut += "Landau";
		else if (calibrate_tof == 2)
			filenameOut += "Gaus";
	}
	if (calibrate_walk)
		filenameOut += "_WalkCalibration";
	filenameOut += ".root";

	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);
	std::vector<double> pos_offset = getOffset(posOffsetFile);
	std::vector<double> time_offset = getOffset(timeOffsetFile);
	std::vector<double> velocity = getOffset(velocityFile);

	for (int i = 0; i < nModules; i++) {
		// we only have to do it when reading "our" dt calibration as we apply the shift to time,
		// Marco applies the shift to position
		pos_offset[i] = -1. * pos_offset[i] * velocity[i];
	}

	// initialize histograms

	int tdiff_nbins = 250;

	// initialize histograms
	TH1F hTof("hTof", "ToF (roughly calibrated)", 10000, -300, 500);
	TH1F hTofL1("hTofL1", "ToF (L1) (roughly calibrated)", 10000, -300, 500);
	TH1F hTofL2("hTofL2", "ToF (L2) (roughly calibrated)", 10000, -300, 500);
	TH1F hTofL3("hTofL3", "ToF (L3) (roughly calibrated)", 10000, -300, 500);
	TH2F hTofVsModuleID("hTofVsModuleID", "ToF vs. module ID", 72, 0, 72, 5000, -250, 250);
	TH1F hSbtTimeOfTrigger("hSbtTimeOfTrigger", "SBT time of trigger", 100000, -300, 500);

	TH2F hTotVsTof("hTotVsTof", "ToT vs. ToF", 2000, -100, 300, 100, 0, 75);
	TH2F hTotVsTofL1("hTotVsTofL1", "ToT vs. ToF (L1)", 2000, -100, 300, 100, 0, 75);
	TH2F hTotVsTofL2("hTotVsTofL2", "ToT vs. ToF (L2)", 2000, -100, 300, 100, 0, 75);
	TH2F hTotVsTofL3("hTotVsTofL3", "ToT vs. ToF (L3)", 2000, -100, 300, 100, 0, 75);

	TH2F *hTotVsTof_d[nModules];
	for (int i = 0; i < nModules; i++) {
		TString name = "";
		name += i;
		hTotVsTof_d[i] =
			new TH2F(("hTotVsTof_d" + name).Data(), ("ToT vs. ToF det=" + name).Data(), 20000, -100, 300, 100, 0, 75);
	}
	TH2F hTotVsTof_L2_VL1("hTotVsTof_L2_VL1", "ToT vs. ToF", 2000, -100, 300, 100, 0, 75);
	TH2F hTotVsTof_L2_L1("hTotVsTof_L2_L1", "ToT vs. ToF", 2000, -100, 300, 100, 0, 75);

	TH2F hModuleIDVsTDiff(
		"hModuleIDVsTDiff", "Time difference between PMTs vs. module ID", tdiff_nbins, -25, 25, 72, 0, 72
	);
	TH2F hxPosVsyPosL1(
		"hxPosVsyPosL1", "x position vs. y position (L1)", tdiff_nbins, -1000, 1000, tdiff_nbins, -1000, 1000
	);
	TH2F hxPosVsyPosL2(
		"hxPosVsyPosL2", "x position vs. y position (L2)", tdiff_nbins, -1000, 1000, tdiff_nbins, -1000, 1000
	);
	TH2F hxPosVsyPosL3(
		"hxPosVsyPosL3", "x position vs. y position (L3)", tdiff_nbins, -1000, 1000, tdiff_nbins, -1000, 1000
	);
	TH2F hxPosVsyPosAll(
		"hxPosVsyPosAll", "x position vs. y position (all)", tdiff_nbins, -1000, 1000, tdiff_nbins, -1000, 1000
	);

	TH2F hModuleIDVsTDiffL1gated(
		"hModuleIDVsTDiffL1gated", "Time difference between PMTs vs. module ID(L1)", tdiff_nbins, -12, 12, 24, 0, 24
	);
	TH2F hModuleIDVsTDiffL2gated(
		"hModuleIDVsTDiffL2gated", "Time difference between PMTs vs. module ID(L2)", tdiff_nbins, -12, 12, 24, 24, 48
	);
	TH2F hModuleIDVsTDiffL3gated(
		"hModuleIDVsTDiffL3gated", "Time difference between PMTs vs. module ID(L3)", tdiff_nbins, -12, 12, 24, 48, 72
	);
	TH2F hModuleIDVsTDiffL1(
		"hModuleIDVsTDiffL1", "Time difference between PMTs vs. module ID(L1)", tdiff_nbins, -12, 12, 24, 0, 24
	);
	TH2F hModuleIDVsTDiffL2(
		"hModuleIDVsTDiffL2", "Time difference between PMTs vs. module ID(L2)", tdiff_nbins, -12, 12, 24, 24, 48
	);
	TH2F hModuleIDVsTDiffL3(
		"hModuleIDVsTDiffL3", "Time difference between PMTs vs. module ID(L3)", tdiff_nbins, -12, 12, 24, 48, 72
	);
	TH2F hModuleIDVsTDiffgatedV0(
		"hModuleIDVsTDiffgatedV0", "Time difference between PMTs vs. module ID gated on V0", 400, -25, 25, 72, 0, 72
	);
	TH2F hModuleIDVsTDiffgatedV1(
		"hModuleIDVsTDiffgatedV1", "Time difference between PMTs vs. module ID gated on V1", 400, -25, 25, 72, 0, 72
	);
	TH2F hModuleIDVsTDiffgatedV2(
		"hModuleIDVsTDiffgatedV2", "Time difference between PMTs vs. module ID gated on V2", 400, -25, 25, 72, 0, 72
	);

	TH2F hVW_ToTvsTDiffD1("hVW_ToTvsTDiffD1", "VW ToT vs time difference (det 1)", 200, -10, 10, 100, 0, 350);
	TH2F hVW_ToTvsTDiffD1_gated(
		"hVW_ToTvsTDiffD1_gated", "VW ToT vs time difference (det 1) gated", 200, -10, 10, 100, 0, 350
	);
	TH2F hVW_ToTvsTDiffD2("hVW_ToTvsTDiffD2", "VW ToT vs time difference (det 2)", 200, -10, 10, 100, 0, 350);
	TH2F hVW_ToTvsTDiffD2_gated(
		"hVW_ToTvsTDiffD2_gated", "VW ToT vs time difference (det 2) gated", 200, -10, 10, 100, 0, 350
	);
	TH2F hVW_ToTvsTDiffD3("hVW_ToTvsTDiffD3", "VW ToT vs time difference (det 3)", 200, -10, 10, 100, 0, 350);
	TH2F hVW_ToTvsTDiffD3_gated(
		"hVW_ToTvsTDiffD3_gated", "VW ToT vs time difference (det 3) gated", 200, -10, 10, 100, 0, 350
	);
	TH2F hVW_ToTvsXposD1("hVW_ToTvsXposD1", "VW ToT vs pos (det 1)", 200, -800, 800, 100, 0, 350);
	TH2F hVW_ToTvsXposD2("hVW_ToTvsXposD2", "VW ToT vs pos (det 2)", 200, -800, 800, 100, 0, 350);
	TH2F hVW_ToTvsXposD3("hVW_ToTvsXposD3", "VW ToT vs pos (det 3)", 200, -800, 800, 100, 0, 350);

	hTof.GetXaxis()->SetTitle("ToF (ns)");
	hTof.GetYaxis()->SetTitle("Counts");
	hTofVsModuleID.GetXaxis()->SetTitle("ModuleID");
	hTofVsModuleID.GetYaxis()->SetTitle("ToF (ns)");
	hTotVsTof.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTof.GetYaxis()->SetTitle("ToT (ns)");
	hTotVsTofL1.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTofL1.GetYaxis()->SetTitle("ToT (ns)");
	hTotVsTofL2.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTofL2.GetYaxis()->SetTitle("ToT (ns)");
	hTotVsTofL3.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTofL3.GetYaxis()->SetTitle("ToT (ns)");
	hTotVsTof_L2_VL1.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTof_L2_VL1.GetYaxis()->SetTitle("ToT (ns)");
	hTotVsTof_L2_L1.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTof_L2_L1.GetYaxis()->SetTitle("ToT (ns)");
	hModuleIDVsTDiff.GetYaxis()->SetTitle("Module ID");
	hModuleIDVsTDiff.GetXaxis()->SetTitle("Time difference (ns)");

	bool hime_veto_bar_fired[3];
	bool calibrationBarL1Hit;
	bool calibrationBarL2Hit;
	bool calibrationBarL3Hit;
	bool calibration_Det35_Hit;
	bool isHitLayer1 = 0;
	bool isHitLayer2 = 0;
	bool isHitLayer3 = 0;
	bool isVetoCharge = 0;
	double tofCalib;
	double pos_calibrated;
	double x_pos_calibrated;
	double y_pos_calibrated;
	// loop over events: fill histograms
	double sbtTimeOfTrigger;
	auto nEvents = chain->GetEntries();

	std::cout << "Number of events: " << nEvents << std::endl;

	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		if (debug) {
			std::cout << "event = " << iEvt << "\t nHits=" << spirit.hime_nHits << std::endl;
		}
		chain->GetEvent(iEvt);

		calibrationBarL1Hit = 0;
		calibrationBarL2Hit = 0;
		calibrationBarL3Hit = 0;
		calibration_Det35_Hit = 0;

		isHitLayer1 = 0;
		isHitLayer2 = 0;
		isHitLayer3 = 0;
		isVetoCharge = 0;

		// used to calibrate offsets in dt
		for (auto h = 0; h < spirit.hime_nHits; h++) {
			int mod = spirit.hime_moduleID[h];

			if (mod == 1) {
				sbtTimeOfTrigger = spirit.hime_tofRaw[h];
				hSbtTimeOfTrigger.Fill(sbtTimeOfTrigger);
			}
			if (mod == 35)
				calibration_Det35_Hit = 1;
			if (mod == 4 || mod == 11 || mod == 18) {
				calibrationBarL1Hit = 1;
			}
			if (mod == 28 || mod == 35 || mod == 42) {
				calibrationBarL2Hit = 1;
			}
			if (mod == 32 || mod == 59 || mod == 66) {
				calibrationBarL3Hit = 1;
			}

			if (mod > 1 && mod < 24) {
				isHitLayer1 = 1;
			}
			if (mod > 23 && mod < 48) {
				isHitLayer2 = 1;
			}
			if (mod > 47 && mod < 72) {
				isHitLayer3 = 1;
			}
		}
		hime_veto_bar_fired[0] = 0;
		hime_veto_bar_fired[1] = 0;
		hime_veto_bar_fired[2] = 0;
		double nVetoWallXpos;
		for (int v_hit = 0; v_hit < spirit.vetoMulti; v_hit++) {
			isVetoCharge = 1;
			if (spirit.vetoBarId[v_hit] == 0) {
				hime_veto_bar_fired[0] = 1;
				hVW_ToTvsTDiffD1.Fill(spirit.vetoTdiff[v_hit] / 40, spirit.vetoTot[v_hit] / 40);
				if (calibrationBarL2Hit) {
					hVW_ToTvsTDiffD1_gated.Fill(spirit.vetoTdiff[v_hit] / 40, spirit.vetoTot[v_hit] / 40);
					//					cout << spirit.vetoTdiff[v_hit] / 40 << " , " <<
					// spirit.vetoTot[v_hit] / 40 << endl;
				}
				nVetoWallXpos = -0.0755 * spirit.vetoTdiff[v_hit] / 40 + 0.0416;
				nVetoWallXpos *= 1000;
				hVW_ToTvsXposD1.Fill(nVetoWallXpos, spirit.vetoTot[v_hit] / 40);
			}
			if (spirit.vetoBarId[v_hit] == 1) {
				hime_veto_bar_fired[1] = 1;
				hVW_ToTvsTDiffD2.Fill(spirit.vetoTdiff[v_hit] / 40, spirit.vetoTot[v_hit] / 40);
				if (calibrationBarL2Hit) {
					hVW_ToTvsTDiffD2_gated.Fill(spirit.vetoTdiff[v_hit] / 40, spirit.vetoTot[v_hit] / 40);
				}
				nVetoWallXpos = -0.0726 * spirit.vetoTdiff[v_hit] / 40 + 0.04402;
				nVetoWallXpos *= 1000;
				hVW_ToTvsXposD2.Fill(nVetoWallXpos, spirit.vetoTot[v_hit] / 40);
			}
			if (spirit.vetoBarId[v_hit] == 2) {
				hime_veto_bar_fired[2] = 1;
				hVW_ToTvsTDiffD3.Fill(spirit.vetoTdiff[v_hit] / 40, spirit.vetoTot[v_hit] / 40);
				if (calibrationBarL2Hit) {
					hVW_ToTvsTDiffD3_gated.Fill(spirit.vetoTdiff[v_hit] / 40, spirit.vetoTot[v_hit] / 40);
				}
				nVetoWallXpos = -0.0759 * spirit.vetoTdiff[v_hit] / 40 - 0.0167;
				nVetoWallXpos *= 1000;
				hVW_ToTvsXposD3.Fill(nVetoWallXpos, spirit.vetoTot[v_hit] / 40);
			}
		}
		for (auto h = 0; h < spirit.hime_nHits; h++) {
			int mod = spirit.hime_moduleID[h];
			//			if (mod != 25) continue;
			double tot = std::sqrt(spirit.hime_tot0[h] * spirit.hime_tot1[h]); // geo mean

			if (mod > 1 && mod < nModules) {
				double dt_calibrated = spirit.hime_tDiff[h] + pos_offset[mod] / velocity[mod];
				pos_calibrated = dt_calibrated * velocity[mod]; // spirit.hime_tDiff[h]*velocity[mod]+pos_offset[mod];

				double totdistance = 4.8;
				if (mod >= 24)
					totdistance += 0.06;
				if (mod >= 48)
					totdistance += 0.06;
				double xhit = 0;
				double yhit = 0;
				double rhit = 0;
				// layer 2
				if (mod > 23 && mod < 48) {
					xhit = (-0.002 - (mod - 35) * 0.004 - 0.040 * (mod - 35 - gRandom->Uniform(0, 1)));
					yhit = pos_calibrated;
					x_pos_calibrated = xhit * 1000;
					y_pos_calibrated = pos_calibrated * 10;
				}
				// layer 1 and 3
				// gap between detectors is 4mm
				// height of the detector is 40mm
				if (mod < 24 || mod >= 48) {
					xhit = x_pos_calibrated;
					if (mod < 24)
						yhit = -0.002 - (mod - 11) * 0.004 - 0.040 * (mod - 11 - gRandom->Uniform(0, 1));
					if (mod >= 48)
						yhit = -0.002 - (mod - 59) * 0.004 - 0.040 * (mod - 59 - gRandom->Uniform(0, 1));
					x_pos_calibrated = pos_calibrated * 10;
					y_pos_calibrated = yhit * 1000;
				}
				// convert to meters
				xhit /= 1000;
				yhit /= 1000;
				rhit = TMath::Sqrt(xhit * xhit + yhit * yhit);

				double posdep = TMath::Sqrt(TMath::Power(rhit, 2) + TMath::Power(totdistance, 2));
				posdep -= totdistance;
				double timecorrection = posdep / 2.9999 * 10;
				if (debug) {
					cout << mod << "\t" << (spirit.hime_tDiff[h] - pos_offset[mod]) << "\t" << "(" << xhit << ","
						 << yhit << ") = " << rhit << "\t" << posdep;
					cout << "\t" << timecorrection << endl;
				}
				tofCalib =
					spirit.hime_tofRaw[h] - (calibrate_tof ? 0 : time_offset[mod]) - TMath::Abs(posdep / 2.9999 * 10.);
				tofCalib += sbtTimeOfTrigger;
				tofCalib -= -110.82;
				if (!calibrate_tof)
					tofCalib += (11.8 + 2. * 4.8 / 2.9999 * 10);
				// tofCalib += 0;

				double totTresh = 22.13;
				//					tofCalib -= (18.574 - 0.1967 * tot - 3.1055);

				if (!calibrate_walk)
					tofCalib += 0; // 15.4;
				if (!calibrate_walk && (tot < totTresh))
					tofCalib -= (-0.1599 * tot - 6.4614);
				if (!calibrate_walk && (tot >= totTresh))
					tofCalib -= (-0.1599 * totTresh - 6.4614);
				//					tofCalib -= (18.574 - 0.2232 * totTresh + 0.0019 * totTresh * totTresh);
				//				if (calibrate_walk && tot > (tofCalib * (-10.) + 200))
				//				if (calibrate_walk && tot > (tofCalib * (-5.1734) - 4.8925))
				if (calibrate_walk && tot > (tofCalib * (-15.) - 90))
					tot = 0;
				double hime_pos_cut = 600;
				if ((calibrate_tof ? (tofCalib > -180) : (tofCalib > -100)) &&
					(calibrate_tof ? (tofCalib < -50) : (tofCalib < 300)) && (tot >= mTotMin && tot <= mTotMax) &&
					(mVetoWallMult0 ? (spirit.vetoMulti == 0) : (spirit.vetoMulti >= 0)) &&
					x_pos_calibrated > -1 * hime_pos_cut && x_pos_calibrated < hime_pos_cut &&
					y_pos_calibrated > -1 * hime_pos_cut && y_pos_calibrated < hime_pos_cut) {

					// if (x_pos_calibrated > -300 && x_pos_calibrated < 300 && y_pos_calibrated > -300 &&
					//	y_pos_calibrated < 300) {
					hTotVsTof.Fill(tofCalib, tot);
					hTotVsTof_d[mod]->Fill(tofCalib, tot);
					hxPosVsyPosAll.Fill(x_pos_calibrated, y_pos_calibrated);
					//}
					if (!isHitLayer1) {
						if (mod >= 24 && mod < 48) {
							hTotVsTof_L2_VL1.Fill(tofCalib, tot);
						}
					}
					if (isHitLayer1) {
						if (mod >= 24 && mod < 48) {
							hTotVsTof_L2_L1.Fill(tofCalib, tot);
						}
					}
					if (mod < 24) {
						hxPosVsyPosL1.Fill(x_pos_calibrated, y_pos_calibrated);
						hTofL1.Fill(tofCalib);
						hTotVsTofL1.Fill(tofCalib, tot);
					}
					if (mod >= 48) {
						hxPosVsyPosL3.Fill(x_pos_calibrated, y_pos_calibrated);
						hTofL3.Fill(tofCalib);
						hTotVsTofL3.Fill(tofCalib, tot);
					}
					if (mod >= 24 && mod < 48) {
						hxPosVsyPosL2.Fill(x_pos_calibrated, y_pos_calibrated);
						hTofL2.Fill(tofCalib);
						hTotVsTofL2.Fill(tofCalib, tot);
					}

					hTof.Fill(tofCalib);

					hTofVsModuleID.Fill(mod, tofCalib);
					hModuleIDVsTDiff.Fill(dt_calibrated, mod);

					if (hime_veto_bar_fired[0])
						hModuleIDVsTDiffgatedV0.Fill(dt_calibrated, mod);
					if (hime_veto_bar_fired[1])
						hModuleIDVsTDiffgatedV1.Fill(dt_calibrated, mod);
					if (hime_veto_bar_fired[2])
						hModuleIDVsTDiffgatedV2.Fill(dt_calibrated, mod);

					/*
										for (int v_hit = 0; v_hit < spirit.vetoMulti; v_hit++) {
											if (spirit.vetoBarId[v_hit] == 0)
												hModuleIDVsTDiffgatedV0.Fill(dt_calibrated, mod);
											if (spirit.vetoBarId[v_hit] == 1)
												hModuleIDVsTDiffgatedV1.Fill(dt_calibrated, mod);
											if (spirit.vetoBarId[v_hit] == 2)
												hModuleIDVsTDiffgatedV2.Fill(dt_calibrated, mod);
				}
					*/

					// layer 1
					if (mod < 24) {
						hModuleIDVsTDiffL1.Fill(dt_calibrated, mod);
						if (calibrationBarL2Hit) {
							hModuleIDVsTDiffL1gated.Fill(dt_calibrated, mod);
						}
					}
					// layer 2
					if (mod >= 24 && mod < 48) {
						hModuleIDVsTDiffL2.Fill(dt_calibrated, mod);
						if (calibrationBarL1Hit) {
							hModuleIDVsTDiffL2gated.Fill(dt_calibrated, mod);
						}
					}
					// layer 3
					if (mod >= 48 && mod < 72) {
						hModuleIDVsTDiffL3.Fill(dt_calibrated, mod);
						if (calibrationBarL2Hit) {
							hModuleIDVsTDiffL3gated.Fill(dt_calibrated, mod);
						}
					}
				}
			}
		}
	}
	TH1F *hTofVsModuleID_d[nModules];
	if (calibrate_tof) {

		std::vector<double> toff(nModules, 0);
		TF1 *ftemp[nModules];
		for (int i = 2; i < nModules; i++) {
			cout << "############### detector = " << i << endl;
			toff[i] = 0;
			TString nnn = "";
			nnn += (i);

			hTofVsModuleID_d[i] = (TH1F *)hTofVsModuleID.ProjectionY(("hTofVsModuleID_d" + nnn).Data(), i + 1, i + 1);
			hTofVsModuleID_d[i]->Rebin(10);
			if (calibrate_tof == 1) {
				hTofVsModuleID_d[i]->Fit("landau");
				ftemp[i] = (TF1 *)hTofVsModuleID_d[i]->GetListOfFunctions()->FindObject("landau");
			}
			double fitxmax;
			if (calibrate_tof == 2) {
				fitxmax = hTofVsModuleID_d[i]->GetBinCenter(hTofVsModuleID_d[i]->GetMaximumBin());
				fitxmax += 5;
				hTofVsModuleID_d[i]->Fit("gaus", "", "", -150, fitxmax);
				ftemp[i] = (TF1 *)hTofVsModuleID_d[i]->GetListOfFunctions()->FindObject("gaus");
			}
			toff[i] = ftemp[i]->GetParameter(1);
		}
		for (int i = 0; i < nModules; i++)
			cout << i << "\t" << toff[i] << endl;
	}

	TH1F *hTotVsTof_t[1000];
	TGraph *offset_vs_tot;
	if (calibrate_walk) {
		TF1 *ftemp[1000];
		double walk_x[40];
		double walk_y[40];
		int k = 0;
		std::vector<double> walk(1000, 0);
		for (int i = 2; i <= 33; i++) { //(hTotVsTof.GetYaxis())->GetNbins(); i++) {
			//	cout << "############### bin = " << i << "\t" << (hTotVsTof.GetYaxis())->GetBinCenter(i) << endl;
			walk[i] = 0;
			TString nnn = "";
			nnn += (i);

			hTotVsTof_t[i] = (TH1F *)hTotVsTof.ProjectionX(("hTotVsTof_t" + nnn).Data(), i + 1, i + 1);
			hTotVsTof_t[i]->Fit("gaus", "q", "", -12, -4);
			ftemp[i] = (TF1 *)hTotVsTof_t[i]->GetListOfFunctions()->FindObject("gaus");
			//			cout << "integral = " << ftemp[i]->GetIntegral() << endl;
			walk[i] = ftemp[i]->GetParameter(1);
			//			hTotVsTof_t[i]->Rebin(10);

			//			cout << i << "\t" ;
			//			cout << (hTotVsTof.GetYaxis())->GetBinCenter(i) << endl; //<< " , " << walk[i] << endl;
			cout << walk[i] << endl;
			walk_x[k] = (hTotVsTof.GetYaxis())->GetBinCenter(i);
			walk_y[k] = walk[i];
			k++;
		}
		offset_vs_tot = new TGraph(k - 1, walk_y, walk_x);

		cout << "done with walk calibration" << endl;
	}
	auto fileOut = new TFile(filenameOut.c_str(), "RECREATE");
	hModuleIDVsTDiffL1gated.Write();
	hModuleIDVsTDiffL2gated.Write();
	hModuleIDVsTDiffL3gated.Write();
	hModuleIDVsTDiffgatedV0.Write();
	hModuleIDVsTDiffgatedV1.Write();
	hModuleIDVsTDiffgatedV2.Write();
	hModuleIDVsTDiffL1.Write();
	hModuleIDVsTDiffL2.Write();
	hModuleIDVsTDiffL3.Write();
	hModuleIDVsTDiff.Write();
	hTof.Write();
	hTotVsTof_L2_VL1.Write();
	hTotVsTof_L2_L1.Write();
	hTofL1.Write();
	hTofL2.Write();
	hTofL3.Write();
	hSbtTimeOfTrigger.Write();

	hxPosVsyPosL1.SetTitle("hit position (layer 1)");
	hxPosVsyPosL2.SetTitle("hit position (layer 2)");
	hxPosVsyPosL3.SetTitle("hit position (layer 3)");
	hxPosVsyPosL1.Write();
	hxPosVsyPosL2.Write();
	hxPosVsyPosL3.Write();
	hTofVsModuleID.Write();
	hxPosVsyPosAll.SetTitle("hit position (all layers)");
	hxPosVsyPosAll.Write();
	hTotVsTof.Write();
	hTotVsTofL1.Write();
	hTotVsTofL2.Write();
	hTotVsTofL3.Write();

	hVW_ToTvsTDiffD1.Write();
	hVW_ToTvsTDiffD1_gated.Write();
	hVW_ToTvsTDiffD2.Write();
	hVW_ToTvsTDiffD2_gated.Write();
	hVW_ToTvsTDiffD3.Write();
	hVW_ToTvsTDiffD3_gated.Write();

	hVW_ToTvsXposD1.Write();
	hVW_ToTvsXposD2.Write();
	hVW_ToTvsXposD3.Write();
	for (int i = 0; i < nModules; i++)
		hTotVsTof_d[i]->Write();
	if (calibrate_tof) {
		for (int i = 2; i < nModules; i++)
			hTofVsModuleID_d[i]->Write();
	}
	if (calibrate_walk) {
		for (int i = 2; i <= 33; i++) { //(hTotVsTof.GetYaxis())->GetNbins(); i++) {
			hTotVsTof_t[i]->Write();
		}
		offset_vs_tot->SetName("offset_vs_tot");
		offset_vs_tot->Write();
	}

	cout << "done saving histograms" << endl;
	fileOut->Close();
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

void addFilesToChain(const std::vector<std::string> &files, TChain *&chain) {
	for (auto fname : files) {
		chain->AddFile(fname.c_str());
	}
	return;
}
