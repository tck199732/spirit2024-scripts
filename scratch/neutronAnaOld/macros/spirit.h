#ifndef SPIRIT_HH
#define SPIRIT_HH

#include "TChain.h"

constexpr int HIME_MAXHITS = 256;
constexpr int SAMURAI_MAXHITS = 256;

struct spiritData {
	unsigned int hime_nHits;
	std::array<double, HIME_MAXHITS> hime_tofRaw;
	std::array<double, HIME_MAXHITS> hime_tDiff;
	std::array<double, HIME_MAXHITS> hime_tSum;
	std::array<double, HIME_MAXHITS> hime_tot0;
	std::array<double, HIME_MAXHITS> hime_tot1;
	std::array<unsigned int, HIME_MAXHITS> hime_moduleID;

	ULong64_t hime_slowScaler;
	ULong64_t hime_fastScaler;
	ULong64_t hime_eventNumber;

	unsigned int runNumber, eventNumber;
	ULong64_t lupoTimeStamp;
	unsigned int kyotoMulti;
	std::array<unsigned int, SAMURAI_MAXHITS> kyotoBarId;
	unsigned int vetoMulti;
	std::array<unsigned int, SAMURAI_MAXHITS> vetoBarId;
	std::array<double, SAMURAI_MAXHITS> vetoTof;
	std::array<double, SAMURAI_MAXHITS> vetoTot;
	std::array<double, SAMURAI_MAXHITS> vetoTdiff;

	void reset();
};

spiritData spirit;

void spiritData::reset() {
	hime_nHits = 0;
	hime_tofRaw.fill(0);
	hime_tDiff.fill(0);
	hime_tSum.fill(0);
	hime_tot0.fill(0);
	hime_tot1.fill(0);
	hime_moduleID.fill(0);

	hime_slowScaler = 0;
	hime_fastScaler = 0;
	hime_eventNumber = 0;

	runNumber = 0;
	eventNumber = 0;
	lupoTimeStamp = 0;
	kyotoMulti = 0;
	kyotoBarId.fill(0);
	vetoMulti = 0;
	vetoBarId.fill(0);
	vetoTof.fill(0);
	vetoTot.fill(0);
	vetoTdiff.fill(0);
	return;
}

void SetBranchAddress(TChain *&chain) {
	chain->SetBranchAddress("hime_nHits", &spirit.hime_nHits);
	chain->SetBranchAddress("hime_tofRaw", &spirit.hime_tofRaw[0]);
	chain->SetBranchAddress("hime_tDiff", &spirit.hime_tDiff[0]);
	chain->SetBranchAddress("hime_tSum", &spirit.hime_tSum[0]);
	chain->SetBranchAddress("hime_tot0", &spirit.hime_tot0[0]);
	chain->SetBranchAddress("hime_tot1", &spirit.hime_tot1[0]);
	chain->SetBranchAddress("hime_moduleID", &spirit.hime_moduleID[0]);

	chain->SetBranchAddress("hime_slowScaler", &spirit.hime_slowScaler);
	chain->SetBranchAddress("hime_fastScaler", &spirit.hime_fastScaler);
	chain->SetBranchAddress("hime_eventNumber", &spirit.hime_eventNumber);

	chain->SetBranchAddress("runNumber", &spirit.runNumber);
	chain->SetBranchAddress("eventNumber", &spirit.eventNumber);
	chain->SetBranchAddress("lupoTimeStamp", &spirit.lupoTimeStamp);
	chain->SetBranchAddress("kyotoMulti", &spirit.kyotoMulti);
	chain->SetBranchAddress("kyotoBarId", &spirit.kyotoBarId[0]);
	chain->SetBranchAddress("vetoMulti", &spirit.vetoMulti);
	chain->SetBranchAddress("vetoBarId", &spirit.vetoBarId[0]);
	chain->SetBranchAddress("vetoTof", &spirit.vetoTof[0]);
	chain->SetBranchAddress("vetoTot", &spirit.vetoTot[0]);
	chain->SetBranchAddress("vetoTdiff", &spirit.vetoTdiff[0]);
	return;
}

#endif
