
Put your answers to non-coding questions in this file as described in
the assignment description.

All answers are to be in plain text ASCII.

/////////////////////////////////
Predicting Cache Miss Rates

- sizeof(long) = 8
- The cache contains 64 blocks; each block is 32 bytes long.
- Arrays are stored in row-major order.
- The only memory accesses to be considered are to the entries of the array a[].

64 blocks = 2^6 ==> therefore you need 6 bits for index
32 bytes = 2^5 ==> need 5 bits for offset
Whatever's left is the tag
address in binary bytes

e.g. a[2][0] = 128 * 2 * 8 = 2048
		columns * row * long size

After you take the index and offset, then tag is determined for the address of direct-mapping.

a) An array of long with 4 rows and 128 columns, a direct-mapped cache, and the function sumA from the program cache-test provided in the first part of the assignment.

The function sumA uses row-major order to access the elements in the array, and does so sequentially (from first byte to last byte). There are 32 bytes in each block, which means that there is room for 4 longs in each block. There are cache misses every 4 bytes because they all map to different cache lines, but miss because the cache is empty (or corresponds to the last block) for the first of 4 accesses. The following 3 accesses can access the data from the cache. Therefore there is 1 miss / 4 accesses.

================
Miss rate = 0.25
================

b) An array of long with 4 rows and 128 columns, a direct-mapped cache, and the function sumB from the program cache-test provided in the first part of the assignment.

The function sumB uses column-major order to access the elements in the array, and it is stored in row-major order. Each row is separated by 1024 bytes (8 * 128). Since this is a direct-mapped cache, when a[2][0] is accessed, the cache block containing a[0][0] is replaced. When a[0][1] is finally accessed (since it is column-major order), the cached block that is relevant will have already been replaced. Thus the access pattern results in misses for every pair of accesses, whether or not the cache is empty.

===============
Miss rate = 1.0
===============

c) An array of long with 4 rows and 128 columns, a direct-mapped cache, and the function sumC from the program cache-test provided in the first part of the assignment.

The function sumC is also in column-major order but it jumps 2 rows and 2 columns for each iteration. This access pattern results in misses for each iteration, but since each iteration accesses 4 elements in the cache at one time, the miss rate is decreased. The 4 elements can be divided into 2 pairs: one with the same row and one with the same column. i.e. a[i][j], a[i][j+1], a[i+1][j], a[i+1][j+1]. Each cache line stores 4 longs, so the second access of the pairs can correctly access the data from the cache (a hit!), that was obtained from the first miss accesses. i.e. 2 misses / 4 accesses.

===============
Miss rate = 0.5
===============

d) An array of long with 4 rows and 128 columns, a two-way set-associative cache with LRU replacement, and the function sumB from the program cache-test provided in the first part of the assignment.

The function sumB uses column-major order to access the elements in the array, and this implementation has 2-way set-associativity. There are 64 cache blocks, so 32 sets. 128 columns is divisible by 32, indicating that elements in the same row will belong to the same set. Since this utilizes LRU (least recently used), when we reach a[0][1], we miss because the array is accessed in a column-major order, and the set would only have the lines for a[3][0] and a[4][0]. The relevant cache data would have already been replaced. This results in a miss every time.

===============
Miss rate = 1.0
===============

e) An array of long with 4 rows and 128 columns, a four-way set-associative cache with LRU replacement, and the function sumB from the program cache-test provided in the first part of the assignment.

The function sumB uses column-major order to access the elements in the array, and this implementation has 4-way set-associativity. Increasing associativity reduces miss rate (by reducing chances of thrashing) on average but increases hit time and miss penalty. With 64 cache blocks, there are 16 sets, with 4 cache lines each. 128 columns is divisible by 16, indicating that elements in the same row belong to the same set. When the second column is reached (i.e. a[0][1], a[1][1], a[2][1]), these would all be hits, since LRU does not replace anything until all 4 lines of a set have been filled. Thus, when a[0][1] is accessed, the LRU is still a[0][0], and the tag and index are the same. 1 miss / 4 accesses.

================
Miss rate = 0.25
================

f) An array of long with 4 rows and 128 columns, a four-way set-associative cache with random replacement, and the function sumB from the program cache-test provided in the first part of the assignment.

Similar to e) which demonstrates a 0.25 miss rate for the above reasons, f) exhibits the same performance, at least for the first half of the array. Once the first half of the array is reached however, the cache will already be filled up, so it is necessary to start replacing data in the cache. Since this replacement is done randomly, the best-case would be the least recently used (LRU), thus identical to e), with an overall 0.25 miss rate. The worst-case however would be that it replaces the data that we are actually going to access next, every single time. This it would be a 100% miss rate for the second half of the array. In this circumstance, the miss rate overall for the worst case would be: (25+100)/2 = 62.5%

