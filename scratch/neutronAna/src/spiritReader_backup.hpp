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

class SpiritReader {
public:
	SpiritReader(const std::vector<int> &runIds, const std::string &spiritDir = "spirit", bool useTpc = false);
	~SpiritReader();

	SpiritReader(const SpiritReader &) = delete;
	SpiritReader &operator=(const SpiritReader &) = delete;

	Long64_t GetEntry(Long64_t entry) {

		if (!mChain) {
			std::cerr << "Chain is not initialized!" << std::endl;
			return -1;
		}
		return mChain->GetEntry(entry);
	}
	Long64_t GetEntries() { return mChain->GetEntries(); }

	// for conveniently accessing data from spiritData, e.g. spirit.hime_nHits
	// build a wrapper variables in SpiritReader

	unsigned int &hime_nHits;
	std::array<double, HIME_MAXHITS> &hime_tofRaw;
	std::array<double, HIME_MAXHITS> &hime_tDiff;
	std::array<double, HIME_MAXHITS> &hime_tSum;
	std::array<double, HIME_MAXHITS> &hime_tot0;
	std::array<double, HIME_MAXHITS> &hime_tot1;
	std::array<unsigned int, HIME_MAXHITS> &hime_moduleID;

	ULong64_t &hime_slowScaler;
	ULong64_t &hime_fastScaler;
	ULong64_t &hime_eventNumber;

	unsigned int &run;
	unsigned int &event;
	ULong64_t &lupots;
	unsigned int &kyoto_multi;
	std::array<unsigned int, SAMURAI_MAXHITS> &kyoto_bar;
	unsigned int &hime_veto_multi;
	std::array<unsigned int, SAMURAI_MAXHITS> &hime_veto_bar;
	std::array<double, SAMURAI_MAXHITS> &hime_veto_tof;
	std::array<double, SAMURAI_MAXHITS> &hime_veto_charge;
	std::array<double, SAMURAI_MAXHITS> &hime_veto_tdiff;
	std::array<double, SAMURAI_MAXHITS> &hime_veto_x;

	Double_t &tbdc_x;
	Double_t &tbdc_y;
	Double_t &tbdc_a;
	Double_t &tbdc_b;

	// from tpc vertex data
	int &eventID;
	int &dataID;
	ULong64_t &time;
	int &tracks;
	int &tracksWphi;
	Double_t &vertex_x;
	Double_t &vertex_y;
	Double_t &vertex_z;

private:
	void setBranchAddresses();
	TChain *mChain;
	spiritData mData;
	bool mUseTpc;
};

#endif
