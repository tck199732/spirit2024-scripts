void load() {

	// TString PROJECT_DIR = gSystem->Getenv("PROJECT_DIR");
	TString PROJECT_DIR = ".";

	gROOT->ProcessLine(Form(".L %s/src/detectorGeometry.cpp+", PROJECT_DIR.Data()));
	gROOT->ProcessLine(Form(".L %s/src/gainMatching.cpp+", PROJECT_DIR.Data()));
	gROOT->ProcessLine(Form(".L %s/src/positionCalibration.cpp+", PROJECT_DIR.Data()));
	gROOT->ProcessLine(Form(".L %s/src/walkCorrection.cpp+", PROJECT_DIR.Data()));
	gROOT->ProcessLine(Form(".L %s/src/tofOffset.cpp+", PROJECT_DIR.Data()));
	gROOT->ProcessLine(Form(".L %s/src/spiritReader.cpp+", PROJECT_DIR.Data()));
}