#include "commons.h"
#include "utils.h"
#include "Partition.h"
#include "Writer.h"
#include "merger.h"
#include "merge_tree.h"
#include <fstream>


const char* tmp_merge_type[] = { "1X", "2X", "3X" };
template <typename Reg, typename Item, ui NREG, bool in_cache = false, bool std_correctness = true>
void mtree_single_thread(ui writer_type = 1, ui K = 2, ui interim_buff_n = 32, ui l2_buff_n = 16384, int tid = 0) {

	SetThreadAffinityMask(GetCurrentThread(), 1 << tid);
	constexpr ui Itemsize = sizeof(Item);
	ui64 tot_n = in_cache ? ((L2_BYTES >> 1) / Itemsize) : (MB(128) / Itemsize);		// L2_POW in terms of Item count
	ui64 size = tot_n * Itemsize;
							
	Item* A, * C, * interimBuf = nullptr, *A_ref;
	Item* _X[MTREE_MAX_WAY], * _endX[MTREE_MAX_WAY];

	origami_merge_tree::MergeReuseTree<Reg, Item>* kway_tree = nullptr;

	constexpr ui interim_buff_size = MB(256);

	if ((tot_n & 15LLU) != 0) tot_n -= 16;

	// how many keys to take in the merge kernel from each stream
	// default key is ui if element does not fit in register
	constexpr ui keys_per_stream = sizeof(Item) > sizeof(Reg) ? (sizeof(Reg) / sizeof(ui) * NREG) : (sizeof(Reg) / sizeof(Item) * NREG);		// keys per stream in merge
	UINT64 chunk = tot_n / K; chunk = (chunk / keys_per_stream) * keys_per_stream; tot_n = chunk * K;
	const ui K_pow = (ui)(log2(K));
	interimBuf = (Item*)(VALLOC(interim_buff_size));
	memset(interimBuf, 0, interim_buff_size);

	if (K_pow & 1) kway_tree = new origami_merge_tree::MergeReuseTreeOdd<Reg, Item>(); 
	else kway_tree = new origami_merge_tree::MergeReuseTreeEven<Reg, Item>(); 
	kway_tree->merge_reuse_init(K, interimBuf, interim_buff_n, l2_buff_n);
		
	
	// setup input and output array
	A = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	C = (Item*)VirtualAlloc(NULL, size + 4096, MEM_COMMIT, PAGE_READWRITE);
	memset(A, 0, size);
	memset(C, 0, size + 4096);

	FOR(i, K, 1) {
		_X[i] = A + i * chunk;
		_endX[i] = A + (i + 1) * chunk;
	}

	origami_utils::random_writer<Item>(A, tot_n);	
	SortEvery<Item>(A, tot_n, chunk);			

	if constexpr (std_correctness) {
		printf("Preparing std::sort reference ... ");
		A_ref = (Item*)VALLOC(size);
		memcpy(A_ref, A, size);
		std::sort(A_ref, A_ref + tot_n);		// if it's k-way merge; whole array will be sorted
		printf("done\n");
	}

	// print setup details
	printf("# Keys: %llu, Chunk: %llu, Threads: %d, Way: %u, L1 buff: %u, L2 buff: %u \n", tot_n, chunk, 1, K, interim_buff_n, l2_buff_n);
	

	ui64 repeat = 100;	

	// test ground
	hrc::time_point st = hrc::now();
	FOR(i, repeat, 1) {
		// do the merge
		//kway_tree->merge_reuse(A, C, chunk, interim_buff_n, l2_buff_n, interimBuf, K);
		kway_tree->merge_reuse(_X, _endX, C, tot_n, interim_buff_n, l2_buff_n, interimBuf, K);
	}
	hrc::time_point en = hrc::now();
	double el = ELAPSED_MS(st, en);
	double sp = tot_n * repeat / el / 1e3;

	printf("Elapsed: %.2f ms, Merged @ %.2f M/s\n", el, sp);

	// merge correctness check
	if constexpr (std_correctness) {
		printf("Checking correctness ... ");
		SortCorrectnessCheckerSTD<Item>(C, A_ref, tot_n);			
		printf("done\n");
		VFREE(A_ref);
	}

	// cleanup
	VFREE(A);
	VFREE(C);
	VFREE(interimBuf);

	kway_tree->merge_reuse_cleanup();
	delete kway_tree;
}