==================
Miss rate:
Best-case = 0.25
Worst-case = 0.625
Range: 0.25-0.625
==================

g) An array of long with 4 rows and 120 columns, a direct-mapped cache, and the function sumB from the program cache-test provided in the first part of the assignment.

The function sumB uses column-major order to access the elements in the array, with 120 columns overall. The access pattern jumps around the data 960 bytes (120 * 8 ==> # cols * size of long) apart, back and forth. 960 is not a multiple of 128, so these jumps will not necessarily update the cache each time. This results in some misses when jumping back. Hence, g) is similar to a), but instead involves the sequential access mapping to non-sequential cache lines. Jumping forward then back will not remove the old cache line, and the forward jump maps to a different cache line. Since each cache line can store 4 longs, this means 1 miss for every 4 accesses.

=================
Miss rate = 0.25
=================

/////////////////////////////////
Improving a Program's Performance

life0:	original
life1:	given function
life2:	IMPROVEMENT 1
		- change initialization to row-major order (newWorld[i][j])
		- better spatial locality if the arrays are accessed in the same order that they are created
		- in this context, the access pattern is sequential
life3:	IMPROVEMENT 2
		- change array accesses to row-major order also, and adjust the for-loops accordingly
		- also changing the computing functions (e.g. checkHealth) to row-major order
life4:	IMPROVEMENT 3
		- combine all accesses that are in the same row to reduce the number of for-loops
		- better locality
		- i.e. all top left, above, and right
			   immediate left, right
			   bottom left, below, right
life5:	IMPROVEMENT 4
		- store elements that have been read for the next iteration, in order to improve temporal locality
		- less reads, and more saving into cache
life6:	IMPROVEMENT 5
		- no need for clear function if each entry in the array will be changed within the for loops anyways... initialize there
		- reduces by 1 for loop
life7:	IMPROVEMENT 6
		- nest all inner loops into one for loop (rows)
life8:	IMPROVEMENT 7
		- add "register" prefix to reduce memory footprint, and also "unsigned"
		IMPROVEMENT 8:
		- calculate all surrounding cells together using local variables colL, colR, rowTop, rowBt,
		IMPROVEMENT 9:
		- put checkHealth and alive into inner loop
		IMPROVEMENT 10:
		- input newly calculated newWorld directly into checkHealth, and alive statement
		- decreases the amount of times newWorld needs to be accessed

WANT A RATIO OF AT MOST 0.0500

	  	|	measured  						|	relative
	  	|	execution 						|	performance
	  	|	time	  						|	to original
===========================================================
life0	| The base implementation took:		|
		|	Best   :      2325752 usec		|	original
		|	Average:      2440478 usec		|
		| The optimized implementation took:|
		|	Best   :      2329496 usec		|
		|	Average:      2485904 usec		|
===========================================================
life1	| The base implementation took:		|
		|	Best   :      2329567 usec		|
		|	Average:      2425193 usec		|
		| The optimized implementation took:|
		|	Best   :      2448814 usec		|
		|	Average:      2515772 usec		|	1.01
===========================================================
life2	| The base implementation took:		|
		|	Best   :      2260593 usec		|
		|	Average:      2306127 usec		|
		| The optimized implementation took:|
		|	Best   :      2231583 usec		|
		|	Average:      2279658 usec		|	0.99
===========================================================
life3	| The base implementation took:		|
		|	Best   :      2249495 usec		|
		|	Average:      2386553 usec		|
		| The optimized implementation took:|
		|	Best   :      409539 usec		|
		|	Average:      418981 usec		|	0.18
===========================================================
life4	| The base implementation took:		|
		|	Best   :      2298592 usec		|
		|	Average:      2384334 usec		|
		| The optimized implementation took:|
		|	Best   :      324097 usec		|
		|	Average:      332323 usec		|	0.14
===========================================================
life5	| The base implementation took:		|
		|	Best   :      2334263 usec		|
		|	Average:      2422438 usec		|
		| The optimized implementation took:|
		|	Best   :      291103 usec		|
		|	Average:      301146 usec		|	0.12
===========================================================
life6	| The base implementation took:		|
		|	Best   :      2293450 usec		|
		|	Average:      2431931 usec		|
		| The optimized implementation took:|
		|	Best   :      270772 usec		|
		|	Average:      278901 usec		|	0.11
===========================================================
life7	| The base implementation took:		|
		|	Best   :      2308761 usec		|
		|	Average:      2359348 usec		|
		| The optimized implementation took:|
		|	Best   :      264580 usec		|
		|	Average:      271849 usec		|	0.11
===========================================================
life8	| The base implementation took:		|
		|	Best   :      2301303 usec		|
		|	Average:      2419781 usec		|
		| The optimized implementation took:|
		|	Best   :      146815 usec		|
		|	Average:      154627 usec		|	0.064 on my computer,
		|									|	may perform different on linux servers