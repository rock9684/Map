#include <algorithm>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"

SUITE(street_queries) {

    //write one or more test(s) to verify "find_street_ids_from_name" function
    //see m1 handout and unittest quickstart for guidance on writing the unit test

    TEST(street_street_segments) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;

        expected = {0, 1, 2, 4908, 4909, 5211, 11668, 11672, 11678, 11682, 11683, 11684, 14806, 16172, 16173, 16174, 16175, 16191, 16192, 16193, 16194};
        actual = find_street_street_segments(1);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1143, 1144, 1145, 1146};
        actual = find_street_street_segments(289);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {2398, 2399, 2400, 2401, 2402, 2403, 2404, 2405, 2406, 2407, 2408, 2409, 2410, 2411, 2412, 12674, 16644};
        actual = find_street_street_segments(644);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3333, 3345, 3653, 3654, 3658, 3659, 4164, 4165, 4166, 4167, 17283, 17284, 17285, 17286, 17287, 17288, 21421, 21422, 21423, 21424};
        actual = find_street_street_segments(892);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5365, 5366};
        actual = find_street_street_segments(1350);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6912};
        actual = find_street_street_segments(1747);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7826, 8501, 8689, 8690, 8691};
        actual = find_street_street_segments(2040);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {10052, 10053, 10054};
        actual = find_street_street_segments(2522);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12679};
        actual = find_street_street_segments(3024);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {16896, 19182, 19183, 19981, 19982, 19983, 19984, 19985, 19986, 19987, 19988, 19989, 21570, 21571, 21572, 21573, 21574, 21575, 21576, 21613, 21614, 21615, 21616, 21617, 21618, 21619, 21620, 21621, 21622, 21623, 21624};
        actual = find_street_street_segments(3283);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

    } //street_street_segments

    TEST(all_street_intersections) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;

        expected = {0, 1, 2, 3, 4, 3112, 3189, 4736, 4737, 4738, 4739, 4744, 4997, 5592, 10283, 10296, 10297, 10301, 10302, 10308, 12589, 12590};
        actual = find_all_street_intersections(1);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1199, 1204, 1205, 1211, 1212};
        actual = find_all_street_intersections(289);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {998, 1801, 1802, 2452, 2453, 2454, 2455, 2456, 2457, 2458, 2459, 2460, 2461, 2462, 2463, 2464, 2465, 2466};
        actual = find_all_street_intersections(644);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3339, 3340, 3353, 3354, 3653, 3654, 3655, 3659, 3660, 3745, 4039, 4040, 4047, 4048, 4049, 4050, 4051, 5885, 13195, 15185, 15186};
        actual = find_all_street_intersections(892);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1738, 5150, 5151};
        actual = find_all_street_intersections(1350);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3673, 4909};
        actual = find_all_street_intersections(1747);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1034, 6741, 7199, 7621, 7622, 8461};
        actual = find_all_street_intersections(2040);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {2991, 2995, 3010, 4027};
        actual = find_all_street_intersections(2522);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {2126, 10896};
        actual = find_all_street_intersections(3024);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3685, 4653, 5879, 7523, 7579, 8407, 8423, 8479, 8498, 8864, 8928, 8947, 9363, 9870, 9883, 12694, 12980, 12981, 13003, 13004, 14106, 14107, 14314, 14474, 14477, 14478, 15205, 15206, 15213, 15214, 15215};
        actual = find_all_street_intersections(3283);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

    } //all_street_intersections

    TEST(intersection_ids_from_street_names) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;

        expected = {1242};
        actual = find_intersection_ids_from_street_names("Clapham Road", "Queenston Road");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_intersection_ids_from_street_names("Dewitt Road", "Michael Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {9230};
        actual = find_intersection_ids_from_street_names("East 13th Street", "Thayer Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {10659};
        actual = find_intersection_ids_from_street_names("Effort Trail", "Grovetree Trail");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_intersection_ids_from_street_names("Kitty Murray Lane", "Larkspur Lane");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1, 2, 3, 4, 3112, 3189, 4741, 4744, 4997, 4999, 5081, 5592, 11722, 11866, 12577, 12578, 12590};
        actual = find_intersection_ids_from_street_names("Queen Elizabeth Way", "<unknown>");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_intersection_ids_from_street_names("Queensdale Avenue East", "Weir Street North");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_intersection_ids_from_street_names("Rochelle Avenue", "Ray Street South");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_intersection_ids_from_street_names("Weaver Drive", "Mozart Drive");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1738};
        actual = find_intersection_ids_from_street_names("Wellington Street North", "Park Street West");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

    } //intersection_ids_from_street_names

} //street_queries

