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
#include <fstream>

namespace datagen {

#define MT					1	// Mersenne-Twister
#define SAME				2	// all same
#define SORTED_SEQ			3	
#define REV_SORTED_SEQ		4
#define ALMOST_SORTED		5	// every k-th element permuted
#define PARETO_NONUNIFORM	6	// Pareto keys
#define PARETO_B2B			7	// Pareto distribution back to back
#define PARETO_SHUFF		8	// Pareto distribution shuffled
#define FIB_SEQ				9

	const char* writer_names_[] = { "Dummy", "Mersenne-Twister", "All Same", "Sorted-sequence", "Reverse-sorted", "Almost-sorted", "Pareto keys", "Pareto-b2b", "Pareto-shuffled", "Fibonacci-sequence", "Fibonacci-sequence2" };
	const int writer_count_ = 10;
		
	void Usage() {
		printf("> Input types:\n");
		for (int i = 1; i < writer_count_; ++i) {
			printf("%2d %s\n", i, writer_names_[i]);
		}
	}

	template <typename Item>
	class Writer {
	public:
		void random_writer(Item* A, ui64 n) {
			if constexpr (std::is_same<Item, ui>::value || std::is_same<Item, int>::value) {
				std::mt19937 g;
				std::uniform_int_distribution<Item> d;
				FOR(i, n, 1) A[i] = d(g);
			}
			else if constexpr (std::is_same<Item, i64>::value || std::is_same<Item, ui64>::value) {
				std::mt19937_64 g;
				std::uniform_int_distribution<Item> d;
				FOR(i, n, 1) A[i] = d(g);
			}
			else if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
				KeyValue<i64, i64> kv;
				std::mt19937_64 g;
				std::uniform_int_distribution<i64> d(0, INT64_MAX);
				FOR(i, n, 1) {
					kv.key = d(g);
					kv.value = kv.key + 1;	// dummy value
					A[i] = kv;
				}
			}
		}

		void pareto_writer(Item* A, ui64 n, int type) {
			ui64 a = 6364136223846793005, c = 1442695040888963407, x = 1;
			double ED = 20;
			double alpha = 1, beta = 7;
			ui64 sum = 0, keys = 0, y = 889;
			ui64 maxF = 0;
			for (ui64 i = 0; i < n; )
			{
				x = x * a + c;
				y = y * a + c;

				// generate frequency from the Pareto distribution with alpha=1; otherwise, the generator gets slow
				double u = (double)y / ((double)(1LLU << 63) * 2);			// uniform [0,1]
				ui64 f = min(ceil(beta * (1 / (1 - u) - 1)), 100000);		// rounded-up Pareto
				if (type == PARETO_B2B || type == PARETO_SHUFF) {
					if (i + f < n) {
						for (int j = 0; j < f; j++) {
							if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
								KeyValue<i64, i64> kv;
								kv.key = x;
								kv.value = x + 1;
								A[i + j] = kv;
							}
							else {
								A[i + j] = x;
							}
						}

						i += f;
					}
					else if (i + 10 >= n) {
						for (; i < n; ++i) {
							if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
								KeyValue<i64, i64> kv;
								kv.key = x;
								kv.value = x + 1;
								A[i] = kv;
							}
							else {
								A[i] = x;
							}
						}
					}

				}
				else if (type == PARETO_NONUNIFORM) {
					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
						KeyValue<i64, i64> kv;
						kv.key = f;
						kv.value = f + 1;
						A[i++] = kv;
					}
					else {
						A[i++] = f;
					}
				}
			}

			if (type == PARETO_SHUFF) {
				printf("> Shuffling ... \n");
				std::random_device rd;
				std::mt19937 g(rd());
				std::shuffle(A, A + n, g);
			}
		}

		void all_same(Item* A, ui64 n) {
			std::mt19937_64 g;
			std::uniform_int_distribution<i64> d;
			i64 x = d(g);
			FOR(i, n, 1) {
				if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
					KeyValue<i64, i64> kv;
					kv.key = x;
					kv.value = x + 1;
					A[i] = kv;
				}
				else A[i] = x;
			}
		}

		void sorted(Item* A, ui64 n) {
			random_writer(A, n);
			std::sort(A, A + n);
		}

		void reverse_sorted(Item* A, ui64 n) {
			random_writer(A, n);
			std::sort(A, A + n, std::greater<>());
		}

		void almost_sorted(Item* A, ui64 n) {
			random_writer(A, n);
			FOR(i, n, 7) {
				if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)
					A[i].key = INT64_MAX;
				else if constexpr (std::is_same<Item, i64>::value)
					A[i] = INT64_MAX;
				else A[i] = UINT32_MAX;
			}
		}

		void fibonacci(Item* A, ui64 n) {
			ui64 a = 0, b = 1, c;
			if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
				KeyValue<i64, i64> kv;
				kv.key = 0; kv.value = kv.key + 1;
				A[0] = kv;
				kv.key = 1; kv.value = kv.key + 1;
				A[1] = kv;
			}
			else {
				A[0] = 0; A[1] = 1;
			}
			ui64 i = 2;
			while (i < n) {
				c = a + b;
				if (c < b) {	// overflow
					a = 0; b = 1;

					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
						KeyValue<i64, i64> kv;
						kv.key = 0; kv.value = kv.key + 1;
						A[i++] = kv;
					}
					else A[i++] = 0;
					if (i < n) {
						if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
							KeyValue<i64, i64> kv;
							kv.key = 1; kv.value = kv.key + 1;
							A[i++] = kv;
						}
						else A[i++] = 1;
					}
				}
				else {
					a = b;
					b = c;
					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
						KeyValue<i64, i64> kv;
						kv.key = b; kv.value = kv.key + 1;
						A[i++] = kv;
					}
					else A[i++] = b;
				}

			}
		}

		void generate(Item* A, ui64 n, int type) {

			printf("Generating data with: %s ... ", writer_names_[type]);

			if (type == MT)
				random_writer(A, n);
			else if (type == SAME)
				all_same(A, n);
			else if (type == SORTED_SEQ)
				sorted(A, n);
			else if (type == REV_SORTED_SEQ)
				reverse_sorted(A, n);
			else if (type == ALMOST_SORTED)
				almost_sorted(A, n);
			else if (type == PARETO_NONUNIFORM || type == PARETO_B2B || type == PARETO_SHUFF)
				pareto_writer(A, n, type);
			else if (type == FIB_SEQ)
				fibonacci(A, n);

			printf("done\n");
		}
	};
}

