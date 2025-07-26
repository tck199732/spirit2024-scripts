#!/usr/bin/env python3

import subprocess
import pathlib
import itertools
import time
import json


run_numbers = {
    # Xe124 + Sn112 runs in June
    'bar23_only_june' : [
        1148, 1149, 1150, 1151, 1152, 1153, 1154, 1155, 1156, 1158, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1174, 1175, 1176, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1189, 1190, 1191, 1192, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210
    ],

    'bar23_batch1_only_june' : [
        1148, 1149, 1150, 1151, 1152, 1153, 1154, 1155, 1156, 1158, 1160, 1161, 1162, 1163, 1164, 1165, 1166, 1167, 1168, 1169, 1170, 1171, 1172, 1174, 1175, 
    ],
    
    'bar23_batch2_only_june' : [
        1176, 1178, 1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1189, 1190, 1191, 1192, 1195, 1196, 1197, 1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 1208, 1210
    ],

    # Xe124 + Sn112 runs in June
    'bar34_only_june' : [ 
        1211, 1212, 1213, 1214, 1215, 1217, 1218, 1219, 1221, 1222, 1223, 1225, 1226, 1227, 1232, 1234, 1235, 1236, 1239, 1240, 1241, 1242, 1243, 1244, 1245 
    ],

    'bar34_batch1_only_june' : [ 
        1211, 1212, 1213, 1214, 1215, 1217, 1218, 1219, 1221, 1222, 1223, 
    ],

    'bar34_batch2_only_june' : [ 
        1225, 1226, 1227, 1232, 1234, 1235, 1236, 1239, 1240, 1241, 1242, 1243, 1244, 1245 
    ],
    
    # Xe136 + Sn124 runs in Nov
    'bar34_only_nov' : [
        # previously not added
        1634, 1635, 1636, 1637, 1638, 1639, 1640, 1641, 1642, 1643, 

        # usual runs
        1644, 1645, 1646, 1647, 1648, 1649, 1650, 1651, 1652, 1653, 1654, 1655, 1656, 1657, 1658, 1659, 1660, 1661, 1662, 1663, 1664, 1665, 1666, 1667, 1668, 1669, 1670, 1671, 1672, 1697, 1699, 1700, 1706, 1707, 1708, 1709, 1710, 1711, 1731, 1732, 1733, 1734, 1741, 1742, 1743, 1744, 1745, 1746, 

        # kyoto >= 3 here to check something
        1747, 1755, 1756, 1757, 1758, 1760, 1761, 1762, 1763, 1764, 1765, 1766, 1767, 1768, 1769, 1770, 1771, 1772, 1773, 
        
        # resumed to kyoto >= 4
        1775, 1776, 1777, 1778, 1779, 1786, 1800, 1801, 1802, 1803, 1804, 1805, 1806, 1807, 1808, 1809, 1810, 1811, 1812, 1813, 1814, 
        
        # cocktail beam runs
        # 1822, 1823, 1833, 1834, 1835, 1836, 1837, 1838, 1839, 1840, 

        1866, 1867, 1868, 1869, 1872, 1873, 1874, 1880, 1883, 1884, 1885, 1886, 1887, 1888, 1889, 1892, 1893, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1904, 1905, 1906, 1907, 1908, 1909, 1910, 1912, 1913, 1914, 1915, 1916,

        # In these runs, Hime and tpc Daq do not match, so no events are merged
        # 1721, 1759, 1794,

        # min-bias-runs,
        # 1673, 1680, 1684, 1691, 1692,

        # junk runs (only a few events, gg noise, etc)
        # 1675, 1676, 1677, 1679, 1681, 1682, 1683, 1685, 1686, 1687, 1688, 1689, 1690, 1693, 1694, 1695, 1696, 1698, 1701, 1702, 1703, 1704, 1705, 1729, 1730, 1735, 1736, 1737, 1738, 1739, 1740, 1748, 1750, 1751, 1752, 1753, 1754, 1774, 1780, 1781, 1782, 1783, 1784, 1785, 1790, 1791, 1792, 1793, 1815, 1816, 1817, 1818, 1819, 1824, 1825, 1826, 1827, 1828, 1829, 1830, 1831, 1832, 1841, 1842, 1843, 1844, 1845, 1846, 1847, 1848, 1849, 1850, 1851, 1852, 1853, 1854, 1855, 1856, 1857, 1858, 1859, 1860, 1861, 1862, 1863, 1864, 1865, 
    ],

    'bar34_batch1_only_nov' : [
        # runs before min-bias
        1634, 1635, 1636, 1637, 1638, 1639, 1640, 1641, 1642, 1643, 1644, 1645, 1646, 1647, 1648, 1649, 1650, 1651, 1652, 1653, 1654, 1655, 1656, 1657, 1658, 1659, 1660, 1661, 1662, 1663, 1664, 1665, 1666, 1667, 1668, 1669, 1670, 1671, 1672, 
    ],

    'bar34_batch2_only_nov' : [
        1697, 1699, 1700, 1706, 1707, 1708, 1709, 1710, 1711, 1731, 1732, 1733, 1734, 1741, 1742, 1743, 1744, 1745, 1746, 
    ],

    'bar34_batch3_only_nov' : [
        # after fixing field cage current issue
        1775, 1776, 1777, 1778, 1779, 1786, 1800, 1801, 1802, 1803, 1804, 1805, 1806, 1807, 1808, 1809, 1810, 1811, 1812, 1813, 1814, 
    ],

    'bar34_batch4_only_nov' : [
        # after cocktail beam
        1866, 1867, 1868, 1869, 1872, 1873, 1874, 1880, 1883, 1884, 1885, 1886, 1887, 1888, 1889, 1892, 1893, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1904, 1905, 1906, 1907, 1908, 1909, 1910, 1912, 1913, 1914, 1915, 1916
    ],
    
    # Sn112 
    'bar4_only_sn112_june' : [ 
        1247, 1248, 1251, 1252, 1253, 1254, 1255, 1256, 1273, 1274, 1275, 1276, 1277, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299,
        # min-bias-runs, I have checked that it does not affect the distribution much 
        1261, 1262, 1263, 1264, 1265, 1266, 1268, 1269, 
    ],

    # target switched to Sn124 to investigate the issue that target ladder was shorted
    'bar4_only_sn124_june' : [ 
        1300, 1304, 1305, 1306, 1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 1318, 1319, 1320, 1321, 1322, 1324, 1325, 1326, 1327, 1331 
    ],
}