SUITE(intersection_queries) {

    TEST(intersection_street_segments) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;

        expected = {0, 16172};
        actual = find_intersection_street_segments(0);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1247, 6976};
        actual = find_intersection_street_segments(1300);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {2875, 2876, 18238};
        actual = find_intersection_street_segments(2900);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {4101, 4102};
        actual = find_intersection_street_segments(4019);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6450, 6451};
        actual = find_intersection_street_segments(6085);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {8027, 11860};
        actual = find_intersection_street_segments(7873);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {10225, 13498, 13499};
        actual = find_intersection_street_segments(9194);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {13705, 13706, 20748};
        actual = find_intersection_street_segments(11367);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {18191, 18213, 18214};
        actual = find_intersection_street_segments(13631);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {20621, 20622, 20623};
        actual = find_intersection_street_segments(14799);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

    } //intersection_street_segments

    TEST(intersection_street_names) {
        std::vector<std::string> expected;
        std::vector<std::string> actual;

        expected = {"Queen Elizabeth Way", "Queen Elizabeth Way"};
        actual = find_intersection_street_names(0);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"Palm Court", "Stoney Brook Drive"};
        actual = find_intersection_street_names(1300);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"<unknown>", "Princess Street", "Princess Street"};
        actual = find_intersection_street_names(2900);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"Pritchard Road", "Pritchard Road"};
        actual = find_intersection_street_names(4019);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"Pier 25 South Gateway", "Pier 25 South Gateway"};
        actual = find_intersection_street_names(6085);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"<unknown>", "Private Road"};
        actual = find_intersection_street_names(7873);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"Brucedale Avenue East", "Brucedale Avenue East", "East 36th Street"};
        actual = find_intersection_street_names(9194);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"<unknown>", "Upper Gage Avenue", "Upper Gage Avenue"};
        actual = find_intersection_street_names(11367);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        actual = find_intersection_street_names(13631);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {"<unknown>", "<unknown>", "<unknown>"};
        actual = find_intersection_street_names(14799);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

    } //intersection_street_names

    TEST(directly_connected) {
        CHECK_EQUAL(1, are_directly_connected(0, 0));

        CHECK_EQUAL(0, are_directly_connected(110, 3402));

        CHECK_EQUAL(0, are_directly_connected(522, 756));

        CHECK_EQUAL(0, are_directly_connected(632, 2654));

        CHECK_EQUAL(0, are_directly_connected(972, 14745));

        CHECK_EQUAL(0, are_directly_connected(1049, 12409));

        CHECK_EQUAL(1, are_directly_connected(1095, 1094));

        CHECK_EQUAL(1, are_directly_connected(1283, 1282));

        CHECK_EQUAL(1, are_directly_connected(1300, 1300));

        CHECK_EQUAL(1, are_directly_connected(1358, 2709));

        CHECK_EQUAL(1, are_directly_connected(1369, 1370));

        CHECK_EQUAL(1, are_directly_connected(1428, 1419));

        CHECK_EQUAL(1, are_directly_connected(1736, 5174));

        CHECK_EQUAL(0, are_directly_connected(1758, 9243));

        CHECK_EQUAL(1, are_directly_connected(1912, 1875));

        CHECK_EQUAL(1, are_directly_connected(1969, 1968));

        CHECK_EQUAL(1, are_directly_connected(2307, 2308));

        CHECK_EQUAL(0, are_directly_connected(2337, 7798));

        CHECK_EQUAL(1, are_directly_connected(2518, 6558));

        CHECK_EQUAL(0, are_directly_connected(2601, 10671));

        CHECK_EQUAL(0, are_directly_connected(2733, 7971));

        CHECK_EQUAL(1, are_directly_connected(2900, 2900));

        CHECK_EQUAL(1, are_directly_connected(2918, 2264));

        CHECK_EQUAL(1, are_directly_connected(3026, 3025));

        CHECK_EQUAL(0, are_directly_connected(3105, 11238));

        CHECK_EQUAL(0, are_directly_connected(3152, 8737));

        CHECK_EQUAL(0, are_directly_connected(3397, 6858));

        CHECK_EQUAL(1, are_directly_connected(3418, 3419));

        CHECK_EQUAL(0, are_directly_connected(3608, 12260));

        CHECK_EQUAL(0, are_directly_connected(3731, 10162));

        CHECK_EQUAL(1, are_directly_connected(4019, 4020));

        CHECK_EQUAL(1, are_directly_connected(4542, 4543));

        CHECK_EQUAL(1, are_directly_connected(4716, 4715));

        CHECK_EQUAL(0, are_directly_connected(5534, 14579));

        CHECK_EQUAL(0, are_directly_connected(5593, 11489));

        CHECK_EQUAL(0, are_directly_connected(6014, 12612));

        CHECK_EQUAL(1, are_directly_connected(6045, 6047));

        CHECK_EQUAL(1, are_directly_connected(6060, 2492));

        CHECK_EQUAL(0, are_directly_connected(6671, 15041));

        CHECK_EQUAL(1, are_directly_connected(6709, 6709));

        CHECK_EQUAL(0, are_directly_connected(6728, 4856));

        CHECK_EQUAL(1, are_directly_connected(6754, 6755));

        CHECK_EQUAL(1, are_directly_connected(6968, 6969));

        CHECK_EQUAL(0, are_directly_connected(7408, 7117));

        CHECK_EQUAL(1, are_directly_connected(7528, 7529));

        CHECK_EQUAL(1, are_directly_connected(7708, 7664));

        CHECK_EQUAL(1, are_directly_connected(7873, 7872));

        CHECK_EQUAL(0, are_directly_connected(8265, 651));

        CHECK_EQUAL(0, are_directly_connected(8408, 11238));

        CHECK_EQUAL(0, are_directly_connected(8443, 1600));

        CHECK_EQUAL(0, are_directly_connected(8615, 11514));

        CHECK_EQUAL(0, are_directly_connected(8746, 143));

        CHECK_EQUAL(1, are_directly_connected(8822, 7036));

        CHECK_EQUAL(1, are_directly_connected(8852, 11836));

        CHECK_EQUAL(1, are_directly_connected(8901, 8902));

        CHECK_EQUAL(0, are_directly_connected(8949, 12118));

        CHECK_EQUAL(1, are_directly_connected(9049, 2691));

        CHECK_EQUAL(1, are_directly_connected(9057, 2653));

        CHECK_EQUAL(1, are_directly_connected(9166, 9167));

        CHECK_EQUAL(1, are_directly_connected(9194, 9194));

        CHECK_EQUAL(0, are_directly_connected(9195, 8560));

        CHECK_EQUAL(1, are_directly_connected(9318, 9319));

        CHECK_EQUAL(0, are_directly_connected(9384, 780));

        CHECK_EQUAL(0, are_directly_connected(9384, 4640));

        CHECK_EQUAL(1, are_directly_connected(9388, 9387));

        CHECK_EQUAL(1, are_directly_connected(9428, 9429));

        CHECK_EQUAL(1, are_directly_connected(9513, 9514));

        CHECK_EQUAL(1, are_directly_connected(9908, 5551));

        CHECK_EQUAL(1, are_directly_connected(10536, 10554));

        CHECK_EQUAL(0, are_directly_connected(10589, 4877));

        CHECK_EQUAL(1, are_directly_connected(10679, 10680));

        CHECK_EQUAL(0, are_directly_connected(10813, 14950));

        CHECK_EQUAL(1, are_directly_connected(11103, 11133));

        CHECK_EQUAL(1, are_directly_connected(11303, 1453));

        CHECK_EQUAL(0, are_directly_connected(11639, 4519));

        CHECK_EQUAL(0, are_directly_connected(11931, 2255));

        CHECK_EQUAL(0, are_directly_connected(11981, 12145));

        CHECK_EQUAL(0, are_directly_connected(12131, 15022));

        CHECK_EQUAL(1, are_directly_connected(12215, 8785));

        CHECK_EQUAL(0, are_directly_connected(12666, 8142));

        CHECK_EQUAL(1, are_directly_connected(12731, 12732));

        CHECK_EQUAL(0, are_directly_connected(12769, 5045));

        CHECK_EQUAL(0, are_directly_connected(12867, 3666));

        CHECK_EQUAL(0, are_directly_connected(12951, 7708));

        CHECK_EQUAL(1, are_directly_connected(13402, 14024));

        CHECK_EQUAL(0, are_directly_connected(13449, 8753));

        CHECK_EQUAL(1, are_directly_connected(13631, 13631));

        CHECK_EQUAL(1, are_directly_connected(13708, 13695));

        CHECK_EQUAL(1, are_directly_connected(13752, 13753));

        CHECK_EQUAL(1, are_directly_connected(13783, 4029));

        CHECK_EQUAL(1, are_directly_connected(13864, 13865));

        CHECK_EQUAL(1, are_directly_connected(13896, 14066));

        CHECK_EQUAL(0, are_directly_connected(13935, 4739));

        CHECK_EQUAL(0, are_directly_connected(14026, 859));

        CHECK_EQUAL(1, are_directly_connected(14314, 14313));

        CHECK_EQUAL(0, are_directly_connected(14510, 8176));

        CHECK_EQUAL(0, are_directly_connected(14561, 13951));

        CHECK_EQUAL(1, are_directly_connected(14655, 14656));

        CHECK_EQUAL(1, are_directly_connected(14799, 14799));

        CHECK_EQUAL(0, are_directly_connected(15010, 930));

    } //directly_connected

    TEST(adjacent_intersections) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;

        expected = {299, 301, 604};
        actual = find_adjacent_intersections(300);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {317, 8707};
        actual = find_adjacent_intersections(318);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {319, 321, 1602};
        actual = find_adjacent_intersections(320);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {352, 357, 438, 440};
        actual = find_adjacent_intersections(358);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {385, 387, 531};
        actual = find_adjacent_intersections(386);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {211, 491};
        actual = find_adjacent_intersections(490);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {385, 490, 492};
        actual = find_adjacent_intersections(491);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {496, 498, 9657};
        actual = find_adjacent_intersections(497);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {505, 507, 515};
        actual = find_adjacent_intersections(512);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {746, 748, 4290};
        actual = find_adjacent_intersections(747);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1043, 1045};
        actual = find_adjacent_intersections(1044);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1262, 1264, 1276};
        actual = find_adjacent_intersections(1263);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1449, 1451, 7572};
        actual = find_adjacent_intersections(1450);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1038, 1591, 8183};
        actual = find_adjacent_intersections(1592);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {998, 1638, 3892};
        actual = find_adjacent_intersections(1639);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {611, 1676};
        actual = find_adjacent_intersections(1677);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1724, 8039};
        actual = find_adjacent_intersections(1723);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3249, 3251, 12735};
        actual = find_adjacent_intersections(3250);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3379, 4086};
        actual = find_adjacent_intersections(3378);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3436, 11511};
        actual = find_adjacent_intersections(3435);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3476, 3482, 3765};
        actual = find_adjacent_intersections(3477);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3574, 3576};
        actual = find_adjacent_intersections(3575);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3709, 3711};
        actual = find_adjacent_intersections(3710);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3796, 3802};
        actual = find_adjacent_intersections(3797);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {739, 740, 10220};
        actual = find_adjacent_intersections(3955);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {4207, 4209};
        actual = find_adjacent_intersections(4208);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {4407, 4421, 4423};
        actual = find_adjacent_intersections(4422);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {4505, 4520, 11437};
        actual = find_adjacent_intersections(4506);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {4677, 4690};
        actual = find_adjacent_intersections(4689);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5367, 5369};
        actual = find_adjacent_intersections(5368);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5488, 5501, 5503, 9244};
        actual = find_adjacent_intersections(5502);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5959};
        actual = find_adjacent_intersections(5961);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6228};
        actual = find_adjacent_intersections(6227);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5694, 5695, 9676};
        actual = find_adjacent_intersections(6317);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5030, 6514, 6516, 8963};
        actual = find_adjacent_intersections(6515);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6529, 9083};
        actual = find_adjacent_intersections(6542);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6565, 6569};
        actual = find_adjacent_intersections(6568);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6572, 9154, 11045};
        actual = find_adjacent_intersections(6571);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6650, 6703, 6705};
        actual = find_adjacent_intersections(6704);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6725, 6831};
        actual = find_adjacent_intersections(6725);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6506, 6935, 8299};
        actual = find_adjacent_intersections(6784);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6874, 7735};
        actual = find_adjacent_intersections(6873);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7052, 7054, 13828};
        actual = find_adjacent_intersections(7053);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7137, 7667, 12118};
        actual = find_adjacent_intersections(7138);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7165, 7704, 7916};
        actual = find_adjacent_intersections(7164);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7221, 8781};
        actual = find_adjacent_intersections(7222);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7232, 7727, 8655};
        actual = find_adjacent_intersections(7344);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6846, 8111};
        actual = find_adjacent_intersections(7370);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7284, 7714, 8410};
        actual = find_adjacent_intersections(7715);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7299, 7783, 8234};
        actual = find_adjacent_intersections(7782);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7911, 8909};
        actual = find_adjacent_intersections(7910);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1117, 7007, 7434};
        actual = find_adjacent_intersections(7930);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {8138, 8140, 14286};
        actual = find_adjacent_intersections(8139);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {8334, 8415, 8486};
        actual = find_adjacent_intersections(8335);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7411, 8439, 14108};
        actual = find_adjacent_intersections(8440);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {7526, 8171, 8202};
        actual = find_adjacent_intersections(8460);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6505, 6906, 7476};
        actual = find_adjacent_intersections(8571);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3836, 7991, 8119};
        actual = find_adjacent_intersections(8805);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_adjacent_intersections(8981);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {9854, 9856, 12452};
        actual = find_adjacent_intersections(9855);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6849, 11292, 11293};
        actual = find_adjacent_intersections(9909);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1866, 2494, 9921};
        actual = find_adjacent_intersections(9920);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6458, 9986, 10725};
        actual = find_adjacent_intersections(9987);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {2845, 10000, 10002};
        actual = find_adjacent_intersections(10001);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {10298, 10307};
        actual = find_adjacent_intersections(10306);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {10498};
        actual = find_adjacent_intersections(10499);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {8463, 10748, 10750};
        actual = find_adjacent_intersections(10749);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {10790, 10792};
        actual = find_adjacent_intersections(10791);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {11352, 11360, 11373};
        actual = find_adjacent_intersections(11351);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {11315, 11349, 11374};
        actual = find_adjacent_intersections(11355);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {8831, 13627};
        actual = find_adjacent_intersections(11656);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {9038};
        actual = find_adjacent_intersections(11673);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {11767};
        actual = find_adjacent_intersections(11766);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {5667, 12063, 12076};
        actual = find_adjacent_intersections(12073);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12338, 12340, 12345};
        actual = find_adjacent_intersections(12339);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12341, 12342, 12344, 12349};
        actual = find_adjacent_intersections(12343);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12458, 12462, 12463};
        actual = find_adjacent_intersections(12457);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12474, 12476, 12488};
        actual = find_adjacent_intersections(12475);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12477, 12486};
        actual = find_adjacent_intersections(12485);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {11713, 11714, 12511};
        actual = find_adjacent_intersections(12512);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {3170, 12541};
        actual = find_adjacent_intersections(12540);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_adjacent_intersections(12552);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {6671, 6672, 12612};
        actual = find_adjacent_intersections(12613);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12860, 12863};
        actual = find_adjacent_intersections(12859);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_adjacent_intersections(13222);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_adjacent_intersections(13224);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {11045};
        actual = find_adjacent_intersections(13292);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {13307, 13359, 13396};
        actual = find_adjacent_intersections(13397);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {1819, 1826, 13524};
        actual = find_adjacent_intersections(13525);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {13975, 13977, 13999};
        actual = find_adjacent_intersections(13976);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {13387, 13994, 14004};
        actual = find_adjacent_intersections(14000);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {14029, 14031};
        actual = find_adjacent_intersections(14030);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {4377, 4378};
        actual = find_adjacent_intersections(14175);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {};
        actual = find_adjacent_intersections(14212);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {12893, 14326};
        actual = find_adjacent_intersections(14325);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {11293};
        actual = find_adjacent_intersections(14448);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {14483, 14485, 14506};
        actual = find_adjacent_intersections(14484);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {15074, 15075, 15097};
        actual = find_adjacent_intersections(15098);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        expected = {2598, 10977, 15273};
        actual = find_adjacent_intersections(15275);
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

    } //adjacent_intersections

} //intersection_queries

