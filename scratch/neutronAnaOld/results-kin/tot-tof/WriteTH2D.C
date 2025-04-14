
void WriteTH2D(
	// clang-format off
    const std::string & filename = "hime.root",
    const std::string& histname = "hHitPatternFull",
    const std::string& outfilename = "hime-hit-pattern.txt"
	// clang-format on
) {
	auto f = new TFile(filename.c_str(), "READ");
	auto h = (TH2D *)f->Get(histname.c_str());
	auto nbinsX = h->GetNbinsX();
	auto nbinsY = h->GetNbinsY();

	std::ofstream ofs(outfilename.c_str());
	for (auto i = 1; i <= nbinsX; i++) {
		for (auto j = 1; j <= nbinsY; j++) {
			ofs << h->GetBinContent(i, j) << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
	f->Close();
	return;
}
