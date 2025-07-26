#ifndef SPIRIT_READER_HPP
#define SPIRIT_READER_HPP

#include "TChain.h"
#include <array>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

constexpr int HIME_MAXHITS = 512;
constexpr int SAMURAI_MAXHITS = 512;

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

	Double_t tbdc_x;
	Double_t tbdc_y;
	Double_t tbdc_a;
	Double_t tbdc_b;

	// from tpc vertex data
	int eventID;
	int dataID;
	ULong64_t time;
	int tracks;
	int tracksWphi;
	Double_t vertex_x;
	Double_t vertex_y;
	Double_t vertex_z;
};

TChain *getChain(const std::vector<int> &runIds, const std::string &spiritDir, bool useTpcData, spiritData &spirit) {

	auto mChain = new TChain("spirit", "spirit");

	for (auto idx : runIds) {
		std::string fname = Form("%s/%04d.root", spiritDir.c_str(), idx);

		if (!std::filesystem::exists(fname)) {
			std::cerr << "File does not exist: " << fname << std::endl;
			continue;
		}
		std::cout << "Adding file: " << fname << std::endl;

		mChain->AddFile(fname.c_str());
	}

	mChain->SetBranchAddress("hime_nHits", &spirit.hime_nHits);
	mChain->SetBranchAddress("hime_tofRaw", &spirit.hime_tofRaw[0]);
	mChain->SetBranchAddress("hime_tDiff", &spirit.hime_tDiff[0]);
	mChain->SetBranchAddress("hime_tSum", &spirit.hime_tSum[0]);
	mChain->SetBranchAddress("hime_tot0", &spirit.hime_tot0[0]);
	mChain->SetBranchAddress("hime_tot1", &spirit.hime_tot1[0]);
	mChain->SetBranchAddress("hime_moduleID", &spirit.hime_moduleID[0]);

	mChain->SetBranchAddress("hime_slowScaler", &spirit.hime_slowScaler);
	mChain->SetBranchAddress("hime_fastScaler", &spirit.hime_fastScaler);
	mChain->SetBranchAddress("hime_eventNumber", &spirit.hime_eventNumber);

	mChain->SetBranchAddress("runNumber", &spirit.run);
	mChain->SetBranchAddress("eventNumber", &spirit.event);
	mChain->SetBranchAddress("lupoTimeStamp", &spirit.lupots);
	mChain->SetBranchAddress("kyotoMulti", &spirit.kyoto_multi);
	mChain->SetBranchAddress("kyotoBarId", &spirit.kyoto_bar[0]);
	mChain->SetBranchAddress("vetoMulti", &spirit.hime_veto_multi);
	mChain->SetBranchAddress("vetoBarId", &spirit.hime_veto_bar[0]);
	mChain->SetBranchAddress("vetoTof", &spirit.hime_veto_tof[0]);
	mChain->SetBranchAddress("vetoTot", &spirit.hime_veto_charge[0]);
	mChain->SetBranchAddress("vetoTdiff", &spirit.hime_veto_tdiff[0]);

	mChain->SetBranchAddress("tbdc_x", &spirit.tbdc_x);
	mChain->SetBranchAddress("tbdc_y", &spirit.tbdc_y);
	mChain->SetBranchAddress("tbdc_a", &spirit.tbdc_a);
	mChain->SetBranchAddress("tbdc_b", &spirit.tbdc_b);

	if (useTpcData) {
		mChain->SetBranchAddress("eventID", &spirit.eventID);
		mChain->SetBranchAddress("dataID", &spirit.dataID);
		mChain->SetBranchAddress("tpc_time", &spirit.time);
		// if (mChain->SetBranchAddress("time", &spirit.time) != 0) {
		// 	std::cout << "Using 'time' branch for TPC data." << std::endl;
		// } else if (mChain->SetBranchAddress("tpc_time", &spirit.time) != 0) {
		// 	std::cout << "Using 'tpc_time' branch for TPC data." << std::endl;
		// } else {
		// 	std::cerr << "No valid time branch found for TPC data." << std::endl;
		// 	return nullptr;
		// }

		mChain->SetBranchAddress("tracks", &spirit.tracks);
		mChain->SetBranchAddress("tracksWphi", &spirit.tracksWphi);
		mChain->SetBranchAddress("vertex_x", &spirit.vertex_x);
		mChain->SetBranchAddress("vertex_y", &spirit.vertex_y);
		mChain->SetBranchAddress("vertex_z", &spirit.vertex_z);
	}
	return mChain;
}
#endif
