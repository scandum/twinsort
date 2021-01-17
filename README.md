Intro
-----

This document describes a stable bottom-up adaptive merge sort named tailsort.

The primary goal is to provide a simplified alternative to [quadsort](https://github.com/scandum/quadsort).

The algorithm is about 150 lines of sparse C code that should be relatively easy to understand.

Twin swap
---------
The twin swap is a pre-sorting routine which turns the array into sorted blocks
of 2 elements. The twin swap routine also contains a reverse order run
detector, so 6 5 4 3 2 1 is sorted into 1 2 3 4 5 6 rather than 5 6 3 4 1 2.

The swap and the run detection are carried out simultaneously, and the reverse
run detector has very little overhead.

Tail sort
---------
The tail sort is a bottom-up merge sort which uses at most n / 2 swap memory.

It merges by copying the right block to swap memory and merging the tails
of each block. The tail merge routine also contains a forward run detector
which has very little overhead.

The routine skips unnecessary merge operations and avoids wasteful boundary
checks.

Performance
-----------
Tailsort is slightly faster than [Timsort](https://github.com/timsort/cpp-TimSort), most notably on random data.

Big O
-----

| Name     | Best | Average | Worst   | Memory | Stable | Partitioning  |
| -------- | ---- | ------- | ------- | ------ | -------| ------------- |
| tailsort |    n | n log n | n log n |      n |    yes |            no |

Tailsort makes n comparisons when the data is already sorted or reverse sorted.

Data types
----------
Tailsort supports 32 and 64 bit data types and pointers. The function interface
is identical to qsort.

Visualization
-------------

In the visualization below eight sorts are performed on various distributions.
These distributions are: Random, Ascending, Ascending Saw, Generic, Descending, Descending Saw, Random Tail, and Wave order.

The upper half shows the swap memory and the bottom half shows the main memory.
Colors are used to differentiate between skip, swap, and merge operations.

[![tailsort visualization](https://github.com/scandum/tailsort/blob/master/tailsort.gif)](https://www.youtube.com/watch?v=vhg0K7WcB4I)

Benchmarks
----------

The following benchmark is based on the [wolfsort](https://github.com/scandum/wolfsort)
benchmark on WSL gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1).

The source code was compiled using g++ -O3 -w -fpermissive bench.c. Each test was ran 100 times
and both the best and average run time is reported. stablesort is c++'s std::stablesort.

|      Name |    Items | Type |     Best |  Average | Repetitions |     Distribution |
| --------- | -------- | ---- | -------- | -------- | ----------- | ---------------- |
|stablesort |   100000 |  i32 | 0.006084 | 0.006174 |           1 |     random order |
|  quadsort |   100000 |  i32 | 0.005838 | 0.005920 |           1 |     random order |
|   timsort |   100000 |  i32 | 0.007556 | 0.007677 |           1 |     random order |
|  tailsort |   100000 |  i32 | 0.006265 | 0.006338 |           1 |     random order |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.000658 | 0.000735 |           1 |        ascending |
|  quadsort |   100000 |  i32 | 0.000057 | 0.000057 |           1 |        ascending |
|   timsort |   100000 |  i32 | 0.000045 | 0.000045 |           1 |        ascending |
|  tailsort |   100000 |  i32 | 0.000126 | 0.000130 |           1 |        ascending |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.001360 | 0.001467 |           1 |    ascending saw |
|  quadsort |   100000 |  i32 | 0.000852 | 0.000874 |           1 |    ascending saw |
|   timsort |   100000 |  i32 | 0.000851 | 0.000874 |           1 |    ascending saw |
|  tailsort |   100000 |  i32 | 0.000883 | 0.000909 |           1 |    ascending saw |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.003891 | 0.003971 |           1 |    generic order |
|  quadsort |   100000 |  i32 | 0.003763 | 0.003855 |           1 |    generic order |
|   timsort |   100000 |  i32 | 0.005463 | 0.005569 |           1 |    generic order |
|  tailsort |   100000 |  i32 | 0.004090 | 0.004154 |           1 |    generic order |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.000904 | 0.000941 |           1 | descending order |
|  quadsort |   100000 |  i32 | 0.000066 | 0.000067 |           1 | descending order |
|   timsort |   100000 |  i32 | 0.000101 | 0.000103 |           1 | descending order |
|  tailsort |   100000 |  i32 | 0.000073 | 0.000074 |           1 | descending order |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.001054 | 0.001095 |           1 |   descending saw |
|  quadsort |   100000 |  i32 | 0.000488 | 0.000513 |           1 |   descending saw |
|   timsort |   100000 |  i32 | 0.000464 | 0.000478 |           1 |   descending saw |
|  tailsort |   100000 |  i32 | 0.000415 | 0.000427 |           1 |   descending saw |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.002040 | 0.002139 |           1 |      random tail |
|  quadsort |   100000 |  i32 | 0.001557 | 0.001588 |           1 |      random tail |
|   timsort |   100000 |  i32 | 0.001979 | 0.002034 |           1 |      random tail |
|  tailsort |   100000 |  i32 | 0.001730 | 0.001764 |           1 |      random tail |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.000978 | 0.001020 |           1 |       wave order |
|  quadsort |   100000 |  i32 | 0.000837 | 0.000910 |           1 |       wave order |
|   timsort |   100000 |  i32 | 0.000834 | 0.001081 |           1 |       wave order |
|  tailsort |   100000 |  i32 | 0.000947 | 0.000996 |           1 |       wave order |

Various small to medium array sizes with random distributions.

|      Name |    Items | Type |     Best |  Average | Repetitions |     Distribution |
| --------- | -------- | ---- | -------- | -------- | ----------- | ---------------- |
|stablesort |        4 |  i32 | 0.000246 | 0.000297 |        8192 |       random 1-4 |
|  quadsort |        4 |  i32 | 0.000081 | 0.000093 |        8192 |       random 1-4 |
|   timsort |        4 |  i32 | 0.000058 | 0.000068 |        8192 |       random 1-4 |
|  tailsort |        4 |  i32 | 0.000137 | 0.000173 |        8192 |       random 1-4 |
|           |          |      |          |          |             |                  |
|stablesort |        8 |  i32 | 0.000316 | 0.000376 |        8192 |       random 5-8 |
|  quadsort |        8 |  i32 | 0.000143 | 0.000182 |        8192 |       random 5-8 |
|   timsort |        8 |  i32 | 0.000184 | 0.000205 |        8192 |       random 5-8 |
|  tailsort |        8 |  i32 | 0.000312 | 0.000318 |        8192 |       random 5-8 |
|           |          |      |          |          |             |                  |
|stablesort |       15 |  i32 | 0.000488 | 0.000519 |        8192 |      random 9-15 |
|  quadsort |       15 |  i32 | 0.000256 | 0.000335 |        8192 |      random 9-15 |
|   timsort |       15 |  i32 | 0.000500 | 0.000562 |        8192 |      random 9-15 |
|  tailsort |       15 |  i32 | 0.000584 | 0.000654 |        8192 |      random 9-15 |
|           |          |      |          |          |             |                  |
|stablesort |       63 |  i32 | 0.000935 | 0.001601 |        4096 |     random 16-63 |
|  quadsort |       63 |  i32 | 0.000717 | 0.000882 |        4096 |     random 16-63 |
|   timsort |       63 |  i32 | 0.001284 | 0.001695 |        4096 |     random 16-63 |
|  tailsort |       63 |  i32 | 0.001162 | 0.001384 |        4096 |     random 16-63 |
|           |          |      |          |          |             |                  |
|stablesort |      127 |  i32 | 0.002242 | 0.003062 |        2048 |    random 64-127 |
|  quadsort |      127 |  i32 | 0.001118 | 0.001550 |        2048 |    random 64-127 |
|   timsort |      127 |  i32 | 0.002737 | 0.004202 |        2048 |    random 64-127 |
|  tailsort |      127 |  i32 | 0.001578 | 0.002111 |        2048 |    random 64-127 |
|           |          |      |          |          |             |                  |
|stablesort |      255 |  i32 | 0.003043 | 0.003894 |        1024 |   random 128-255 |
|  quadsort |      255 |  i32 | 0.001338 | 0.001646 |        1024 |   random 128-255 |
|   timsort |      255 |  i32 | 0.004213 | 0.005767 |        1024 |   random 128-255 |
|  tailsort |      255 |  i32 | 0.001860 | 0.002664 |        1024 |   random 128-255 |
|           |          |      |          |          |             |                  |
|stablesort |      511 |  i32 | 0.004004 | 0.005125 |         512 |   random 256-511 |
|  quadsort |      511 |  i32 | 0.001617 | 0.002182 |         512 |   random 256-511 |
|   timsort |      511 |  i32 | 0.005238 | 0.006810 |         512 |   random 256-511 |
|  tailsort |      511 |  i32 | 0.002102 | 0.003530 |         512 |   random 256-511 |
|           |          |      |          |          |             |                  |
|stablesort |     1023 |  i32 | 0.004048 | 0.004828 |         256 |  random 512-1023 |
|  quadsort |     1023 |  i32 | 0.001321 | 0.002199 |         256 |  random 512-1023 |
|   timsort |     1023 |  i32 | 0.004934 | 0.006115 |         256 |  random 512-1023 |
|  tailsort |     1023 |  i32 | 0.002515 | 0.004026 |         256 |  random 512-1023 |
|           |          |      |          |          |             |                  |
|stablesort |     2047 |  i32 | 0.004244 | 0.004706 |         128 | random 1024-2047 |
|  quadsort |     2047 |  i32 | 0.002125 | 0.003452 |         128 | random 1024-2047 |
|   timsort |     2047 |  i32 | 0.005315 | 0.006145 |         128 | random 1024-2047 |
|  tailsort |     2047 |  i32 | 0.003483 | 0.004334 |         128 | random 1024-2047 |
|           |          |      |          |          |             |                  |
|stablesort |     4095 |  i32 | 0.004894 | 0.005130 |          64 | random 2048-4095 |
|  quadsort |     4095 |  i32 | 0.003934 | 0.004389 |          64 | random 2048-4095 |
|   timsort |     4095 |  i32 | 0.006599 | 0.006943 |          64 | random 2048-4095 |
|  tailsort |     4095 |  i32 | 0.004663 | 0.005272 |          64 | random 2048-4095 |
