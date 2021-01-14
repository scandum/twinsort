Intro
-----

This document describes a stable bottom-up adaptive merge sort named tailsort.

The primary goal is to provide a simplified alternative to quadsort.

https://github.com/scandum/quadsort

Twin swap
---------
The twin swap is a pre-sorting routine which turns the array into sorted blocks
of 2 elements. The twin swap routine also contains a reverse order run
detector, so 6 5 4 3 2 1 is sorted into 1 2 3 4 5 6 rather than 5 6 3 4 1 2.

Tail sort
---------
The tail sort is a bottom-up merge sort which uses at most n / 2 swap memory.
It merges by copying the right block to swap memory and merging the tails
of each block. The tail merge routine also contains a forward run detector.

Performance
-----------
Based on limited benchmarks tailsort performs better than Timsort while the
algorithm is about 150 lines of sparse C code.

Overall it's slower than quadsort, most notably on small arrays.

Data types
----------
Tailsort supports 32 and 64 bit data types and pointers. The function interface
is identical to qsort.

Benchmarks
----------

Various distributions on 1 million items. It reports the best run as well as the
average for 100 runs. The qsort implementation in this benchmark is a stable merge
sort.

|      Name |    Items | Type |     Best |  Average | Comparisons |     Distribution |
| --------- | -------- | ---- | -------- | -------- | ----------- | ---------------- |
|  tailsort |  1000000 |  i32 | 0.096996 | 0.097749 |  19661346.0 |     random order |
|     qsort |  1000000 |  i32 | 0.102335 | 0.103779 |  18674640.0 |     random order |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.002453 | 0.002514 |    999999.0 |        ascending |
|     qsort |  1000000 |  i32 | 0.026731 | 0.027275 |   9884992.0 |        ascending |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.014439 | 0.014533 |   4315090.0 |    ascending saw |
|     qsort |  1000000 |  i32 | 0.034847 | 0.035383 |  10884989.0 |    ascending saw |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.064900 | 0.065580 |  19602170.0 |    generic order |
|     qsort |  1000000 |  i32 | 0.070316 | 0.071467 |  18617804.0 |    generic order |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.002406 | 0.002426 |    999999.0 | descending order |
|     qsort |  1000000 |  i32 | 0.026055 | 0.026408 |  10066432.0 | descending order |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.019105 | 0.019433 |   9663460.0 |   descending saw |
|     qsort |  1000000 |  i32 | 0.034639 | 0.035033 |  13906008.0 |   descending saw |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.027756 | 0.027930 |   6955814.0 |      random tail |
|     qsort |  1000000 |  i32 | 0.046052 | 0.046743 |  12249920.0 |      random tail |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.052989 | 0.053432 |  11663871.0 |      random half |
|     qsort |  1000000 |  i32 | 0.066568 | 0.067532 |  14530032.0 |      random half |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.028641 | 0.029048 |  15932766.0 |       wave order |
|     qsort |  1000000 |  i32 | 0.035086 | 0.035905 |  14656080.0 |       wave order |
|           |          |      |          |          |             |                  |
|  tailsort |  1000000 |  i32 | 0.041549 | 0.041957 |  15932766.0 |           stable |
|     qsort |  1000000 |  i32 | 0.045836 | 0.046945 |  14656080.0 |           stable |

Various small to medium array sizes with random distributions.

|      Name |    Items | Type |     Best |  Average | Comparisons |     Distribution |
| --------- | -------- | ---- | -------- | -------- | ----------- | ---------------- |
|  tailsort |        4 |  i32 | 0.000123 | 0.000127 |         1.8 |       random 1-4 |
|     qsort |        4 |  i32 | 0.000207 | 0.000211 |         2.0 |       random 1-4 |
|           |          |      |          |          |             |                  |
|  tailsort |        8 |  i32 | 0.000456 | 0.000500 |        13.2 |       random 5-8 |
|     qsort |        8 |  i32 | 0.000583 | 0.000594 |        11.8 |       random 5-8 |
|           |          |      |          |          |             |                  |
|  tailsort |       15 |  i32 | 0.000996 | 0.001113 |        41.0 |      random 9-15 |
|     qsort |       15 |  i32 | 0.001206 | 0.001790 |        31.1 |      random 9-15 |
|           |          |      |          |          |             |                  |
|  tailsort |       63 |  i32 | 0.002166 | 0.002888 |       204.9 |     random 16-63 |
|     qsort |       63 |  i32 | 0.003336 | 0.003812 |       166.1 |     random 16-63 |
|           |          |      |          |          |             |                  |
|  tailsort |      127 |  i32 | 0.003194 | 0.005022 |       624.5 |    random 64-127 |
|     qsort |      127 |  i32 | 0.005843 | 0.006800 |       516.6 |    random 64-127 |
|           |          |      |          |          |             |                  |
|  tailsort |      255 |  i32 | 0.003613 | 0.005705 |      1428.7 |   random 128-255 |
|     qsort |      255 |  i32 | 0.006883 | 0.007746 |      1211.6 |   random 128-255 |
|           |          |      |          |          |             |                  |
|  tailsort |      511 |  i32 | 0.004633 | 0.005721 |      3244.7 |   random 256-511 |
|     qsort |      511 |  i32 | 0.008299 | 0.009183 |      2822.7 |   random 256-511 |
|           |          |      |          |          |             |                  |
|  tailsort |     1023 |  i32 | 0.005276 | 0.007279 |      5976.8 |  random 512-1023 |
|     qsort |     1023 |  i32 | 0.007721 | 0.008297 |      5175.6 |  random 512-1023 |
|           |          |      |          |          |             |                  |
|  tailsort |     2047 |  i32 | 0.005745 | 0.006515 |     11339.1 | random 1024-2047 |
|     qsort |     2047 |  i32 | 0.007272 | 0.007651 |      9619.9 | random 1024-2047 |
|           |          |      |          |          |             |                  |
|  tailsort |     4095 |  i32 | 0.006793 | 0.007030 |     23955.4 | random 2048-4095 |
|     qsort |     4095 |  i32 | 0.007672 | 0.007854 |     20300.7 | random 2048-4095 |

