#include "cJSON.h"
#include "cJSON.c"
#include <stdio.h>
#include <malloc.h>

struct CaseData
{
	int confirmedCases;
	double percentInfected;
	double peoplePerKilo;
	double casesPerKilo;
	double avgNewCases;
};

struct VaccineData
{
	int totalVaccinated;
	int totalPartiallyVaccinated;
	double percentVaccinated;
	int herdImmunity;
};

struct RecoveryData
{
	double avgRecoveries;
	double percentImmune;
};

struct DeathData
{
	int totalDeaths;
	double avgDeaths;
};

struct Country
{
	struct CaseData* cases;
	struct VaccineData* vaccines;
	struct RecoveryData* recoveries;
	struct DeathData* deaths;
	struct Country* nextCountry;
	char countryName[50];
	long timeAdded;
};

// Pointers for the linked list / cache
struct Country* listStartPointer;
struct Country* listIndexPointer;

char sampleCasesResponse[] = "{\"All\": {\"confirmed\": 4022653, \"recovered\": 3465576, \"deaths\": 120807, \"country\": \"Italy\", \"population\": 59359900, \"sq_km_area\": 301316, \"life_expectancy\": 79, \"elevation_in_meters\": 538, \"continent\": \"Europe\", \"abbreviation\": \"IT\", \"location\": \"Southern Europe\", \"iso\": 380, \"capital_city\": \"Roma\"}, \"Abruzzo\": {\"lat\": \"42.35122196\", \"long\": \"13.39843823\", \"confirmed\": 71218, \"recovered\": 60432, \"deaths\": 2403, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Basilicata\": {\"lat\": \"40.63947052\", \"long\": \"15.80514834\", \"confirmed\": 23956, \"recovered\": 17192, \"deaths\": 534, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Calabria\": {\"lat\": \"38.90597598\", \"long\": \"16.59440194\", \"confirmed\": 60074, \"recovered\": 43849, \"deaths\": 1011, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Campania\": {\"lat\": \"40.83956555\", \"long\": \"14.25084984\", \"confirmed\": 391792, \"recovered\": 294011, \"deaths\": 6393, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Emilia-Romagna\": {\"lat\": \"44.49436681\", \"long\": \"11.3417208\", \"confirmed\": 369250, \"recovered\": 312018, \"deaths\": 12876, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Friuli Venezia Giulia\": {\"lat\": \"45.6494354\", \"long\": \"13.76813649\", \"confirmed\": 105156, \"recovered\": 93878, \"deaths\": 3700, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Lazio\": {\"lat\": \"41.89277044\", \"long\": \"12.48366722\", \"confirmed\": 323754, \"recovered\": 271385, \"deaths\": 7673, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Liguria\": {\"lat\": \"44.41149315\", \"long\": \"8.9326992\", \"confirmed\": 99416, \"recovered\": 89762, \"deaths\": 4183, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Lombardia\": {\"lat\": \"45.46679409\", \"long\": \"9.190347404\", \"confirmed\": 804620, \"recovered\": 719265, \"deaths\": 32870, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Marche\": {\"lat\": \"43.61675973\", \"long\": \"13.5188753\", \"confirmed\": 97705, \"recovered\": 88565, \"deaths\": 2938, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Molise\": {\"lat\": \"41.55774754\", \"long\": \"14.65916051\", \"confirmed\": 13228, \"recovered\": 12124, \"deaths\": 475, \"updated\": \"2021/04/30 22:20:42+00\"}, \"P.A. Bolzano\": {\"lat\": \"46.49933453\", \"long\": \"11.35662422\", \"confirmed\": 71142, \"recovered\": 68942, \"deaths\": 1161, \"updated\": \"2021/04/30 22:20:42+00\"}, \"P.A. Trento\": {\"lat\": \"46.06893511\", \"long\": \"11.12123097\", \"confirmed\": 43999, \"recovered\": 41579, \"deaths\": 1344, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Piemonte\": {\"lat\": \"45.0732745\", \"long\": \"7.680687483\", \"confirmed\": 345393, \"recovered\": 318439, \"deaths\": 11255, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Puglia\": {\"lat\": \"41.12559576\", \"long\": \"16.86736689\", \"confirmed\": 234841, \"recovered\": 181376, \"deaths\": 5873, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Sardegna\": {\"lat\": \"39.21531192\", \"long\": \"9.110616306\", \"confirmed\": 54507, \"recovered\": 36030, \"deaths\": 1384, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Sicilia\": {\"lat\": \"38.11569725\", \"long\": \"13.3623567\", \"confirmed\": 208487, \"recovered\": 178181, \"deaths\": 5410, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Toscana\": {\"lat\": \"43.76923077\", \"long\": \"11.25588885\", \"confirmed\": 226842, \"recovered\": 199989, \"deaths\": 6184, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Umbria\": {\"lat\": \"43.10675841\", \"long\": \"12.38824698\", \"confirmed\": 54442, \"recovered\": 50157, \"deaths\": 1352, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Valle d\'Aosta\": {\"lat\": \"45.73750286\", \"long\": \"7.320149366\", \"confirmed\": 10932, \"recovered\": 9725, \"deaths\": 458, \"updated\": \"2021/04/30 22:20:42+00\"}, \"Veneto\": {\"lat\": \"45.43490485\", \"long\": \"12.33845213\", \"confirmed\": 411899, \"recovered\": 378677, \"deaths\": 11330, \"updated\": \"2021/04/30 22:20:42+00\"}}";
char sampleHistoryResponse[] = "{\"All\": {\"country\": \"France\", \"population\": 64979548, \"sq_km_area\": 551500, \"life_expectancy\": \"78.8\", \"elevation_in_meters\": 375, \"continent\": \"Europe\", \"abbreviation\": \"FR\", \"location\": \"Western Europe\", \"iso\": 250, \"capital_city\": \"Paris\", \"dates\": {\"2021-04-29\": 5547670, \"2021-04-28\": 5521227, \"2021-04-27\": 5490025, \"2021-04-26\": 5460793, \"2021-04-25\": 5456417, \"2021-04-24\": 5432085, \"2021-04-23\": 5399600, \"2021-04-22\": 5367544, \"2021-04-21\": 5334144, \"2021-04-20\": 5300321, \"2021-04-19\": 5256318, \"2021-04-18\": 5249622, \"2021-04-17\": 5221888, \"2021-04-16\": 5186109, \"2021-04-15\": 5149776, \"2021-04-14\": 5111882, \"2021-04-13\": 5068534, \"2021-04-12\": 5030401, \"2021-04-11\": 5021865, \"2021-04-10\": 4903965, \"2021-04-09\": 4903965, \"2021-04-08\": 4903965, \"2021-04-07\": 4807123, \"2021-04-06\": 4807123, \"2021-04-05\": 4799078, \"2021-04-04\": 4788117, \"2021-04-03\": 4707488, \"2021-04-02\": 4708648, \"2021-04-01\": 4661841, \"2021-03-31\": 4611392, \"2021-03-30\": 4553481, \"2021-03-29\": 4522779, \"2021-03-28\": 4513685, \"2021-03-27\": 4477503, \"2021-03-26\": 4434884, \"2021-03-25\": 4393255, \"2021-03-24\": 4347614, \"2021-03-23\": 4283389, \"2021-03-22\": 4268711, \"2021-03-21\": 4252919, \"2021-03-20\": 4222935, \"2021-03-19\": 4187630, \"2021-03-18\": 4152604, \"2021-03-17\": 4117650, \"2021-03-16\": 4080012, \"2021-03-15\": 4050010, \"2021-03-14\": 4043769, \"2021-03-13\": 4017844, \"2021-03-12\": 3988194, \"2021-03-11\": 3963156, \"2021-03-10\": 3935990, \"2021-03-09\": 3906363, \"2021-03-08\": 3883334, \"2021-03-07\": 3878007, \"2021-03-06\": 3856182, \"2021-03-05\": 3833572, \"2021-03-04\": 3810605, \"2021-03-03\": 3785326, \"2021-03-02\": 3759247, \"2021-03-01\": 3736390, \"2021-02-28\": 3732426, \"2021-02-27\": 3712474, \"2021-02-26\": 3689034, \"2021-02-25\": 3664050, \"2021-02-24\": 3639501, \"2021-02-23\": 3608271, \"2021-02-22\": 3588972, \"2021-02-21\": 3584326, \"2021-02-20\": 3562707, \"2021-02-19\": 3541282, \"2021-02-18\": 3517177, \"2021-02-17\": 3495775, \"2021-02-16\": 3471268, \"2021-02-15\": 3451894, \"2021-02-14\": 3447518, \"2021-02-13\": 3431805, \"2021-02-12\": 3410996, \"2021-02-11\": 3390952, \"2021-02-10\": 3370645, \"2021-02-09\": 3345558, \"2021-02-08\": 3327305, \"2021-02-07\": 3322988, \"2021-02-06\": 3303273, \"2021-02-05\": 3284085, \"2021-02-04\": 3238798, \"2021-02-03\": 3239256, \"2021-02-02\": 3213360, \"2021-02-01\": 3190701, \"2021-01-31\": 3186354, \"2021-01-30\": 3167274, \"2021-01-29\": 3143417, \"2021-01-28\": 3120634, \"2021-01-27\": 3097374, \"2021-01-26\": 3070458, \"2021-01-25\": 3048598, \"2021-01-24\": 3044358, \"2021-01-23\": 3026320, \"2021-01-22\": 3002910, \"2021-01-21\": 2979915, \"2021-01-20\": 2957547, \"2021-01-19\": 2931050, \"2021-01-18\": 2907711, \"2021-01-17\": 2903975, \"2021-01-16\": 2887532, \"2021-01-15\": 2866413, \"2021-01-14\": 2845701, \"2021-01-13\": 2824473, \"2021-01-12\": 2800770, \"2021-01-11\": 2781503, \"2021-01-10\": 2777921, \"2021-01-09\": 2762120, \"2021-01-08\": 2742349, \"2021-01-07\": 2722728, \"2021-01-06\": 2701215, \"2021-01-05\": 2676195, \"2021-01-04\": 2655915, \"2021-01-03\": 2651893, \"2021-01-02\": 2639404, \"2021-01-01\": 2636045, \"2020-12-31\": 2616902, \"2020-12-30\": 2597124, \"2020-12-29\": 2570767, \"2020-12-28\": 2559575, \"2020-12-27\": 2556621, \"2020-12-26\": 2548388, \"2020-12-25\": 2545358, \"2020-12-24\": 2525234, \"2020-12-23\": 2503693, \"2020-12-22\": 2488889, \"2020-12-21\": 2477255, \"2020-12-20\": 2471458, \"2020-12-19\": 2458839, \"2020-12-18\": 2441737, \"2020-12-17\": 2426063, \"2020-12-16\": 2407983, \"2020-12-15\": 2390419, \"2020-12-14\": 2379291, \"2020-12-13\": 2376228, \"2020-12-12\": 2364740, \"2020-12-11\": 2350923, \"2020-12-10\": 2337594, \"2020-12-09\": 2324300, \"2020-12-08\": 2309760, \"2020-12-07\": 2296249, \"2020-12-06\": 2292838, \"2020-12-05\": 2281889, \"2020-12-04\": 2269080, \"2020-12-03\": 2257894, \"2020-12-02\": 2245324, \"2020-12-01\": 2231344, \"2020-11-30\": 2223393, \"2020-11-29\": 2219502, \"2020-11-28\": 2209739, \"2020-11-27\": 2197283, \"2020-11-26\": 2184846, \"2020-11-25\": 2171967, \"2020-11-24\": 2155722, \"2020-11-23\": 2146869, \"2020-11-22\": 2142417, \"2020-11-21\": 2129287, \"2020-11-20\": 2111646, \"2020-11-19\": 2088784, \"2020-11-18\": 2068297, \"2020-11-17\": 2039938, \"2020-11-16\": 1994728, \"2020-11-15\": 1985427, \"2020-11-14\": 1958235, \"2020-11-13\": 1926314, \"2020-11-12\": 1902644, \"2020-11-11\": 1870380, \"2020-11-10\": 1834515, \"2020-11-09\": 1812919, \"2020-11-08\": 1792764, \"2020-11-07\": 1754202, \"2020-11-06\": 1667604, \"2020-11-05\": 1607147, \"2020-11-04\": 1549838, \"2020-11-03\": 1597139, \"2020-11-02\": 1525184, \"2020-11-01\": 1420870, \"2020-10-31\": 1374691, \"2020-10-30\": 1339640, \"2020-10-29\": 1290446, \"2020-10-28\": 1243855, \"2020-10-27\": 1207430, \"2020-10-26\": 1174279, \"2020-10-25\": 1102262, \"2020-10-24\": 1095708, \"2020-10-23\": 1050468, \"2020-10-22\": 1008483, \"2020-10-21\": 967717, \"2020-10-20\": 941085, \"2020-10-19\": 920916, \"2020-10-18\": 907677, \"2020-10-17\": 877840, \"2020-10-16\": 845740, \"2020-10-15\": 820685, \"2020-10-14\": 790959, \"2020-10-13\": 768380, \"2020-10-12\": 755597, \"2020-10-11\": 747092, \"2020-10-10\": 731155, \"2020-10-09\": 704478, \"2020-10-08\": 684164, \"2020-10-07\": 666849, \"2020-10-06\": 648116, \"2020-10-05\": 637918, \"2020-10-04\": 632834, \"2020-10-03\": 620330, \"2020-10-02\": 603920, \"2020-10-01\": 591783, \"2020-09-30\": 578769, \"2020-09-29\": 566196, \"2020-09-28\": 558374, \"2020-09-27\": 554304, \"2020-09-26\": 543688, \"2020-09-25\": 529501, \"2020-09-24\": 513732, \"2020-09-23\": 497914, \"2020-09-22\": 485929, \"2020-09-21\": 476162, \"2020-09-20\": 470864, \"2020-09-19\": 460328, \"2020-09-18\": 446961, \"2020-09-17\": 433871, \"2020-09-16\": 423924, \"2020-09-15\": 414196, \"2020-09-14\": 406609, \"2020-09-13\": 400533, \"2020-09-12\": 394270, \"2020-09-11\": 383854, \"2020-09-10\": 374586, \"2020-09-09\": 365025, \"2020-09-08\": 357473, \"2020-09-07\": 351017, \"2020-09-06\": 346921, \"2020-09-05\": 340013, \"2020-09-04\": 331578, \"2020-09-03\": 322935, \"2020-09-02\": 316047, \"2020-09-01\": 309247, \"2020-08-31\": 304349, \"2020-08-30\": 301554, \"2020-08-29\": 296265, \"2020-08-28\": 290929, \"2020-08-27\": 283698, \"2020-08-26\": 272643, \"2020-08-25\": 272643, \"2020-08-24\": 269452, \"2020-08-23\": 267589, \"2020-08-22\": 263129, \"2020-08-21\": 259717, \"2020-08-20\": 255214, \"2020-08-19\": 250539, \"2020-08-18\": 246821, \"2020-08-17\": 244761, \"2020-08-16\": 244335, \"2020-08-15\": 241371, \"2020-08-14\": 238240, \"2020-08-13\": 235540, \"2020-08-12\": 232966, \"2020-08-11\": 230540, \"2020-08-10\": 229203, \"2020-08-09\": 224602, \"2020-08-08\": 224602, \"2020-08-07\": 224602, \"2020-08-06\": 222398, \"2020-08-05\": 220874, \"2020-08-04\": 219253, \"2020-08-03\": 218351, \"2020-08-02\": 215135, \"2020-08-01\": 215135, \"2020-07-31\": 215135, \"2020-07-30\": 213927, \"2020-07-29\": 212676, \"2020-07-28\": 211337, \"2020-07-27\": 210821, \"2020-07-26\": 208553, \"2020-07-25\": 208553, \"2020-07-24\": 208553, \"2020-07-23\": 207650, \"2020-07-22\": 206641, \"2020-07-21\": 205773, \"2020-07-20\": 205308, \"2020-07-19\": 203516, \"2020-07-18\": 203516, \"2020-07-17\": 203516, \"2020-07-16\": 202819, \"2020-07-15\": 202383, \"2020-07-14\": 201755, \"2020-07-13\": 201755, \"2020-07-12\": 200569, \"2020-07-11\": 200569, \"2020-07-10\": 200569, \"2020-07-09\": 200027, \"2020-07-08\": 199699, \"2020-07-07\": 199170, \"2020-07-06\": 198850, \"2020-07-05\": 197994, \"2020-07-04\": 197994, \"2020-07-03\": 197994, \"2020-07-02\": 197591, \"2020-07-01\": 197238, \"2020-06-30\": 196554, \"2020-06-29\": 196390, \"2020-06-28\": 196111, \"2020-06-27\": 196521, \"2020-06-26\": 196267, \"2020-06-25\": 195256, \"2020-06-24\": 195256, \"2020-06-23\": 195256, \"2020-06-22\": 194693, \"2020-06-21\": 194599, \"2020-06-20\": 194535, \"2020-06-19\": 194121, \"2020-06-18\": 193576, \"2020-06-17\": 193256, \"2020-06-16\": 192915, \"2020-06-15\": 192369, \"2020-06-14\": 192317, \"2020-06-13\": 192043, \"2020-06-12\": 191679, \"2020-06-11\": 191040, \"2020-06-10\": 190743, \"2020-06-09\": 190266, \"2020-06-08\": 189782, \"2020-06-07\": 189621, \"2020-06-06\": 189330, \"2020-06-05\": 188808, \"2020-06-04\": 188286, \"2020-06-03\": 184015, \"2020-06-02\": 187265, \"2020-06-01\": 187986, \"2020-05-31\": 187817, \"2020-05-30\": 187608, \"2020-05-29\": 185842, \"2020-05-28\": 185300, \"2020-05-27\": 182018, \"2020-05-26\": 181880, \"2020-05-25\": 181684, \"2020-05-24\": 181410, \"2020-05-23\": 181969, \"2020-05-22\": 181444, \"2020-05-21\": 180752, \"2020-05-20\": 180499, \"2020-05-19\": 180143, \"2020-05-18\": 179650, \"2020-05-17\": 179250, \"2020-05-16\": 179140, \"2020-05-15\": 178837, \"2020-05-14\": 178353, \"2020-05-13\": 177790, \"2020-05-12\": 177330, \"2020-05-11\": 176651, \"2020-05-10\": 176338, \"2020-05-09\": 176132, \"2020-05-08\": 175743, \"2020-05-07\": 175218, \"2020-05-06\": 174596, \"2020-05-05\": 170467, \"2020-05-04\": 169405, \"2020-05-03\": 168887, \"2020-05-02\": 168637, \"2020-05-01\": 167846, \"2020-04-30\": 167326, \"2020-04-29\": 166193, \"2020-04-28\": 167650, \"2020-04-27\": 164592, \"2020-04-26\": 160907, \"2020-04-25\": 160295, \"2020-04-24\": 158653, \"2020-04-23\": 155881, \"2020-04-22\": 157603, \"2020-04-21\": 155821, \"2020-04-20\": 153197, \"2020-04-19\": 150752, \"2020-04-18\": 149683, \"2020-04-17\": 145896, \"2020-04-16\": 144035, \"2020-04-15\": 131476, \"2020-04-14\": 128272, \"2020-04-13\": 110836, \"2020-04-12\": 107712, \"2020-04-11\": 56972, \"2020-04-10\": 55538, \"2020-04-09\": 54003, \"2020-04-08\": 50242, \"2020-04-07\": 46400, \"2020-04-06\": 49934, \"2020-04-05\": 47299, \"2020-04-04\": 46483, \"2020-04-03\": 63588, \"2020-04-02\": 58404, \"2020-04-01\": 56362, \"2020-03-31\": 51579, \"2020-03-30\": 44029, \"2020-03-29\": 39761, \"2020-03-28\": 37162, \"2020-03-27\": 32609, \"2020-03-26\": 28856, \"2020-03-25\": 24967, \"2020-03-24\": 22054, \"2020-03-23\": 19856, \"2020-03-22\": 16533, \"2020-03-21\": 14282, \"2020-03-20\": 12612, \"2020-03-19\": 10871, \"2020-03-18\": 9043, \"2020-03-17\": 7652, \"2020-03-16\": 6633, \"2020-03-15\": 4499, \"2020-03-14\": 4469, \"2020-03-13\": 3661, \"2020-03-12\": 2281, \"2020-03-11\": 2281, \"2020-03-10\": 1773, \"2020-03-09\": 1401, \"2020-03-08\": 1115, \"2020-03-07\": 938, \"2020-03-06\": 613, \"2020-03-05\": 423, \"2020-03-04\": 285, \"2020-03-03\": 212, \"2020-03-02\": 191, \"2020-03-01\": 130, \"2020-02-29\": 100, \"2020-02-28\": 57, \"2020-02-27\": 38, \"2020-02-26\": 18, \"2020-02-25\": 14, \"2020-02-24\": 12, \"2020-02-23\": 12, \"2020-02-22\": 12, \"2020-02-21\": 12, \"2020-02-20\": 12, \"2020-02-19\": 12, \"2020-02-18\": 12, \"2020-02-17\": 12, \"2020-02-16\": 12, \"2020-02-15\": 12, \"2020-02-14\": 11, \"2020-02-13\": 11, \"2020-02-12\": 11, \"2020-02-11\": 11, \"2020-02-10\": 11, \"2020-02-09\": 11, \"2020-02-08\": 11, \"2020-02-07\": 6, \"2020-02-06\": 6, \"2020-02-05\": 6, \"2020-02-04\": 6, \"2020-02-03\": 6, \"2020-02-02\": 6, \"2020-02-01\": 6, \"2020-01-31\": 5, \"2020-01-30\": 5, \"2020-01-29\": 5, \"2020-01-28\": 4, \"2020-01-27\": 3, \"2020-01-26\": 3, \"2020-01-25\": 3, \"2020-01-24\": 2, \"2020-01-23\": 0, \"2020-01-22\": 0}}}";