SUITE(distance_time) {

    TEST(distance_between_two_points) {
        double expected;
        double actual;

        expected = 24823.31335593922;
        actual = find_distance_between_two_points(LatLon(43.1689, -80.0282), LatLon(43.2994, -79.7797));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 15630.17960236029;
        actual = find_distance_between_two_points(LatLon(43.1858, -79.6283), LatLon(43.2195, -79.8154));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 6672.870490809326;
        actual = find_distance_between_two_points(LatLon(43.2755, -79.9196), LatLon(43.2458, -79.9912));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 20574.94245128574;
        actual = find_distance_between_two_points(LatLon(43.2241, -79.9054), LatLon(43.2608, -79.6566));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 5372.465070829538;
        actual = find_distance_between_two_points(LatLon(43.2861, -79.7876), LatLon(43.2379, -79.7836));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 24710.92112794723;
        actual = find_distance_between_two_points(LatLon(43.3107, -79.6557), LatLon(43.294, -79.9601));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 5055.09442280112;
        actual = find_distance_between_two_points(LatLon(43.3162, -79.666), LatLon(43.3116, -79.7282));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 2384.329623676293;
        actual = find_distance_between_two_points(LatLon(43.2027, -79.6535), LatLon(43.2222, -79.6414));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 5741.516155731717;
        actual = find_distance_between_two_points(LatLon(43.2384, -79.9007), LatLon(43.2829, -79.8648));
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 8736.711153840281;
        actual = find_distance_between_two_points(LatLon(43.2432, -79.7435), LatLon(43.1703, -79.7834));
        CHECK(relative_error(expected, actual) < 0.05);

    } //distance_between_two_points

    TEST(street_segment_length) {
        double expected;
        double actual;

        expected = 809.9449602075517;
        actual = find_street_segment_length(0);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 129.6519723560345;
        actual = find_street_segment_length(1866);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 73.21183332250604;
        actual = find_street_segment_length(4164);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 82.21546473852037;
        actual = find_street_segment_length(5771);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 188.0273169268441;
        actual = find_street_segment_length(8737);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 18.76282538795119;
        actual = find_street_segment_length(11305);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 40.38287675241861;
        actual = find_street_segment_length(13201);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 222.8002267568292;
        actual = find_street_segment_length(16322);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 64.85965144357679;
        actual = find_street_segment_length(19573);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 19.63235893407543;
        actual = find_street_segment_length(21249);
        CHECK(relative_error(expected, actual) < 0.05);

    } //street_segment_length

    TEST(street_length) {
        double expected;
        double actual;

        expected = 20866.45234509198;
        actual = find_street_length(1);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 346.3631944515578;
        actual = find_street_length(289);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 2699.265290522044;
        actual = find_street_length(644);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 10192.90812039052;
        actual = find_street_length(892);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 182.3629937410935;
        actual = find_street_length(1350);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 223.5524032089631;
        actual = find_street_length(1747);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 465.4903902602714;
        actual = find_street_length(2040);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 404.6823314384133;
        actual = find_street_length(2522);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 224.6602765525578;
        actual = find_street_length(3024);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 3113.791643139723;
        actual = find_street_length(3283);
        CHECK(relative_error(expected, actual) < 0.05);

    } //street_length

    TEST(street_segment_travel_time) {
        double expected;
        double actual;

        expected = 0.485966976124531;
        actual = find_street_segment_travel_time(0);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.1555823668272414;
        actual = find_street_segment_travel_time(1866);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.08785419998700725;
        actual = find_street_segment_travel_time(4164);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.09865855768622445;
        actual = find_street_segment_travel_time(5771);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.225632780312213;
        actual = find_street_segment_travel_time(8737);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.02251539046554143;
        actual = find_street_segment_travel_time(11305);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.04845945210290233;
        actual = find_street_segment_travel_time(13201);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.2673602721081951;
        actual = find_street_segment_travel_time(16322);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.07783158173229215;
        actual = find_street_segment_travel_time(19573);
        CHECK(relative_error(expected, actual) < 0.05);

        expected = 0.02355883072089052;
        actual = find_street_segment_travel_time(21249);
        CHECK(relative_error(expected, actual) < 0.05);

    } //street_segment_travel_time

} //distance_time