# all runs with only bar 4 placed
run_numbers['bar4_only_june'] = run_numbers['bar4_only_sn112_june'] + run_numbers['bar4_only_sn124_june']

# all runs with bar 3 in June
run_numbers['bar3_june'] = run_numbers['bar23_only_june'] + run_numbers['bar34_only_june']

# all runs with bar 4 in June
run_numbers['bar4_june'] = run_numbers['bar34_only_june'] + run_numbers['bar4_only_sn112_june'] + run_numbers['bar4_only_sn124_june']

# all runs in June
run_numbers['all_june'] = (
    run_numbers['bar23_only_june'] + 
    run_numbers['bar34_only_june'] + 
    run_numbers['bar4_only_sn112_june'] + 
    run_numbers['bar4_only_sn124_june']
)

# all runs with bar 3 and 4 in both campaign
run_numbers['bar34_only_all'] = run_numbers['bar34_only_june'] + run_numbers['bar34_only_nov']
# all runs with bar 3 in both campaign
run_numbers['bar3_all'] = run_numbers['bar23_only_june'] + run_numbers['bar34_only_june'] + run_numbers['bar34_only_nov']
# all runs with bar 4 in both campaign
run_numbers['bar4_all'] = (
    run_numbers['bar4_only_sn112_june'] + 
    run_numbers['bar4_only_sn124_june'] + 
    run_numbers['bar34_only_june'] +
    run_numbers['bar34_only_nov']
)
# all runs in both campaign
run_numbers['all'] = (
    run_numbers['bar23_only_june'] + 
    run_numbers['bar34_only_june'] + 
    run_numbers['bar4_only_sn112_june'] + 
    run_numbers['bar4_only_sn124_june'] + 
    run_numbers['bar34_only_nov']
)


#####################################################################################################################
# make sure the run numbers are not repeated
for k, v in run_numbers.items():
    if len(v) != len(set(v)):
        raise ValueError(f"Run numbers for {k} contain duplicates: {v}")
    print(f"{k} has {len(v)} unique runs.")

#####################################################################################################################
current_dir = pathlib.Path(__file__).parent.resolve()
result_dir = current_dir / 'results'
macro_dir = current_dir / 'macros'
macro_name = 'Ana.C'
data_dirs = [current_dir / name for name in ['spirit-tpc', 'spirit']]

#####################################################################################################################

useWalkCorrection = True
# useGainMatching = True
useGainMatching = False
useVetoWall = True
# useTpcData = False  
# useTpcVertex = False

useTpcData = True
useTpcVertex = True

tof_ranges = [
    (-1000, 1000),
    (1.5, 1000),
]

tot_ranges = [
    (0, 1000),
]

x_range = (-1000, 1000)
y_range = (-1000, 1000)

data_dir = data_dirs[0] if useTpcData else data_dirs[1]
result_dir = result_dir / ('AnaWithTpc' if useTpcData else 'Ana')

#####################################################################################################################
for anal_idx, (tof_range, tot_range) in enumerate(itertools.product(tof_ranges, tot_ranges)):

    params = {
        'useWalkCorrection': useWalkCorrection,
        'useGainMatching': useGainMatching,
        'useVetoWall': useVetoWall,
        'useTpcVertex': useTpcVertex,
        'useTpcData': useTpcData,
        'tof_range': tof_range,
        'tot_range': tot_range,
        'run_numbers': run_numbers,
        'elapsed_time': {}
    }

    output_dir = result_dir / f'{anal_idx:02d}'
    output_dir.mkdir(parents=True, exist_ok=True)

    for key, runs in run_numbers.items():
        runs_cpp_vec = '{' + ', '.join(map(str, runs)) + '}'

        args = ', '.join(map(str, [
            runs_cpp_vec,
            f"\\\"{str(output_dir / f'{key}.root')}\\\"",
            1 if useWalkCorrection else 0,  # useWalkCorrection:
            1 if useGainMatching else 0,  # useGainMatching:
            1 if useVetoWall else 0,  # useVetoWall:
            1 if useTpcVertex else 0,  # useTpcVertex:
            1 if useTpcData else 0,  # useTpcData:

            rf'{{{tof_range[0]}, {tof_range[1]}}}',  # tof_range
            rf'{{{tot_range[0]}, {tot_range[1]}}}',  # tot_range
            rf'{{{x_range[0]}, {x_range[1]}}}',  # x_range
            rf'{{{y_range[0]}, {y_range[1]}}}',  # y_range

            f"\\\"{str(data_dir)}\\\""
        ]))

        start_time = time.time()
        command = f'root -l -b -q "{str(macro_dir)}/load.C" "{str(macro_dir)}/{macro_name}({args})"'
        subprocess.run(command, shell=True)
        elapsed_time = time.time() - start_time

        params['elapsed_time'][key] = elapsed_time

    with open(str(output_dir / f'params.json'), 'w') as f:
        json.dump(params, f, indent=4)
