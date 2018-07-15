#include "libMutex.h"
#include "libThermistor.h"

SemaphoreHandle_t LibThermistor::s_mutex;
bool LibThermistor::s_isInitialized;
struct LibThermistor::Conversion* LibThermistor::s_types[] = {
    [USP12387]   = s_convTableUSP12387,
    [SC30F103AN] = s_convTableSC30F103AN,
};
struct LibThermistor::Conversion LibThermistor::s_convTableUSP12387[] = {
    { .rt = 336479.00, .temp = {  -40, -40.0 } },
    { .rt = 314904.00, .temp = {  -39, -38.2 } },
    { .rt = 294848.00, .temp = {  -38, -36.4 } },
    { .rt = 276194.00, .temp = {  -37, -34.6 } },
    { .rt = 258838.00, .temp = {  -36, -32.8 } },
    { .rt = 242681.00, .temp = {  -35, -31.0 } },
    { .rt = 227632.00, .temp = {  -34, -29.2 } },
    { .rt = 213610.00, .temp = {  -33, -27.4 } },
    { .rt = 200539.00, .temp = {  -32, -25.6 } },
    { .rt = 188349.00, .temp = {  -31, -23.8 } },
    { .rt = 176974.00, .temp = {  -30, -22.0 } },
    { .rt = 166356.00, .temp = {  -29, -20.2 } },
    { .rt = 156441.00, .temp = {  -28, -18.4 } },
    { .rt = 147177.00, .temp = {  -27, -16.6 } },
    { .rt = 138518.00, .temp = {  -26, -14.8 } },
    { .rt = 130421.00, .temp = {  -25, -13.0 } },
    { .rt = 122847.00, .temp = {  -24, -11.2 } },
    { .rt = 115759.00, .temp = {  -23,  -9.4 } },
    { .rt = 109122.00, .temp = {  -22,  -7.6 } },
    { .rt = 102906.00, .temp = {  -21,  -5.8 } },
    { .rt =  97081.00, .temp = {  -20,  -4.0 } },
    { .rt =  91621.00, .temp = {  -19,  -2.2 } },
    { .rt =  86501.00, .temp = {  -18,  -0.4 } },
    { .rt =  81698.00, .temp = {  -17,   1.4 } },
    { .rt =  77190.00, .temp = {  -16,   3.2 } },
    { .rt =  72957.00, .temp = {  -15,   5.0 } },
    { .rt =  68982.00, .temp = {  -14,   6.8 } },
    { .rt =  65246.00, .temp = {  -13,   8.6 } },
    { .rt =  61736.00, .temp = {  -12,  10.4 } },
    { .rt =  58434.00, .temp = {  -11,  12.2 } },
    { .rt =  55329.00, .temp = {  -10,  14.0 } },
    { .rt =  52407.00, .temp = {   -9,  15.8 } },
    { .rt =  49656.00, .temp = {   -8,  17.6 } },
    { .rt =  47066.00, .temp = {   -7,  19.4 } },
    { .rt =  44626.00, .temp = {   -6,  21.2 } },
    { .rt =  42327.00, .temp = {   -5,  23.0 } },
    { .rt =  40159.00, .temp = {   -4,  24.8 } },
    { .rt =  38115.00, .temp = {   -3,  26.6 } },
    { .rt =  36187.00, .temp = {   -2,  28.4 } },
    { .rt =  34368.00, .temp = {   -1,  30.2 } },
    { .rt =  32650.00, .temp = {    0,  32.0 } },
    { .rt =  31029.00, .temp = {    1,  33.8 } },
    { .rt =  29498.00, .temp = {    2,  35.6 } },
    { .rt =  28052.00, .temp = {    3,  37.4 } },
    { .rt =  26685.00, .temp = {    4,  39.2 } },
    { .rt =  25392.00, .temp = {    5,  41.0 } },
    { .rt =  24170.00, .temp = {    6,  42.8 } },
    { .rt =  23013.00, .temp = {    7,  44.6 } },
    { .rt =  21918.00, .temp = {    8,  46.4 } },
    { .rt =  20882.00, .temp = {    9,  48.2 } },
    { .rt =  19901.00, .temp = {   10,  50.0 } },
    { .rt =  18971.00, .temp = {   11,  51.8 } },
    { .rt =  18090.00, .temp = {   12,  53.6 } },
    { .rt =  17255.00, .temp = {   13,  55.4 } },
    { .rt =  16463.00, .temp = {   14,  57.2 } },
    { .rt =  15712.00, .temp = {   15,  59.0 } },
    { .rt =  14999.00, .temp = {   16,  60.8 } },
    { .rt =  14323.00, .temp = {   17,  62.6 } },
    { .rt =  13681.00, .temp = {   18,  64.4 } },
    { .rt =  13072.00, .temp = {   19,  66.2 } },
    { .rt =  12493.00, .temp = {   20,  68.0 } },
    { .rt =  11942.00, .temp = {   21,  69.8 } },
    { .rt =  11419.00, .temp = {   22,  71.6 } },
    { .rt =  10922.00, .temp = {   23,  73.4 } },
    { .rt =  10450.00, .temp = {   24,  75.2 } },
    { .rt =  10000.00, .temp = {   25,  77.0 } },
    { .rt =   9572.00, .temp = {   26,  78.8 } },
    { .rt =   9165.00, .temp = {   27,  80.6 } },
    { .rt =   8777.00, .temp = {   28,  82.4 } },
    { .rt =   8408.00, .temp = {   29,  84.2 } },
    { .rt =   8057.00, .temp = {   30,  86.0 } },
    { .rt =   7722.00, .temp = {   31,  87.8 } },
    { .rt =   7402.00, .temp = {   32,  89.6 } },
    { .rt =   7098.00, .temp = {   33,  91.4 } },
    { .rt =   6808.00, .temp = {   34,  93.2 } },
    { .rt =   6531.00, .temp = {   35,  95.0 } },
    { .rt =   6267.00, .temp = {   36,  96.8 } },
    { .rt =   6015.00, .temp = {   37,  98.6 } },
    { .rt =   5775.00, .temp = {   38, 100.4 } },
    { .rt =   5545.00, .temp = {   39, 102.2 } },
    { .rt =   5326.00, .temp = {   40, 104.0 } },
    { .rt =   5117.00, .temp = {   41, 105.8 } },
    { .rt =   4917.00, .temp = {   42, 107.6 } },
    { .rt =   4725.00, .temp = {   43, 109.4 } },
    { .rt =   4543.00, .temp = {   44, 111.2 } },
    { .rt =   4368.00, .temp = {   45, 113.0 } },
    { .rt =   4201.00, .temp = {   46, 114.8 } },
    { .rt =   4041.00, .temp = {   47, 116.6 } },
    { .rt =   3888.00, .temp = {   48, 118.4 } },
    { .rt =   3742.00, .temp = {   49, 120.2 } },
    { .rt =   3602.00, .temp = {   50, 122.0 } },
    { .rt =   3468.00, .temp = {   51, 123.8 } },
    { .rt =   3340.00, .temp = {   52, 125.6 } },
    { .rt =   3217.00, .temp = {   53, 127.4 } },
    { .rt =   3099.00, .temp = {   54, 129.2 } },
    { .rt =   2986.00, .temp = {   55, 131.0 } },
    { .rt =   2878.00, .temp = {   56, 132.8 } },
    { .rt =   2774.00, .temp = {   57, 134.6 } },
    { .rt =   2675.00, .temp = {   58, 136.4 } },
    { .rt =   2579.00, .temp = {   59, 138.2 } },
    { .rt =   2488.00, .temp = {   60, 140.0 } },
    { .rt =   2400.00, .temp = {   61, 141.8 } },
    { .rt =   2316.00, .temp = {   62, 143.6 } },
    { .rt =   2235.00, .temp = {   63, 145.4 } },
    { .rt =   2157.00, .temp = {   64, 147.2 } },
    { .rt =   2083.00, .temp = {   65, 149.0 } },
    { .rt =   2011.00, .temp = {   66, 150.8 } },
    { .rt =   1942.00, .temp = {   67, 152.6 } },
    { .rt =   1876.00, .temp = {   68, 154.4 } },
    { .rt =   1813.00, .temp = {   69, 156.2 } },
    { .rt =   1752.00, .temp = {   70, 158.0 } },
    { .rt =   1693.00, .temp = {   71, 159.8 } },
    { .rt =   1637.00, .temp = {   72, 161.6 } },
    { .rt =   1582.00, .temp = {   73, 163.4 } },
    { .rt =   1530.00, .temp = {   74, 165.2 } },
    { .rt =   1480.00, .temp = {   75, 167.0 } },
    { .rt =   1432.00, .temp = {   76, 168.8 } },
    { .rt =   1385.00, .temp = {   77, 170.6 } },
    { .rt =   1340.00, .temp = {   78, 172.4 } },
    { .rt =   1297.00, .temp = {   79, 174.2 } },
    { .rt =   1255.00, .temp = {   80, 176.0 } },
    { .rt =   1215.00, .temp = {   81, 177.8 } },
    { .rt =   1177.00, .temp = {   82, 179.6 } },
    { .rt =   1140.00, .temp = {   83, 181.4 } },
    { .rt =   1104.00, .temp = {   84, 183.2 } },
    { .rt =   1070.00, .temp = {   85, 185.0 } },
    { .rt =   1037.00, .temp = {   86, 186.8 } },
    { .rt =   1005.00, .temp = {   87, 188.6 } },
    { .rt =    973.80, .temp = {   88, 190.4 } },
    { .rt =    944.10, .temp = {   89, 192.2 } },
    { .rt =    915.50, .temp = {   90, 194.0 } },
    { .rt =    887.80, .temp = {   91, 195.8 } },
    { .rt =    861.20, .temp = {   92, 197.6 } },
    { .rt =    835.40, .temp = {   93, 199.4 } },
    { .rt =    810.60, .temp = {   94, 201.2 } },
    { .rt =    786.60, .temp = {   95, 203.0 } },
    { .rt =    763.50, .temp = {   96, 204.8 } },
    { .rt =    741.20, .temp = {   97, 206.6 } },
    { .rt =    719.60, .temp = {   98, 208.4 } },
    { .rt =    698.70, .temp = {   99, 210.2 } },
    { .rt =    678.60, .temp = {  100, 212.0 } },
    { .rt =    659.10, .temp = {  101, 213.8 } },
    { .rt =    640.30, .temp = {  102, 215.6 } },
    { .rt =    622.20, .temp = {  103, 217.4 } },
    { .rt =    604.60, .temp = {  104, 219.2 } },
    { .rt =    587.60, .temp = {  105, 221.0 } },
    { .rt =    571.20, .temp = {  106, 222.8 } },
    { .rt =    555.30, .temp = {  107, 224.6 } },
    { .rt =    539.90, .temp = {  108, 226.4 } },
    { .rt =    525.00, .temp = {  109, 228.2 } },
    { .rt =    510.60, .temp = {  110, 230.0 } },
    { .rt =    496.70, .temp = {  111, 231.8 } },
    { .rt =    483.20, .temp = {  112, 233.6 } },
    { .rt =    470.10, .temp = {  113, 235.4 } },
    { .rt =    457.50, .temp = {  114, 237.2 } },
    { .rt =    445.30, .temp = {  115, 239.0 } },
    { .rt =    433.40, .temp = {  116, 240.8 } },
    { .rt =    421.90, .temp = {  117, 242.6 } },
    { .rt =    410.80, .temp = {  118, 244.4 } },
    { .rt =    400.00, .temp = {  119, 246.2 } },
    { .rt =    389.60, .temp = {  120, 248.0 } },
    { .rt =    379.40, .temp = {  121, 249.8 } },
    { .rt =    369.60, .temp = {  122, 251.6 } },
    { .rt =    360.10, .temp = {  123, 253.4 } },
    { .rt =    350.90, .temp = {  124, 255.2 } },
    { .rt =    341.90, .temp = {  125, 257.0 } },
    { .rt =    0,                              },
};
struct LibThermistor::Conversion LibThermistor::s_convTableSC30F103AN[] = {
    { .rt =    342270.39, .temp = {  -40,  -40.0 } },
    { .rt =    320139.28, .temp = {  -39,  -38.2 } },
    { .rt =    299579.82, .temp = {  -38,  -36.4 } },
    { .rt =    280471.15, .temp = {  -37,  -34.6 } },
    { .rt =    262702.36, .temp = {  -36,  -32.8 } },
    { .rt =    246171.67, .temp = {  -35,  -31.0 } },
    { .rt =    230785.54, .temp = {  -34,  -29.2 } },
    { .rt =    216458.05, .temp = {  -33,  -27.4 } },
    { .rt =    203110.18, .temp = {  -32,  -25.6 } },
    { .rt =    190669.24, .temp = {  -31,  -23.8 } },
    { .rt =    179068.35, .temp = {  -30,  -22.0 } },
    { .rt =    168245.89, .temp = {  -29,  -20.2 } },
    { .rt =    158145.14, .temp = {  -28,  -18.4 } },
    { .rt =    148713.79, .temp = {  -27,  -16.6 } },
    { .rt =    139903.61, .temp = {  -26,  -14.8 } },
    { .rt =    131670.12, .temp = {  -25,  -13.0 } },
    { .rt =    123972.24, .temp = {  -24,  -11.2 } },
    { .rt =    116772.06, .temp = {  -23,   -9.4 } },
    { .rt =    110034.56, .temp = {  -22,   -7.6 } },
    { .rt =    103727.35, .temp = {  -21,   -5.8 } },
    { .rt =     97820.49, .temp = {  -20,   -4.0 } },
    { .rt =     92286.31, .temp = {  -19,   -2.2 } },
    { .rt =     87099.15, .temp = {  -18,   -0.4 } },
    { .rt =     82235.31, .temp = {  -17,    1.4 } },
    { .rt =     77672.78, .temp = {  -16,    3.2 } },
    { .rt =     73391.20, .temp = {  -15,    5.0 } },
    { .rt =     69371.70, .temp = {  -14,    6.8 } },
    { .rt =     65596.74, .temp = {  -13,    8.6 } },
    { .rt =     62050.08, .temp = {  -12,   10.4 } },
    { .rt =     58716.64, .temp = {  -11,   12.2 } },
    { .rt =     55582.39, .temp = {  -10,   14.0 } },
    { .rt =     52634.34, .temp = {  -9,    15.8 } },
    { .rt =     49860.37, .temp = {  -8,    17.6 } },
    { .rt =     47249.24, .temp = {  -7,    19.4 } },
    { .rt =     44790.49, .temp = {  -6,    21.2 } },
    { .rt =     42474.38, .temp = {  -5,    23.0 } },
    { .rt =     40291.84, .temp = {  -4,    24.8 } },
    { .rt =     38234.45, .temp = {  -3,    26.6 } },
    { .rt =     36294.33, .temp = {  -2,    28.4 } },
    { .rt =     34464.16, .temp = {  -1,    30.2 } },
    { .rt =     32737.09, .temp = {   0,    32.0 } },
    { .rt =     31106.77, .temp = {   1,    33.8 } },
    { .rt =     29567.21, .temp = {   2,    35.6 } },
    { .rt =     28112.91, .temp = {   3,    37.4 } },
    { .rt =     26738.67, .temp = {   4,    39.2 } },
    { .rt =     25439.66, .temp = {   5,    41.0 } },
    { .rt =     24211.34, .temp = {   6,    42.8 } },
    { .rt =     23049.49, .temp = {   7,    44.6 } },
    { .rt =     21950.16, .temp = {   8,    46.4 } },
    { .rt =     20909.64, .temp = {   9,    48.2 } },
    { .rt =     19924.47, .temp = {  10,    50.0 } },
    { .rt =     18991.41, .temp = {  11,    51.8 } },
    { .rt =     18107.44, .temp = {  12,    53.6 } },
    { .rt =     17269.69, .temp = {  13,    55.4 } },
    { .rt =     16475.51, .temp = {  14,    57.2 } },
    { .rt =     15722.41, .temp = {  15,    59.0 } },
    { .rt =     15008.04, .temp = {  16,    60.8 } },
    { .rt =     14330.20, .temp = {  17,    62.6 } },
    { .rt =     13686.84, .temp = {  18,    64.4 } },
    { .rt =     13076.02, .temp = {  19,    66.2 } },
    { .rt =     12495.92, .temp = {  20,    68.0 } },
    { .rt =     11944.83, .temp = {  21,    69.8 } },
    { .rt =     11421.17, .temp = {  22,    71.6 } },
    { .rt =     10923.40, .temp = {  23,    73.4 } },
    { .rt =     10450.13, .temp = {  24,    75.2 } },
    { .rt =     10000.02, .temp = {  25,    77.0 } },
    { .rt =      9571.82, .temp = {  26,    78.8 } },
    { .rt =      9164.34, .temp = {  27,    80.6 } },
    { .rt =      8776.48, .temp = {  28,    82.4 } },
    { .rt =      8407.19, .temp = {  29,    84.2 } },
    { .rt =      8055.49, .temp = {  30,    86.0 } },
    { .rt =      7720.44, .temp = {  31,    87.8 } },
    { .rt =      7401.19, .temp = {  32,    89.6 } },
    { .rt =      7096.89, .temp = {  33,    91.4 } },
    { .rt =      6806.78, .temp = {  34,    93.2 } },
    { .rt =      6530.12, .temp = {  35,    95.0 } },
    { .rt =      6266.22, .temp = {  36,    96.8 } },
    { .rt =      6014.43, .temp = {  37,    98.6 } },
    { .rt =      5774.12, .temp = {  38,   100.4 } },
    { .rt =      5544.73, .temp = {  39,   102.2 } },
    { .rt =      5325.69, .temp = {  40,   104.0 } },
    { .rt =      5116.49, .temp = {  41,   105.8 } },
    { .rt =      4916.64, .temp = {  42,   107.6 } },
    { .rt =      4725.68, .temp = {  43,   109.4 } },
    { .rt =      4543.15, .temp = {  44,   111.2 } },
    { .rt =      4368.66, .temp = {  45,   113.0 } },
    { .rt =      4201.81, .temp = {  46,   114.8 } },
    { .rt =      4042.22, .temp = {  47,   116.6 } },
    { .rt =      3889.54, .temp = {  48,   118.4 } },
    { .rt =      3743.44, .temp = {  49,   120.2 } },
    { .rt =      3603.60, .temp = {  50,   122.0 } },
    { .rt =      3469.72, .temp = {  51,   123.8 } },
    { .rt =      3341.53, .temp = {  52,   125.6 } },
    { .rt =      3218.75, .temp = {  53,   127.4 } },
    { .rt =      3101.13, .temp = {  54,   129.2 } },
    { .rt =      2988.43, .temp = {  55,   131.0 } },
    { .rt =      2880.41, .temp = {  56,   132.8 } },
    { .rt =      2776.87, .temp = {  57,   134.6 } },
    { .rt =      2677.58, .temp = {  58,   136.4 } },
    { .rt =      2582.37, .temp = {  59,   138.2 } },
    { .rt =      2491.04, .temp = {  60,   140.0 } },
    { .rt =      2403.41, .temp = {  61,   141.8 } },
    { .rt =      2319.32, .temp = {  62,   143.6 } },
    { .rt =      2238.60, .temp = {  63,   145.4 } },
    { .rt =      2161.11, .temp = {  64,   147.2 } },
    { .rt =      2086.71, .temp = {  65,   149.0 } },
    { .rt =      2015.24, .temp = {  66,   150.8 } },
    { .rt =      1946.59, .temp = {  67,   152.6 } },
    { .rt =      1880.63, .temp = {  68,   154.4 } },
    { .rt =      1817.24, .temp = {  69,   156.2 } },
    { .rt =      1756.31, .temp = {  70,   158.0 } },
    { .rt =      1697.73, .temp = {  71,   159.8 } },
    { .rt =      1641.41, .temp = {  72,   161.6 } },
    { .rt =      1587.23, .temp = {  73,   163.4 } },
    { .rt =      1535.12, .temp = {  74,   165.2 } },
    { .rt =      1484.98, .temp = {  75,   167.0 } },
    { .rt =      1436.73, .temp = {  76,   168.8 } },
    { .rt =      1390.29, .temp = {  77,   170.6 } },
    { .rt =      1345.59, .temp = {  78,   172.4 } },
    { .rt =      1302.54, .temp = {  79,   174.2 } },
    { .rt =      1261.08, .temp = {  80,   176.0 } },
    { .rt =      1221.16, .temp = {  81,   177.8 } },
    { .rt =      1182.69, .temp = {  82,   179.6 } },
    { .rt =      1145.62, .temp = {  83,   181.4 } },
    { .rt =      1109.90, .temp = {  84,   183.2 } },
    { .rt =      1075.47, .temp = {  85,   185.0 } },
    { .rt =      1042.28, .temp = {  86,   186.8 } },
    { .rt =      1010.27, .temp = {  87,   188.6 } },
    { .rt =       979.41, .temp = {  88,   190.4 } },
    { .rt =       949.63, .temp = {  89,   192.2 } },
    { .rt =       920.91, .temp = {  90,   194.0 } },
    { .rt =       893.20, .temp = {  91,   195.8 } },
    { .rt =       866.45, .temp = {  92,   197.6 } },
    { .rt =       840.63, .temp = {  93,   199.4 } },
    { .rt =       815.71, .temp = {  94,   201.2 } },
    { .rt =       791.65, .temp = {  95,   203.0 } },
    { .rt =       768.41, .temp = {  96,   204.8 } },
    { .rt =       745.97, .temp = {  97,   206.6 } },
    { .rt =       724.29, .temp = {  98,   208.4 } },
    { .rt =       703.34, .temp = {  99,   210.2 } },
    { .rt =       683.10, .temp = { 100,   212.0 } },
    { .rt =       663.50, .temp = { 101,   213.8 } },
    { .rt =       644.57, .temp = { 102,   215.6 } },
    { .rt =       626.26, .temp = { 103,   217.4 } },
    { .rt =       608.57, .temp = { 104,   219.2 } },
    { .rt =       591.46, .temp = { 105,   221.0 } },
    { .rt =       574.92, .temp = { 106,   222.8 } },
    { .rt =       558.92, .temp = { 107,   224.6 } },
    { .rt =       543.44, .temp = { 108,   226.4 } },
    { .rt =       528.47, .temp = { 109,   228.2 } },
    { .rt =       513.98, .temp = { 110,   230.0 } },
    { .rt =       499.96, .temp = { 111,   231.8 } },
    { .rt =       486.39, .temp = { 112,   233.6 } },
    { .rt =       473.25, .temp = { 113,   235.4 } },
    { .rt =       460.53, .temp = { 114,   237.2 } },
    { .rt =       448.21, .temp = { 115,   239.0 } },
    { .rt =       436.27, .temp = { 116,   240.8 } },
    { .rt =       424.71, .temp = { 117,   242.6 } },
    { .rt =       413.51, .temp = { 118,   244.4 } },
    { .rt =       402.66, .temp = { 119,   246.2 } },
    { .rt =       392.15, .temp = { 120,   248.0 } },
    { .rt =       381.95, .temp = { 121,   249.8 } },
    { .rt =       372.07, .temp = { 122,   251.6 } },
    { .rt =       362.49, .temp = { 123,   253.4 } },
    { .rt =       353.20, .temp = { 124,   255.2 } },
    { .rt =       344.19, .temp = { 125,   257.0 } },
    { .rt =         0,                             },
};

