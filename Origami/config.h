#pragma once


#define L2_BYTES MB(1)

#define REG_TYPE 3		// 0: scalar, 1,2,3: sse, avx2, avx512
#define KEY_TYPE 0		// 0: 32, 1: 64, 2: 64+64

/**
*	Required parameters -----------------------
	_P1_NREG			# number of registers in P1					--> number of registers available, typically 16 or 32
	_P1_N				# sort size in P1							--> switch point from IN_REGISTER_SORT to BMERGE, get from bench_sort_phases:phase2_switch_point_test()
	_P1_SWITCH			# point of switch from MCMERGE to MRMERGE	--> get from bench_sort_phases:phase1_matrix_merge_test()
	_P2_MERGE_UNROLL	# in cache merge unroll						--> get from bench_bmerge:bmerge_test()
	_P2_MERGE_NREG_1x	# number of registers in BMERGE no unroll	
	_P2_MERGE_NREG_2x	# number of registers in BMERGE 2X unroll
	_P2_MERGE_NREG_3x	# number of registers in BMERGE 3X unroll
	_P3_MERGE_UNROLL	# out of cache merge unroll
	_P3_MERGE_NREG_1x	# number of registers in BMERGE no unroll
	_P3_MERGE_NREG_2x	# number of registers in BMERGE 2X unroll
	_P3_MERGE_NREG_3x	# number of registers in BMERGE 3X unroll	--> get from bench_bmerge:bmerge_test()
	_MT_L1_BUFF_N		# MTREE internal buffer size at L1
	_MT_L2_BUFF_N		# MTREE internal buffer size at L2
	_MTREE_NREG			# number of registers in MTREE merge
	_MTREE_MIN_K		# minimum merge-way to avoid mem bottleneck	--> get from bench_mtree:
*/


// setting up parameters
#if REG_TYPE == 0
#if KEY_TYPE == 0
	using Itemtype = ui;
#define _P1_NREG			8
#define _P1_SWITCH			8
#define _P1_N				8
#define _P2_MERGE_UNROLL	1
#define _P2_MERGE_NREG_1x	4
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	1
#define _P3_MERGE_NREG_1x	4
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#elif KEY_TYPE == 1
	using Itemtype = i64;
#define _P1_NREG			8
#define _P1_SWITCH			8
#define _P1_N				8
#define _P2_MERGE_UNROLL	1
#define _P2_MERGE_NREG_1x	4
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	1
#define _P3_MERGE_NREG_1x	4
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#else 
	using Itemtype = KeyValue<i64, i64>;
#define _P1_NREG			16
#define _P1_SWITCH			16
#define _P1_N				16
#define _P2_MERGE_UNROLL	1
#define _P2_MERGE_NREG_1x	2
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	1
#define _P3_MERGE_NREG_1x	2
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1
#endif 
	using Regtype = Itemtype;


#elif REG_TYPE == 1
	using Regtype = sse;
#if KEY_TYPE == 0
	using Itemtype = ui;
#define _P1_NREG			32
#define _P1_SWITCH			32
#define _P1_N				32
#define _P2_MERGE_UNROLL	2
#define _P2_MERGE_NREG_1x	2
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	2
#define _P3_MERGE_NREG_2x	2
#define _P3_MERGE_NREG_3x	1

#elif KEY_TYPE == 1
	using Itemtype = i64;
#define _P1_NREG			32
#define _P1_SWITCH			32
#define _P1_N			32
#define _P2_MERGE_UNROLL	2
#define _P2_MERGE_NREG_1x	4
#define _P2_MERGE_NREG_2x	2
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	4
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#else 
	using Itemtype = KeyValue<i64, i64>;
#define _P1_NREG			32
#define _P1_SWITCH			16
#define _P1_N				16
#define _P2_MERGE_UNROLL	2
#define _P2_MERGE_NREG_1x	4
#define _P2_MERGE_NREG_2x	2
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	4
#define _P3_MERGE_NREG_2x	2
#define _P3_MERGE_NREG_3x	4
#endif 


#elif REG_TYPE == 2
	using Regtype = avx2;
#if KEY_TYPE == 0
	using Itemtype = ui;
#define _P1_NREG			32
#define _P1_SWITCH			64
#define _P1_N				256
#define _P2_MERGE_UNROLL	3
#define _P2_MERGE_NREG_1x	1
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	1
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#elif KEY_TYPE == 1
	using Itemtype = i64;
#define _P1_NREG			32
#define _P1_SWITCH			32
#define _P1_N				128
#define _P2_MERGE_UNROLL	3
#define _P2_MERGE_NREG_1x	1
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	1
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#else 
	using Itemtype = KeyValue<i64, i64>;
#define _P1_NREG			32
#define _P1_SWITCH			32
#define _P1_N				32
#define _P2_MERGE_UNROLL	3
#define _P2_MERGE_NREG_1x	1
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	4
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	1
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1
#endif 


#else 
	using Regtype = avx512;
#if KEY_TYPE == 0
	using Itemtype = ui;
#define _P1_NREG			64
#define _P1_SWITCH			64
#define _P1_N				1024
#define _P2_MERGE_UNROLL	3
#define _P2_MERGE_NREG_1x	1
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	1
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#elif KEY_TYPE == 1
	using Itemtype = i64;
#define _P1_NREG			64
#define _P1_SWITCH			64
#define _P1_N				512
#define _P2_MERGE_UNROLL	3
#define _P2_MERGE_NREG_1x	1
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	1
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1

#else 
	using Itemtype = KeyValue<i64, i64>;
#define _P1_NREG			128
#define _P1_SWITCH			64
#define _P1_N				512
#define _P2_MERGE_UNROLL	3
#define _P2_MERGE_NREG_1x	1
#define _P2_MERGE_NREG_2x	1
#define _P2_MERGE_NREG_3x	1
#define _P3_MERGE_UNROLL	2
#define _P3_MERGE_NREG_1x	1
#define _P3_MERGE_NREG_2x	1
#define _P3_MERGE_NREG_3x	1
#endif 
#endif 
#define _P2_N				((L2_BYTES >> 1) / sizeof(Itemtype))




// merge-tree 
#define MTREE_VECTORIZED			
#define MTREE_NREG			1			
#define MTREE_MAX_LEVEL		11
#define MTREE_MAX_WAY		2048