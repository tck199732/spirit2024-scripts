
const int nLayers = 3;
const int nModules = 72;
const double C_LIGHT = 0.299792458; // m/ns

const double FRONT_DISTANCE = 4.8;	// m
const double LAYER_DISTANCE = 0.06; // m

spiritData spirit;

void positionAna(
	// clang-format off
    const std::vector<int> runIds = { 
        1148, 1149, 1150, 1151, 1152, 1153, 1154, 1155, 1156, 1158, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1174, 1175, 1176, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1189, 1190, 1191, 1192, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210,

        1211, 1212, 1213, 1214, 1215, 1217, 1218, 1219, 1221, 1222, 1223, 1225, 1226, 1227, 1232, 1234, 1235, 1236, 1239, 1240, 1241, 1242, 1243, 1244, 1245,

        // 1247, 1248, 1251, 1252, 1253, 1254, 1255, 1256, 1268, 1269, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299,

        // 1261, 1262, 1263, 1264, 1265, 1266, 

        // 1300, 1304, 1305, 1306, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1320, 1321, 1322, 1324, 1325, 1326, 1327, 1331,

        // 1644, 1645, 1646, 1647, 1648, 1649, 1650, 1651, 1652, 1653, 1654, 1655, 1656, 1657, 1658, 1659, 1660, 1661, 1662, 1663, 1664, 1665, 1666, 1667, 1668, 1669, 1670, 1671, 1672, 1673, 1675, 1676, 1677, 1679, 1680, 1681, 1682, 1683, 1684, 1685, 1686, 1687, 1688, 1689, 1690, 1691, 1692, 1693, 1694, 1695, 1696, 1697, 1698, 1699, 1700, 1701, 1702, 1703, 1704, 1705, 1706, 1707, 1708, 1709, 1710, 1711, 1729, 1730, 1731, 1732, 1733, 1734, 1735, 1736, 1737, 1738, 1739, 1740, 1741, 1742, 1743, 1744, 1745, 1746, 1747, 1748, 1750, 1751, 1752, 1753, 1754, 1755, 1756, 1757, 1758, 1760, 1761, 1762, 1763, 1764, 1765, 1766, 1767, 1768, 1769, 1770, 1771, 1772, 1773, 1774, 1775, 1776, 1777, 1778, 1779, 1780, 1781, 1782, 1783, 1784, 1785, 1786, 1788, 1790, 1791, 1792, 1793, 1800, 1801, 1802, 1803, 1804, 1805, 1806, 1807, 1808, 1809, 1810, 1811, 1812, 1813, 1814, 1815, 1816, 1817, 1818, 1819, 1822, 1823, 1824, 1825, 1826, 1827, 1828, 1829, 1830, 1831, 1832, 1833, 1834, 1835, 1836, 1837, 1838, 1839, 1840, 1841, 1842, 1843, 1844, 1845, 1846, 1847, 1848, 1849, 1850, 1851, 1852, 1853, 1854, 1855, 1856, 1857, 1858, 1859, 1860, 1861, 1862, 1863, 1864, 1865, 1866, 1867, 1868, 1869, 1872, 1873, 1874, 1880, 1883, 1884, 1885, 1886, 1887, 1888, 1889, 1892, 1893, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1904, 1905, 1906, 1612, 1613, 1615, 1907, 1908, 1909, 1910, 1912, 1913, 1914, 1915, 1916

    }, 
    const std::string &outputFilename = "output.root",

    bool useVetoWall = true,
    // bool useTpcVertex = true,
    bool useTpcVertex = false,

    bool useTpcData = true,
    // bool useTpcData = false,

    // const std::string &spiritDir="spirit"
    const std::string &spiritDir="spirit-tpc",

    const std::string &tdiffOffsetFile = "database/calibration/tdiff/position-calib-muon.json",
    const std::string &modulePositionFilename = "database/module_positions.dat"

)
// clang-format on
{

	auto chain = getChain(runIds, spiritDir, useTpcData, spirit);
	auto spiritEntries = chain->GetEntries();

	auto detGeometry = DetectorGeometry(modulePositionFilename, nLayers, nModules);
	auto posCalib = PositionCalibration(tdiffOffsetFile, nModules);

	/****************************************************************************************************************/
	// hime
	auto hHitPattern = new TH2D("hHitPattern", "", 200, -1000, 1000, 200, -1000, 1000);
	auto hBarHitPattern = new TH2D("hBarHitPattern", "", 200, -1000, 1000, nModules, 0, nModules);

	hHitPattern->Sumw2();
	hBarHitPattern->Sumw2();

	TH2D *hHitPatternLayer[nLayers];

	for (int i = 0; i < nLayers; i++) {
		hHitPatternLayer[i] = new TH2D(Form("hHitPatternLayer%d", i), "", 200, -1000, 1000, 200, -1000, 1000);
		hHitPatternLayer[i]->Sumw2();
	}

	TH1D *hBarHit[nModules];

	for (int i = 0; i < nModules; i++) {

		hBarHit[i] = new TH1D(Form("hBarHit%d", i), "", 200, -1000, 1000);
		hBarHit[i]->Sumw2();
	}

	/****************************************************************************************************************/

	for (auto iEvt = 0; iEvt < spiritEntries; iEvt++) {

		chain->GetEntry(iEvt);

		auto cutVertex = (useTpcVertex && useTpcData) ? spirit.tracksWphi == 0 : false;
		if (cutVertex) {
			continue;
		}

		auto vetoEvent = useVetoWall ? spirit.hime_veto_multi > 0 : false;
		if (vetoEvent) {
			continue;
		}

		for (int ihit = 0; ihit < spirit.hime_nHits; ihit++) {

			int moduleId = spirit.hime_moduleID[ihit];
			int layerId = detGeometry.getLayer(moduleId);

			if (moduleId <= 1 || moduleId >= nModules) {
				continue;
			}

			auto tDiff = spirit.hime_tDiff[ihit];
			auto barPosition = posCalib.calculatePosition(moduleId, tDiff);
			auto randomPosition = detGeometry.getRandomizedPosition(moduleId);

			double xHit = 0, yHit = 0;
			if (layerId == 0 || layerId == 2) {
				xHit = barPosition;
				yHit = randomPosition;
			} else {
				yHit = barPosition;
				xHit = randomPosition;
			}

			hHitPattern->Fill(xHit, yHit);
			hBarHitPattern->Fill(layerId == 0 || layerId == 2 ? xHit : yHit, moduleId);
			hHitPatternLayer[layerId]->Fill(xHit, yHit);
			hBarHit[moduleId]->Fill(layerId == 0 || layerId == 2 ? xHit : yHit);
		}
	}

	// scale all histograms by the number of events

	hHitPattern->Scale(1. / spiritEntries);
	hBarHitPattern->Scale(1. / spiritEntries);

	for (int i = 0; i < nLayers; i++) {
		hHitPatternLayer[i]->Scale(1. / spiritEntries);
	}

	for (int i = 0; i < nModules; i++) {
		hBarHit[i]->Scale(1. / spiritEntries);
	}

	auto ofile = new TFile(outputFilename.c_str(), "RECREATE");

	hHitPattern->Write();
	hBarHitPattern->Write();
	for (int i = 0; i < nLayers; i++) {
		hHitPatternLayer[i]->Write();
	}
	for (int i = 0; i < nModules; i++) {
		hBarHit[i]->Write();
	}
	ofile->Close();
}
