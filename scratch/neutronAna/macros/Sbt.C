#include "spirit.h"

void CheckSbtLeftTiming(
	// clang-format off
	const std::vector<int> &runIds =
		{   
            // commissioning
            // 1036, 1037, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 1048, 
            // physics run (Normal data run)
            1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168,
            1169, 1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177, 1178, 
            1179, 1180, 1183, 1184, 1185, 1186, 1188, 1189, 1190, 1191, 1192,
            1194, 1195, 
            1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 1218, 1219, 1221, 1222, 1223, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1234, 1235, 1236, 1238, 1239, 1240, 1241, 1242,
			1243, 1244, 1245, 1246, 1247, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1300,
		},
    const std::string& filenameOut = "triggerCounts.dat"
	// clang-format on
) {

	std::ofstream ofs(filenameOut);

	auto header =
		Form("%5s\t%10s\t%8s\t%8s\t%8s", "runId", "nEvents", "tpcCounter", "sbtLeftCounter", "sbtRightCounter");
	ofs << header << '\n';

	for (auto id : runIds) {
		std::cout << "Processing run " << id << std::endl;
		auto chain = new TChain("spirit", "spirit");
		chain->Add(Form("spirit/data%04d.root", id));
		SetBranchAddress(chain);
		auto nEvents = chain->GetEntries();

		int sbtLeftCounter = 0, sbtRightCounter = 0, tpcCounter = 0;
		auto is_sbt_left_present = false;
		auto is_sbt_right_present = false;
		auto is_tpc_present = false;

		for (auto iEvt = 0; iEvt < nEvents; iEvt++) {
			chain->GetEvent(iEvt);
			for (auto ihit = 0; ihit < spirit.hime_nHits; ihit++) {
				auto hitModule = spirit.hime_moduleID[ihit];
				if (hitModule == 0) {
					tpcCounter++;
					is_tpc_present = true;
				}
				if (hitModule == 1) {
					sbtLeftCounter++;
					is_sbt_left_present = true;
				}
				if (hitModule == 9) {
					sbtRightCounter++;
					is_sbt_right_present = true;
				}
			}
		}
		if (!is_sbt_left_present) {
			std::cout << "Sbt left is not present in run " << id << std::endl;
		}
		auto countsStr = Form("%5d\t%10lld\t%8d\t%8d\t%8d", id, nEvents, tpcCounter, sbtLeftCounter, sbtRightCounter);

		ofs << countsStr << '\n';
		delete chain;
	}
	ofs.close();
	return;
}
