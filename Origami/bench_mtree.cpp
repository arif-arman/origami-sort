/*--------------------------------------------------------------------------------------------
 - Origami: A High-Performance Mergesort Framework											 -
 - Copyright(C) 2021 Arif Arman, Dmitri Loguinov											 -
 - Produced via research carried out by the Texas A&M Internet Research Lab                  -
 -                                                                                           -
 - This program is free software : you can redistribute it and/or modify                     -
 - it under the terms of the GNU General Public License as published by                      -
 - the Free Software Foundation, either version 3 of the License, or                         -
 - (at your option) any later version.                                                       -
 -                                                                                           -
 - This program is distributed in the hope that it will be useful,                           -
 - but WITHOUT ANY WARRANTY; without even the implied warranty of                            -
 - MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                               -
 - GNU General Public License for more details.                                              -
 -                                                                                           -
 - You should have received a copy of the GNU General Public License                         -
 - along with this program. If not, see < http://www.gnu.org/licenses/>.                     -
 --------------------------------------------------------------------------------------------*/

#include "commons.h"
#include "utils.h"
#include "partition.h"
#include "writer.h"
#include "merger.h"
#include "merge_tree.h"
#include <fstream>

template <typename Reg, typename Item, ui NREG, bool in_cache = false, bool std_correctness = true>
void mtree_single_thread(ui writer_type = 1, ui K = 2, ui l1_buff_n = 32, ui l2_buff_n = 1024, int tid = 0) {
	SetThreadAffinityMask(GetCurrentThread(), 1 << tid);
	constexpr ui Itemsize = sizeof(Item);
	ui64 tot_n = in_cache ? ((L2_BYTES >> 1) / Itemsize) : (MB(128) / Itemsize);		// L2_POW in terms of Item count
	ui64 size = tot_n * Itemsize;
							
	Item* A, * C, * interimBuf = nullptr, *A_ref;
	Item* _X[MTREE_MAX_WAY], * _endX[MTREE_MAX_WAY];

	origami_merge_tree::MergeTree<Reg, Item>* kway_tree = nullptr;

	constexpr ui interim_buff_size = MB(256);

	if ((tot_n & 15LLU) != 0) tot_n -= 16;

	// how many keys to take in the merge kernel from each stream
	// default key is ui if element does not fit in register
	constexpr ui keys_per_stream = sizeof(Item) > sizeof(Reg) ? (sizeof(Reg) / sizeof(ui) * NREG) : (sizeof(Reg) / sizeof(Item) * NREG);		// keys per stream in merge
	UINT64 chunk = tot_n / K; chunk = (chunk / keys_per_stream) * keys_per_stream; tot_n = chunk * K;
	const ui K_pow = (ui)(log2(K));
	interimBuf = (Item*)(VALLOC(interim_buff_size));
	memset(interimBuf, 0, interim_buff_size);

	if (K_pow & 1) kway_tree = new origami_merge_tree::MergeTreeOdd<Reg, Item>(); 
	else kway_tree = new origami_merge_tree::MergeTreeEven<Reg, Item>(); 
	kway_tree->merge_init(K, interimBuf, l1_buff_n, l2_buff_n);
	
	// setup input and output array
	A = (Item*)VALLOC(size);
	C = (Item*)VALLOC(size + 4096);
	memset(A, 0, size);
	memset(C, 0, size + 4096);

	FOR(i, K, 1) {
		_X[i] = A + i * chunk;
		_endX[i] = A + (i + 1) * chunk;
	}

	datagen::Writer<Item> writer;
	writer.generate(A, tot_n, writer_type);
	SortEvery<Item>(A, tot_n, chunk);			

	// print setup details
	printf("Way: %u, # Keys: %llu, Chunk: %llu, Threads: %d, L1 buff: %u, L2 buff: %u \n", K, tot_n, chunk, 1, l1_buff_n, l2_buff_n);
	if constexpr (std_correctness) {
		printf("Preparing std::sort reference ... ");
		A_ref = (Item*)VALLOC(size);
		memcpy(A_ref, A, size);
		std::sort(A_ref, A_ref + tot_n);		
		printf("done\n");
	}

	ui64 repeat = 100;	

	// test ground
	hrc::time_point st = hrc::now();
	FOR(i, repeat, 1) {
		kway_tree->merge(_X, _endX, C, tot_n, l1_buff_n, l2_buff_n, interimBuf, K);
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

	kway_tree->merge_cleanup();
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
			printf("Blocks: %llu, Speed: %.2f _M/s, %.2f Blocks/s (%.2f ms)\n", block_count, block_count * block_n / el / 1e3, block_count / el * 1e3, el);
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
void kway_merge_worker(ui64 t_idx, ui n_threads, ui n_cores, origami_merge_tree::MergeTree<Reg, Item>* kway_tree, Item** _X, Item** _endX, Item* C, ui64 tot_n, ui l1_buff_n, ui l2_buff_n, Item* interimBuf, ui K) {
	int coreid = t_idx;
	if (coreid < n_threads) {
		if (coreid < n_cores) coreid <<= 1;
		else coreid = ((coreid % n_cores) << 1) + 1;
		SetThreadAffinityMask(GetCurrentThread(), 1 << coreid);
	}
	FOR(i, 1e9, 1) {
		kway_tree->merge(_X, _endX, C, tot_n, l1_buff_n, l2_buff_n, interimBuf, K);
		InterlockedIncrement64(&kway_merges_done);
	}
}

template <typename Reg, typename Item, ui NREG, bool in_cache = false, bool std_correctness = false>
void mtree_multi_thread(ui n_threads, ui n_cores, int choice = 0, ui writer_type = 1, ui K = 2, ui l1_buff_n = 32, ui l2_buff_n = 16384, int tid = 0) {
	SetThreadAffinityMask(GetCurrentThread(), 1 << tid);
	constexpr ui Itemsize = sizeof(Item);
	ui64 n_per_thread = in_cache ? ((L2_BYTES>> 1) / Itemsize) : (MB(32) / Itemsize);		
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
	event_exit = CreateEvent(NULL, true, false, NULL);				// manual reset, initial false
	if (!event_exit) {
		printf("Exit event creation failed with error: %lu\n", GetLastError());
		exit(1);
	}
	stat_thread = new std::thread(StatsThread, event_exit, &kway_merges_done, n_per_thread);

	
	origami_merge_tree::MergeTree<Reg, Item>* kway_tree[MAX_THREADS];
	constexpr ui interim_buff_size = GB(1);
	if ((n_per_thread & 15LLU) != 0) n_per_thread -= 16;

	// how many keys to take in the merge kernel from each stream
	// default key is ui if element does not fit in register
	constexpr ui keys_per_stream = sizeof(Item) > sizeof(Reg) ? (sizeof(Reg) / sizeof(ui) * NREG) : (sizeof(Reg) / sizeof(Item) * NREG);		// keys per stream in merge

	UINT64 chunk = n_per_thread / K; chunk = (chunk / keys_per_stream) * keys_per_stream; n_per_thread = chunk * K;
	const ui K_pow = (ui)(log2(K));
	interimBuf = (Item*)(VALLOC(interim_buff_size));
	memset(interimBuf, 0, interim_buff_size);
	ui64 interim_buf_n = GB(1) / sizeof(Item);
	ui64 n_per_thread_interim_buff = l1_buff_n / n_threads;
	Item* ptr = interimBuf;
	FOR(i, n_threads, 1) {
		if (K_pow & 1) kway_tree[i] = new origami_merge_tree::MergeTreeOdd<Reg, Item>(); 
		else kway_tree[i] = new origami_merge_tree::MergeTreeEven<Reg, Item>(); 
		kway_tree[i]->merge_init(K, ptr, l1_buff_n, l2_buff_n);
		interimBufs[i] = ptr;
		ptr += n_per_thread_interim_buff + 64 / sizeof(Item);		
	}
	ui64 tot_n = n_per_thread * n_threads;
	ui64 size = tot_n * Itemsize;

	// setup input and output array
	A = (Item*)VALLOC(size);
	C = (Item*)VALLOC(size + 4096);
	memset(A, 0, size);
	memset(C, 0, size + 4096);

	ptr = A;
	FOR(i, n_threads, 1) {
		FOR(j, K, 1) {
			_X[i][j] = ptr + j * chunk;
			_endX[i][j] = ptr + (j + 1) * chunk;
		}
		ptr += n_per_thread;
	}

	datagen::Writer<Item> writer;
	writer.generate(A, tot_n, writer_type);
	SortEvery<Item>(A, tot_n, chunk);			

	if constexpr (std_correctness) {
		printf("Preparing std::sort reference ... ");
		A_ref = (Item*)VALLOC(size);
		memcpy(A_ref, A, size);
		if (choice == 3) std::sort(A_ref, A_ref + tot_n);		
		else SortEvery<Item>(A_ref, tot_n, chunk << 1);
		printf("done\n");
	}

	// print setup details
	printf("# Keys tot: %llu, per_thread: %llu, Chunk: %llu, Threads: %d, Way: %u, L1 buff: %u, L2 buff: %u \n", tot_n, n_per_thread, chunk, n_threads, K, l1_buff_n, l2_buff_n);
	

	ui64 repeat = 1;

	// test ground
	hrc::time_point st = hrc::now();
	FOR(i, repeat, 1) {
		FOR(j, n_threads, 1) 
			threads[j] = new std::thread(kway_merge_worker<Reg, Item, NREG>, j, n_threads, n_cores, kway_tree[j], _X[j], _endX[j], C + j * n_per_thread, n_per_thread, l1_buff_n, l2_buff_n, interimBufs[j], K);
		FOR(j, n_threads, 1)
			threads[j]->join();
		SetEvent(event_exit);
		stat_thread->join();
		delete stat_thread;
		CloseHandle(event_exit);
		FOR(j, n_threads, 1)
			delete threads[j];
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
	FOR(i, n_threads, 1) {
		kway_tree[i]->merge_cleanup();
		delete kway_tree[i];
	}
	FOR(i, n_threads, 1) {
		delete _X[i];
		delete _endX[i];
	}
}

// bench mtree -- Table 8 in paper
void mtree_single_thread_test(int argc, char** argv) {
	constexpr ui NREG = 1;
	ui writer_type = MT;

	print_size<Regtype, Itemtype>();
	PRINT_DASH(40);

	// k-way test
	ui K = 4, l1_buff_n_pow = 5, l2_buff_n_pow = 14;
	FOR_INIT(i, 2, 11, 1)
		mtree_single_thread<Regtype, Itemtype, NREG>(writer_type, 1 << i, (1LU << l1_buff_n_pow), (1LU << l2_buff_n_pow));
}

void mtree_multi_thread_test(int argc, char** argv) {
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

	// bench mtree -- Table 8 in paper
	//mtree_single_thread_test(argc, argv);
	
	mtree_multi_thread_test(argc, argv);
	system("pause");
	return 0;
}