__int64 kway_merges_done = 0;

void StatsThread(HANDLE event_exit, __int64* total_blocks, ui64 block_n) {
	__int64 prev_block_count = 0;
	ui curr_iter = 0;
	ui target_iters = 10;		// let it run for 40s
	DWORD timeout_ms = 4000;
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);
	printf("Stats thread starting ... \n");
	hrc::time_point st = hrc::now();	// stat thread start time
	while (true) {
		DWORD ret = WaitForSingleObject(event_exit, timeout_ms); // time-out every 4 secs
		if (ret == WAIT_OBJECT_0) {
			// event_exit was set
			break;
		}
		else if (ret == WAIT_TIMEOUT) {
			EnterCriticalSection(&cs);
			hrc::time_point en = hrc::now();
			__int64 total_blocks_loc = *total_blocks;
			double el = ELAPSED_MS(st, en); st = en;
			LeaveCriticalSection(&cs);
			ui64 block_count = total_blocks_loc - prev_block_count;
			prev_block_count = total_blocks_loc;
			printf("Blocks: %llu, Speed: %.2f _M/s, %.2f _M Blocks/s (%.2f ms)\n", block_count, block_count * block_n / el / 1e3, block_count / el / 1e3, el);
			++curr_iter;
			if (curr_iter == target_iters) {
				printf("Exiting ... \n");
				DeleteCriticalSection(&cs);
				exit(1);
			}
		}
		else {
			printf("Returned something else, Error %lu\n", GetLastError());
		}
	}
	printf("Stats thread exiting ... \n");
	DeleteCriticalSection(&cs);
}

template <typename Reg, typename Item, ui NREG>
void kway_merge_worker(ui64 t_idx, ui n_threads, ui n_cores, origami_merge_tree::MergeReuseTree<Reg, Item>* kway_tree, Item** _X, Item** _endX, Item* C, ui64 tot_n, ui l1_buff_n, ui l2_buff_n, Item* interimBuf, ui K) {
	int coreid = t_idx;
	if (coreid < n_threads) {
		if (coreid < n_cores) coreid <<= 1;
		else coreid = ((coreid % n_cores) << 1) + 1;
		SetThreadAffinityMask(GetCurrentThread(), 1 << coreid);
	}
	FOR(i, 1e9, 1) {
		kway_tree->merge_reuse(_X, _endX, C, tot_n, l1_buff_n, l2_buff_n, interimBuf, K);
		InterlockedIncrement64(&kway_merges_done);
	}
}

