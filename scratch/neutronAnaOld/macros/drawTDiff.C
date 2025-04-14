#include "spirit.h"
#include <vector>

std::vector<double> getOffset(const std::string &filename);
void addFilesToChain(const std::vector<std::string> &files, TChain *&chain);

const int nModules = 72;
void drawTDiff(
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
    const std::string& filenameOut = "output-test.root",
			const std::string& posOffsetFile = "database/pos_offsets.txt", 
    const std::string& timeOffsetFile = "database/time_offsets.txt", 
    bool debug = 0
	// clang-format on
) {
	auto chain = new TChain("spirit", "spirit");
	addFilesToChain(files, chain);
	SetBranchAddress(chain);
	auto pos_offset = getOffset(posOffsetFile);
	auto time_offset = getOffset(timeOffsetFile);

	// for (int i = 0; i < nModules; i++) {
	// std::cout << "Module " << i << " offset: " << time_offset[i] << std::endl;
	// std::cout << "Module " << i << " offset: " << pos_offset[i] << std::endl;
	// }

	// initialize histograms

	int tdiff_nbins = 60;

	// initialize histograms
	TH1F hTof("hTof", "ToF (roughly calibrated)", 10000, -300, 500);
	TH2F hTofVsModuleID("hTofVsModuleID", "ToF vs. module ID", 72, 0, 72, 10000, -150, 250);

	TH2F hTotVsTof("hTotVsTof", "ToT vs. ToF", 200, -200, 500, 100, 0, 75);
	TH2F hModuleIDVsTDiff("hModuleIDVsTDiff", "Time difference between PMTs vs. module ID", 400, -25, 25, 72, 0, 72);
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
	TH2F hModuleIDVsTDiffgatedV0("hModuleIDVsTDiffgatedV0", "Time difference between PMTs vs. module ID gated on V0", 400, -25, 25, 72, 0, 72);
	TH2F hModuleIDVsTDiffgatedV1("hModuleIDVsTDiffgatedV1", "Time difference between PMTs vs. module ID gated on V1", 400, -25, 25, 72, 0, 72);
	TH2F hModuleIDVsTDiffgatedV2("hModuleIDVsTDiffgatedV2", "Time difference between PMTs vs. module ID gated on V2", 400, -25, 25, 72, 0, 72);
	hTof.GetXaxis()->SetTitle("ToF (ns)");
	hTof.GetYaxis()->SetTitle("Counts");
	hTofVsModuleID.GetXaxis()->SetTitle("ModuleID");
	hTofVsModuleID.GetYaxis()->SetTitle("ToF (ns)");
	hTotVsTof.GetXaxis()->SetTitle("ToF (ns)");
	hTotVsTof.GetYaxis()->SetTitle("ToT (ns)");
	hModuleIDVsTDiff.GetXaxis()->SetTitle("Module ID");
	hModuleIDVsTDiff.GetYaxis()->SetTitle("Time difference (ns)");

	bool calibrationBarL1Hit;
	bool calibrationBarL2Hit;
	bool calibrationBarL3Hit;
	bool calibration_Det36_Hit;
	double tofCalib;
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

			if (mod > 1 && mod < 72) {
				double totdistance = 4.8;
				if (mod >= 24)
					totdistance += 0.06;
				if (mod >= 48)
					totdistance += 0.06;
				double xhit=0;
				double yhit=0;
				double rhit = 0;
				if (mod > 23 && mod < 48)
				  {
				    xhit = (0.042 * (mod - 35 - 0.5));
				    yhit = spirit.hime_tDiff[h] - pos_offset[mod]-0.3;
				    yhit = xhit * 7/100;
				  }
				if (mod<24 || mod >= 48)
				  {
				    xhit = spirit.hime_tDiff[h] - pos_offset[mod]-0.3;
								    xhit = xhit * 7/100;
				    if (mod < 24)
				      yhit = 0.042 * (mod - 11 - 0.5);
				    if (mod >= 48)
				      yhit = 0.042 * (mod - 59 - 0.5);
				    
				  }
				rhit = TMath::Sqrt(xhit*xhit + yhit*yhit);
				double posdep = TMath::Power( rhit, 2) + totdistance * totdistance;
				posdep = TMath::Sqrt(posdep);
				posdep -= totdistance;
				double timecorrection  =posdep/3. * 10;
				if (debug)
				  {
				    cout<<mod<<"\t"
				    <<(spirit.hime_tDiff[h] - pos_offset[mod])<<"\t"
				    <<"("<<xhit<<","<<yhit<<") = "<<rhit<<"\t"<<posdep;
				    cout << "\t" << timecorrection<<endl;
				  }
				tofCalib = spirit.hime_tofRaw[h] - time_offset[mod] - TMath::Abs(posdep / 3. * 10.);

				double  tot = std::sqrt(spirit.hime_tot0[h] * spirit.hime_tot1[h]); // geo mean
				hTotVsTof.Fill(spirit.hime_tofRaw[h], tot);

				if ((tofCalib > -50) && (tofCalib < 60) && (tot < 150) && spirit.hime_veto_multi == 0)
				  {
				    

				    //					hTofVsModuleID.Fill(mod, spirit.hime_tofRaw[h]);

					if (mod < 72)
						hTof.Fill(tofCalib);
					hTofVsModuleID.Fill(mod, tofCalib);
					hModuleIDVsTDiff.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);

					
					for (int v_hit = 0; v_hit < spirit.hime_veto_multi; v_hit++)
					  {
					    if (spirit.hime_veto_bar[v_hit] == 0) 
					      hModuleIDVsTDiffgatedV0.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
					    if (spirit.hime_veto_bar[v_hit] == 1) 
					      hModuleIDVsTDiffgatedV1.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
					    if (spirit.hime_veto_bar[v_hit] == 2) 
					      hModuleIDVsTDiffgatedV2.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
					  }
					// layer 1
					if (mod < 24) {
					  hModuleIDVsTDiffL1.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
					  if (calibrationBarL2Hit) {
					    hModuleIDVsTDiffL1gated.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
					  }
					  
					}
					
					// layer 2
					if (mod >= 24 && mod < 48) {
						hModuleIDVsTDiffL2.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
						if (calibrationBarL1Hit) {
							hModuleIDVsTDiffL2gated.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
						}
					}

					// layer 3
					if (mod >= 48 && mod < 72) {
						hModuleIDVsTDiffL3.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
						if (calibrationBarL2Hit) {
							hModuleIDVsTDiffL3gated.Fill((spirit.hime_tDiff[h] - pos_offset[mod]), mod);
						}
					}
				}
			}
		}
	}
	/*

		hModuleIDVsTDiff->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);

		// layer 1
		if (mod < 24) {
			hModuleIDVsTDiffL1->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);
			if (calibrationBarL2Hit) {
				hModuleIDVsTDiffL1gated->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);
			}
		}

		// layer 2
		if (mod >= 24 && mod < 48) {
			hModuleIDVsTDiffL2->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);
			if (calibrationBarL1Hit) {
				hModuleIDVsTDiffL2gated->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);
			}
		}

		// layer 3
		if (mod >= 48 && mod < 72) {
			hModuleIDVsTDiffL3->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);
			if (calibrationBarL2Hit) {
				hModuleIDVsTDiffL3gated->Fill((spirit.hime_tDiff[h] - offset[mod]), mod);
			}
		}
	}
}
*/

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
	hTofVsModuleID.Write();
	hTotVsTof.Write();
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
