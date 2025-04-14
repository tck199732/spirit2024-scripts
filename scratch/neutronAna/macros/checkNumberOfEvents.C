#include "spirit.h"

void checkNumberOfEvents(
	// clang-format off
    const std::vector<int> runIds = {
        1148, 1149, 1150, 1151, 1152, 1153, 1154, 1155, 1156, 1158, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1174, 1175, 1176, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1189, 1190, 1191, 1192, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210, 1211, 1212, 1213, 1214, 1215, 1217, 1218, 1219, 1221, 1222, 1223, 1225, 1226, 1227, 1232, 1234, 1235, 1236, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1247, 1248, 1251, 1252, 1253, 1254, 1255, 1256, 1261, 1262, 1263, 1264, 1265, 1266, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1300, 1304, 1305, 1306, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1320, 1321, 1322, 1324, 1325, 1326, 1327, 1331
    }, 
    const std::string &bdcDir = "bdc",
    const std::string &spiritDir = "spirit"
	// clang-format on
) {

	for (auto idx : runIds) {
		auto spiritChain = getSpiritChain({idx}, spiritDir);
		auto bdcChain = getBDCChain({idx}, bdcDir);

		auto spiritEntries = -1;
		auto bdcEntries = -1;

		if (spiritChain) {
			spiritEntries = spiritChain->GetEntries();
		}
		if (bdcChain) {
			bdcEntries = bdcChain->GetEntries();
		}

		if (spiritEntries != bdcEntries) {
			std::cerr << "run: " << idx << " BDC: " << bdcEntries << " Spirit: " << spiritEntries << std::endl;
		}

		delete spiritChain;
		delete bdcChain;
	}
}