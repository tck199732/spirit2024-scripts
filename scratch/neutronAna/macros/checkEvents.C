#include "spirit.h"

void checkEvents(
	// clang-format off
    const int&runId=1283
	// clang-format on
) {

	auto chain = new TChain("spirit", "spirit");
	std::string fname = Form("spirit/data%04d.root", runId);

	auto status = chain->AddFile(fname.c_str());
	if (status == 0) {
		std::cerr << "Could not open file " << fname << std::endl;
		return;
	}
	SetBranchAddress(chain); // from spirit.hh
	auto nEntries = chain->GetEntries();
	std::cout << "runId: " << runId << " Number of entries: " << nEntries << std::endl;
}
