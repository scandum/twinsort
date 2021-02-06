Intro
-----

This document describes a stable bottom-up adaptive merge sort named twinsort.

The primary goal is to provide a simplified alternative to [quadsort](https://github.com/scandum/quadsort).

The algorithm is about 150 lines of sparse C code that should be relatively easy to understand and port.

Twin swap
---------
The twin_swap function is a pre-sorting routine which turns the array into sorted blocks
of 2 elements. 8 9 4 1 2 3 6 0 5 7 becomes 8 9 1 4 2 3 0 6 5 7.

The twin swap routine also contains a reverse order run detector, so 6 5 4 3 2 1 is sorted
into 1 2 3 4 5 6 rather than 5 6 3 4 1 2.

The swap and the run detection are carried out simultaneously, subsequently the reverse
run detector has very little overhead.

Tail sort
---------
The tail_sort function is a bottom-up merge sort which uses at most n / 2 swap memory.

It merges by copying the right block to swap memory and merging the tails
of each block. The routine skips unnecessary merge operations and boundary checks.

The tail merge routine also contains a forward run detector with minimal overhead.

Performance
-----------
Overall twinsort is slightly faster than [Timsort](https://github.com/timsort/cpp-TimSort), most notably on random data.

Big O
-----

| Name     | Best | Average | Worst   | Memory | Stable | Partitioning  |
| -------- | ---- | ------- | ------- | ------ | -------| ------------- |
| twinsort |    n | n log n | n log n |      n |    yes |            no |

Twinsort makes n comparisons when the data is already sorted or reverse sorted.

Data types
----------
Twinsort only supports integers, this to keep things simple.

Visualization
-------------

In the visualization below eight sorts are performed on various distributions.
These distributions are: Random, Ascending, Ascending Saw, Generic, Descending, Descending Saw, Random Tail, and Wave order.

The upper half shows the swap memory and the bottom half shows the main memory.
Colors are used to differentiate between skip, swap, and merge operations.

[![tailsort visualization](https://github.com/scandum/twinsort/blob/main/tailsort.gif)](https://www.youtube.com/watch?v=vhg0K7WcB4I)

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
|  twinsort |   100000 |  i32 | 0.006265 | 0.006338 |           1 |     random order |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.000658 | 0.000735 |           1 |        ascending |
|  quadsort |   100000 |  i32 | 0.000057 | 0.000057 |           1 |        ascending |
|   timsort |   100000 |  i32 | 0.000045 | 0.000045 |           1 |        ascending |
|  twinsort |   100000 |  i32 | 0.000126 | 0.000130 |           1 |        ascending |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.001360 | 0.001467 |           1 |    ascending saw |
|  quadsort |   100000 |  i32 | 0.000852 | 0.000874 |           1 |    ascending saw |
|   timsort |   100000 |  i32 | 0.000851 | 0.000874 |           1 |    ascending saw |
|  twinsort |   100000 |  i32 | 0.000883 | 0.000909 |           1 |    ascending saw |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.003891 | 0.003971 |           1 |    generic order |
|  quadsort |   100000 |  i32 | 0.003763 | 0.003855 |           1 |    generic order |
|   timsort |   100000 |  i32 | 0.005463 | 0.005569 |           1 |    generic order |
|  twinsort |   100000 |  i32 | 0.004090 | 0.004154 |           1 |    generic order |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.000904 | 0.000941 |           1 | descending order |
|  quadsort |   100000 |  i32 | 0.000066 | 0.000067 |           1 | descending order |
|   timsort |   100000 |  i32 | 0.000101 | 0.000103 |           1 | descending order |
|  twinsort |   100000 |  i32 | 0.000073 | 0.000074 |           1 | descending order |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.001054 | 0.001095 |           1 |   descending saw |
|  quadsort |   100000 |  i32 | 0.000488 | 0.000513 |           1 |   descending saw |
|   timsort |   100000 |  i32 | 0.000464 | 0.000478 |           1 |   descending saw |
|  twinsort |   100000 |  i32 | 0.000415 | 0.000427 |           1 |   descending saw |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.002040 | 0.002139 |           1 |      random tail |
|  quadsort |   100000 |  i32 | 0.001557 | 0.001588 |           1 |      random tail |
|   timsort |   100000 |  i32 | 0.001979 | 0.002034 |           1 |      random tail |
|  twinsort |   100000 |  i32 | 0.001730 | 0.001764 |           1 |      random tail |
|           |          |      |          |          |             |                  |
|stablesort |   100000 |  i32 | 0.000978 | 0.001020 |           1 |       wave order |
|  quadsort |   100000 |  i32 | 0.000837 | 0.000910 |           1 |       wave order |
|   timsort |   100000 |  i32 | 0.000834 | 0.001081 |           1 |       wave order |
|  twinsort |   100000 |  i32 | 0.000947 | 0.000996 |           1 |       wave order |