void removeCacheEntry(struct Country* entryPointer)
{
	listIndexPointer = listStartPointer;

	while (listIndexPointer != NULL)
	{
		if (listIndexPointer->nextCountry == entryPointer)
		{
			listIndexPointer->nextCountry = entryPointer->nextCountry;

			free(entryPointer);

			return;
		}

		listIndexPointer = listIndexPointer->nextCountry;
	}
}

// Averages the values of the last thirty days in a "dates" JSON object provided by the API
float avgLastThirty(cJSON* datesObject)
{
	cJSON* list;
	int index, total;

	if (datesObject == NULL)
	{
		return 0.0F;
	}

	// The child pointer of a JSON object provides a linked list of its contained values
	list = datesObject->child;

	// Parse through 30 entries of the list, which is already in order from the current date going backwards (thanks API),
	// then get the change in cases from the previous day to the current day for each entry, adding them up to the total value
	for (index = 0; index < 30; index++)
	{
		total += (list->valueint - list->next->valueint);
		list = list->next;
	}

	// Finally return the average total case increase per day by dividing by the number of increases parsed (30)
	return total / 30.0F;
}

struct CaseData* parseCasesJSON(char casesJSON[], char casesHistoryJSON[])
{
	// Parse JSON
	cJSON* casesJsonObject = cJSON_Parse(casesJSON);
	cJSON* historyJsonObject = cJSON_Parse(casesHistoryJSON);

