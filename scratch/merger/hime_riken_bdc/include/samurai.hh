#ifndef SAMURAI_HH
#define SAMURAI_HH

#include <array>

#include "TChain.h"

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

#endif