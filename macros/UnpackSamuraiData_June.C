#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"

struct VmeEasiroc {
	std::array<int, 64> hadc; // high gain
	std::array<int, 64> ladc; // low gain
	std::array<int, 64> ltdc; // leading edge
	std::array<int, 64> ttdc; // trailing edge
							  // std::array<int, 67> scaler;
	void reset() {
		hadc.fill(-1);
		ladc.fill(-1);
		ltdc.fill(-1);
		ttdc.fill(-1);
	}
};

struct V1290 {
	std::array<int, 32> ltdc;
	std::array<int, 32> ttdc;
	void reset() {
		ltdc.fill(-1);
		ttdc.fill(-1);
	}
};

struct SIS3820 {
	// http://ribfdaq.rarfadv.riken.go.jp/~samurai/babiscr.php
	// also see 'database/scaler_b2f.json'
	std::array<int, 32> scaler;
	void reset() { scaler.fill(0); }
};

struct AMTTDC {
	std::array<int, 64> tdc_x1; // geo 2
	std::array<int, 64> tdc_y1; // geo 3
	std::array<int, 64> tdc_x2; // geo 4
	std::array<int, 64> tdc_y2; // geo 5

	void reset() {
		tdc_x1.fill(-1);
		tdc_y1.fill(-1);
		tdc_x2.fill(-1);
		tdc_y2.fill(-1);
	}
};

struct MQDC32 {
	std::array<int, 32> qdc;
	void reset() { qdc.fill(-1); }
};

struct MTDC32 {
	std::array<int, 32> tdc;
	std::array<int, 2> trigger;
	void reset() { tdc.fill(-1); }
};

struct Event {
	unsigned int run, event;
	// uint64_t lupots;
	ULong64_t lupots;
	// int timestamp, scaler;
	VmeEasiroc easiroc;
	V1290 v1290;
	SIS3820 sis3820;
	AMTTDC amttdc;
	MQDC32 mqdc32;
	MTDC32 mtdc32;
	void reset() {
		run = -1;
		event = -1;
		lupots = 0;
		easiroc.reset();
		v1290.reset();
		sis3820.reset();
		amttdc.reset();
		mqdc32.reset();
		mtdc32.reset();
	}
};

Event event;

void SetTreeBranches(TTree *&tree);