	if (casesJsonObject == NULL || historyJsonObject == NULL)
	{
		printf("Cases or case history JSON response from API was invalid!\n");

		return NULL;
	}

	// Attempt to fetch main JSON objects
	cJSON* casesAllObject = cJSON_GetObjectItem(casesJsonObject, "All");
	cJSON* historyAllObject = cJSON_GetObjectItem(historyJsonObject, "All");

	if (casesAllObject != NULL && historyAllObject != NULL)
	{
		// Allocate memory for a new data structure to store the data
		struct CaseData* caseData = (struct CaseData*) malloc(sizeof(struct CaseData));

		// Fetch JSON objects of relevant data
		cJSON* confirmed = cJSON_GetObjectItem(casesAllObject, "confirmed");
		cJSON* population = cJSON_GetObjectItem(casesAllObject, "population");
		cJSON* area = cJSON_GetObjectItem(casesAllObject, "sq_km_area");
		cJSON* dates = cJSON_GetObjectItem(historyAllObject, "dates");

		// Pre-calculate statistics and add them to the data structure
		caseData->confirmedCases = confirmed->valueint;
		caseData->percentInfected = confirmed->valuedouble / population->valuedouble;
		caseData->peoplePerKilo = population->valuedouble / area->valuedouble;
		caseData->casesPerKilo = confirmed->valuedouble / area->valuedouble;
		caseData->avgNewCases = avgLastThirty(dates);

		return caseData;
	}