LibThermistor::LibThermistor() :
    m_units(CELSIUS),
    m_type(SC30F103AN)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibThermistor::~LibThermistor()
{
}

// TEMP_AINx = 2.048 * Rt/(107000 + Rt)
// 1/TEMP_AINx = (107000 / Rt + 1) / 2.048
// 1 / Rt = (2.048 /TEMP_AINx - 1) / 107000
float LibThermistor::convertVoltageToTemp(float ain, int standard)
{
    float rt = 107000 / (2.048 / ain - 1);
    int i;
    float temp;
    struct Conversion* convTable = s_types[m_type];
    if (rt > convTable[0].rt) {
        return convTable[0].temp[standard];
    }
    for (i = 0; convTable[i].rt; i++) {
        if (convTable[i].rt >= rt && rt >= convTable[i + 1].rt) {
            if (convTable[i + 1].rt == 0) {
                temp = convTable[i].temp[standard];
                break;
            }
            else {
                temp = convTable[i + 1].temp[standard]
                  + (rt - convTable[i + 1].rt)
                  * (convTable[i].temp[standard] - convTable[i + 1].temp[standard])
                  / (convTable[i].rt - convTable[i + 1].rt);
                 break;
            }
        }
    }
    if (!convTable[i].rt) {
        return convTable[i].temp[standard];
    }
    return temp;
}

