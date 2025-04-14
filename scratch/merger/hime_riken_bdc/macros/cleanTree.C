#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TStyle.h"
#include <array>
#include <iostream>
#include <vector>

constexpr int MAXHITS = 256;
struct himeData {
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

struct himeDataClean {
	unsigned int nHits;
	std::array<double, MAXHITS> tofRaw;
	std::array<double, MAXHITS> tDiff;
	std::array<double, MAXHITS> tSum;
	std::array<double, MAXHITS> tot0;
	std::array<double, MAXHITS> tot1;
	std::array<unsigned int, MAXHITS> moduleID;

	ULong_t slowScaler;
	ULong_t fastScaler;
	ULong_t eventNumber;

	void reset();
};

himeData HIME;
himeDataClean HIME_CLEAN;

void setChain(TChain *&chain);
void setBranch(TTree *&tree);
void cleanTree(
	// clang-format off
    const std::string &filename = "taras-marco/data1039.root",
    const std::string &filenameOut = "data1039.root"
	// clang-format on
) {
	auto chain = new TChain("tree", "tree");
	chain->Add(filename.c_str());
	setChain(chain);

	auto cleanedTree = new TTree("hime", "hime");
	setBranch(cleanedTree);

	auto nevents = chain->GetEntries();
	for (auto i = 0; i < nevents; i++) {
		HIME_CLEAN.reset();
		chain->GetEntry(i);

		if (chain->GetBranchStatus("EventNumber")) {
			HIME_CLEAN.eventNumber = static_cast<ULong_t>(HIME.EventNumber);
		} else {
			HIME_CLEAN.eventNumber = HIME.eventNumber;
		}

		HIME_CLEAN.slowScaler = HIME.slowScaler;
		HIME_CLEAN.fastScaler = HIME.fastScaler;

		HIME_CLEAN.nHits = HIME.nHits;
		for (auto j = 0; j < HIME.nHits; j++) {
			HIME_CLEAN.tDiff[j] = (*(HIME.tDiff))[j];
			HIME_CLEAN.tSum[j] = (*(HIME.tSum))[j];
			HIME_CLEAN.tofRaw[j] = (*(HIME.tofRaw))[j];
			HIME_CLEAN.tot0[j] = (*(HIME.tot0))[j];
			HIME_CLEAN.tot1[j] = (*(HIME.tot1))[j];
			HIME_CLEAN.moduleID[j] = (*(HIME.moduleID))[j];
		}
		cleanedTree->Fill();
	}

	auto file = new TFile(filenameOut.c_str(), "RECREATE");
	cleanedTree->Write();
	file->Close();
}

void setChain(TChain *&chain) {

	chain->SetBranchStatus("*", 0);

	chain->SetBranchAddress("nHits", &HIME.nHits);
	chain->SetBranchAddress("tDiff", &HIME.tDiff);
	chain->SetBranchAddress("tSum", &HIME.tSum);
	chain->SetBranchAddress("tofRaw", &HIME.tofRaw);
	chain->SetBranchAddress("tot0", &HIME.tot0);
	chain->SetBranchAddress("tot1", &HIME.tot1);
	chain->SetBranchAddress("moduleID", &HIME.moduleID);

	chain->SetBranchAddress("slowScaler", &HIME.slowScaler);
	chain->SetBranchAddress("fastScaler", &HIME.fastScaler);

	// get names of all branches
	std::vector<std::string> names;
	TObjArray *br = chain->GetListOfBranches();
	for (int i = 0; i < br->GetEntries(); i++) {
		names.push_back(br->At(i)->GetName());
	}

	auto it = std::find(names.begin(), names.end(), "EventNumber");
	if (it != names.end()) {
		chain->SetBranchAddress("EventNumber", &HIME.EventNumber);
		chain->SetBranchStatus("EventNumber", 1);
	} else {
		chain->SetBranchAddress("eventNumber", &HIME.eventNumber);
		chain->SetBranchStatus("eventNumber", 1);
	}
	return;
}

void setBranch(TTree *&tree) {

	tree->Branch("eventNumber", &HIME_CLEAN.eventNumber, "eventNumber/g");
	tree->Branch("slowScaler", &HIME_CLEAN.slowScaler, "slowScaler/l");
	tree->Branch("fastScaler", &HIME_CLEAN.fastScaler, "fastScaler/l");
	tree->Branch("nHits", &HIME_CLEAN.nHits, "nHits/i");
	tree->Branch("tDiff", &HIME_CLEAN.tDiff[0], "tDiff[nHits]/D");
	tree->Branch("tSum", &HIME_CLEAN.tSum[0], "tSum[nHits]/D");
	tree->Branch("tofRaw", &HIME_CLEAN.tofRaw[0], "tofRaw[nHits]/D");
	tree->Branch("tot0", &HIME_CLEAN.tot0[0], "tot0[nHits]/D");
	tree->Branch("tot1", &HIME_CLEAN.tot1[0], "tot1[nHits]/D");
	tree->Branch("moduleID", &HIME_CLEAN.moduleID[0], "moduleID[nHits]/i");
	return;
}

void himeDataClean::reset() {
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