void UnpackSamuraiData(
	// clang-format off
    TString indata = "ridf/data1145.ridf", 
    TString outdata = "unpacked/data1145.root"
	// clang-format on
) {

	gSystem->Load("libanaroot.so");
	auto estore = new TArtEventStore();
	estore->Open(indata.Data());
	auto rawevent = estore->GetRawEventObject();

	auto fout = new TFile(outdata.Data(), "RECREATE");
	auto ridfTree = new TTree("ridf", "ridf");
	SetTreeBranches(ridfTree);

	long nEvents = 0;
	while (estore->GetNextEvent()) {
		// 8, 25, 33, 34, 35, 36
		// 0 is not used
		// what is module 32 ?

		event.reset();
		event.run = rawevent->GetRunNumber();
		event.event = rawevent->GetEventNumber();
		// event.timestamp = rawevent->GetTimeStamp();
		// event.scaler = rawevent->GetNumScaler();

		for (auto i = 0; i < rawevent->GetNumSeg(); i++) {

			auto seg = rawevent->GetSegment(i);
			auto fp = seg->GetFP();
			auto device = seg->GetDevice(); // not used
			auto addr = seg->GetAddress();
			auto mod = seg->GetModule();
			auto det = seg->GetDetector();
			auto ndata = seg->GetNumData();

			if (det == 2 && mod == 33 && addr == 85) {

				for (auto j = 0; j < ndata; j++) {
					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();

					if (geo != 5) {
						std::cerr << "Error!! Geo for SBT QDC must be 5!: " << geo << std::endl;
						break;
					}
					// 8,9 for 25 mu; 10,11 for 100 mu ?
					if (!(ch == 8 || ch == 9 || ch == 10 || ch == 11)) {
						std::cerr << "Error!! Channel for SBT QDC must be 8, 9, 10, or 11!: " << ch << std::endl;
						break;
					}
					event.mqdc32.qdc[ch] = val;
				}
			}

			if (det == 3 && mod == 34 && addr == 85) {
				for (auto j = 0; j < ndata; j++) {
					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();

					if (geo != 6) {
						std::cerr << "Error!! Geo for SBT TDC must be 6!: " << geo << std::endl;
						break;
					}
					if (ch < 0 || ch > 33) {
						std::cerr << "Error!! Channel for SBT TDC must be between 0 and 31!: " << ch << std::endl;
						break;
					}

					// 6,7 for 25 mu; 8,9 for 100 mu ?
					if (ch == 6 || ch == 7 || ch == 8 || ch == 9) {
						event.mtdc32.tdc[ch] = val;
					} else if (ch == 32) {
						event.mtdc32.trigger[0] = val;
					} else if (ch == 33) {
						std::cerr << "Warning : Trigger 1 (ch 33) is not used" << std::endl;
						event.mtdc32.trigger[1] = val;
					} else {
						std::cerr << "Error!! Channel for SBT TDC must be 6, 7, 8, 9 or 32, 33! : " << ch << std::endl;
						break;
					}
				}
			}

			/**
			 * det = 40 used before
			 * changed to 50 but in conflict with hime veto
			 */
			if (mod == 35 && addr == 98) {
				for (auto j = 0; j < ndata; j++) {
					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();

					if (ch > 64) {
						std::cerr << "Error!! Channel for EASIROC should be less than 64!!: " << ch << std::endl;
						break;
					}

					/*
					 * @file TArtDecoderVmeEasyroc.cc
					 * geo:0 High gain ADC
					 * geo:1 Low gain ADC
					 * geo:2 Leading edge TDC
					 * geo:3 Trailing edge TDC
					 * geo:4 Scaler
					 */

					switch (geo) {
					case 0:
						event.easiroc.hadc[ch] = val;
						break;
					case 1:
						event.easiroc.ladc[ch] = val;
						break;
					case 2:
						event.easiroc.ltdc[ch] = val;
						break;
					case 3:
						event.easiroc.ttdc[ch] = val;
						break;

					default:
						break;
					}
				}
			}

			if (det == 61 && mod == 8 && addr == 96) {
				/*
				 * @file TArtDecoderFixed32.cc
				 * geo:0
				 * @brief for the scaler in B2F
				 * @details Scaler, 61, spvme00, 96, SIS3820, 8
				 */
				for (auto j = 0; j < ndata; j++) {
					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();
					if (geo != 0) {
						std::cerr << "Error!! Geo for Fixed16 must be 0!: " << geo << std::endl;
					}
					event.sis3820.scaler[ch] = val;
				}
			}

			if (det == 50 && mod == 25 && addr == 101) {
				/**
				 * @file TArtDecoderV1290.cc
				 * @brief for HIME veto
				 */
				for (auto j = 0; j < ndata; j++) {

					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();
					auto edge = rawData->GetEdge(); // 0 = leading, 1 = trailing

					if (ch < 0 || ch > 31) {
						std::cerr << "Error!! Channel for V1290 (TDC measurement) must be between 0 and 31!: " << ch
								  << std::endl;
						break;
					}

					// geo == 6 is for tdc data
					if (geo != 6) {
						std::cerr << "Error!! Geo for V1290 (TDC measurement) must be 6!: " << geo << std::endl;
						break;
					}

					if (edge == 0) {
						event.v1290.ltdc[ch] = val;
					} else if (edge == 1) {
						event.v1290.ttdc[ch] = val;
					}
				}
			}

			if (det == 27 && mod == 36 && addr == 59) {
				/**
				 * @file TArtDecoderAMTTDC
				 * @brief for AMTTDC
				 */
				for (auto j = 0; j < ndata; j++) {
					auto rawData = seg->GetData(j);
					auto geo = rawData->GetGeo();
					auto ch = rawData->GetCh();
					auto val = rawData->GetVal();
					auto edge = rawData->GetEdge(); // 0 = leading, 1 = trailing

					if (!(geo == 2 || geo == 3 || geo == 4 || geo == 5)) {
						std::cerr << "Error!! Geo for AMTTDC TDC must be 2, 3, 4, or 5!: " << geo << std::endl;
						break;
					}

					if (ch < 0 || ch > 63) {
						std::cerr << "Error!! Channel for AMTTDC TDC must be between 0 and 63!: " << ch << std::endl;
						break;
					}
					switch (geo) {
					case 2: {
						event.amttdc.tdc_x1[ch] = val;
						break;
					}
					case 3: {
						event.amttdc.tdc_y1[ch] = val;
						break;
					}
					case 4: {
						event.amttdc.tdc_x2[ch] = val;
						break;
					}
					case 5: {
						event.amttdc.tdc_y2[ch] = val;
						break;
					}
					}
				}
			}

			if (fp == 13 && det == 60 && addr == 85 && mod == 42) {
				auto rawData = seg->GetData(0);
				auto valh = static_cast<ULong64_t>(rawData->GetVal());
				rawData = seg->GetData(1);
				auto vall = static_cast<ULong64_t>(rawData->GetVal());
				event.lupots = (valh << 32) + vall;
			}
		}
		nEvents++;
		estore->ClearData();
		ridfTree->Fill();
	}

	fout->Write();
	fout->Close();
}

void SetTreeBranches(TTree *&tree) {
	tree->Branch("run", &event.run, "run/i");
	tree->Branch("event", &event.event, "event/i");
	tree->Branch("lupots", &event.lupots, "lupots/l");

	tree->Branch("easiroc_hadc", &event.easiroc.hadc[0], "easiroc_hadc[64]/I");
	tree->Branch("easiroc_ladc", &event.easiroc.ladc[0], "easiroc_ladc[64]/I");
	tree->Branch("easiroc_ltdc", &event.easiroc.ltdc[0], "easiroc_ltdc[64]/I");
	tree->Branch("easiroc_ttdc", &event.easiroc.ttdc[0], "easiroc_ttdc[64]/I");

	tree->Branch("v1290_ltdc", &event.v1290.ltdc[0], "v1290_ltdc[32]/I");
	tree->Branch("v1290_ttdc", &event.v1290.ttdc[0], "v1290_ttdc[32]/I");

	tree->Branch("sis3820_scaler", &event.sis3820.scaler[0], "sis3820_scaler[32]/I");
	tree->Branch("amttdc_x1", &event.amttdc.tdc_x1[0], "amttdc_x1[64]/I");
	tree->Branch("amttdc_y1", &event.amttdc.tdc_y1[0], "amttdc_y1[64]/I");
	tree->Branch("amttdc_x2", &event.amttdc.tdc_x2[0], "amttdc_x2[64]/I");
	tree->Branch("amttdc_y2", &event.amttdc.tdc_y2[0], "amttdc_y2[64]/I");

	tree->Branch("mqdc32_qdc", &event.mqdc32.qdc[0], "mqdc32_qdc[32]/I");
	tree->Branch("mtdc32_tdc", &event.mtdc32.tdc[0], "mtdc32_tdc[32]/I");
	tree->Branch("mtdc32_trigger", &event.mtdc32.trigger[0], "mtdc32_trigger[2]/I");

	return;
}