	return NULL;
}

struct VaccineData* parseVaccineJSON(char vaccineJSON[])
{
	// Parse JSON
	cJSON* vaccinesJsonObject = cJSON_Parse(vaccineJSON);

	if (vaccinesJsonObject == NULL)
	{
		printf("Vaccine JSON response from API was invalid!\n");

		return NULL;
	}

	// Attempt to fetch main JSON object
	cJSON* vaccinesAllObject = cJSON_GetObjectItem(vaccinesJsonObject, "All");

	if (vaccinesAllObject != NULL)
	{
		// Allocate memory for a new data structure to store the data
		struct VaccineData* vaccineData = (struct VaccineData*) malloc(sizeof(struct VaccineData));

		// Fetch JSON objects of relevant data
		cJSON* vaccinated = cJSON_GetObjectItem(vaccinesAllObject, "people_vaccinated");
		cJSON* partially_vaccinated = cJSON_GetObjectItem(vaccinesAllObject, "people_partially_vaccinated");
		cJSON* population = cJSON_GetObjectItem(vaccinesAllObject, "population");
		
		// Pre-calculate statistics and add them to the data structure
		vaccineData->totalVaccinated = vaccinated->valueint;
		vaccineData->totalPartiallyVaccinated = partially_vaccinated->valueint;
		vaccineData->percentVaccinated = vaccinated->valuedouble / population->valuedouble;
		vaccineData->herdImmunity = (population->valueint * 2) - (vaccinated->valueint * 2) - partially_vaccinated->valueint;

		return vaccineData;
	}

