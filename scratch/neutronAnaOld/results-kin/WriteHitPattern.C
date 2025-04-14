
void WriteHitPattern() {
	auto f = new TFile("hime-hit-pattern.root", "READ");

	auto h = (TH2D *)f->Get("hHitPatternFull");
	auto nbinsX = h->GetNbinsX();
	auto nbinsY = h->GetNbinsY();

	std::ofstream ofs("hime-hit-pattern.txt");
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
