void checkEntries(const int &runId = 1608) {

	auto hime = new TChain("tree", "tree");
	auto samurai = new TChain("samurai", "samurai");

	std::string hime_pth = Form("hime/data%04d.root", runId);
	std::string samurai_pth = Form("samurai/data%04d.root", runId);

	int hime_entries = 0;
	int samurai_entries = 0;

	if (gSystem->AccessPathName(hime_pth.c_str())) {
		hime_entries = -1;
	} else {
		hime->Add(hime_pth.c_str());
		hime_entries = hime->GetEntries();
	}

	if (gSystem->AccessPathName(samurai_pth.c_str())) {
		samurai_entries = -1;
	} else {
		samurai->Add(samurai_pth.c_str());
		samurai_entries = samurai->GetEntries();
	}

	std::cout << "Run ID: " << runId << "\t";
	std::cout << "HIME entries: " << hime_entries << "\t";
	std::cout << " SAMURAI entries: " << samurai_entries << std::endl;
}