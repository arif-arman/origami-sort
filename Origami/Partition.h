#pragma once
#include "commons.h"
#include "utils.h"


namespace partition_templated {
	/*
	S:	final partition indices -> return result
	B:	B[i] holds the start index of segment i
	L:	left bound of partitions
	R:	right bound of partitions
	A:	input array
	K:	# of sorted segments
	D:	size of each final partition
	*/
	ui64 n_steps = 0;
	ui64 bsearch_acc_cnt = 0;

	template <typename Item>
	FORCEINLINE Item get_zero() {
		if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			KeyValue<i64, i64> kv;
			kv.key = 0;
			kv.value = 0;
			return kv;
		}
		else return 0;
	}

	template <typename Item>
	FORCEINLINE Item get_max() {
		if constexpr (std::is_same<Item, ui>::value)
			return UINT32_MAX;
		else if constexpr (std::is_same<Item, i64>::value)
			return INT64_MAX;
		else if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			KeyValue<i64, i64> kv;
			kv.key = INT64_MAX;
			kv.value = INT64_MAX;
			return kv;
		}
	}

	template <typename Item>
	FORCEINLINE void Partition(Item* S, Item* B, Item* L, Item* R, Item* A, ui64 K, ui64 D) {
		// Partition the values
		/*FOR_INIT(k, 1, K + 1, 1) {
			L[k] = B[k - 1]; R[k] = B[k];
			S[k] = L[k] + D / K + (k <= (D % K));
		}*/
		Item lmax = get_zero<Item>(); 
		Item rmin = get_max<Item>(); //UINT32_MAX;
		ui64 seg_of_max = 0, seg_of_min = 0;
		ui64 delta_max = 1, delta_min = 1, delta = 1;

		while (1) {
			//if (R[seg_of_max] - L[seg_of_max] >= 16) ++n_steps; 
			//if (R[seg_of_min] - L[seg_of_min] >= 16) ++n_steps;
			// Find max of all values at S[k] - 1 and minimum at S[k],
			// considering only those S[k] - 1 and S[k] that are bounded by L[k] .. R[k]
			lmax = get_zero<Item>(); rmin = get_max<Item>(); // UINT32_MAX;
			FOR_INIT(k, 1, K + 1, 1) {
				if (S[k] > L[k]) {
					if (A[S[k] - 1] > lmax) {
						lmax = A[S[k] - 1];
						seg_of_max = k;
					}
				}
				if (S[k] < R[k]) {
					if (A[S[k]] < rmin) {
						rmin = A[S[k]];
						seg_of_min = k;
					}
				}
			}

			// Exit if invalid(lmax) or invalid(rmin) or lmax < rmin
			if (lmax < rmin)
				break;

			// Eliminate excluded regions from further consideration
			R[seg_of_max] = S[seg_of_max] - 1;
			L[seg_of_min] = S[seg_of_min] + 1;

			// Reposition S[k] in seg_of_max and seg_of_min
			delta_max = (R[seg_of_max] - L[seg_of_max]) / 2;
			delta_min = (R[seg_of_min] - L[seg_of_min]) / 2;
			delta = MIN(delta_max, delta_min);

			S[seg_of_max] = R[seg_of_max] - delta;
			S[seg_of_min] = L[seg_of_min] + delta;
		}
#ifdef DBG_PRINT
		printf("Partitioning complete in %llu steps\n", n_steps);
#endif
	}

	template <typename Item>
	FORCEINLINE void Partition_Ptrs(Item** L, Item** R, Item** S, ui64 K) {
		// Partition the values
		Item lmax = get_zero<Item>(); // 0;
		Item rmin = get_max<Item>(); // Keymax;
		ui64 seg_of_max = 0, seg_of_min = 0;
		ui64 delta_max = 1, delta_min = 1, delta = 1;

		while (1) {
			// Find max of all values at S[k] - 1 and minimum at S[k],
			// considering only those S[k] - 1 and S[k] that are bounded by L[k] .. R[k]
			lmax = get_zero<Item>(); rmin = get_max<Item>(); // Keymax;
			//FOR_INIT(k, 1, K + 1, 1) {
			FOR(k, K, 1) {
				if (S[k] > L[k]) {
					Item x = (S[k] - 1)[0];
					if (x > lmax) {
						lmax = x;
						seg_of_max = k;
					}
				}
				if (S[k] < R[k]) {
					Item x = S[k][0];
					if (x < rmin) {
						rmin = x;
						seg_of_min = k;
					}
				}
			}

			// Exit if invalid(lmax) or invalid(rmin) or lmax < rmin
			if (lmax < rmin)
				break;

			// Eliminate excluded regions from further consideration
			R[seg_of_max] = S[seg_of_max] - 1;
			L[seg_of_min] = S[seg_of_min] + 1;

			// Reposition S[k] in seg_of_max and seg_of_min
			delta_max = (R[seg_of_max] - L[seg_of_max]) / 2;
			delta_min = (R[seg_of_min] - L[seg_of_min]) / 2;
			delta = MIN(delta_max, delta_min);

			S[seg_of_max] = R[seg_of_max] - delta;
			S[seg_of_min] = L[seg_of_min] + delta;
		}
#ifdef DBG_PRINT
		printf("Partitioning complete in %llu steps\n", n_steps);
#endif
	}
}
