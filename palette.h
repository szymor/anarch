/*
  @file palette.h

  General purpose HSV-based 256 color palette.

  by Miloslav Ciz (drummyfish), 2019

  Released under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
  plus a waiver of all other intellectual property. The goal of this work is
  be and remain completely in the public domain forever, available for any use
  whatsoever.
*/

#ifndef PALETTE_256_H
#define PALETTE_256_H

SFG_PROGRAM_MEMORY uint16_t paletteRGB565[256] = {
#if 1
// manually adjusted, more saturated palette
0, 6371, 14855, 27436, 38066, 48631, 59228, 65535, 6241, 14563, 24966, 33320, 
43755, 52142, 62577, 64885, 6337, 16772, 25190, 35689, 44139, 52559, 63058, 
65333, 6402, 14851, 23334, 31816, 40268, 50830, 59314, 65526, 4354, 10755, 
15174, 21576, 30027, 36462, 42929, 51190, 2306, 8709, 13096, 19532, 25935, 
30323, 36790, 47098, 4356, 8711, 15115, 19536, 27956, 32377, 38846, 47103, 
4227, 8519, 12812, 17136, 25588, 31961, 38334, 46751, 2115, 8424, 14732, 21040, 
27380, 33721, 40030, 48511, 6244, 12520, 22924, 31280, 39669, 48089, 56445, 
64927, 8290, 16614, 24969, 33325, 43761, 52148, 62585, 64892, 10240, 18464, 
26657, 38946, 47202, 55524, 63781, 64074, 10400, 18753, 27170, 37601, 48034, 
56421, 64837, 65002, 6496, 14944, 23425, 31937, 40418, 48869, 57317, 61418, 
352, 4704, 7041, 7362, 15842, 20196, 24550, 30697, 354, 611, 2949, 5288, 7658, 
12013, 8175, 20467, 357, 617, 910, 5298, 9686, 7931, 14335, 24575, 69, 233, 
4461, 4594, 8918, 2907, 13311, 19679, 4133, 2089, 4173, 8306, 10455, 16667, 
20863, 27263, 6149, 14377, 22574, 28819, 39063, 45371, 53631, 57983, 10242, 
18469, 26664, 38987, 47247, 55537, 63797, 64119, 10272, 18432, 26624, 34848, 
45056, 53312, 61504, 63520, 10336, 18624, 26976, 35296, 45728, 54048, 62400, 
64609, 8544, 16992, 25440, 31872, 42400, 50880, 59328, 65504, 4448, 2656, 
7008, 11392, 13728, 14016, 20416, 26593, 2400, 608, 864, 3200, 5504, 1698, 
1985, 2019, 353, 614, 872, 1163, 1422, 1713, 2005, 2039, 197, 361, 557, 753, 
950, 5273, 1406, 9759, 5, 9, 2093, 81, 4214, 186, 2270, 351, 2052, 4105, 4109, 
8209, 8278, 12314, 16414, 18527, 10245, 14378, 22541, 30738, 38934, 47130, 55326, 
61471, 10241, 18435, 26630, 34824, 45066, 53293, 61519, 63601
#else 
// original palette
0, 8484, 19017, 27501, 38034, 46518, 57051, 65535, 8354, 16709, 25096, 33450,
41805, 50192, 58546, 64853, 8386, 16805, 25224, 33642, 42061, 50480, 58898,
65269, 6402, 14853, 23304, 29706, 38157, 46608, 55058, 61429, 4354, 10757,
17160, 23562, 29965, 36368, 42770, 49141, 4355, 10758, 17161, 21516, 27920,
34323, 38678, 45049, 4323, 10759, 17163, 21519, 27923, 34327, 38683, 45055,
4292, 10632, 17004, 21296, 27668, 34008, 38300, 44671, 4260, 10568, 16908,
23216, 29524, 35864, 42172, 48479, 6308, 14664, 23052, 29360, 37716, 46104,
54460, 60767, 8355, 16710, 25098, 33453, 41809, 50196, 58552, 64859, 8257,
16546, 24836, 33093, 41382, 49672, 57929, 64170, 8353, 16738, 25124, 33509,
41894, 50248, 58633, 64970, 6401, 12802, 21252, 27653, 36102, 42504, 50953,
57322, 2305, 6658, 11012, 15365, 19718, 24072, 28425, 32746, 2306, 4612, 8967,
11273, 13580, 17934, 20240, 22515, 2307, 4615, 8971, 11279, 13587, 17943, 20251,
22527, 2180, 4392, 8652, 10864, 13076, 17304, 19516, 21727, 2116, 6312, 10508,
14672, 18868, 23064, 25180, 29375, 6212, 12456, 20748, 26960, 35252, 41496,
49756, 55999, 8258, 16549, 24840, 33099, 41390, 49681, 57940, 64183, 8192,
16384, 24576, 32768, 40960, 49152, 57344, 63488, 8320, 16640, 24960, 33312,
41632, 49952, 58304, 64576, 6400, 14848, 23296, 29696, 38144, 46592, 52992,
61408, 2304, 6656, 8960, 13312, 15616, 19968, 22272, 26592, 256, 513, 769, 1026,
1283, 1539, 1796, 2021, 258, 517, 776, 1035, 1294, 1552, 1811, 2038, 164, 360,
556, 752, 948, 1144, 1308, 1503, 36, 104, 140, 208, 244, 312, 348, 415, 2052,
4104, 8204, 10256, 14356, 16408, 18460, 22559, 6148, 14344, 20492, 28688, 34836,
43032, 51228, 57375, 8194, 16388, 24582, 32777, 40971, 49165, 57359, 63505
#endif
};

/** Adds value (brightness), possibly negative, to given color (represented by
  its palette index). If you know you'll only be either adding or subtracting,
  use plusValue() or minusValue() functions, which should be faster. */
static inline uint8_t palette_addValue(uint8_t color, int8_t add)
{
  uint8_t newValue = color + add;
  
  if ((newValue >> 3) == (color >> 3))
    return newValue;
  else
    return add > 0 ? (color | 0x07) : 0;
}

/** Adds a positive value (brightness) to given color (represented by its
  palette index). This should be a little bit faster than addValue(). */
static inline uint8_t palette_plusValue(uint8_t color, uint8_t plus)
{
  uint8_t newValue = color + plus;
  return ((newValue >> 3) == (color >> 3)) ? newValue : (color | 0x07);
}

/** Subtracts a positive value (brightness) from given color (represented by
  its palette index). This should be a little bit faster than addValue(). */
static inline uint8_t palette_minusValue(uint8_t color, uint8_t minus)
{
  uint8_t newValue = color - minus;
  return ((newValue >> 3) == (color >> 3)) ? newValue : 0;
}

#endif //guard
