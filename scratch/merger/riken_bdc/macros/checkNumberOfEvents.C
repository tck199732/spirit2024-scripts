void checkNumberOfEvents(
	// clang-format off
    const int& runNumber = 1221,
    const std::string & rikenDir="riken",
    const std::string& bdcDir="bdc",
    const std::string& bdcDirNew="bdc_3.12"
	// clang-format on
) {

	auto rikenChain = new TChain("samurai", "samurai");
	auto bdcChain = new TChain("TBDC", "TBDC");
	auto bdcNewChain = new TChain("TBDC", "TBDC");

	auto rikenFile = TString::Format("%s/data%04d.root", rikenDir.c_str(), runNumber);
	auto bdcFile = TString::Format("%s/bdc_%04d.root", bdcDir.c_str(), runNumber);
	auto bdcNewFile = TString::Format("%s/bdc_%04d.root", bdcDirNew.c_str(), runNumber);

	// -1 means file not found
	auto entriesBdc = -1;
	auto entriesBdcNew = -1;
	auto entriesRiken = -1;

	if (!gSystem->AccessPathName(rikenFile)) {
		rikenChain->Add(rikenFile);
		entriesRiken = rikenChain->GetEntries();
	}

	if (!gSystem->AccessPathName(bdcFile)) {
		bdcChain->Add(bdcFile);
		entriesBdc = bdcChain->GetEntries();
	}

	if (!gSystem->AccessPathName(bdcNewFile)) {
		bdcNewChain->Add(bdcNewFile);
		entriesBdcNew = bdcNewChain->GetEntries();
	}

	std::cout << "Run number: " << runNumber << " Riken: " << entriesRiken << " BDC: " << entriesBdc
			  << " BDC new: " << entriesBdcNew << std::endl;
}