template <typename Reg, typename Item, ui NREG, bool in_cache = false, bool std_correctness = false>
void mtree_multi_thread(ui n_threads, ui n_cores, int choice = 0, ui writer_type = 1, ui K = 2, ui interim_buff_n = 32, ui l2_buff_n = 16384, int tid = 0) {
#define KWAY_TREE_TYPE 1				// 0 -> binary tree, 1 -> quad tree
#define MAX_THREADS 128

	SetThreadAffinityMask(GetCurrentThread(), 1 << tid);
	constexpr ui Itemsize = sizeof(Item);
	ui64 n_per_thread = in_cache ? ((L2_BYTES>> 1) / Itemsize) : (MB(128) / Itemsize);		
	Item* A, * C, * interimBuf = nullptr, * A_ref;
	Item** _X[MAX_THREADS], ** _endX[MAX_THREADS];
	Item* interimBufs[MAX_THREADS];
	FOR(i, n_threads, 1) {
		_X[i] = new Item * [MTREE_MAX_WAY];
		_endX[i] = new Item * [MTREE_MAX_WAY];
	}
	std::thread** threads = new std::thread * [n_threads];
	std::thread* stat_thread = nullptr;
	HANDLE event_exit = NULL;
	event_exit = CreateEvent(NULL, true, false, NULL); // manual reset, initial false
	if (!event_exit) {
		printf("Exit event creation failed with error: %lu\n", GetLastError());
		exit(1);
	}
	stat_thread = new std::thread(StatsThread, event_exit, &kway_merges_done, n_per_thread);

#if KWAY_TREE_TYPE == 1
	origami_merge_tree::MergeReuseTree<Reg, Item>* kway_tree[MAX_THREADS];
#endif
	constexpr ui interim_buff_size = GB(1);

	if ((n_per_thread & 15LLU) != 0) n_per_thread -= 16;

	// how many keys to take in the merge kernel from each stream
	// default key is ui if element does not fit in register
	constexpr ui keys_per_stream = sizeof(Item) > sizeof(Reg) ? (sizeof(Reg) / sizeof(ui) * NREG) : (sizeof(Reg) / sizeof(Item) * NREG);		// keys per stream in merge

	UINT64 chunk = n_per_thread >> 1;
	// no unroll
	if (choice == 0) {
		chunk = (chunk / keys_per_stream) * keys_per_stream; n_per_thread = chunk << 1;
	}
	// 2x unroll
	else if (choice == 1) {
		chunk = n_per_thread / 4; chunk = (chunk / keys_per_stream) * keys_per_stream; n_per_thread = chunk * 4;
	}
	// 3x unroll
	else if (choice == 2) {
		chunk = n_per_thread / 6; chunk = (chunk / keys_per_stream) * keys_per_stream; n_per_thread = chunk * 6;
	}
	// kx unroll
	else if (choice == 3) {
		chunk = n_per_thread / K; chunk = (chunk / keys_per_stream) * keys_per_stream; n_per_thread = chunk * K;
		const ui K_pow = (ui)(log2(K));
		interimBuf = (Item*)(VALLOC(interim_buff_size));
		memset(interimBuf, 0, interim_buff_size);
		ui64 interim_buf_n = GB(1) / sizeof(Item);
		ui64 n_per_thread_interim_buff = interim_buff_n / n_threads;
		Item* ptr = interimBuf;
		FOR(i, n_threads, 1) {
			if (K_pow & 1) kway_tree[i] = new origami_merge_tree::MergeReuseTreeOdd<Reg, Item>(); // irl_merge_tree_encoded::MergeReuseTreeOdd(); //
			else kway_tree[i] = new origami_merge_tree::MergeReuseTreeEven<Reg, Item>(); // irl_merge_tree_encoded::MergeReuseTreeEven(); //
			kway_tree[i]->merge_reuse_init(K, ptr, interim_buff_n, l2_buff_n);
			interimBufs[i] = ptr;
			ptr += n_per_thread_interim_buff;		}
	}

	ui64 tot_n = n_per_thread * n_threads;
	ui64 size = tot_n * Itemsize;

	// setup input and output array
	A = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	C = (Item*)VirtualAlloc(NULL, size + 4096, MEM_COMMIT, PAGE_READWRITE);
	memset(A, 0, size);
	memset(C, 0, size + 4096);

	if (choice == 3) {
		Item* ptr = A;
		FOR(i, n_threads, 1) {
			FOR(j, K, 1) {
				_X[i][j] = ptr + j * chunk;
				_endX[i][j] = ptr + (j + 1) * chunk;
			}
			ptr += n_per_thread;
		}
		
	}

	//datagen::Generate(A, tot_n, writer_type, Keymax);
	origami_utils::random_writer<Item>(A, tot_n);	//origami_utils::print_arr<Item>(A, 20);
	//origami_utils::pareto_writer<Item>(A, tot_n, PARETO_NONUNIFORM);	//origami_utils::print_arr<Item>(A, 20);
	// shift the pareto dist to first 32-bits for encoding test
	/*if constexpr (std::is_same<Item, ui64>::value) {
		FOR(i, tot_n, 1) A[i] = A[i] << 32;
	}*/

	SortEvery<Item>(A, tot_n, chunk);			//origami_utils::print_arr<Item>(A, 20);

	if constexpr (std_correctness) {
		printf("Preparing std::sort reference ... ");
		A_ref = (Item*)VALLOC(size);
		memcpy(A_ref, A, size);
		if (choice == 3) std::sort(A_ref, A_ref + tot_n);		// if it's k-way merge; whole array will be sorted
		else SortEvery<Item>(A_ref, tot_n, chunk << 1);
		printf("done\n");
	}

	// print setup details
	if (choice == 3) printf("# Keys tot: %llu, per_thread: %llu, Chunk: %llu, Threads: %d, Way: %u, L1 buff: %u, L2 buff: %u \n", tot_n, n_per_thread, chunk, n_threads, K, interim_buff_n, l2_buff_n);
	else printf("# Keys: %llu, Chunk: %llu, Threads: %d, Unroll: %u x, Nreg: %u x %u\n", tot_n, chunk, 1, choice + 1, NREG, NREG);

	ui64 repeat = 1;

	// test ground
	hrc::time_point st = hrc::now();
	FOR(i, repeat, 1) {
		// do the merge
		switch (choice) {
		case 3:
			FOR(j, n_threads, 1) 
				threads[j] = new std::thread(kway_merge_worker<Reg, Item, NREG>, j, n_threads, n_cores, kway_tree[j], _X[j], _endX[j], C + j * n_per_thread, n_per_thread, interim_buff_n, l2_buff_n, interimBufs[j], K);
			FOR(j, n_threads, 1)
				threads[j]->join();
			SetEvent(event_exit);
			stat_thread->join();
			delete stat_thread;
			CloseHandle(event_exit);
			FOR(j, n_threads, 1)
				delete threads[j];
			break;
		}
	}
	hrc::time_point en = hrc::now();
	double el = ELAPSED_MS(st, en);
	double sp = tot_n * repeat / el / 1e3;

	printf("Elapsed: %.2f ms, Merged @ %.2f M/s\n", el, sp);




	// print swap_count
	// printf("SWAPS: %llu\n", irl_merge_tree_encoded::insertion_swap_count);

	// print sorted runs size --> for k-way merge only
	/*
	PRINT_DASH(30); PRINT_DASH(30);
	printf("*** ");
	ui cur_run = 1;
	ui64 _tot_sum_run = 0;		// for sanity check
	FOR_INIT(i, 1, tot_n, 1) {
		if (C[i] < C[i - 1]) {
			printf("%u, ", cur_run);
			_tot_sum_run += cur_run;
			cur_run = 1;
		}
		else ++cur_run;
	}
	_tot_sum_run += cur_run;
	printf("%u\n", cur_run);					// last run
	printf("Tot: %llu\n", _tot_sum_run);		// should match tot_n
	PRINT_DASH(30); PRINT_DASH(30);
	*/

	// print run size with conflicts: size of runs that have the same (n - k) bits
	/*
	if constexpr (std::is_same<Item, ui64>::value) {
		PRINT_DASH(30); PRINT_DASH(30);
		printf("*** ");
		const ui encode_mask = K - 1;		// for K = 4, mask = 0b11, K = 16, mask = 0b1111 and so on
		ui check_key = (C[0] >> ((Itemsize << 3) - 32)) | encode_mask;		// set the last log(K) bits to 1
		//printf("%llX, %lX\n", C[0], check_key);
		ui cur_run = 1, tot_run = 0;
		FOR_INIT(i, 1, tot_n, 1) {
			ui cur_key = (C[i] >> ((Itemsize << 3) - 32)) | encode_mask;
			//printf("%llX, %lX\n", C[i], cur_key);
			if (cur_key > check_key) {
				printf("%u, ", cur_run);
				tot_run += cur_run;
				cur_run = 1;
				check_key = cur_key;
			}
			else ++cur_run;
		}
		tot_run += cur_run;
		printf("%u\n", cur_run);				// last run
		printf("Tot: %llu\n", tot_run);	// should match tot_n
		PRINT_DASH(30); PRINT_DASH(30);
	}
	*/

	// merge correctness check
	if constexpr (std_correctness) {
		printf("Checking correctness ... ");
		if (choice == 3) {
			//origami_utils::print_arr<Item>(C, 20);
			/*FOR(i, tot_n, 1) {
				printf("%3u: %16llX\n", i, C[i]);
			}*/
			SortCorrectnessCheckerSTD<Item>(C, A_ref, tot_n);
		}
		else {
			FOR(i, tot_n, (chunk << 1))
				SortCorrectnessCheckerSTD<Item>(C + i, A_ref + i, (chunk << 1));
		}

		printf("done\n");
		VFREE(A_ref);
	}

	// cleanup
	VFREE(A);
	VFREE(C);
	if (choice == 3) {
		VFREE(interimBuf);
		FOR(i, n_threads, 1) {
			kway_tree[i]->merge_reuse_cleanup();
			delete kway_tree[i];
		}
	}
	FOR(i, n_threads, 1) {
		delete _X[i];
		delete _endX[i];
	}

	//PRINT_DASH(50);
}