SUITE(spatial_queries) {

    TEST(closest_poi) {
        CHECK_EQUAL(80, find_closest_point_of_interest(LatLon(43.1689, -80.0282)));

        CHECK_EQUAL(63, find_closest_point_of_interest(LatLon(43.2994, -79.7797)));

        CHECK_EQUAL(388, find_closest_point_of_interest(LatLon(43.1858, -79.6283)));

        CHECK_EQUAL(52, find_closest_point_of_interest(LatLon(43.2195, -79.8154)));

        CHECK_EQUAL(356, find_closest_point_of_interest(LatLon(43.2458, -79.9912)));

        CHECK_EQUAL(439, find_closest_point_of_interest(LatLon(43.2755, -79.9196)));

        CHECK_EQUAL(414, find_closest_point_of_interest(LatLon(43.2861, -79.7876)));

        CHECK_EQUAL(192, find_closest_point_of_interest(LatLon(43.2379, -79.7836)));

        CHECK_EQUAL(63, find_closest_point_of_interest(LatLon(43.3116, -79.7282)));

        CHECK_EQUAL(414, find_closest_point_of_interest(LatLon(43.3162, -79.666)));

    } //closest_poi

    TEST(closest_intersection) {
        CHECK_EQUAL(13205, find_closest_intersection(LatLon(43.1689, -80.0282)));

        CHECK_EQUAL(8993, find_closest_intersection(LatLon(43.2994, -79.7797)));

        CHECK_EQUAL(1461, find_closest_intersection(LatLon(43.1858, -79.6283)));

        CHECK_EQUAL(7138, find_closest_intersection(LatLon(43.2195, -79.8154)));

        CHECK_EQUAL(9069, find_closest_intersection(LatLon(43.2458, -79.9912)));

        CHECK_EQUAL(2074, find_closest_intersection(LatLon(43.2755, -79.9196)));

        CHECK_EQUAL(4218, find_closest_intersection(LatLon(43.2861, -79.7876)));

        CHECK_EQUAL(5018, find_closest_intersection(LatLon(43.2379, -79.7836)));

        CHECK_EQUAL(8993, find_closest_intersection(LatLon(43.3116, -79.7282)));

        CHECK_EQUAL(7485, find_closest_intersection(LatLon(43.3162, -79.666)));

    } //closest_intersection

} //spatial_queries

