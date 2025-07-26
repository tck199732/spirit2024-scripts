
spiritData spirit;

void checkKyoto(
	// clang-format off
    const std::string &outputFilename = "logs/kyoto.txt",
    const std::string &spiritDir="spirit",
    const std::string & treeName = "spirit"

	// clang-format on
) {

	// get all root files in the spirit directory
	std::vector<std::string> filenames;
	std::vector<int> runIds;
	for (const auto &entry : std::filesystem::directory_iterator(spiritDir.c_str())) {
		if (entry.path().extension() == ".root") {
			filenames.push_back(entry.path().string());
			auto filename = entry.path().filename().string();
			int runId = std::stoi(filename.substr(0, filename.find_first_of('.')));
			runIds.push_back(runId);
		}
	}

	std::sort(filenames.begin(), filenames.end());
	std::sort(runIds.begin(), runIds.end());

	std::ofstream outFile(outputFilename);
	outFile << "Run ID, Kyoto Trigger" << std::endl;
	for (int iRun = 0; iRun < runIds.size(); iRun++) {
		int runID = runIds[iRun];
		auto path = filenames[iRun];

		auto chain = getChain({runID}, spiritDir, false, spirit);
		auto entries = chain->GetEntries();

		auto hKyoto = new TH1D("hKyoto", "", 64, 0, 64);
		for (auto iEvt = 0; iEvt < entries; iEvt++) {
			chain->GetEntry(iEvt);
			hKyoto->Fill(spirit.kyoto_multi);
		}

		int maxBin = hKyoto->GetMaximumBin();
		int maxValue = hKyoto->GetBinCenter(maxBin) - 0.5;
		outFile << runID << ", " << maxValue << std::endl;

		delete hKyoto;
		delete chain;
	}
}