void mtree_single_thread_test_driver(int argc, char** argv) {

	constexpr ui NREG = 1;
	ui writer_type = 1;

	printf("Reg size: %lu bits, Item size: %lu bits, NREG: %lu\n", sizeof(Regtype) << 3, sizeof(Itemtype) << 3, NREG);
	PRINT_DASH(40);


	
	// k-way test
	ui K = 4, l1_buff_n_pow = 5, l2_buff_n_pow = 14;
	//ui K = 16, l1_buff_n_pow = 5, l2_buff_n_pow = 14;
	//if (argc >= 4)
		//K = atoi(argv[1]), l1_buff_n_pow = atoi(argv[2]), l2_buff_n_pow = atoi(argv[3]);
	//mtree_single_thread<Regtype, Itemtype, NREG>(writer_type, K, (1LU << l1_buff_n_pow), (1LU << l2_buff_n_pow));

	FOR_INIT(i, 2, 11, 1)
		mtree_single_thread<Regtype, Itemtype, NREG>(writer_type, 1 << i, (1LU << l1_buff_n_pow), (1LU << l2_buff_n_pow));
}

void mtree_multi_thread_test_driver(int argc, char** argv) {
#define REG_TYPE 1		// 0: scalar, 1,2,3: sse, avx2, avx512
#define KEY_TYPE 0		// 0: 32, 1: 64, 2: 64+64

#if KEY_TYPE == 0
	using Itemtype = ui; // KeyValue<i64, i64>;// ui; // KeyValue2<ui, KV2_SZ>; // ui; // i64; // 
#elif KEY_TYPE == 1
	using Itemtype = i64;
#else 
	using Itemtype = KeyValue<i64, i64>;
#endif 

	// parameters
#if REG_TYPE == 0
	using Regtype = Itemtype;
#elif REG_TYPE == 1
	using Regtype = sse;
#elif REG_TYPE == 2
	using Regtype = avx2;
#else 
	using Regtype = sse;
#endif 

	constexpr ui NREG = 1;
	int choice = 1;
	ui writer_type = 1;

	printf("Reg size: %lu bits, Item size: %lu bits, NREG: %lu\n", sizeof(Regtype) << 3, sizeof(Itemtype) << 3, NREG);
	PRINT_DASH(40);

	// k-way test
	ui n_threads = 2, n_cores = 2;
	ui K = 4, l1_buff_n_pow = 12, l2_buff_n_pow = 12;
	if (argc >= 6)
		K = atoi(argv[1]), l1_buff_n_pow = atoi(argv[2]), l2_buff_n_pow = atoi(argv[3]), n_threads = atoi(argv[4]), n_cores = atoi(argv[5]);
	mtree_multi_thread<Regtype, Itemtype, NREG, true>(n_threads, n_cores, 3, writer_type, K, (1LU << l1_buff_n_pow), (1LU << l2_buff_n_pow));
	


}

int main(int argc, char** argv)
{
	SetThreadAffinityMask(GetCurrentThread(), 1 << 0);

	mtree_single_thread_test_driver(argc, argv);
	//mtree_multi_thread_test_driver(argc, argv);
	system("pause");
	return 0;
}