	return NULL;
}

struct RecoveryData* parseRecoveryJSON(char recoveredHistoryJSON[], struct VaccineData* vaccineData)
{
	// Parse JSON
	cJSON* historyJsonObject = cJSON_Parse(recoveredHistoryJSON);

	if (historyJsonObject == NULL || vaccineData == NULL)
	{
		printf("Recovery history JSON response from API or provided vaccine data was invalid!\n");

		return NULL;
	}

	// Attempt to fetch main JSON object
	cJSON* historyAllObject = cJSON_GetObjectItem(historyJsonObject, "All");

	if (historyAllObject != NULL)
	{
		// Allocate memory for a new data structure to store the data
		struct RecoveryData* recoveryData = (struct RecoveryData*) malloc(sizeof(struct RecoveryData));

		// Fetch JSON objects of relevant data
		cJSON* dates = cJSON_GetObjectItem(historyAllObject, "dates");
		cJSON* population = cJSON_GetObjectItem(historyAllObject, "population");

		// Pre-calculate statistics and add them to the data structure
		recoveryData->avgRecoveries = avgLastThirty(dates);
		recoveryData->percentImmune = (((double) vaccineData->totalVaccinated) + dates->child->valuedouble) / population->valuedouble;

		return recoveryData;
	}

	return NULL;
}

