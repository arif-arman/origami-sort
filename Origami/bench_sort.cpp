#include "commons.h"
#include "Writer.h"
#include "utils.h"
#include "merge_utils.h"
#include "sorter.h"
#include <iostream>

template<typename Reg, typename Item>
void sort_bench(ui writer_type, int argc, char** argv) {
//#define STD_CORRECTNESS
	ui size_pow = atoi(argv[1]);
	ui n_threads = atoi(argv[2]);
	ui n_cores, min_k = 2;
	if (n_threads > 1) {
		n_cores = atoi(argv[3]);
		min_k = atoi(argv[4]);
	}
	else {
		min_k = atoi(argv[3]);
	}

	print_size<Reg, Item>();
	const ui Itemsize = sizeof(Item);
	ui64 size = (1LLU << size_pow);
	constexpr ui repeat = 3;
	ui64 n_items = size / Itemsize;

	printf("Running origami-sort --> n: %llu ...\n", n_items);

	Item* data = (Item*)VALLOC(size);
	Item* end = data + n_items;
	Item* output = (Item*)VALLOC(size);
	Item* data_back = nullptr;
	Item* kway_buf = nullptr;
	ui64 kway_buf_size = MB(256);
	if (min_k > 2) {
		kway_buf = (Item*)VALLOC(kway_buf_size);
		memset(kway_buf, 0, kway_buf_size);
	}

	datagen::Writer<Item> writer;
	writer.generate(data, n_items, writer_type);
	memset(output, 0, size);

	if (repeat > 1) {
		data_back = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
		memcpy(data_back, data, size);
	}
#ifdef STD_CORRECTNESS
	printf("Sorting with std::sort ... ");
	Item* sorted = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE); 
	memcpy(sorted, data, size);
	hrc::time_point st1 = hrc::now();
	SortEvery(sorted, n_items, n_items);
	hrc::time_point en1 = hrc::now();
	printf("done, ");
	double el1 = ELAPSED_MS(st1, en1);
	printf("elapsed: %.2f ms, Speed: %.2f M/s\n", el1, (n_items / el1 / 1e3));
#endif	

	double avgS = 0;
	FOR(i, repeat, 1) {
		if (repeat > 0) {
			memcpy(data, data_back, size);
			memset(output, 0, size);
			if (n_threads > 1) 
				memset(kway_buf, 0, kway_buf_size);
		}

		Item* data2 = data;
		Item* end2 = data2 + n_items;
		Item* output2 = output;
		Item* o = data;

		hrc::time_point st1 = hrc::now();
		switch (n_threads) {
		case 1:
			o = origami_sorter::sort_single_thread<Item, Reg>(data2, output2, end2, n_items, min_k, kway_buf);
			break;
		default:
			o = origami_sorter::sort_multi_thread<Item, Reg>(data2, output2, n_items, n_threads, n_cores, min_k, kway_buf);
		}
		
		hrc::time_point en1 = hrc::now();

		printf("\r                               \r");

		double el = ELAPSED(st1, en1);
		double sp = double(n_items) / el / 1e6;
		avgS += sp;

#ifdef STD_CORRECTNESS
		printf("Iter %3lu done, checking correctness w/ std::sort ... ", i);
		if (!SortCorrectnessCheckerSTD(o, sorted, n_items)) {
			printf("Correctness error @ %llu\n", i);
			exit(1);
		}
		printf("done\r                                                                    \r");
#else 
		if (!SortCorrectnessChecker(o, n_items)) {
			printf("Correctness error @ %llu\n", i);
			//break;
			//system("pause");
			//exit(1);
		}
#endif
		printf("\rIter: %llu / %llu", i + 1, repeat);
	}
	avgS /= repeat;
	printf("\nSpeed: %.2f M keys/sec\n", avgS);
	// prints to prevent compiler optimizations
	if (*((int*)data + 3) & 0x1234 == *(int*)output) printf("%u %u\n", data[13], output[13]);
	PRINT_DASH(50);

	VFREE(data);
	VFREE(output);

	if (repeat > 1) VFREE(data_back);
	if (min_k > 2) VFREE(kway_buf);

#ifdef STD_CORRECTNESS
	VirtualFree(sorted, 0, MEM_RELEASE);
#endif 
#undef STD_CORRECTNESS
}

int main(int argc, char** argv) {

	// single thread sort test
	sort_bench<Regtype, Itemtype>(MT, argc, argv);
	system("pause");

	return 0;
}