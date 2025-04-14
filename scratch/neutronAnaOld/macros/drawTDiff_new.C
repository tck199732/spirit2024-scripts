#include "spirit.h"
#include <vector>

std::vector<double> getOffset(const std::string &filename);
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);

const int nModules = 72;
void drawTDiff_new(
	int mTotMin = 0,		 // min threshold for ToT
	int mTotMax = 1000,		 // max threshold for ToT
	bool mVetoWallMult0 = 0, // if 1 then mutl 0 in veto is required
	int calibrate_tof =
		0, // if >0 then tof offsets are ignored and new offsets are calculated (using landau or gaus fit)
		   // clang-format off
    const std::vector<std::string> &files = {
        "spirit/data1036.root",
        "spirit/data1037.root",
        "spirit/data1038.root",
        "spirit/data1039.root",
        "spirit/data1040.root",
        "spirit/data1041.root",
        "spirit/data1042.root",
        "spirit/data1043.root",
        "spirit/data1044.root",
        "spirit/data1045.root",
        "spirit/data1046.root",
        "spirit/data1047.root",
        "spirit/data1048.root"
    },
	std::string filenameOut = "hime_anal",
	const std::string& posOffsetFile = "database/pos_offsets_marco.txt", 
//	const std::string& posOffsetFile = "database/pos_offsets.txt", 
    const std::string& timeOffsetFile = "database/time_offsets_gaus.txt", 
    const std::string& velocityFile = "database/velocity_marco.txt", 
    bool debug = 0
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
	filenameOut += ".root";

	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);
	std::vector<double> pos_offset = getOffset(posOffsetFile);
	std::vector<double> time_offset = getOffset(timeOffsetFile);
	std::vector<double> velocity = getOffset(velocityFile);
	/*
	for (int i = 0; i < nModules; i++) {
		// we only have to do it when reading "our" dt calibration as we apply the shift to time,
		// Marco applies the shift to position
		pos_offset[i] = -1. * pos_offset[i] * velocity[i];
	}
	*/
	// initialize histograms

	int tdiff_nbins = 250;

	// initialize histograms
	TH1F hTof("hTof", "ToF (roughly calibrated)", 10000, -300, 500);
	TH1F hTofL1("hTofL1", "ToF (L1) (roughly calibrated)", 10000, -300, 500);
	TH1F hTofL2("hTofL2", "ToF (L2) (roughly calibrated)", 10000, -300, 500);
	TH1F hTofL3("hTofL3", "ToF (L3) (roughly calibrated)", 10000, -300, 500);
	TH2F hTofVsModuleID("hTofVsModuleID", "ToF vs. module ID", 72, 0, 72, 10000, -150, 250);

	TH2F hTotVsTof("hTotVsTof", "ToT vs. ToF", 500, -200, 500, 300, 0, 75);
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
	hTof.GetXaxis()->SetTitle("ToF (ns)");
	hTof.GetYaxis()->SetTitle("Counts");
	hTofVsModuleID.GetXaxis()->SetTitle("ModuleID");
	hTofVsModuleID.GetYaxis()->SetTitle("ToF (ns)");
	hTotVsTof.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTof.GetYaxis()->SetTitle("ToT (ns)");
	hModuleIDVsTDiff.GetYaxis()->SetTitle("Module ID");
	hModuleIDVsTDiff.GetXaxis()->SetTitle("Time difference (ns)");

	bool calibrationBarL1Hit;
	bool calibrationBarL2Hit;
	bool calibrationBarL3Hit;
	bool calibration_Det36_Hit;
	double tofCalib;
	double pos_calibrated;
	double x_pos_calibrated;
	double y_pos_calibrated;
	// loop over events: fill histograms

	auto nEvents = chain->GetEntries();
	for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
		if (debug) {
			std::cout << "event = " << iEvt << "\t nHits=" << spirit.hime_nHits << std::endl;
		}
		chain->GetEvent(iEvt);

		calibrationBarL1Hit = 0;
		calibrationBarL2Hit = 0;
		calibrationBarL3Hit = 0;
		calibration_Det36_Hit = 0;

		// used to calibrate offsets in dt
		for (auto h = 0; h < spirit.hime_nHits; h++) {
			int mod = spirit.hime_moduleID[h];
			if (mod == 4 || mod == 11 || mod == 18) {
				calibrationBarL1Hit = 1;
			}
			if (mod == 28 || mod == 35 || mod == 42) {
				calibrationBarL2Hit = 1;
			}
			if (mod == 32 || mod == 59 || mod == 66) {
				calibrationBarL3Hit = 1;
			}
		}

		for (auto h = 0; h < spirit.hime_nHits; h++) {
			int mod = spirit.hime_moduleID[h];
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
					cout << mod << "\t" << (spirit.hime_tDiff[h] - pos_offset[mod]) << "\t"
						 << "(" << xhit << "," << yhit << ") = " << rhit << "\t" << posdep;
					cout << "\t" << timecorrection << endl;
				}
				tofCalib =
					spirit.hime_tofRaw[h] - (calibrate_tof ? 0 : time_offset[mod]) - TMath::Abs(posdep / 2.9999 * 10.);
				if (!calibrate_tof)
					tofCalib += (11.9 + 4.8 / 2.9999 * 10);
				if ((calibrate_tof ? (tofCalib > -180) : (tofCalib > -20)) &&
					(calibrate_tof ? (tofCalib < -50) : (tofCalib < 60)) && (tot >= mTotMin && tot <= mTotMax) &&
					(mVetoWallMult0 ? (spirit.hime_veto_multi == 0) : (spirit.hime_veto_multi >= 0))) {
					hTotVsTof.Fill(tofCalib, tot);

					hxPosVsyPosAll.Fill(x_pos_calibrated, y_pos_calibrated);
					if (mod < 24) {
						hxPosVsyPosL1.Fill(x_pos_calibrated, y_pos_calibrated);
						hTofL1.Fill(tofCalib);
					}
					if (mod >= 48) {
						hxPosVsyPosL3.Fill(x_pos_calibrated, y_pos_calibrated);
						hTofL3.Fill(tofCalib);
					}
					if (mod >= 24 && mod < 48) {
						hxPosVsyPosL2.Fill(x_pos_calibrated, y_pos_calibrated);
						hTofL2.Fill(tofCalib);
					}

					hTof.Fill(tofCalib);

					hTofVsModuleID.Fill(mod, tofCalib);
					hModuleIDVsTDiff.Fill(dt_calibrated, mod);

					for (int v_hit = 0; v_hit < spirit.hime_veto_multi; v_hit++) {
						if (spirit.hime_veto_bar[v_hit] == 0)
							hModuleIDVsTDiffgatedV0.Fill(dt_calibrated, mod);
						if (spirit.hime_veto_bar[v_hit] == 1)
							hModuleIDVsTDiffgatedV1.Fill(dt_calibrated, mod);
						if (spirit.hime_veto_bar[v_hit] == 2)
							hModuleIDVsTDiffgatedV2.Fill(dt_calibrated, mod);
					}
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
	hTofL1.Write();
	hTofL2.Write();
	hTofL3.Write();

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
	if (calibrate_tof) {
		for (int i = 2; i < nModules; i++)
			hTofVsModuleID_d[i]->Write();
	}
	fileOut->Close();
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