int LibThermistor::readTemp(int channel, float& value)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    int adcChannel;
    switch (channel) {
    case AIN_A:
        adcChannel = TEMP_AIN_A;
        break;
    case AIN_B:
        adcChannel = TEMP_AIN_B;
        break;
    case AIN_C:
        adcChannel = TEMP_AIN_C;
        break;
    case AIN_D:
        adcChannel = TEMP_AIN_D;
        break;
    default:
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    float ain;
    result = m_libAdc.read(adcChannel, ain);
    if (result != LibAdc::OKAY) {
        switch (channel) {
        case AIN_A:
            result = ERROR_READ_TEMP_AIN_A;
            break;
        case AIN_B:
            result = ERROR_READ_TEMP_AIN_B;
            break;
        case AIN_C:
            result = ERROR_READ_TEMP_AIN_C;
            break;
        case AIN_D:
            result = ERROR_READ_TEMP_AIN_D;
            break;
        }
        return result;
    }
    value = convertVoltageToTemp(ain, m_units);
    return OKAY;
}

int LibThermistor::readTemp(int channel, uint32& value)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    int adcChannel;
    switch (channel) {
    case AIN_A:
        adcChannel = TEMP_AIN_A;
        break;
    case AIN_B:
        adcChannel = TEMP_AIN_B;
        break;
    case AIN_C:
        adcChannel = TEMP_AIN_C;
        break;
    case AIN_D:
        adcChannel = TEMP_AIN_D;
        break;
    default:
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    uint32 ain;
    result = m_libAdc.read(adcChannel, ain);
    if (result != LibAdc::OKAY) {
        switch (channel) {
        case AIN_A:
            result = ERROR_READ_TEMP_AIN_A;
            break;
        case AIN_B:
            result = ERROR_READ_TEMP_AIN_B;
            break;
        case AIN_C:
            result = ERROR_READ_TEMP_AIN_C;
            break;
        case AIN_D:
            result = ERROR_READ_TEMP_AIN_D;
            break;
        }
        return result;
    }
    value = ain;
    return OKAY;
}

int LibThermistor::setType(int type)
{
    switch (type) {
    default:
        return ERROR_INVALID_TYPE;
    case USP12387:
    case SC30F103AN:
        m_type = type;
        break;
    }
    return OKAY;
}

int LibThermistor::getType()
{
    return m_type;
}

int LibThermistor::setUnits(int units)
{
    switch (units) {
    default:
        return ERROR_INVALID_UNITS;
    case CELSIUS:
    case FAHRENHEIT:
        m_units = units;
        break;
    }
    return OKAY;
}

int LibThermistor::getUnits()
{
    return m_units;
}
