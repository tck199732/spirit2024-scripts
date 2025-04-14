constexpr int SAMURAI_MAXHITS = 256;
struct samuraiData {
	unsigned int runNumber, eventNumber;
	ULong64_t lupoTimeStamp;

	unsigned int kyotoMulti;
	std::array<unsigned int, SAMURAI_MAXHITS> kyotoBarId;
	unsigned int vetoMulti;
	std::array<unsigned int, SAMURAI_MAXHITS> vetoBarId;
	std::array<double, SAMURAI_MAXHITS> vetoTof;
	std::array<double, SAMURAI_MAXHITS> vetoTot;
	std::array<double, SAMURAI_MAXHITS> vetoTdiff;

	Double_t tbdc_x;
	Double_t tbdc_y;
	Double_t tbdc_a;
	Double_t tbdc_b;
};

samuraiData samurai;

void setSamuraiBranchAddr(TChain *&chain);

void checkBdc(
	// clang-format off
    const int& runNumber = 1217,
    const std::string & rikenDir="riken_bdc",
    const std::string & outDir="bdc_xy_figs"
	// clang-format on
) {

	auto rikenChain = new TChain("riken", "riken");
	auto rikenFile = TString::Format("%s/%04d.root", rikenDir.c_str(), runNumber);

	// -1 means file not found
	auto entriesRiken = -1;

	if (!gSystem->AccessPathName(rikenFile)) {
		rikenChain->Add(rikenFile);
	}

	setSamuraiBranchAddr(rikenChain);

	auto hBdcXY = new TH2D("hBdcXY", "", 200, -100, 100, 200, -100, 100);
	hBdcXY->Sumw2();

	for (auto iEvt = 0; iEvt < rikenChain->GetEntries(); iEvt++) {
		rikenChain->GetEntry(iEvt);
		hBdcXY->Fill(samurai.tbdc_x, samurai.tbdc_y);
	}

	auto canvas = new TCanvas("canvas", "canvas", 800, 600);
	hBdcXY->Draw("COLZ");
	canvas->SaveAs(Form("%s/%04d.png", outDir.c_str(), runNumber));

	auto outputFile = new TFile(Form("%s/%04d.root", outDir.c_str(), runNumber), "RECREATE");
	hBdcXY->Write();
	outputFile->Close();
	delete outputFile;
	delete canvas;
}

void setSamuraiBranchAddr(TChain *&chain) {
	chain->SetBranchAddress("runNumber", &samurai.runNumber);
	chain->SetBranchAddress("eventNumber", &samurai.eventNumber);
	chain->SetBranchAddress("lupoTimeStamp", &samurai.lupoTimeStamp);

	chain->SetBranchAddress("kyotoMulti", &samurai.kyotoMulti);
	chain->SetBranchAddress("kyotoBarId", &samurai.kyotoBarId[0]);
	chain->SetBranchAddress("vetoMulti", &samurai.vetoMulti);
	chain->SetBranchAddress("vetoBarId", &samurai.vetoBarId[0]);
	chain->SetBranchAddress("vetoTof", &samurai.vetoTof[0]);
	chain->SetBranchAddress("vetoTot", &samurai.vetoTot[0]);
	chain->SetBranchAddress("vetoTdiff", &samurai.vetoTdiff[0]);

	// bdc from Park
	chain->SetBranchAddress("bdc_x", &samurai.tbdc_x);
	chain->SetBranchAddress("bdc_y", &samurai.tbdc_y);
	chain->SetBranchAddress("bdc_a", &samurai.tbdc_a);
	chain->SetBranchAddress("bdc_b", &samurai.tbdc_b);

	return;
}