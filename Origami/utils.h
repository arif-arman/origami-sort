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