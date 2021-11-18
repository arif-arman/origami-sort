#pragma once
#include "commons.h"
#include <iomanip>

template <typename Reg, typename Item>
void print_size() {
	printf("Reg: %d bits, Item: %d bits\n", sizeof(Reg) << 3, sizeof(Item) << 3);
}

template <typename Item>
void SortEvery(Item* data, uint64_t nItems, uint64_t sorted_per) {
#pragma omp parallel for
	for(__int64 i = 0; i < nItems; i += sorted_per) {
		Item* data2 = data + i;
		std::sort(data2, data2 + sorted_per);
	}
}

template <typename Item = ui>
bool SortCorrectnessChecker(Item* data, uint32_t n) {
	for (uint32_t j = 0; j < n - 1; ++j) {
		if (data[j + 1] < data[j]) {
			printf("Order violation @ index %lu\n", j + 1);
			return false;
		}
	}
	return true;
	
}

template <typename Item>
bool SortCorrectnessCheckerSTD(Item* data, Item* sorted, ui64 n) {
	FOR(i, n, 1) {
		if (data[i] != sorted[i]) {
			printf("Order violation @ index %llu: data: %llX, sorted: %llX\n", i, data[i], sorted[i]);
			return false;
		}
	}
	return true;
}