#ifndef HIME_HH
#define HIME_HH

#include "TChain.h"

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

struct himeDataRaw {
	int nHits;
	std::vector<float> *tDiff;
	std::vector<float> *tSum;
	std::vector<float> *tofRaw;
	std::vector<float> *tot0;
	std::vector<float> *tot1;
	std::vector<int> *moduleID;
	ULong_t slowScaler;
	ULong_t fastScaler;
	int EventNumber;
	ULong_t eventNumber;
};

void himeData::reset() {
	slowScaler = 0;
	fastScaler = 0;
	eventNumber = 0;
	nHits = 0;
	tofRaw.fill(0);
	tDiff.fill(0);
	tSum.fill(0);
	tot0.fill(0);
	tot1.fill(0);
	moduleID.fill(0);
	return;
}

himeData hime;
himeDataRaw himeRaw;

void setHimeRawBranchAddr(TChain *chain) {

	chain->SetBranchStatus("*", 0);
	chain->SetBranchAddress("nHits", &himeRaw.nHits);
	chain->SetBranchAddress("tDiff", &himeRaw.tDiff);
	chain->SetBranchAddress("tSum", &himeRaw.tSum);
	chain->SetBranchAddress("tofRaw", &himeRaw.tofRaw);
	chain->SetBranchAddress("tot0", &himeRaw.tot0);
	chain->SetBranchAddress("tot1", &himeRaw.tot1);
	chain->SetBranchAddress("moduleID", &himeRaw.moduleID);
	chain->SetBranchAddress("slowScaler", &himeRaw.slowScaler);
	chain->SetBranchAddress("fastScaler", &himeRaw.fastScaler);

	// get names of all branches
	std::vector<std::string> names;
	TObjArray *br = chain->GetListOfBranches();
	for (int i = 0; i < br->GetEntries(); i++) {
		names.push_back(br->At(i)->GetName());
	}

	auto it = std::find(names.begin(), names.end(), "EventNumber");
	if (it != names.end()) {
		chain->SetBranchAddress("EventNumber", &himeRaw.EventNumber);
		chain->SetBranchStatus("EventNumber", 1);
	} else {
		chain->SetBranchAddress("eventNumber", &himeRaw.eventNumber);
		chain->SetBranchStatus("eventNumber", 1);
	}
	return;
}

void cleanHimeBranches(TChain *&chain) {
	hime.reset();
	hime.nHits = himeRaw.nHits;
	for (int i = 0; i < hime.nHits; i++) {
		hime.tDiff[i] = himeRaw.tDiff->at(i);
		hime.tSum[i] = himeRaw.tSum->at(i);
		hime.tofRaw[i] = himeRaw.tofRaw->at(i);
		hime.tot0[i] = himeRaw.tot0->at(i);
		hime.tot1[i] = himeRaw.tot1->at(i);
		hime.moduleID[i] = himeRaw.moduleID->at(i);
	}
	hime.slowScaler = ULong64_t(himeRaw.slowScaler);
	hime.fastScaler = ULong64_t(himeRaw.fastScaler);

	std::vector<std::string> names;
	TObjArray *br = chain->GetListOfBranches();
	for (int i = 0; i < br->GetEntries(); i++) {
		names.push_back(br->At(i)->GetName());
	}
	auto it = std::find(names.begin(), names.end(), "EventNumber");
	if (it != names.end()) {
		hime.eventNumber = himeRaw.EventNumber;
	} else {
		hime.eventNumber = himeRaw.eventNumber;
	}

	return;
}

#endif