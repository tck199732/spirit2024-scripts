constexpr int SAMURAI_MAXHITS = 256;
constexpr int HIME_MAXHITS = 256;
struct himeData {
	unsigned int nHits;
	std::array<double, HIME_MAXHITS> tofRaw;
	std::array<double, HIME_MAXHITS> tDiff;
	std::array<double, HIME_MAXHITS> tSum;
	std::array<double, HIME_MAXHITS> tot0;
	std::array<double, HIME_MAXHITS> tot1;
	std::array<unsigned int, HIME_MAXHITS> moduleID;
	ULong64_t slowScaler;
	ULong64_t fastScaler;
	ULong_t eventNumber;
	void reset();
};

struct samuraiData {
	unsigned int run, event;
	ULong64_t lupots;

	unsigned int kyoto_multi;
	std::array<unsigned int, SAMURAI_MAXHITS> kyoto_bar;
	unsigned int hime_veto_multi;
	std::array<unsigned int, SAMURAI_MAXHITS> hime_veto_bar;
	std::array<double, SAMURAI_MAXHITS> hime_veto_tof;
	std::array<double, SAMURAI_MAXHITS> hime_veto_charge;
	std::array<double, SAMURAI_MAXHITS> hime_veto_tdiff;
	std::array<double, SAMURAI_MAXHITS> hime_veto_x;
	void reset();
};

himeData hime;
samuraiData samurai;

void setHimeBranchAddr(TChain *&chain);
void setSamuraiBranchAddr(TChain *&chain);

void checkTimeStamps(
	// clang-format off
    const std::string &hime_filename = "hime/data1150.root",
    const std::string &samurai_filename = "samurai/data1150.root",
    const std::string &out_filename = "checkTimeStamps.root"
	// clang-format on
) {
	auto himeChain = new TChain("hime", "hime");
	auto samuraiChain = new TChain("samurai", "samurai");

	himeChain->AddFile(hime_filename.c_str());
	samuraiChain->AddFile(samurai_filename.c_str());

	setHimeBranchAddr(himeChain);
	setSamuraiBranchAddr(samuraiChain);

	auto nEntriesHime = himeChain->GetEntries();
	auto nEntriesSamurai = samuraiChain->GetEntries();

	auto hFastSlowScaleCheck = new TH1D("hFastSlowScaleCheck", "hFastSlowScaleCheck", 300, -3, 3);

	std::ofstream outHimeScalerDiff("himeScalerDiff.txt");

	ULong64_t lastTimeStampHime = 0;
	for (auto i = 0; i < nEntriesHime; i++) {
		himeChain->GetEntry(i);
		auto currTimeStampHime = hime.fastScaler;
		auto diffTimeStampHime = currTimeStampHime - lastTimeStampHime;
		lastTimeStampHime = currTimeStampHime;
		std::cout << "i: " << i << " currTimeStampHime: " << currTimeStampHime
				  << " diffTimeStampHime: " << diffTimeStampHime << std::endl;
		if (i > 0) {
			outHimeScalerDiff << diffTimeStampHime << std::endl;
		}
		hFastSlowScaleCheck->Fill(hime.fastScaler - 4 * hime.slowScaler);
	}
	outHimeScalerDiff.close();

	std::ofstream outSamuraiScalerDiff("samuraiScalerDiff.txt");

	ULong64_t lastTimeStampSamurai = 0;
	for (auto i = 0; i < nEntriesSamurai; i++) {
		samuraiChain->GetEntry(i);
		auto currTimeStampSamurai = samurai.lupots;
		auto diffTimeStampSamurai = currTimeStampSamurai - lastTimeStampSamurai;
		lastTimeStampSamurai = currTimeStampSamurai;

		if (i > 0) {
			outSamuraiScalerDiff << diffTimeStampSamurai << std::endl;
		}
	}

	outSamuraiScalerDiff.close();

	auto outFile = new TFile(out_filename.c_str(), "RECREATE");
	hFastSlowScaleCheck->Write();
	outFile->Close();
	return;
}

void setSamuraiBranchAddr(TChain *&chain) {
	chain->SetBranchAddress("run", &samurai.run);
	chain->SetBranchAddress("event", &samurai.event);
	chain->SetBranchAddress("lupots", &samurai.lupots);
	chain->SetBranchAddress("kyoto_multi", &samurai.kyoto_multi);
	chain->SetBranchAddress("kyoto_bar", &samurai.kyoto_bar[0]);
	chain->SetBranchAddress("hime_veto_multi", &samurai.hime_veto_multi);
	chain->SetBranchAddress("hime_veto_bar", &samurai.hime_veto_bar[0]);
	chain->SetBranchAddress("hime_veto_tof", &samurai.hime_veto_tof[0]);
	chain->SetBranchAddress("hime_veto_charge", &samurai.hime_veto_charge[0]);
	chain->SetBranchAddress("hime_veto_tdiff", &samurai.hime_veto_tdiff[0]);
	chain->SetBranchAddress("hime_veto_x", &samurai.hime_veto_x[0]);
	return;
}

void setHimeBranchAddr(TChain *&chain) {
	chain->SetBranchAddress("nHits", &hime.nHits);
	chain->SetBranchAddress("tofRaw", &hime.tofRaw[0]);
	chain->SetBranchAddress("tDiff", &hime.tDiff[0]);
	chain->SetBranchAddress("tSum", &hime.tSum[0]);
	chain->SetBranchAddress("tot0", &hime.tot0[0]);
	chain->SetBranchAddress("tot1", &hime.tot1[0]);
	chain->SetBranchAddress("moduleID", &hime.moduleID[0]);
	chain->SetBranchAddress("slowScaler", &hime.slowScaler);
	chain->SetBranchAddress("fastScaler", &hime.fastScaler);
	chain->SetBranchAddress("eventNumber", &hime.eventNumber);
	return;
}