struct DeathData* parseDeathJSON(char deathsHistoryJSON[])
{
	// Parse JSON
	cJSON* historyJsonObject = cJSON_Parse(deathsHistoryJSON);

	if (historyJsonObject == NULL)
	{
		printf("Death history JSON response from API was invalid!\n");

		return NULL;
	}

	// Attempt to fetch main JSON object
	cJSON* historyAllObject = cJSON_GetObjectItem(historyJsonObject, "All");

	if (historyAllObject != NULL)
	{
		// Allocate memory for a new data structure to store the data
		struct DeathData* deathData = (struct DeathData*) malloc(sizeof(struct DeathData));

		// Fetch JSON objects of relevant data
		cJSON* dates = cJSON_GetObjectItem(historyAllObject, "dates");

		// Pre-calculate statistics and add them to the data structure
		deathData->totalDeaths = dates->child->valueint;
		deathData->avgDeaths = avgLastThirty(dates);

		return deathData;
	}

	return NULL;
}

struct Country* fetchCountryData(char countryName[])
{
	// Check if the country data is in the cache before attempting to use the API
	if (listStartPointer != NULL)
	{
		listIndexPointer = listStartPointer;

		// Traverse the cache, if the country name matches and the entry isn't expired, use the cached country
		while (listIndexPointer != NULL)
		{
			if (strcmp(countryName, listIndexPointer->countryName) != 0)
			{
				// Cache entries expire after 15 minutes (900 seconds), skip to fetching from API if it's expired
				if (time(NULL) - listIndexPointer->timeAdded >= 900)
				{
					removeCacheEntry(listIndexPointer);

					goto fetch;
				}

				return listIndexPointer;
			}

			listIndexPointer = listIndexPointer->nextCountry;
		}
	}

fetch:

}

int main()
{
	parseCasesJSON(sampleCasesResponse, sampleHistoryResponse);

	return 0;
}
