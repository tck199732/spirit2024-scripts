#include "spiritReader.hpp"

SpiritReader::SpiritReader(const std::vector<int> &runIds, const std::string &spiritDir, bool useTpc) :
	mUseTpc(useTpc),
	hime_nHits(mData.hime_nHits),
	hime_tofRaw(mData.hime_tofRaw),
	hime_tDiff(mData.hime_tDiff),
	hime_tSum(mData.hime_tSum),
	hime_tot0(mData.hime_tot0),
	hime_tot1(mData.hime_tot1),
	hime_moduleID(mData.hime_moduleID),
	hime_slowScaler(mData.hime_slowScaler),
	hime_fastScaler(mData.hime_fastScaler),
	hime_eventNumber(mData.hime_eventNumber),
	run(mData.run),
	event(mData.event),
	lupots(mData.lupots),
	kyoto_multi(mData.kyoto_multi),
	kyoto_bar(mData.kyoto_bar),
	hime_veto_multi(mData.hime_veto_multi),
	hime_veto_bar(mData.hime_veto_bar),
	hime_veto_tof(mData.hime_veto_tof),
	hime_veto_charge(mData.hime_veto_charge),
	hime_veto_tdiff(mData.hime_veto_tdiff),
	hime_veto_x(mData.hime_veto_x),
	tbdc_x(mData.tbdc_x),
	tbdc_y(mData.tbdc_y),
	tbdc_a(mData.tbdc_a),
	tbdc_b(mData.tbdc_b),
	eventID(mData.eventID),
	dataID(mData.dataID),
	time(mData.time),
	tracks(mData.tracks),
	tracksWphi(mData.tracksWphi),
	vertex_x(mData.vertex_x),
	vertex_y(mData.vertex_y),
	vertex_z(mData.vertex_z) {

	mChain = new TChain("spirit", "spirit");

	for (auto idx : runIds) {
		std::string fname = Form("%s/%04d.root", spiritDir.c_str(), idx);

		if (!std::filesystem::exists(fname)) {
			std::cerr << "File does not exist: " << fname << std::endl;
			continue;
		}
		std::cout << "Adding file: " << fname << std::endl;

		mChain->AddFile(fname.c_str());
	}

	setBranchAddresses();
}

void SpiritReader::setBranchAddresses() {
	if (!mChain) {
		std::cerr << "Chain is not initialized!" << std::endl;
		return;
	}

	mChain->SetBranchAddress("hime_nHits", &mData.hime_nHits);
	mChain->SetBranchAddress("hime_tofRaw", &mData.hime_tofRaw[0]);
	mChain->SetBranchAddress("hime_tDiff", &mData.hime_tDiff[0]);
	mChain->SetBranchAddress("hime_tSum", &mData.hime_tSum[0]);
	mChain->SetBranchAddress("hime_tot0", &mData.hime_tot0[0]);
	mChain->SetBranchAddress("hime_tot1", &mData.hime_tot1[0]);
	mChain->SetBranchAddress("hime_moduleID", &mData.hime_moduleID[0]);

	mChain->SetBranchAddress("hime_slowScaler", &mData.hime_slowScaler);
	mChain->SetBranchAddress("hime_fastScaler", &mData.hime_fastScaler);
	mChain->SetBranchAddress("hime_eventNumber", &mData.hime_eventNumber);

	mChain->SetBranchAddress("runNumber", &mData.run);
	mChain->SetBranchAddress("eventNumber", &mData.event);
	mChain->SetBranchAddress("lupoTimeStamp", &mData.lupots);
	mChain->SetBranchAddress("kyotoMulti", &mData.kyoto_multi);
	mChain->SetBranchAddress("kyotoBarId", &mData.kyoto_bar[0]);
	mChain->SetBranchAddress("vetoMulti", &mData.hime_veto_multi);
	mChain->SetBranchAddress("vetoBarId", &mData.hime_veto_bar[0]);
	mChain->SetBranchAddress("vetoTof", &mData.hime_veto_tof[0]);
	mChain->SetBranchAddress("vetoTot", &mData.hime_veto_charge[0]);
	mChain->SetBranchAddress("vetoTdiff", &mData.hime_veto_tdiff[0]);

	mChain->SetBranchAddress("tbdc_x", &mData.tbdc_x);
	mChain->SetBranchAddress("tbdc_y", &mData.tbdc_y);
	mChain->SetBranchAddress("tbdc_a", &mData.tbdc_a);
	mChain->SetBranchAddress("tbdc_b", &mData.tbdc_b);

	if (mUseTpc) {
		mChain->SetBranchAddress("eventID", &mData.eventID);
		mChain->SetBranchAddress("dataID", &mData.dataID);
		mChain->SetBranchAddress("time", &mData.time);
		mChain->SetBranchAddress("tracks", &mData.tracks);
		mChain->SetBranchAddress("tracksWphi", &mData.tracksWphi);
		mChain->SetBranchAddress("vertex_x", &mData.vertex_x);
		mChain->SetBranchAddress("vertex_y", &mData.vertex_y);
		mChain->SetBranchAddress("vertex_z", &mData.vertex_z);
	}
}

SpiritReader::~SpiritReader() {
	if (mChain) {
		delete mChain;
		mChain = nullptr;
	}
}