#pragma once

#include "commons.h"
#include "merge_utils.h"
#include "merge_networks.h"

namespace origami_merger {

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


	// scalar
	// with tail handling
	template <typename Item, ui nreg>
	FORCEINLINE void mergebl2_scalar(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C) {
		Item* a = A, * b = B, * endA = (A + lenA), * endB = (B + lenB), * c = C;
		register Item a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;

		Item SENTINEL = get_max<Item>();
		Item* SENTINEL_PTR = &SENTINEL;

		if constexpr (nreg == 1) {
			a0 = a[0]; a1 = b[0];
		}
		else if constexpr (nreg == 2) {
			a0 = a[0]; a1 = a[1]; a2 = b[0]; a3 = b[1];
		}
		else if constexpr (nreg == 3) {
			a0 = a[0]; a1 = a[1]; a2 = a[2]; a3 = b[0]; a4 = b[1]; a5 = b[2];
		}
		else if constexpr (nreg == 4) {
			a0 = a[0]; a1 = a[1]; a2 = a[2]; a3 = a[3];
			a4 = b[0]; a5 = b[1]; a6 = b[2]; a7 = b[3];
		}
		else if constexpr (nreg == 5) {
			a0 = a[0]; a1 = a[1]; a2 = a[2]; a3 = a[3]; a4 = a[4];
			a5 = b[0]; a6 = b[1]; a7 = b[2]; a8 = b[3]; a9 = b[4];
		}
		else if constexpr (nreg == 8) {
			a0 = a[0]; a1 = a[1]; a2 = a[2]; a3 = a[3]; a4 = a[4]; a5 = a[5]; a6 = a[6]; a7 = a[7];
			a8 = b[0]; a9 = b[1]; a10 = b[2]; a11 = b[3]; a12 = b[4]; a13 = b[5]; a14 = b[6]; a15 = b[7];
		}

		a += nreg;
		b += nreg;

		Item* loadFrom = a, * opposite = b;


		// method 1
		while (loadFrom != endA && loadFrom != endB) {
			//printf("%llX, %llX, %llX, %llX\n", loadFrom, opposite, endA, endB);
			bool first = *loadFrom <= *opposite; //true;
			/*if constexpr (nreg == 1) first = *loadFrom <= *opposite;
			else if constexpr (nreg == 2) first = a1 <= a3;
			else if constexpr (nreg == 3) first = a2 <= a5;
			else if constexpr (nreg == 4) first = a3 <= a7;
			else if constexpr (nreg == 5) first = a4 <= a9;
			else if constexpr (nreg == 8) first = a7 <= a15;*/

			Item* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp;

			if constexpr (nreg == 1) {
				SWAP2(0, 1);
				c[0] = a0;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0];
			}
			else if constexpr (nreg == 2) {
				SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
				c[0] = a0; c[1] = a1;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; a1 = loadFrom[1];
			}
			else if constexpr (nreg == 3) {
				SWAP2(0, 3); SWAP2(1, 4); SWAP2(2, 5); SWAP2(1, 3); SWAP2(2, 4); SWAP2(2, 3);
				/*SWAP2(0, 4); SWAP2(1, 5);
				SWAP2(1, 3); SWAP2(2, 4);
				SWAP2(0, 1); SWAP2(2, 3); SWAP2(4, 5);*/

				c[0] = a0; c[1] = a1; c[2] = a2;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; a1 = loadFrom[1]; a2 = loadFrom[2];

			}
			else if constexpr (nreg == 4) {
				SWAP2(0, 4);	SWAP2(1, 5);	SWAP2(2, 6);	SWAP2(3, 7);
				SWAP2(2, 4);	SWAP2(3, 5);
				SWAP2(1, 2);	SWAP2(3, 4);

				c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; a1 = loadFrom[1]; a2 = loadFrom[2]; a3 = loadFrom[3];
			}
			else if constexpr (nreg == 5) {
				// ripco best
				/*SWAP2(0, 5); SWAP2(1, 6); SWAP2(2, 7); SWAP2(3, 8); SWAP2(4, 9);
				SWAP2(3, 6);
				SWAP2(4, 6);
				SWAP2(3, 5);
				SWAP2(2, 5); SWAP2(6, 8); SWAP2(1, 3); SWAP2(4, 7);
				SWAP2(2, 3); SWAP2(4, 5); SWAP2(6, 7);*/
				// Batcher's
				SWAP2(0, 8); SWAP2(1, 9); SWAP2(2, 6); SWAP2(3, 7);
				SWAP2(1, 5); SWAP2(4, 8); SWAP2(0, 2);
				SWAP2(4, 6); SWAP2(3, 5); SWAP2(7, 9);
				SWAP2(0, 1); SWAP2(2, 3); SWAP2(4, 5); SWAP2(6, 7); SWAP2(8, 9);

				c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3; c[4] = a4;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; a1 = loadFrom[1]; a2 = loadFrom[2]; a3 = loadFrom[3]; a4 = loadFrom[4];
			}
			else if constexpr (nreg == 8) {
				SWAP2(0, 8); SWAP2(1, 9); SWAP2(2, 10); SWAP2(3, 11); SWAP2(4, 12); SWAP2(5, 13); SWAP2(6, 14); SWAP2(7, 15);
				SWAP2(4, 8); SWAP2(5, 9); SWAP2(6, 10); SWAP2(7, 11);
				SWAP2(2, 4); SWAP2(3, 5); SWAP2(6, 8); SWAP2(7, 9); //SWAP2(10, 12); SWAP2(11, 13);
				SWAP2(1, 2); SWAP2(3, 4); SWAP2(5, 6); SWAP2(7, 8);	//SWAP2(9, 10); SWAP2(11, 12); SWAP2(13, 14);

				c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3; c[4] = a4; c[5] = a5; c[6] = a6; c[7] = a7;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; a1 = loadFrom[1]; a2 = loadFrom[2]; a3 = loadFrom[3]; a4 = loadFrom[4]; a5 = loadFrom[5]; a6 = loadFrom[6]; a7 = loadFrom[7];
			}
			c += nreg;
			loadFrom += nreg;
		}

		if constexpr (nreg == 1) {
			SWAP2(0, 1);
			c[0] = a0; 
		}
		else if constexpr (nreg == 2) {
			SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
			c[0] = a0; c[1] = a1; 
		}
		else if constexpr (nreg == 3) {
			SWAP2(0, 3); SWAP2(1, 4); SWAP2(2, 5); SWAP2(1, 3); SWAP2(2, 4); SWAP2(2, 3);
			c[0] = a0; c[1] = a1; c[2] = a2; 
		}
		else if constexpr (nreg == 4) {
			SWAP2(0, 4);	SWAP2(1, 5);	SWAP2(2, 6);	SWAP2(3, 7);
			SWAP2(2, 4);	SWAP2(3, 5);
			SWAP2(1, 2);	SWAP2(3, 4); SWAP2(5, 6);
			c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;
		}
		c += nreg;

		Item* endOp = (loadFrom == endA) ? endB : endA;
		while (opposite != endOp) {
			_mm_prefetch((char*)(opposite + 64), _MM_HINT_T2);

			if constexpr (nreg == 1) {
				a0 = opposite[0];
				SWAP2(0, 1);
				c[0] = a0;
			}
			else if constexpr (nreg == 2) {
				a0 = opposite[0]; a1 = opposite[1];
				SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
				c[0] = a0; c[1] = a1;
			}
			else if constexpr (nreg == 3) {
				a0 = opposite[0]; a1 = opposite[1]; a2 = opposite[2];
				SWAP2(0, 3); SWAP2(1, 4); SWAP2(2, 5); SWAP2(1, 3); SWAP2(2, 4); SWAP2(2, 3);
				/*SWAP2(0, 4); SWAP2(1, 5);
				SWAP2(1, 3); SWAP2(2, 4);
				SWAP2(0, 1); SWAP2(2, 3); SWAP2(4, 5);*/
				c[0] = a0; c[1] = a1; c[2] = a2;
			}
			else if constexpr (nreg == 4) {
				a0 = opposite[0]; a1 = opposite[1];	a2 = opposite[2]; a3 = opposite[3];
				SWAP2(0, 4);	SWAP2(1, 5);	SWAP2(2, 6);	SWAP2(3, 7);
				SWAP2(2, 4);	SWAP2(3, 5);
				SWAP2(1, 2);	SWAP2(3, 4);
				c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3;
			}
			opposite += nreg;
			c += nreg;
		}

		if constexpr (nreg == 1) {
			c[0] = a1;
		}
		else if constexpr (nreg == 2) {
			c[0] = a2; c[1] = a3;
		}
		else if constexpr (nreg == 3) {
			SWAP2(3, 5); SWAP2(3, 4); SWAP2(4, 5);
			c[0] = a3; c[1] = a4; c[2] = a5;
		}
		else if constexpr (nreg == 4) {
			SWAP2(4, 6); SWAP2(5, 7); SWAP2(4, 5); SWAP2(6, 7); SWAP2(5, 6);
			c[0] = a4; c[1] = a5; c[2] = a6; c[3] = a7;
		}
		c += nreg;

	}

	// scalar KV --> for i64, i64 only (03.09.21)
	// w/ tail handling
	template <typename Item, ui nreg>
	FORCEINLINE void mergebl2_scalar_kv(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C) {

		i64* a = (i64 *)A, * b = (i64*)B, * endA = (i64*)(A + lenA), * endB = (i64*)(B + lenB), * c = (i64*)C;
		register i64 a0, a1, a2, a3, a4, a5, a6, a7;
		register i64 b0, b1, b2, b3, b4, b5, b6, b7;
		constexpr ui64 INC = (nreg << 1);

		if constexpr (nreg == 1) {
			a0 = a[0]; b0 = a[1];
			a1 = b[0]; b1 = b[1];
		}
		else if constexpr (nreg == 2) {
			a0 = a[0]; b0 = a[1]; a1 = a[2]; b1 = a[3];
			a2 = b[0]; b2 = b[1]; a3 = b[2]; b3 = b[3];
		}
		else if constexpr (nreg == 3) {
			a0 = a[0]; b0 = a[1]; a1 = a[2]; b1 = a[3]; a2 = a[4]; b2 = a[5];
			a3 = b[0]; b3 = b[1]; a4 = b[2]; b4 = b[3]; a5 = b[4]; b5 = b[5];
		}
		else if constexpr (nreg == 4) {
			a0 = a[0]; b0 = a[1]; a1 = a[2]; b1 = a[3]; a2 = a[4]; b2 = a[5]; a3 = a[6]; b3 = a[7];
			a4 = b[0]; b4 = b[1]; a5 = b[2]; b5 = b[3]; a6 = b[4]; b6 = b[5]; a7 = b[6]; b7 = b[7];
		}
		
		a += INC;
		b += INC;

		i64* loadFrom = a, * opposite = b;

		while (loadFrom != endA && loadFrom != endB) {
			bool first = true;
			if constexpr (nreg == 1) first = *loadFrom <= *opposite;
			else if constexpr (nreg == 2) first = a1 < a3;
			else if constexpr (nreg == 3) first = a2 < a5;
			else if constexpr (nreg == 4) first = a3 < a7;

			i64* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp;

			if constexpr (nreg == 1) {
				SWAPKV2(0, 1);
				c[0] = a0; c[1] = b0;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; b0 = loadFrom[1];
			}
			else if constexpr (nreg == 2) {
				SWAPKV2(0, 2); SWAPKV2(1, 3); SWAPKV2(1, 2);
				c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; b0 = loadFrom[1];
				a1 = loadFrom[2]; b1 = loadFrom[3];
			}
			else if constexpr (nreg == 3) {
				SWAPKV2(0, 3); SWAPKV2(1, 4); SWAPKV2(2, 5); SWAPKV2(1, 3); SWAPKV2(2, 4); SWAPKV2(2, 3);
				/*SWAPKV2(0, 4); SWAPKV2(1, 5);
				SWAPKV2(1, 3); SWAPKV2(2, 4);
				SWAPKV2(0, 1); SWAPKV2(2, 3); SWAPKV2(4, 5);*/

				c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1; c[4] = a2; c[5] = b2;
				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; b0 = loadFrom[1];
				a1 = loadFrom[2]; b1 = loadFrom[3];
				a2 = loadFrom[4]; b2 = loadFrom[5];
			}
			else if constexpr (nreg == 4) {
				SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
				SWAPKV2(2, 4);	SWAPKV2(3, 5);
				SWAPKV2(1, 2);	SWAPKV2(3, 4);

				c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1; c[4] = a2; c[5] = b2; c[6] = a3; c[7] = b3;

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				a0 = loadFrom[0]; b0 = loadFrom[1];
				a1 = loadFrom[2]; b1 = loadFrom[3];
				a2 = loadFrom[4]; b2 = loadFrom[5];
				a3 = loadFrom[6]; b3 = loadFrom[7];
			}
			c += INC;
			loadFrom += INC;
		}
		// tail handle
		
		if constexpr (nreg == 1) {
			SWAPKV2(0, 1);
			c[0] = a0; c[1] = b0;
		}
		else if constexpr (nreg == 2) {
			SWAPKV2(0, 2); SWAPKV2(1, 3); SWAPKV2(1, 2);
			c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1;
		}
		else if constexpr (nreg == 3) {
			SWAPKV2(0, 3); SWAPKV2(1, 4); SWAPKV2(2, 5); SWAPKV2(1, 3); SWAPKV2(2, 4); SWAPKV2(2, 3);
			/*SWAPKV2(0, 4); SWAPKV2(1, 5);
			SWAPKV2(1, 3); SWAPKV2(2, 4);
			SWAPKV2(0, 1); SWAPKV2(2, 3); SWAPKV2(4, 5);*/

			c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1; c[4] = a2; c[5] = b2;
		}
		else if constexpr (nreg == 4) {
			SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
			SWAPKV2(2, 4);	SWAPKV2(3, 5);
			SWAPKV2(1, 2);	SWAPKV2(3, 4);

			c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1; c[4] = a2; c[5] = b2; c[6] = a3; c[7] = b3;
		}
		c += INC;

		i64* endOp = (loadFrom == endA) ? endB : endA;
		while (opposite != endOp) {
			_mm_prefetch((char*)(opposite + 64), _MM_HINT_T2);

			if constexpr (nreg == 1) {
				a0 = opposite[0]; b0 = opposite[1];
				SWAPKV2(0, 1);
				c[0] = a0; c[1] = b0;				
			}
			else if constexpr (nreg == 2) {
				a0 = opposite[0]; b0 = opposite[1];
				a1 = opposite[2]; b1 = opposite[3];
				SWAPKV2(0, 2); SWAPKV2(1, 3); SWAPKV2(1, 2);
				c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1;
			}
			else if constexpr (nreg == 3) {
				a0 = opposite[0]; b0 = opposite[1];
				a1 = opposite[2]; b1 = opposite[3];
				a2 = opposite[4]; b2 = opposite[5];
				SWAPKV2(0, 3); SWAPKV2(1, 4); SWAPKV2(2, 5); SWAPKV2(1, 3); SWAPKV2(2, 4); SWAPKV2(2, 3);
				/*SWAPKV2(0, 4); SWAPKV2(1, 5);
				SWAPKV2(1, 3); SWAPKV2(2, 4);
				SWAPKV2(0, 1); SWAPKV2(2, 3); SWAPKV2(4, 5);*/

				c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1; c[4] = a2; c[5] = b2;
			}
			else if constexpr (nreg == 4) {
				a0 = opposite[0]; b0 = opposite[1];
				a1 = opposite[2]; b1 = opposite[3];
				a2 = opposite[4]; b2 = opposite[5];
				a3 = opposite[6]; b3 = opposite[7];
				SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
				SWAPKV2(2, 4);	SWAPKV2(3, 5);
				SWAPKV2(1, 2);	SWAPKV2(3, 4);

				c[0] = a0; c[1] = b0; c[2] = a1; c[3] = b1; c[4] = a2; c[5] = b2; c[6] = a3; c[7] = b3;
			}

			opposite += INC;
			c += INC;
		}

		if constexpr (nreg == 1) {
			c[0] = a1; c[1] = b1;
		}
		else if constexpr (nreg == 2) {
			c[0] = a2; c[1] = b2; c[2] = a3; c[3] = b3;
		}
		else if constexpr (nreg == 3) {
			SWAPKV2(3, 5); SWAPKV2(3, 4); SWAPKV2(4, 5);
			c[0] = a3; c[1] = b3; c[2] = a4; c[3] = b4; c[4] = a5; c[5] = b5;
		}
		else if constexpr (nreg == 4) {
			SWAPKV2(4, 6); SWAPKV2(5, 7); SWAPKV2(4, 5); SWAPKV2(6, 7); SWAPKV2(5, 6);
			c[0] = a4; c[1] = b4; c[2] = a5; c[3] = b5; c[4] = a6; c[5] = b6; c[6] = a7; c[7] = b7;
		}
		c += INC;
	}

	// 2x mergebl2
	template <typename Item, ui nreg>
	FORCEINLINE void mergebl2_scalar_2x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD, Item* E, Item* F) {

		// NOTE: tail handling not added

		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = E;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = F;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;

		constexpr ui INC = nreg;

		register Item a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;

		if constexpr (nreg == 1) {
			a0 = X0[0]; a1 = Y0[0];
			a2 = X1[0]; a3 = Y1[0];
		}
		else if constexpr (nreg == 2) {
			a0 = X0[0]; a1 = X0[1]; a2 = Y0[0]; a3 = Y0[1];
			a4 = X1[0]; a5 = X1[1]; a6 = Y1[0]; a7 = Y1[1];
		}
		else if constexpr (nreg == 3) {
			a0 = X0[0]; a1 = X0[1]; a2 = X0[2]; a3 = Y0[0]; a4 = Y0[1]; a5 = Y0[2];
			a6 = X1[0]; a7 = X1[1]; a8 = X1[2]; a9 = Y1[0]; a10 = Y1[1]; a11 = Y1[2];
		}
		else if constexpr (nreg == 4) {
			a0 = X0[0]; a1 = X0[1]; a2 = X0[2]; a3 = X0[3];
			a4 = Y0[0]; a5 = Y0[1]; a6 = Y0[2]; a7 = Y0[3];

			a8 = X1[0]; a9 = X1[1]; a10 = X1[2]; a11 = X1[3];
			a12 = Y1[0]; a13 = Y1[1]; a14 = Y1[2]; a15 = Y1[3];
		}
		
		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1;
		Item* opposite0 = Y0, * opposite1 = Y1;

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1)
		{
			bool first = *loadFrom0 <= *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 <= *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			if constexpr (nreg == 1) {
				SWAP2(0, 1); SWAP2(2, 3);
				E[0] = a0;
				F[0] = a2;
				a0 = loadFrom0[0];
				a2 = loadFrom1[0];
			}
			else if constexpr (nreg == 2) {
				SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
				SWAP2(4, 6); SWAP2(5, 7); SWAP2(5, 6);

				E[0] = a0; E[1] = a1;
				F[0] = a4; F[1] = a5;

				a0 = loadFrom0[0]; a1 = loadFrom0[1];
				a4 = loadFrom1[0]; a5 = loadFrom1[1];
			}
			else if constexpr (nreg == 3) {
				SWAP2(0, 3); SWAP2(1, 4); SWAP2(2, 5); SWAP2(1, 3); SWAP2(2, 4); SWAP2(2, 3);
				SWAP2(6, 9); SWAP2(7, 10); SWAP2(8, 11); SWAP2(7, 9); SWAP2(8, 10); SWAP2(8, 9);

				E[0] = a0; E[1] = a1; E[2] = a2;
				F[0] = a6; F[1] = a7; F[2] = a8;

				a0 = loadFrom0[0]; a1 = loadFrom0[1]; a2 = loadFrom0[2];
				a6 = loadFrom1[0]; a7 = loadFrom1[1]; a8 = loadFrom1[2];
			}
			else if constexpr (nreg == 4) {
				SWAP2(0, 4); SWAP2(1, 5); SWAP2(2, 6); SWAP2(3, 7);
				SWAP2(2, 4); SWAP2(3, 5);
				SWAP2(1, 2); SWAP2(3, 4);

				SWAP2(8, 12); SWAP2(9, 13); SWAP2(10, 14); SWAP2(11, 15);
				SWAP2(10, 12); SWAP2(11, 13);
				SWAP2(9, 10); SWAP2(11, 12);

				E[0] = a0; E[1] = a1; E[2] = a2; E[3] = a3;
				F[0] = a8; F[1] = a9; F[2] = a10; F[3] = a11;

				a0 = loadFrom0[0];	a1 = loadFrom0[1];	a2 = loadFrom0[2];	a3 = loadFrom0[3];
				a8 = loadFrom1[0];	a9 = loadFrom1[1];	a10 = loadFrom1[2];	a11 = loadFrom1[3];
			}
			{
				_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
				_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
			}

			loadFrom0 += INC; loadFrom1 += INC;

			E += INC; F += INC;
		}
	}

	template <typename Item, ui nreg>
	FORCEINLINE void mergebl2_scalar_kv_2x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD, Item* E, Item* F) {

		i64* a = (i64*)A, * b = (i64*)B, * c = (i64*)C, *d = (i64*)D, * endA = (i64*)(A + lenA), * endB = (i64*)(B + lenB), * endC = (i64*)(C + lenC), * endD = (i64*)(D + lenD);
		i64* e = (i64*)E, * f = (i64*)F;
		register i64 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
		register i64 b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15;
		
		constexpr ui64 INC = (nreg << 1);

		if constexpr (nreg == 1) {
			a0 = a[0]; b0 = a[1];
			a1 = b[0]; b1 = b[1];
			a2 = c[0]; b2 = c[1];
			a3 = d[0]; b3 = d[1];
		}
		else if constexpr (nreg == 2) {
			a0 = a[0]; b0 = a[1]; a1 = a[2]; b1 = a[3];
			a2 = b[0]; b2 = b[1]; a3 = b[2]; b3 = b[3];

			a4 = c[0]; b4 = c[1]; a5 = c[2]; b5 = c[3];
			a6 = d[0]; b6 = d[1]; a7 = d[2]; b7 = d[3];
		}
		else if constexpr (nreg == 3) {
			a0 = a[0]; b0 = a[1]; a1 = a[2]; b1 = a[3]; a2 = a[4]; b2 = a[5];
			a3 = b[0]; b3 = b[1]; a4 = b[2]; b4 = b[3]; a5 = b[4]; b5 = b[5];

			a6 = c[0]; b6 = c[1]; a7 = c[2]; b7 = c[3]; a8 = c[4]; b8 = c[5];
			a9 = d[0]; b9 = d[1]; a10 = d[2]; b10 = d[3]; a11 = d[4]; b11 = d[5];
		}
		else if constexpr (nreg == 4) {
			a0 = a[0]; b0 = a[1]; a1 = a[2]; b1 = a[3]; a2 = a[4]; b2 = a[5]; a3 = a[6]; b3 = a[7];
			a4 = b[0]; b4 = b[1]; a5 = b[2]; b5 = b[3]; a6 = b[4]; b6 = b[5]; a7 = b[6]; b7 = b[7];

			a8 = c[0]; b8 = c[1]; a9 = c[2]; b9 = c[3]; a10 = c[4]; b10 = c[5]; a11 = c[6]; b11 = c[7];
			a12 = d[0]; b12 = d[1]; a13 = d[2]; b13 = d[3]; a14 = d[4]; b14 = d[5]; a15 = d[6]; b15 = d[7];
		}

		a += INC;
		b += INC;
		c += INC;
		d += INC;

		i64* loadFrom0 = a, * opposite0 = b;
		i64* loadFrom1 = a, * opposite1 = b;

		while (loadFrom0 != endA && loadFrom0 != endB && loadFrom1 != endC && loadFrom1 != endD) {
			bool first = true;
			if constexpr (nreg == 1) first = *loadFrom0 <= *opposite0;
			else if constexpr (nreg == 2) first = a1 < a3;
			else if constexpr (nreg == 3) first = a2 < a5;
			else if constexpr (nreg == 4) first = a3 < a7;

			i64* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = true;
			if constexpr (nreg == 1) first = *loadFrom1 <= *opposite1;
			else if constexpr (nreg == 2) first = a5 < a7;
			else if constexpr (nreg == 3) first = a8 < a11;
			else if constexpr (nreg == 4) first = a11 < a15;

			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			if constexpr (nreg == 1) {
				SWAPKV2(0, 1);
				SWAPKV2(2, 3);
				e[0] = a0; f[1] = b0;
				f[0] = a2; f[1] = b2;

				{
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
				}
				a0 = loadFrom0[0]; b0 = loadFrom0[1];
				a2 = loadFrom0[0]; b2 = loadFrom0[1];
			}
			else if constexpr (nreg == 2) {
				SWAPKV2(0, 2); SWAPKV2(1, 3); SWAPKV2(1, 2);
				SWAPKV2(4, 6); SWAPKV2(5, 7); SWAPKV2(5, 6);
				e[0] = a0; e[1] = b0; e[2] = a1; e[3] = b1;
				f[0] = a4; f[1] = b4; f[2] = a5; f[3] = b5;

				{
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
				}
				a0 = loadFrom0[0]; b0 = loadFrom0[1];
				a1 = loadFrom0[2]; b1 = loadFrom0[3];
				a4 = loadFrom1[0]; b4 = loadFrom1[1];
				a5 = loadFrom1[2]; b4 = loadFrom1[3];
			}
			else if constexpr (nreg == 3) {
				SWAPKV2(0, 3); SWAPKV2(1, 4); SWAPKV2(2, 5); SWAPKV2(1, 3); SWAPKV2(2, 4); SWAPKV2(2, 3);
				SWAPKV2(6, 9); SWAPKV2(7, 10); SWAPKV2(8, 11); SWAPKV2(7, 9); SWAPKV2(8, 10); SWAPKV2(8, 9);
				e[0] = a0; e[1] = b0; e[2] = a1; e[3] = b1; e[4] = a2; e[5] = b2;
				f[0] = a6; f[1] = b6; f[2] = a7; f[3] = b7; f[4] = a8; f[5] = b8;

				{
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
				}
				a0 = loadFrom0[0]; b0 = loadFrom0[1];
				a1 = loadFrom0[2]; b1 = loadFrom0[3];
				a2 = loadFrom0[4]; b2 = loadFrom0[5];

				a6 = loadFrom1[0]; b6 = loadFrom1[1];
				a7 = loadFrom1[2]; b7 = loadFrom1[3];
				a8 = loadFrom1[4]; b8 = loadFrom1[5];
			}
			else if constexpr (nreg == 4) {
				SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
				SWAPKV2(2, 4);	SWAPKV2(3, 5);
				SWAPKV2(1, 2);	SWAPKV2(3, 4);

				SWAPKV2(8, 12);	SWAPKV2(9, 13);	SWAPKV2(10, 14);	SWAPKV2(11, 15);
				SWAPKV2(10, 12); SWAPKV2(11, 13);
				SWAPKV2(9, 10);	SWAPKV2(11, 12);

				e[0] = a0; e[1] = b0; e[2] = a1; e[3] = b1; e[4] = a2; e[5] = b2; e[6] = a3; e[7] = b3;
				f[0] = a8; f[1] = b8; f[2] = a9; f[3] = b9; f[4] = a10; f[5] = b10; f[6] = a11; f[7] = b11;

				{
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
				}
				a0 = loadFrom0[0]; b0 = loadFrom0[1];
				a1 = loadFrom0[2]; b1 = loadFrom0[3];
				a2 = loadFrom0[4]; b2 = loadFrom0[5];
				a3 = loadFrom0[6]; b3 = loadFrom0[7];

				a8 = loadFrom1[0]; b8 = loadFrom1[1];
				a9 = loadFrom1[2]; b9 = loadFrom1[3];
				a10 = loadFrom1[4]; b10 = loadFrom1[5];
				a11 = loadFrom1[6]; b11 = loadFrom1[7];
			}
			e += INC;
			f += INC;
			loadFrom0 += INC;
			loadFrom1 += INC;
		}
		// handle tail
	}

	// 3x mergebl2
	template <typename Item, ui nreg>
	FORCEINLINE void mergebl2_scalar_3x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD, Item* E, ui64 lenE, Item* F, ui64 lenF,
		Item* G, Item* H, Item* I) {

		// NOTE: tail handling not added
		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = G;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = H;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;
		Item* __restrict X2 = E, * __restrict Y2 = F, * __restrict C2 = I;
		Item* __restrict endX2 = E + lenE, * __restrict endY2 = F + lenF;

		constexpr ui INC = nreg;

		register Item a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23;

		if constexpr (nreg == 1) {
			a0 = X0[0]; a1 = Y0[0];
			a2 = X1[0]; a3 = Y1[0];
			a4 = X2[0]; a5 = Y2[0];
		}
		else if constexpr (nreg == 2) {
			a0 = X0[0]; a1 = X0[1]; a2 = Y0[0]; a3 = Y0[1];
			a4 = X1[0]; a5 = X1[1]; a6 = Y1[0]; a7 = Y1[1];
			a8 = X2[0]; a9 = X2[1]; a10 = Y2[0]; a11 = Y2[1];
		}
		else if constexpr (nreg == 3) {
			a0 = X0[0]; a1 = X0[1]; a2 = X0[2];
			a3 = Y0[0]; a4 = Y0[1]; a5 = Y0[2];
			a6 = X1[0]; a7 = X1[1]; a8 = X1[2];
			a9 = Y1[0]; a10 = Y1[1]; a11 = Y1[2];
			a12 = X2[0]; a13 = X2[1]; a14 = X2[2];
			a15 = Y2[0]; a16 = Y2[1]; a17 = Y2[2];
		}
		else if constexpr (nreg == 4) {
			a0 = X0[0]; a1 = X0[1]; a2 = X0[2]; a3 = X0[3];
			a4 = Y0[0]; a5 = Y0[1]; a6 = Y0[2]; a7 = Y0[3];

			a8 = X1[0]; a9 = X1[1]; a10 = X1[2]; a11 = X1[3];
			a12 = Y1[0]; a13 = Y1[1]; a14 = Y1[2]; a15 = Y1[3];

			a16 = X2[0]; a17 = X2[1]; a18 = X2[2]; a19 = X2[3];
			a20 = Y2[0]; a21 = Y2[1]; a22 = Y2[2]; a23 = Y2[3];
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, *loadFrom2 = X2;
		Item* opposite0 = Y0, * opposite1 = Y1, *opposite2 = Y2;

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2)
		{
			bool first = *loadFrom0 <= *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 <= *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			first = *loadFrom2 <= *opposite2;
			tmp = first ? loadFrom2 : opposite2;
			opposite2 = first ? opposite2 : loadFrom2;
			loadFrom2 = tmp;

			if constexpr (nreg == 1) {
				SWAP2(0, 1); SWAP2(2, 3); SWAP2(4, 5);
				G[0] = a0;
				H[0] = a2;
				I[0] = a4;
				a0 = loadFrom0[0];
				a2 = loadFrom1[0];
				a4 = loadFrom2[0];
			}
			else if constexpr (nreg == 2) {
				SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
				SWAP2(4, 6); SWAP2(5, 7); SWAP2(5, 6);
				SWAP2(8, 10); SWAP2(9, 11); SWAP2(9, 10);

				G[0] = a0; G[1] = a1;
				H[0] = a4; H[1] = a5;
				I[0] = a8; I[1] = a9;

				a0 = loadFrom0[0]; a1 = loadFrom0[1];
				a4 = loadFrom1[0]; a5 = loadFrom1[1];
				a8 = loadFrom2[0]; a9 = loadFrom2[1];
			}
			else if constexpr (nreg == 3) {
				SWAP2(0, 3); SWAP2(1, 4); SWAP2(2, 5); SWAP2(1, 3); SWAP2(2, 4); SWAP2(2, 3);
				SWAP2(6, 9); SWAP2(7, 10); SWAP2(8, 11); SWAP2(7, 9); SWAP2(8, 10); SWAP2(8, 9);
				SWAP2(12, 15); SWAP2(13, 16); SWAP2(14, 17); SWAP2(13, 15); SWAP2(14, 16); SWAP2(14, 15);

				G[0] = a0; G[1] = a1; G[2] = a2; 
				H[0] = a6; H[1] = a7; H[2] = a8;
				I[0] = a12; I[1] = a13; I[2] = a14; 

				a0 = loadFrom0[0];	a1 = loadFrom0[1];	a2 = loadFrom0[2];	
				a6 = loadFrom1[0];	a7 = loadFrom1[1];	a8 = loadFrom1[2];
				a12 = loadFrom2[0];	a13 = loadFrom2[1];	a14 = loadFrom2[2];
			}
			else if constexpr (nreg == 4) {
				SWAP2(0, 4); SWAP2(1, 5); SWAP2(2, 6); SWAP2(3, 7);
				SWAP2(2, 4); SWAP2(3, 5);
				SWAP2(1, 2); SWAP2(3, 4);

				SWAP2(8, 12); SWAP2(9, 13); SWAP2(10, 14); SWAP2(11, 15);
				SWAP2(10, 12); SWAP2(11, 13);
				SWAP2(9, 10); SWAP2(11, 12);

				SWAP2(16, 20); SWAP2(17, 21); SWAP2(18, 22); SWAP2(19, 23);
				SWAP2(18, 20); SWAP2(19, 21);
				SWAP2(17, 18); SWAP2(19, 20);

				G[0] = a0; G[1] = a1; G[2] = a2; G[3] = a3;
				H[0] = a8; H[1] = a9; H[2] = a10; H[3] = a11;
				I[0] = a16; I[1] = a17; I[2] = a18; I[3] = a19;

				a0 = loadFrom0[0];	a1 = loadFrom0[1];	a2 = loadFrom0[2];	a3 = loadFrom0[3];
				a8 = loadFrom1[0];	a9 = loadFrom1[1];	a10 = loadFrom1[2];	a11 = loadFrom1[3];
				a16 = loadFrom2[0];	a17 = loadFrom2[1];	a18 = loadFrom2[2];	a19 = loadFrom2[3];
			}

			_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
			_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
			_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
		
			loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC;

			G += INC; H += INC; I += INC;
		}
		// handle tail
	}

	// 3x mergebl2
	template <typename Item, ui nreg>
	FORCEINLINE void mergebl2_scalar_4x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD, Item* E, ui64 lenE, Item* F, ui64 lenF, Item* G, ui64 lenG, Item* H, ui64 lenH,
		Item* I, Item* J, Item* K, Item* L) {

		// NOTE: tail handling not added
		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = I;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = J;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;
		Item* __restrict X2 = E, * __restrict Y2 = F, * __restrict C2 = K;
		Item* __restrict endX2 = E + lenE, * __restrict endY2 = F + lenF;
		Item* __restrict X3 = G, * __restrict Y3 = H, * __restrict C3 = L;
		Item* __restrict endX3 = G + lenG, * __restrict endY3 = H + lenH;

		constexpr ui INC = nreg;

		register Item a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;

		if constexpr (nreg == 1) {
			a0 = X0[0]; a1 = Y0[0];
			a2 = X1[0]; a3 = Y1[0];
			a4 = X2[0]; a5 = Y2[0];
			a6 = X3[0]; a7 = Y3[0];
		}
		else if constexpr (nreg == 2) {
			a0 = X0[0]; a1 = X0[1]; a2 = Y0[0]; a3 = Y0[1];
			a4 = X1[0]; a5 = X1[1]; a6 = Y1[0]; a7 = Y1[1];
			a8 = X2[0]; a9 = X2[1]; a10 = Y2[0]; a11 = Y2[1];
			a12 = X3[0]; a13 = X3[1]; a14 = Y3[0]; a15 = Y3[1];
		}
		else if constexpr (nreg == 3) {
			a0 = X0[0]; a1 = X0[1]; a2 = X0[2];
			a3 = Y0[0]; a4 = Y0[1]; a5 = Y0[2];
			a6 = X1[0]; a7 = X1[1]; a8 = X1[2];
			a9 = Y1[0]; a10 = Y1[1]; a11 = Y1[2];
			a12 = X2[0]; a13 = X2[1]; a14 = X2[2];
			a15 = Y2[0]; a16 = Y2[1]; a17 = Y2[2];
			a18 = X3[0]; a19 = X3[1]; a20 = X3[2];
			a21 = Y3[0]; a22 = Y3[1]; a23 = Y3[2];
		}
		else if constexpr (nreg == 4) {
			a0 = X0[0]; a1 = X0[1]; a2 = X0[2]; a3 = X0[3];
			a4 = Y0[0]; a5 = Y0[1]; a6 = Y0[2]; a7 = Y0[3];

			a8 = X1[0]; a9 = X1[1]; a10 = X1[2]; a11 = X1[3];
			a12 = Y1[0]; a13 = Y1[1]; a14 = Y1[2]; a15 = Y1[3];

			a16 = X2[0]; a17 = X2[1]; a18 = X2[2]; a19 = X2[3];
			a20 = Y2[0]; a21 = Y2[1]; a22 = Y2[2]; a23 = Y2[3];

			a24 = X3[0]; a25 = X3[1]; a26 = X3[2]; a27 = X3[3];
			a28 = Y3[0]; a29 = Y3[1]; a30 = Y3[2]; a31 = Y3[3];
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC; X3 += INC; Y3 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2, * loadFrom3 = X3;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2, * opposite3 = Y3;

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2 && loadFrom3 != endX3 && loadFrom3 != endY3)
		{
			bool first = *loadFrom0 <= *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 <= *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			first = *loadFrom2 <= *opposite2;
			tmp = first ? loadFrom2 : opposite2;
			opposite2 = first ? opposite2 : loadFrom2;
			loadFrom2 = tmp;

			first = *loadFrom3 <= *opposite3;
			tmp = first ? loadFrom3 : opposite3;
			opposite3 = first ? opposite3 : loadFrom3;
			loadFrom3 = tmp;

			if constexpr (nreg == 1) {
				SWAP2(0, 1); SWAP2(2, 3); SWAP2(4, 5); SWAP2(6, 7);
				I[0] = a0;
				J[0] = a2;
				K[0] = a4;
				L[0] = a6;
				a0 = loadFrom0[0];
				a2 = loadFrom1[0];
				a4 = loadFrom2[0];
				a6 = loadFrom3[0];
			}
			else if constexpr (nreg == 2) {
				SWAP2(0, 2); SWAP2(1, 3); SWAP2(1, 2);
				SWAP2(4, 6); SWAP2(5, 7); SWAP2(5, 6);
				SWAP2(8, 10); SWAP2(9, 11); SWAP2(9, 10);
				SWAP2(12, 14); SWAP2(13, 15); SWAP2(11, 12);

				I[0] = a0; I[1] = a1;
				J[0] = a4; J[1] = a5;
				K[0] = a8; K[1] = a9;
				L[0] = a12; L[1] = a13;

				a0 = loadFrom0[0]; a1 = loadFrom0[1];
				a4 = loadFrom1[0]; a5 = loadFrom1[1];
				a8 = loadFrom2[0]; a9 = loadFrom2[1];
				a12 = loadFrom3[0]; a13 = loadFrom3[1];
			}
			else if constexpr (nreg == 3) {
				SWAP2(0, 3); SWAP2(1, 4); SWAP2(2, 5); SWAP2(1, 3); SWAP2(2, 4); SWAP2(2, 3);
				SWAP2(6, 9); SWAP2(7, 10); SWAP2(8, 11); SWAP2(7, 9); SWAP2(8, 10); SWAP2(8, 9);
				SWAP2(12, 15); SWAP2(13, 16); SWAP2(14, 17); SWAP2(13, 15); SWAP2(14, 16); SWAP2(14, 15);
				SWAP2(18, 21); SWAP2(19, 22); SWAP2(20, 23); SWAP2(19, 21); SWAP2(20, 22); SWAP2(20, 21);

				I[0] = a0; I[1] = a1; I[2] = a2;
				J[0] = a6; J[1] = a7; J[2] = a8;
				K[0] = a12; K[1] = a13; K[2] = a14;
				L[0] = a18; L[1] = a19; L[2] = a20;

				a0 = loadFrom0[0];	a1 = loadFrom0[1];	a2 = loadFrom0[2];
				a6 = loadFrom1[0];	a7 = loadFrom1[1];	a8 = loadFrom1[2];
				a12 = loadFrom2[0];	a13 = loadFrom2[1];	a14 = loadFrom2[2];
				a18 = loadFrom3[0];	a19 = loadFrom3[1];	a20 = loadFrom3[2];
			}
			else if constexpr (nreg == 4) {
				SWAP2(0, 4); SWAP2(1, 5); SWAP2(2, 6); SWAP2(3, 7);
				SWAP2(2, 4); SWAP2(3, 5);
				SWAP2(1, 2); SWAP2(3, 4);

				SWAP2(8, 12); SWAP2(9, 13); SWAP2(10, 14); SWAP2(11, 15);
				SWAP2(10, 12); SWAP2(11, 13);
				SWAP2(9, 10); SWAP2(11, 12);

				SWAP2(16, 20); SWAP2(17, 21); SWAP2(18, 22); SWAP2(19, 23);
				SWAP2(18, 20); SWAP2(19, 21);
				SWAP2(17, 18); SWAP2(19, 20);

				SWAP2(24, 28); SWAP2(25, 29); SWAP2(26, 30); SWAP2(27, 31);
				SWAP2(26, 28); SWAP2(27, 29);
				SWAP2(25, 26); SWAP2(27, 28);

				I[0] = a0; I[1] = a1; I[2] = a2; I[3] = a3;
				J[0] = a8; J[1] = a9; J[2] = a10; J[3] = a11;
				K[0] = a16; K[1] = a17; K[2] = a18; K[3] = a19;
				L[0] = a24; L[1] = a25; L[2] = a26; L[3] = a27;

				a0 = loadFrom0[0];	a1 = loadFrom0[1];	a2 = loadFrom0[2];	a3 = loadFrom0[3];
				a8 = loadFrom1[0];	a9 = loadFrom1[1];	a10 = loadFrom1[2];	a11 = loadFrom1[3];
				a16 = loadFrom2[0];	a17 = loadFrom2[1];	a18 = loadFrom2[2];	a19 = loadFrom2[3];
				a24 = loadFrom3[0];	a25 = loadFrom3[1];	a26 = loadFrom3[2];	a27 = loadFrom3[3];
			}

			_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
			_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
			_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
			_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);

			loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC; loadFrom3 += INC;

			I += INC; J += INC; K += INC; L += INC;
		}
		// handle tail
	}


	// with tail handling + correctness verified
	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C) {

		Reg* a = (Reg*)A, * b = (Reg*)B, * endA = (Reg*)(A + lenA), * endB = (Reg*)(B + lenB), * c = (Reg*)C;
		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;


		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); 

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, a); 
			origami_utils::load<Reg>(a1, b); 
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, a); origami_utils::load<Reg>(a1, a + 1);
			origami_utils::load<Reg>(a2, b); origami_utils::load<Reg>(a3, b + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, a); origami_utils::load<Reg>(a1, a + 1); origami_utils::load<Reg>(a2, a + 2);
			origami_utils::load<Reg>(a3, b); origami_utils::load<Reg>(a4, b + 1); origami_utils::load<Reg>(a5, b + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, a); origami_utils::load<Reg>(a1, a + 1); origami_utils::load<Reg>(a2, a + 2); origami_utils::load<Reg>(a3, a + 3); 
			origami_utils::load<Reg>(a4, b); origami_utils::load<Reg>(a5, b + 1); origami_utils::load<Reg>(a6, b + 2); origami_utils::load<Reg>(a7, b + 3);
		}
		else if constexpr (nreg == 5) {
			origami_utils::load<Reg>(a0, a); origami_utils::load<Reg>(a1, a + 1); origami_utils::load<Reg>(a2, a + 2); origami_utils::load<Reg>(a3, a + 3); origami_utils::load<Reg>(a4, a + 4);
			origami_utils::load<Reg>(a5, b); origami_utils::load<Reg>(a6, b + 1); origami_utils::load<Reg>(a7, b + 2); origami_utils::load<Reg>(a8, b + 3); origami_utils::load<Reg>(a9, b + 4);
		}

		a += nreg;
		b += nreg;	

		Reg* loadFrom = a, * opposite = b;

		while (loadFrom != endA && loadFrom != endB) {
			bool first = *(Item*)loadFrom < *(Item*)opposite;

			Reg* tmp = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp;

			if constexpr (nreg == 1) {
				// irl_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, c);
				
				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, loadFrom);
			}
			else if constexpr (nreg == 2) {
				//irl_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);
				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, loadFrom); origami_utils::load<Reg>(a1, loadFrom + 1);
			}
			else if constexpr (nreg == 3) {
				origami_utils::rswap<Reg, Item>(a0, a3); origami_utils::rswap<Reg, Item>(a1, a4); origami_utils::rswap<Reg, Item>(a2, a5);
				origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a2, a3);
				/*origami_utils::rswap<Reg, Item>(a0, a4); origami_utils::rswap<Reg, Item>(a1, a5);
				origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4);
				origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5);*/

				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1); origami_utils::store<Reg, stream>(a2, c + 2);

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, loadFrom); origami_utils::load<Reg>(a1, loadFrom + 1); origami_utils::load<Reg>(a2, loadFrom + 2);
			}
			else if constexpr (nreg == 4) {
				//irl_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_utils::rswap<Reg, Item>(a0, a4);	origami_utils::rswap<Reg, Item>(a1, a5);	origami_utils::rswap<Reg, Item>(a2, a6);	origami_utils::rswap<Reg, Item>(a3, a7);
				origami_utils::rswap<Reg, Item>(a2, a4);	origami_utils::rswap<Reg, Item>(a3, a5);
				origami_utils::rswap<Reg, Item>(a1, a2);	origami_utils::rswap<Reg, Item>(a3, a4);
				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);
				origami_utils::store<Reg, stream>(a2, c + 2); origami_utils::store<Reg, stream>(a3, c + 3);

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, loadFrom); origami_utils::load<Reg>(a1, loadFrom + 1);
				origami_utils::load<Reg>(a2, loadFrom + 2); origami_utils::load<Reg>(a3, loadFrom + 3);
			}
			else if constexpr (nreg == 5) {
				// irl_merge_network::merge_network10<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9);
				origami_utils::rswap<Reg, Item>(a0, a8); origami_utils::rswap<Reg, Item>(a1, a9); origami_utils::rswap<Reg, Item>(a2, a6); origami_utils::rswap<Reg, Item>(a3, a7);
				origami_utils::rswap<Reg, Item>(a1, a5); origami_utils::rswap<Reg, Item>(a4, a8); origami_utils::rswap<Reg, Item>(a0, a2);
				origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a3, a5); origami_utils::rswap<Reg, Item>(a7, a9);
				origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5); origami_utils::rswap<Reg, Item>(a6, a7);
				origami_utils::rswap<Reg, Item>(a8, a9);

				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);
				origami_utils::store<Reg, stream>(a2, c + 2); origami_utils::store<Reg, stream>(a3, c + 3); origami_utils::store<Reg, stream>(a4, c + 4);

				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, loadFrom); origami_utils::load<Reg>(a1, loadFrom + 1);
				origami_utils::load<Reg>(a2, loadFrom + 2); origami_utils::load<Reg>(a3, loadFrom + 3); origami_utils::load<Reg>(a4, loadFrom + 4);
			}

			c += nreg;
			loadFrom += nreg;
		}
		// tail handle
		if constexpr (nreg == 1) {
			// irl_merge_network::merge_network2<Reg, Item>(a0, a1);
			origami_utils::rswap<Reg, Item>(a0, a1);
			origami_utils::store<Reg, stream>(a0, c);
		}
		else if constexpr (nreg == 2) {
			//irl_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
			origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
			origami_utils::rswap<Reg, Item>(a1, a2);

			origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::rswap<Reg, Item>(a0, a3); origami_utils::rswap<Reg, Item>(a1, a4); origami_utils::rswap<Reg, Item>(a2, a5);
			origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a2, a3);
			/*origami_utils::rswap<Reg, Item>(a0, a4); origami_utils::rswap<Reg, Item>(a1, a5);
			origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4);
			origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5);*/

			origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1); origami_utils::store<Reg, stream>(a2, c + 2);
		}
		else if constexpr (nreg == 4) {
			//irl_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
			origami_utils::rswap<Reg, Item>(a0, a4);	origami_utils::rswap<Reg, Item>(a1, a5);	origami_utils::rswap<Reg, Item>(a2, a6);	origami_utils::rswap<Reg, Item>(a3, a7);
			origami_utils::rswap<Reg, Item>(a2, a4);	origami_utils::rswap<Reg, Item>(a3, a5);
			origami_utils::rswap<Reg, Item>(a1, a2);	origami_utils::rswap<Reg, Item>(a3, a4);

			origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);
			origami_utils::store<Reg, stream>(a2, c + 2); origami_utils::store<Reg, stream>(a3, c + 3);
		}
		c += nreg;

		Reg* endOp = (loadFrom == endA) ? endB : endA;

		
		while (opposite != endOp) {
			_mm_prefetch((char*)(opposite + 64), _MM_HINT_T2);

			if constexpr (nreg == 1) {
				origami_utils::load<Reg>(a0, opposite);
				// irl_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_utils::rswap<Reg, Item>(a0, a1);

				origami_utils::store<Reg, stream>(a0, c);
			}
			else if constexpr (nreg == 2) {
				origami_utils::load<Reg>(a0, opposite); origami_utils::load<Reg>(a1, opposite + 1);

				//irl_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);

				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);
			}
			else if constexpr (nreg == 3) {
				origami_utils::load<Reg>(a0, opposite); origami_utils::load<Reg>(a1, opposite + 1); origami_utils::load<Reg>(a2, opposite + 2);
				origami_utils::rswap<Reg, Item>(a0, a3); origami_utils::rswap<Reg, Item>(a1, a4); origami_utils::rswap<Reg, Item>(a2, a5);
				origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a2, a3);
				/*origami_utils::rswap<Reg, Item>(a0, a4); origami_utils::rswap<Reg, Item>(a1, a5);
				origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4);
				origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5);*/

				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1); origami_utils::store<Reg, stream>(a2, c + 2);
			}
			else if constexpr (nreg == 4) {
				origami_utils::load<Reg>(a0, opposite); origami_utils::load<Reg>(a1, opposite + 1);
				origami_utils::load<Reg>(a2, opposite + 2); origami_utils::load<Reg>(a3, opposite + 3);

				// irl_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_utils::rswap<Reg, Item>(a0, a4);	origami_utils::rswap<Reg, Item>(a1, a5);	origami_utils::rswap<Reg, Item>(a2, a6);	origami_utils::rswap<Reg, Item>(a3, a7);
				origami_utils::rswap<Reg, Item>(a2, a4);	origami_utils::rswap<Reg, Item>(a3, a5);
				origami_utils::rswap<Reg, Item>(a1, a2);	origami_utils::rswap<Reg, Item>(a3, a4);

				origami_utils::store<Reg, stream>(a0, c); origami_utils::store<Reg, stream>(a1, c + 1);
				origami_utils::store<Reg, stream>(a2, c + 2); origami_utils::store<Reg, stream>(a3, c + 3);
			}
			opposite += nreg;
			c += nreg;
		}

		if constexpr (nreg == 1) {
			origami_utils::store<Reg, stream>(a1, c);
		}
		else if constexpr (nreg == 2) {
			origami_utils::store<Reg, stream>(a2, c); origami_utils::store<Reg, stream>(a3, c + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::rswap<Reg, Item>(a3, a5); origami_utils::rswap<Reg, Item>(a3, a4); origami_utils::rswap<Reg, Item>(a4, a5);
			origami_utils::store<Reg, stream>(a3, c); origami_utils::store<Reg, stream>(a4, c + 1); origami_utils::store<Reg, stream>(a5, c + 2);
		}
		else if constexpr (nreg == 4) {
			// irl_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
			origami_utils::rswap<Reg, Item>(a5, a6);	// to fix the last set order
			origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a5, a7);
			origami_utils::rswap<Reg, Item>(a5, a7);

			origami_utils::store<Reg, stream>(a4, c); origami_utils::store<Reg, stream>(a5, c + 1);
			origami_utils::store<Reg, stream>(a6, c + 2); origami_utils::store<Reg, stream>(a7, c + 3);
		}
		c += nreg;
	}
	
	// 2x mergebl2
	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_2x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD, Item* E, Item* F) {

		// ** NOTE: tail handling not added for nreg = 4

		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = E;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = F;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;
		
		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1); origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1); origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1); origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3); 
			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1); origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1;
		Item* opposite0 = Y0, * opposite1 = Y1;

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1)
		{
			bool first = *loadFrom0 < *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 < *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			if constexpr (nreg == 1) {
				origami_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_merge_network::merge_network2<Reg, Item>(a2, a3);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
			}
			else if constexpr (nreg == 2) {
				origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom1);	origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
				origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
				origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
				origami_utils::load<Reg>(a6, (Reg*)loadFrom1);	origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
				origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom1);	origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);	origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);
			}

			_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
			_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);

			loadFrom0 += INC; loadFrom1 += INC;

			C0 += INC; C1 += INC;
		}
		/*printf("LF0: %llX, OP0: %llX, ENDX0: %llX, ENDY0: %llX\n", loadFrom0, opposite0, endX0, endY0);
		printf("LF1: %llX, OP1: %llX, ENDX1: %llX, ENDY1: %llX\n", loadFrom1, opposite1, endX1, endY1);
		return;*/

		// handle remaining pairs of merges
		if (loadFrom0 == endX0 || loadFrom0 == endY0) {
			//printf("0 finished first\n");
			if constexpr (nreg == 1) {
				origami_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
			}
			else if constexpr (nreg == 2) {
				origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
			}
			C0 += INC;
			Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
			
			
			
			while (opposite0 != endOpposite0) {
				_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
				if constexpr (nreg == 1) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0);
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				}
				else if constexpr (nreg == 2) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1);
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2);
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)opposite0 + 3);
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
				}
				opposite0 += INC;
				C0 += INC;
			}
			if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a1, (Reg*)C0);
			else if constexpr (nreg == 2) {
				origami_utils::store<Reg, stream>(a2, (Reg*)C0);
				origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_utils::store<Reg, stream>(a3, (Reg*)C0);
				origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
				origami_utils::store<Reg, stream>(a4, (Reg*)C0);
				origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 2);
				origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 3);
			}
			C0 += INC;
			
			while (loadFrom1 != endX1 && loadFrom1 != endY1) {
				Item comp1 = *loadFrom1;
				Item comp2 = *opposite1;
				Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
				opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				if constexpr (nreg == 1) {
					origami_merge_network::merge_network2<Reg, Item>(a2, a3);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				}
				else if constexpr (nreg == 2) {
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
					origami_utils::load<Reg>(a4, (Reg*)loadFrom1);	origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
					origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
					origami_utils::load<Reg>(a6, (Reg*)loadFrom1);	origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom1);	origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);	origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);
				}
				loadFrom1 += INC;
				C1 += INC;
			}
			if constexpr (nreg == 1) {
				origami_merge_network::merge_network2<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
			}
			else if constexpr (nreg == 2) {
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
				origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
				origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
				origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
			}
			C1 += INC;
			Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;

			
			
			while (opposite1 != endOpposite1) {
				_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
				if constexpr (nreg == 1) {
					origami_utils::load<Reg>(a2, (Reg*)opposite1);
					origami_merge_network::merge_network2<Reg, Item>(a2, a3);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				}
				else if constexpr (nreg == 2) {
					origami_utils::load<Reg>(a4, (Reg*)opposite1); origami_utils::load<Reg>(a5, (Reg*)opposite1 + 1);
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_utils::load<Reg>(a6, (Reg*)opposite1); origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a8, (Reg*)opposite1 + 2);
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
					origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_utils::load<Reg>(a8, (Reg*)opposite1); origami_utils::load<Reg>(a9, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a10, (Reg*)opposite1 + 2); origami_utils::load<Reg>(a11, (Reg*)opposite1 + 3);
					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
				}
				opposite1 += INC;
				C1 += INC;
			}
			if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a3, (Reg*)C1);
			else if constexpr (nreg == 2) {
				origami_utils::store<Reg, stream>(a6, (Reg*)C1);
				origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_utils::store<Reg, stream>(a9, (Reg*)C1);
				origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network4<Reg, Item>(a12, a13, a14, a15);
				origami_utils::store<Reg, stream>(a12, (Reg*)C1);
				origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 2);
				origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 3);
			}
			C1 += INC;
		}
		else if (loadFrom1 == endX1 || loadFrom1 == endY1) {
			//printf("1 finished first\n");
			if constexpr (nreg == 1) {
				origami_merge_network::merge_network2<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
			}
			else if constexpr (nreg == 2) {
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
				origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
				origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
				origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
			}
			C1 += INC;
			Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;

			
			
			while (opposite1 != endOpposite1) {
				_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
				if constexpr (nreg == 1) {
					origami_utils::load<Reg>(a2, (Reg*)opposite1);
					origami_merge_network::merge_network2<Reg, Item>(a2, a3);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				}
				else if constexpr (nreg == 2) {
					origami_utils::load<Reg>(a4, (Reg*)opposite1); origami_utils::load<Reg>(a5, (Reg*)opposite1 + 1);
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_utils::load<Reg>(a6, (Reg*)opposite1); origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a8, (Reg*)opposite1 + 2);
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
					origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_utils::load<Reg>(a8, (Reg*)opposite1); origami_utils::load<Reg>(a9, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a10, (Reg*)opposite1 + 2); 	origami_utils::load<Reg>(a11, (Reg*)opposite1 + 3);

					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
				}
				opposite1 += INC;
				C1 += INC;
			}
			if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a3, (Reg*)C1);
			else if constexpr (nreg == 2) {
				origami_utils::store<Reg, stream>(a6, (Reg*)C1);
				origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_utils::store<Reg, stream>(a9, (Reg*)C1);
				origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network4<Reg, Item>(a12, a13, a14, a15);
				origami_utils::store<Reg, stream>(a12, (Reg*)C1);
				origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 2);
				origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 3);
			}
			C1 += INC;

			while (loadFrom0 != endX0 && loadFrom0 != endY0) {
				Item comp1 = *loadFrom0;
				Item comp2 = *opposite0;
				Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
				opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				if constexpr (nreg == 1) {
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				}
				else if constexpr (nreg == 2) {
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
				}
				loadFrom0 += INC;
				C0 += INC;
			}
			if constexpr (nreg == 1) {
				origami_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
			}
			else if constexpr (nreg == 2) {
				origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
			}
			C0 += INC;
			Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;

			while (opposite0 != endOpposite0) {
				_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
				if constexpr (nreg == 1) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0);
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				}
				else if constexpr (nreg == 2) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1);
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2);
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1);
					origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2); origami_utils::load<Reg>(a3, (Reg*)opposite0 + 3);
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
				}
				opposite0 += INC;
				C0 += INC;
			}
			if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a1, (Reg*)C0);
			else if constexpr (nreg == 2) {
				origami_utils::store<Reg, stream>(a2, (Reg*)C0);
				origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_utils::store<Reg, stream>(a3, (Reg*)C0);
				origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
				origami_utils::store<Reg, stream>(a4, (Reg*)C0); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 3);
			}
			C0 += INC;
		}
	}

	// 3x mergebl2
	// we only try for 1 reg per stream; it seems for unrolling a shorter sorting network is better
	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_3x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD,
		Item* E, ui64 lenE, Item* F, ui64 lenF, Item* G, Item* H, Item* I) {
		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = G;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = H;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;
		Item* __restrict X2 = E, * __restrict Y2 = F, * __restrict C2 = I;
		Item* __restrict endX2 = E + lenE, * __restrict endY2 = F + lenF;

		
		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23;
		
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);
		origami_utils::init_reg<Reg>(a20); origami_utils::init_reg<Reg>(a21); origami_utils::init_reg<Reg>(a22); origami_utils::init_reg<Reg>(a23);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
			origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a8, (Reg*)X2); origami_utils::load<Reg>(a9, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a10, (Reg*)Y2); origami_utils::load<Reg>(a11, (Reg*)Y2 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0);		origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0);		origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1);		origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1);		origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
			origami_utils::load<Reg>(a12, (Reg*)X2);	origami_utils::load<Reg>(a13, (Reg*)X2 + 1);	origami_utils::load<Reg>(a14, (Reg*)X2 + 2);
			origami_utils::load<Reg>(a15, (Reg*)Y2);	origami_utils::load<Reg>(a16, (Reg*)Y2 + 1);	origami_utils::load<Reg>(a17, (Reg*)Y2 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);

			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);

			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);

			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);

			origami_utils::load<Reg>(a16, (Reg*)X2); origami_utils::load<Reg>(a17, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a18, (Reg*)X2 + 2); origami_utils::load<Reg>(a19, (Reg*)X2 + 3);

			origami_utils::load<Reg>(a20, (Reg*)Y2); origami_utils::load<Reg>(a21, (Reg*)Y2 + 1);
			origami_utils::load<Reg>(a22, (Reg*)Y2 + 2); origami_utils::load<Reg>(a23, (Reg*)Y2 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2;

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2)
		{
			bool first = *loadFrom0 < *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 < *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			first = *loadFrom2 < *opposite2;
			tmp = first ? loadFrom2 : opposite2;
			opposite2 = first ? opposite2 : loadFrom2;
			loadFrom2 = tmp;

			if constexpr (nreg == 1) {
				//origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5);
				origami_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_merge_network::merge_network2<Reg, Item>(a2, a3);
				origami_merge_network::merge_network2<Reg, Item>(a4, a5);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);

				if constexpr (stream) {
					if constexpr (sizeof(Reg) == 64) {		// AVX-512
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}
				}

				

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a2, (Reg*)loadFrom1); origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
			}
			else if constexpr (nreg == 2) {
				/*origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);

				origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a5, a7);
				origami_utils::rswap<Reg, Item>(a5, a6);

				origami_utils::rswap<Reg, Item>(a8, a10); origami_utils::rswap<Reg, Item>(a9, a11);
				origami_utils::rswap<Reg, Item>(a9, a10);*/

				origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
				origami_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);


				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C1);
				origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a8, (Reg*)C2);
				origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom2);
				origami_utils::load<Reg>(a9, (Reg*)loadFrom2 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
				origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
				origami_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);

				origami_utils::store<Reg, stream>(a6, (Reg*)C1);
				origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

				origami_utils::store<Reg, stream>(a12, (Reg*)C2);
				origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1);
				origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);

				origami_utils::load<Reg>(a6, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);

				origami_utils::load<Reg>(a12, (Reg*)loadFrom2);
				origami_utils::load<Reg>(a13, (Reg*)loadFrom2 + 1);
				origami_utils::load<Reg>(a14, (Reg*)loadFrom2 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
				origami_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);


				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
				origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);

				origami_utils::store<Reg, stream>(a8, (Reg*)C1);
				origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2);
				origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

				origami_utils::store<Reg, stream>(a16, (Reg*)C2);
				origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
				origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2);
				origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
				origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);

				origami_utils::load<Reg>(a8, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1);
				origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);
				origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);

				origami_utils::load<Reg>(a16, (Reg*)loadFrom2);
				origami_utils::load<Reg>(a17, (Reg*)loadFrom2 + 1);
				origami_utils::load<Reg>(a18, (Reg*)loadFrom2 + 2);
				origami_utils::load<Reg>(a19, (Reg*)loadFrom2 + 3);
			}

			loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC;
			C0 += INC; C1 += INC; C2 += INC;
		}
		// tail handle --> only works for nreg == 1
		if (loadFrom0 == endX0 || loadFrom0 == endY0) {
			//printf("0 finished first\n");
			origami_utils::rswap<Reg, Item>(a0, a1);
			origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
			Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;

			
			while (opposite0 != endOpposite0) {
				_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, (Reg*)opposite0);
				opposite0 += INC;
				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
			}
			origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;
						
			// process rest of the two
			while (loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2) {
				Item comp1 = *loadFrom1;
				Item comp2 = *opposite1;
				Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
				opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				comp1 = *loadFrom2;
				comp2 = *opposite2;
				tmp = comp1 < comp2 ? loadFrom2 : opposite2;
				opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
				loadFrom2 = tmp;

				origami_utils::rswap<Reg, Item>(a2, a3);
				origami_utils::rswap<Reg, Item>(a4, a5);

				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);

				origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

				_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
				_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

				loadFrom1 += INC;
				loadFrom2 += INC;

				C1 += INC;
				C2 += INC;
			}

			while (loadFrom1 != endX1 && loadFrom1 != endY1) {
				Item comp1 = *loadFrom1;
				Item comp2 = *opposite1;
				Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
				opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				origami_utils::rswap<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				loadFrom1 += INC;
				C1 += INC;
			}
			origami_utils::rswap<Reg, Item>(a2, a3);
			origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
			Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;

			while (opposite1 != endOpposite1) {
				_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a2, (Reg*)opposite1);
				opposite1 += INC;
				origami_utils::rswap<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
			}
			origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;

			while (loadFrom2 != endX2 && loadFrom2 != endY2) {
				Item comp1 = *loadFrom2;
				Item comp2 = *opposite2;
				Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
				opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
				loadFrom2 = tmp;

				origami_utils::rswap<Reg, Item>(a4, a5);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
				loadFrom2 += INC;
				C2 += INC;
			}
			origami_utils::rswap<Reg, Item>(a4, a5);
			origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
			Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;

			while (opposite2 != endOpposite2) {
				_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a4, (Reg*)opposite2);
				opposite2 += INC;
				origami_utils::rswap<Reg, Item>(a4, a5);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
			}
			origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;
		}
		else if (loadFrom1 == endX1 || loadFrom1 == endY1) {
			//printf("1 finished first\n");
			origami_utils::rswap<Reg, Item>(a2, a3);
			origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
			Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;

			while (opposite1 != endOpposite1) {
				_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a2, (Reg*)opposite1);
				opposite1 += INC;
				origami_utils::rswap<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
			}
			origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;

			// process rest of the two
			while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom2 != endX2 && loadFrom2 != endY2) {
				Item comp1 = *loadFrom0;
				Item comp2 = *opposite0;
				Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
				opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				comp1 = *loadFrom2;
				comp2 = *opposite2;
				tmp = comp1 < comp2 ? loadFrom2 : opposite2;
				opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
				loadFrom2 = tmp;

				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::rswap<Reg, Item>(a4, a5);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

				_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
				_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

				loadFrom0 += INC;
				loadFrom2 += INC;

				C0 += INC;
				C2 += INC;

			}

			while (loadFrom0 != endX0 && loadFrom0 != endY0) {
				Item comp1 = *loadFrom0;
				Item comp2 = *opposite0;
				Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
				opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				loadFrom0 += INC;
				C0 += INC;
			}
			origami_utils::rswap<Reg, Item>(a0, a1);
			origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
			Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;

			while (opposite0 != endOpposite0) {
				_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, (Reg*)opposite0);
				opposite0 += INC;
				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
			}
			origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;

			while (loadFrom2 != endX2 && loadFrom2 != endY2) {
				Item comp1 = *loadFrom2;
				Item comp2 = *opposite2;
				Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
				opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
				loadFrom2 = tmp;

				origami_utils::rswap<Reg, Item>(a4, a5);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
				loadFrom2 += INC;
				C2 += INC;
			}
			origami_utils::rswap<Reg, Item>(a4, a5);
			origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
			Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;

			while (opposite2 != endOpposite2) {
				_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a4, (Reg*)opposite2);
				opposite2 += INC;
				origami_utils::rswap<Reg, Item>(a4, a5);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
			}
			origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;
		}
		else if (loadFrom2 == endX2 || loadFrom2 == endY2) {
			//printf("2 finished first\n");
			origami_utils::rswap<Reg, Item>(a4, a5);
			origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
			Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;

			while (opposite2 != endOpposite2) {
				_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a4, (Reg*)opposite2);
				opposite2 += INC;
				origami_utils::rswap<Reg, Item>(a4, a5);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
			}
			origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;

			// process rest of the two
			while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1) {
				Item comp1 = *loadFrom0;
				Item comp2 = *opposite0;
				Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
				opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				comp1 = *loadFrom1;
				comp2 = *opposite1;
				tmp = comp1 < comp2 ? loadFrom1 : opposite1;
				opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::rswap<Reg, Item>(a2, a3);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom1);

				_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
				_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);

				loadFrom0 += INC;
				loadFrom1 += INC;

				C0 += INC;
				C1 += INC;
			}
			while (loadFrom0 != endX0 && loadFrom0 != endY0) {
				Item comp1 = *loadFrom0;
				Item comp2 = *opposite0;
				Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
				opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				loadFrom0 += INC;
				C0 += INC;
			}
			origami_utils::rswap<Reg, Item>(a0, a1);
			origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
			Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;

			while (opposite0 != endOpposite0) {
				_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a0, (Reg*)opposite0);
				opposite0 += INC;
				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
			}
			origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;

			while (loadFrom1 != endX1 && loadFrom1 != endY1) {
				Item comp1 = *loadFrom1;
				Item comp2 = *opposite1;
				Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
				opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				origami_utils::rswap<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				loadFrom1 += INC;
				C1 += INC;
			}
			origami_utils::rswap<Reg, Item>(a2, a3);
			origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
			Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;

			while (opposite1 != endOpposite1) {
				_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
				origami_utils::load<Reg>(a2, (Reg*)opposite1);
				opposite1 += INC;
				origami_utils::rswap<Reg, Item>(a2, a3);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
			}
			origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;
		}
	}

	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_4x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD,
		Item* E, ui64 lenE, Item* F, ui64 lenF, Item* G, ui64 lenG, Item* H, ui64 lenH, 
		Item* I, Item* J, Item* K, Item* L) {
		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = I;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = J;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;
		Item* __restrict X2 = E, * __restrict Y2 = F, * __restrict C2 = K;
		Item* __restrict endX2 = E + lenE, * __restrict endY2 = F + lenF;
		Item* __restrict X3 = G, * __restrict Y3 = H, * __restrict C3 = L;
		Item* __restrict endX3 = G + lenG, * __restrict endY3 = H + lenH;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);
		origami_utils::init_reg<Reg>(a20); origami_utils::init_reg<Reg>(a21); origami_utils::init_reg<Reg>(a22); origami_utils::init_reg<Reg>(a23);
		origami_utils::init_reg<Reg>(a24); origami_utils::init_reg<Reg>(a25); origami_utils::init_reg<Reg>(a26); origami_utils::init_reg<Reg>(a27);
		origami_utils::init_reg<Reg>(a28); origami_utils::init_reg<Reg>(a29); origami_utils::init_reg<Reg>(a30); origami_utils::init_reg<Reg>(a31);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
			origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
			origami_utils::load<Reg>(a6, (Reg*)X3); origami_utils::load<Reg>(a7, (Reg*)Y3);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a8, (Reg*)X2); origami_utils::load<Reg>(a9, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a10, (Reg*)Y2); origami_utils::load<Reg>(a11, (Reg*)Y2 + 1);
			origami_utils::load<Reg>(a12, (Reg*)X3); origami_utils::load<Reg>(a13, (Reg*)X3 + 1);
			origami_utils::load<Reg>(a14, (Reg*)Y3); origami_utils::load<Reg>(a15, (Reg*)Y3 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0);		origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0);		origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1);		origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1);		origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
			origami_utils::load<Reg>(a12, (Reg*)X2);	origami_utils::load<Reg>(a13, (Reg*)X2 + 1);	origami_utils::load<Reg>(a14, (Reg*)X2 + 2);
			origami_utils::load<Reg>(a15, (Reg*)Y2);	origami_utils::load<Reg>(a16, (Reg*)Y2 + 1);	origami_utils::load<Reg>(a17, (Reg*)Y2 + 2);
			origami_utils::load<Reg>(a18, (Reg*)X3);	origami_utils::load<Reg>(a19, (Reg*)X3 + 1);	origami_utils::load<Reg>(a20, (Reg*)X3 + 2);
			origami_utils::load<Reg>(a21, (Reg*)Y3);	origami_utils::load<Reg>(a22, (Reg*)Y3 + 1);	origami_utils::load<Reg>(a23, (Reg*)Y3 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);

			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);
			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);

			origami_utils::load<Reg>(a16, (Reg*)X2); origami_utils::load<Reg>(a17, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a18, (Reg*)X2 + 2); origami_utils::load<Reg>(a19, (Reg*)X2 + 3);
			origami_utils::load<Reg>(a20, (Reg*)Y2); origami_utils::load<Reg>(a21, (Reg*)Y2 + 1);
			origami_utils::load<Reg>(a22, (Reg*)Y2 + 2); origami_utils::load<Reg>(a23, (Reg*)Y2 + 3);

			origami_utils::load<Reg>(a24, (Reg*)X3); origami_utils::load<Reg>(a25, (Reg*)X3 + 1);
			origami_utils::load<Reg>(a26, (Reg*)X3 + 2); origami_utils::load<Reg>(a27, (Reg*)X3 + 3);
			origami_utils::load<Reg>(a28, (Reg*)Y3); origami_utils::load<Reg>(a29, (Reg*)Y3 + 1);
			origami_utils::load<Reg>(a30, (Reg*)Y3 + 2); origami_utils::load<Reg>(a31, (Reg*)Y3 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC; X3 += INC; Y3 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2, * loadFrom3 = X3;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2, * opposite3 = Y3;

		//origami_utils::transpose<Item, Reg>(a1, a3, a5, a7);

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2 && loadFrom3 != endX3 && loadFrom3 != endY3)
		{
			bool first = *loadFrom0 < *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 < *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			first = *loadFrom2 < *opposite2;
			tmp = first ? loadFrom2 : opposite2;
			opposite2 = first ? opposite2 : loadFrom2;
			loadFrom2 = tmp;

			first = *loadFrom3 < *opposite3;
			tmp = first ? loadFrom3 : opposite3;
			opposite3 = first ? opposite3 : loadFrom3;
			loadFrom3 = tmp;

			if constexpr (nreg == 1) {
				//origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5);
				origami_merge_network::merge_network2<Reg, Item>(a0, a1);
				origami_merge_network::merge_network2<Reg, Item>(a2, a3);
				origami_merge_network::merge_network2<Reg, Item>(a4, a5);
				origami_merge_network::merge_network2<Reg, Item>(a6, a7);

				/*origami_utils::transpose<Item, Reg>(a0, a2, a4, a6);
				origami_utils::cswap<Reg, Item>(a0, a2, a4, a6, a1, a3, a5, a7);
				origami_utils::transpose<Item, Reg>(a0, a2, a4, a6);*/

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);
				origami_utils::store<Reg, stream>(a6, (Reg*)C3);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a2, (Reg*)loadFrom1); 
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2); origami_utils::load<Reg>(a6, (Reg*)loadFrom3);
			}
			else if constexpr (nreg == 2) {
				/*origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);

				origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a5, a7);
				origami_utils::rswap<Reg, Item>(a5, a6);

				origami_utils::rswap<Reg, Item>(a8, a10); origami_utils::rswap<Reg, Item>(a9, a11);
				origami_utils::rswap<Reg, Item>(a9, a10);*/

				origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
				origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
				origami_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);
				origami_merge_network::merge_network4<Reg, Item>(a12, a13, a14, a15);


				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C1);
				origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a8, (Reg*)C2);
				origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);
				origami_utils::store<Reg, stream>(a12, (Reg*)C3);
				origami_utils::store<Reg, stream>(a13, (Reg*)C3 + 1);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom2);
				origami_utils::load<Reg>(a9, (Reg*)loadFrom2 + 1);
				origami_utils::load<Reg>(a12, (Reg*)loadFrom3);
				origami_utils::load<Reg>(a13, (Reg*)loadFrom3 + 1);
			}
			else if constexpr (nreg == 3) {
				origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
				origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
				origami_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);
				origami_merge_network::merge_network6<Reg, Item>(a18, a19, a20, a21, a22, a23);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);

				origami_utils::store<Reg, stream>(a6, (Reg*)C1);
				origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

				origami_utils::store<Reg, stream>(a12, (Reg*)C2);
				origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1);
				origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);

				origami_utils::store<Reg, stream>(a18, (Reg*)C3);
				origami_utils::store<Reg, stream>(a19, (Reg*)C3 + 1);
				origami_utils::store<Reg, stream>(a20, (Reg*)C3 + 2);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);

				origami_utils::load<Reg>(a6, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);

				origami_utils::load<Reg>(a12, (Reg*)loadFrom2);
				origami_utils::load<Reg>(a13, (Reg*)loadFrom2 + 1);
				origami_utils::load<Reg>(a14, (Reg*)loadFrom2 + 2);

				origami_utils::load<Reg>(a18, (Reg*)loadFrom3);
				origami_utils::load<Reg>(a19, (Reg*)loadFrom3 + 1);
				origami_utils::load<Reg>(a20, (Reg*)loadFrom3 + 2);
			}
			else if constexpr (nreg == 4) {
				origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
				origami_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);
				origami_merge_network::merge_network8<Reg, Item>(a24, a25, a26, a27, a28, a29, a30, a31);


				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
				origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);

				origami_utils::store<Reg, stream>(a8, (Reg*)C1);
				origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
				origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2);
				origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

				origami_utils::store<Reg, stream>(a16, (Reg*)C2);
				origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
				origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2);
				origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);

				origami_utils::store<Reg, stream>(a24, (Reg*)C3);
				origami_utils::store<Reg, stream>(a25, (Reg*)C3 + 1);
				origami_utils::store<Reg, stream>(a26, (Reg*)C3 + 2);
				origami_utils::store<Reg, stream>(a27, (Reg*)C3 + 3);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
				origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);

				origami_utils::load<Reg>(a8, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1);
				origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);
				origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);

				origami_utils::load<Reg>(a16, (Reg*)loadFrom2);
				origami_utils::load<Reg>(a17, (Reg*)loadFrom2 + 1);
				origami_utils::load<Reg>(a18, (Reg*)loadFrom2 + 2);
				origami_utils::load<Reg>(a19, (Reg*)loadFrom2 + 3);

				origami_utils::load<Reg>(a24, (Reg*)loadFrom3);
				origami_utils::load<Reg>(a25, (Reg*)loadFrom3 + 1);
				origami_utils::load<Reg>(a26, (Reg*)loadFrom3 + 2);
				origami_utils::load<Reg>(a27, (Reg*)loadFrom3 + 3);
			}

			loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC; loadFrom3 += INC;
			C0 += INC; C1 += INC; C2 += INC; C3 += INC;
		}
	}

	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_8x(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD,
		Item* E, ui64 lenE, Item* F, ui64 lenF, Item* G, ui64 lenG, Item* H, ui64 lenH,
		Item* I, ui64 lenI, Item* J, ui64 lenJ, Item* K, ui64 lenK, Item* L, ui64 lenL,
		Item* M, ui64 lenM, Item* N, ui64 lenN, Item* O, ui64 lenO, Item* P, ui64 lenP,
		Item* Q, Item* R, Item* S, Item* T, Item* U, Item* V, Item* W, Item* X) {
		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = Q;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = R;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;
		Item* __restrict X2 = E, * __restrict Y2 = F, * __restrict C2 = S;
		Item* __restrict endX2 = E + lenE, * __restrict endY2 = F + lenF;
		Item* __restrict X3 = G, * __restrict Y3 = H, * __restrict C3 = T;
		Item* __restrict endX3 = G + lenG, * __restrict endY3 = H + lenH;
		Item* __restrict X4 = I, * __restrict Y4 = J, * __restrict C4 = U;
		Item* __restrict endX4 = I + lenI, * __restrict endY4 = J + lenJ;
		Item* __restrict X5 = K, * __restrict Y5 = L, * __restrict C5 = V;
		Item* __restrict endX5 = K + lenK, * __restrict endY5 = L + lenL;
		Item* __restrict X6 = M, * __restrict Y6 = N, * __restrict C6 = W;
		Item* __restrict endX6 = M + lenM, * __restrict endY6 = N + lenN;
		Item* __restrict X7 = O, * __restrict Y7 = P, * __restrict C7 = X;
		Item* __restrict endX7 = O + lenO, * __restrict endY7 = P + lenP;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);
		origami_utils::init_reg<Reg>(a20); origami_utils::init_reg<Reg>(a21); origami_utils::init_reg<Reg>(a22); origami_utils::init_reg<Reg>(a23);
		origami_utils::init_reg<Reg>(a24); origami_utils::init_reg<Reg>(a25); origami_utils::init_reg<Reg>(a26); origami_utils::init_reg<Reg>(a27);
		origami_utils::init_reg<Reg>(a28); origami_utils::init_reg<Reg>(a29); origami_utils::init_reg<Reg>(a30); origami_utils::init_reg<Reg>(a31);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
			origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
			origami_utils::load<Reg>(a6, (Reg*)X3); origami_utils::load<Reg>(a7, (Reg*)Y3);
			origami_utils::load<Reg>(a8, (Reg*)X4); origami_utils::load<Reg>(a9, (Reg*)Y4);
			origami_utils::load<Reg>(a10, (Reg*)X5); origami_utils::load<Reg>(a11, (Reg*)Y5);
			origami_utils::load<Reg>(a12, (Reg*)X6); origami_utils::load<Reg>(a13, (Reg*)Y6);
			origami_utils::load<Reg>(a14, (Reg*)X7); origami_utils::load<Reg>(a15, (Reg*)Y7);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC; X3 += INC; Y3 += INC;
		X4 += INC; Y4 += INC; X5 += INC; Y5 += INC; X6 += INC; Y6 += INC; X7 += INC; Y7 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2, * loadFrom3 = X3, * loadFrom4 = X4, * loadFrom5 = X5, * loadFrom6 = X6, * loadFrom7 = X7;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2, * opposite3 = Y3, * opposite4 = Y4, * opposite5 = Y5, * opposite6 = Y6, * opposite7 = Y7;

		origami_utils::transpose<Item, Reg>(a1, a3, a5, a7, a9, a11, a13, a15);

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2 && loadFrom3 != endX3 && loadFrom3 != endY3
			&& loadFrom4 != endX4 && loadFrom4 != endY4 && loadFrom5 != endX5 && loadFrom5 != endY5 && loadFrom6 != endX6 && loadFrom6 != endY6 && loadFrom7 != endX7 && loadFrom7 != endY7)
		{
			bool first = *loadFrom0 <= *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 <= *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			first = *loadFrom2 <= *opposite2;
			tmp = first ? loadFrom2 : opposite2;
			opposite2 = first ? opposite2 : loadFrom2;
			loadFrom2 = tmp;

			first = *loadFrom3 <= *opposite3;
			tmp = first ? loadFrom3 : opposite3;
			opposite3 = first ? opposite3 : loadFrom3;
			loadFrom3 = tmp;

			first = *loadFrom4 <= *opposite4;
			tmp = first ? loadFrom4 : opposite4;
			opposite4 = first ? opposite4 : loadFrom4;
			loadFrom4 = tmp;

			first = *loadFrom5 <= *opposite5;
			tmp = first ? loadFrom5 : opposite5;
			opposite5 = first ? opposite5 : loadFrom5;
			loadFrom5 = tmp;

			first = *loadFrom6 <= *opposite6;
			tmp = first ? loadFrom6 : opposite6;
			opposite6 = first ? opposite6 : loadFrom6;
			loadFrom6 = tmp;

			first = *loadFrom7 <= *opposite7;
			tmp = first ? loadFrom7 : opposite7;
			opposite7 = first ? opposite7 : loadFrom7;
			loadFrom7 = tmp;

			if constexpr (nreg == 1) {		
				origami_utils::transpose<Item, Reg>(a0, a2, a4, a6, a8, a10, a12, a14);
				origami_utils::cswap<Reg, Item>(a0, a2, a4, a6, a8, a10, a12, a14, a1, a3, a5, a7, a9, a11, a13, a15);
				origami_utils::transpose<Item, Reg>(a0, a2, a4, a6, a8, a10, a12, a14);
				
				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);
				origami_utils::store<Reg, stream>(a6, (Reg*)C3);
				origami_utils::store<Reg, stream>(a8, (Reg*)C4);
				origami_utils::store<Reg, stream>(a10, (Reg*)C5);
				origami_utils::store<Reg, stream>(a12, (Reg*)C6);
				origami_utils::store<Reg, stream>(a14, (Reg*)C7);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom4 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom5 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom6 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom7 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom4 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom5 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom6 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom7 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2); origami_utils::load<Reg>(a6, (Reg*)loadFrom3);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom4); origami_utils::load<Reg>(a10, (Reg*)loadFrom5);
				origami_utils::load<Reg>(a12, (Reg*)loadFrom6); origami_utils::load<Reg>(a14, (Reg*)loadFrom7);
			}
			loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC; loadFrom3 += INC;
			loadFrom4 += INC; loadFrom5 += INC; loadFrom6 += INC; loadFrom7 += INC;
			C0 += INC; C1 += INC; C2 += INC; C3 += INC;
			C4 += INC; C5 += INC; C6 += INC; C7 += INC;
		}
	}


	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_16x(Item* A0, ui64 lenA0, Item* B0, ui64 lenB0, Item* A1, ui64 lenA1, Item* B1, ui64 lenB1,
		Item* A2, ui64 lenA2, Item* B2, ui64 lenB2, Item* A3, ui64 lenA3, Item* B3, ui64 lenB3,
		Item* A4, ui64 lenA4, Item* B4, ui64 lenB4, Item* A5, ui64 lenA5, Item* B5, ui64 lenB5,
		Item* A6, ui64 lenA6, Item* B6, ui64 lenB6, Item* A7, ui64 lenA7, Item* B7, ui64 lenB7,
		Item* A8, ui64 lenA8, Item* B8, ui64 lenB8, Item* A9, ui64 lenA9, Item* B9, ui64 lenB9,
		Item* A10, ui64 lenA10, Item* B10, ui64 lenB10, Item* A11, ui64 lenA11, Item* B11, ui64 lenB11,
		Item* A12, ui64 lenA12, Item* B12, ui64 lenB12, Item* A13, ui64 lenA13, Item* B13, ui64 lenB13,
		Item* A14, ui64 lenA14, Item* B14, ui64 lenB14, Item* A15, ui64 lenA15, Item* B15, ui64 lenB15,
		Item* C, Item* D, Item* E, Item* F, Item* G, Item* H, Item* I, Item* J, Item* K, Item* L, Item* M, Item* N, Item* O, Item* P, Item* Q, Item* R) {
		
		Item* __restrict X0 = A0, * __restrict Y0 = B0, * __restrict C0 = C;
		Item* __restrict endX0 = A0 + lenA0, * __restrict endY0 = B0 + lenB0;
		Item* __restrict X1 = A1, * __restrict Y1 = B1, * __restrict C1 = D;
		Item* __restrict endX1 = A1 + lenA1, * __restrict endY1 = B1 + lenB1;
		Item* __restrict X2 = A2, * __restrict Y2 = B2, * __restrict C2 = E;
		Item* __restrict endX2 = A2 + lenA2, * __restrict endY2 = B2 + lenB2;
		Item* __restrict X3 = A3, * __restrict Y3 = B3, * __restrict C3 = F;
		Item* __restrict endX3 = A3 + lenA3, * __restrict endY3 = B3 + lenB3;
		Item* __restrict X4 = A4, * __restrict Y4 = B4, * __restrict C4 = G;
		Item* __restrict endX4 = A4 + lenA4, * __restrict endY4 = B4 + lenB4;
		Item* __restrict X5 = A5, * __restrict Y5 = B5, * __restrict C5 = H;
		Item* __restrict endX5 = A5 + lenA5, * __restrict endY5 = B5 + lenB5;
		Item* __restrict X6 = A6, * __restrict Y6 = B6, * __restrict C6 = I;
		Item* __restrict endX6 = A6 + lenA6, * __restrict endY6 = B6 + lenB6;
		Item* __restrict X7 = A7, * __restrict Y7 = B7, * __restrict C7 = J;
		Item* __restrict endX7 = A7 + lenA7, * __restrict endY7 = B7 + lenB7;
		Item* __restrict X8 = A8, * __restrict Y8 = B8, * __restrict C8 = K;
		Item* __restrict endX8 = A8 + lenA8, * __restrict endY8 = B8 + lenB8;
		Item* __restrict X9 = A9, * __restrict Y9 = B9, * __restrict C9 = L;
		Item* __restrict endX9 = A9 + lenA9, * __restrict endY9 = B9 + lenB9;
		Item* __restrict X10 = A10, * __restrict Y10 = B10, * __restrict C10 = M;
		Item* __restrict endX10 = A10 + lenA10, * __restrict endY10 = B10 + lenB10;
		Item* __restrict X11 = A11, * __restrict Y11 = B11, * __restrict C11 = N;
		Item* __restrict endX11 = A11 + lenA11, * __restrict endY11 = B11 + lenB11;
		Item* __restrict X12 = A12, * __restrict Y12 = B12, * __restrict C12 = O;
		Item* __restrict endX12 = A12 + lenA12, * __restrict endY12 = B12 + lenB12;
		Item* __restrict X13 = A13, * __restrict Y13 = B13, * __restrict C13 = P;
		Item* __restrict endX13 = A13 + lenA13, * __restrict endY13 = B13 + lenB13;
		Item* __restrict X14 = A14, * __restrict Y14 = B14, * __restrict C14 = Q;
		Item* __restrict endX14 = A14 + lenA14, * __restrict endY14 = B14 + lenB14; 
		Item* __restrict X15 = A15, * __restrict Y15 = B15, * __restrict C15 = R;
		Item* __restrict endX15 = A15 + lenA15, * __restrict endY15 = B15 + lenB15;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);
		origami_utils::init_reg<Reg>(a20); origami_utils::init_reg<Reg>(a21); origami_utils::init_reg<Reg>(a22); origami_utils::init_reg<Reg>(a23);
		origami_utils::init_reg<Reg>(a24); origami_utils::init_reg<Reg>(a25); origami_utils::init_reg<Reg>(a26); origami_utils::init_reg<Reg>(a27);
		origami_utils::init_reg<Reg>(a28); origami_utils::init_reg<Reg>(a29); origami_utils::init_reg<Reg>(a30); origami_utils::init_reg<Reg>(a31);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
			origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
			origami_utils::load<Reg>(a6, (Reg*)X3); origami_utils::load<Reg>(a7, (Reg*)Y3);
			origami_utils::load<Reg>(a8, (Reg*)X4); origami_utils::load<Reg>(a9, (Reg*)Y4);
			origami_utils::load<Reg>(a10, (Reg*)X5); origami_utils::load<Reg>(a11, (Reg*)Y5);
			origami_utils::load<Reg>(a12, (Reg*)X6); origami_utils::load<Reg>(a13, (Reg*)Y6);
			origami_utils::load<Reg>(a14, (Reg*)X7); origami_utils::load<Reg>(a15, (Reg*)Y7);
			origami_utils::load<Reg>(a16, (Reg*)X8); origami_utils::load<Reg>(a17, (Reg*)Y8);
			origami_utils::load<Reg>(a18, (Reg*)X9); origami_utils::load<Reg>(a19, (Reg*)Y9);
			origami_utils::load<Reg>(a20, (Reg*)X10); origami_utils::load<Reg>(a21, (Reg*)Y10);
			origami_utils::load<Reg>(a22, (Reg*)X11); origami_utils::load<Reg>(a23, (Reg*)Y11);
			origami_utils::load<Reg>(a24, (Reg*)X12); origami_utils::load<Reg>(a25, (Reg*)Y12);
			origami_utils::load<Reg>(a26, (Reg*)X13); origami_utils::load<Reg>(a27, (Reg*)Y13);
			origami_utils::load<Reg>(a28, (Reg*)X14); origami_utils::load<Reg>(a29, (Reg*)Y14);
			origami_utils::load<Reg>(a30, (Reg*)X15); origami_utils::load<Reg>(a31, (Reg*)Y15);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC; X3 += INC; Y3 += INC;
		X4 += INC; Y4 += INC; X5 += INC; Y5 += INC; X6 += INC; Y6 += INC; X7 += INC; Y7 += INC;
		X8 += INC; Y8 += INC; X9 += INC; Y9 += INC; X10 += INC; Y10 += INC; X11 += INC; Y11 += INC;
		X12 += INC; Y12 += INC; X13 += INC; Y13 += INC; X14 += INC; Y14 += INC; X15 += INC; Y15 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2, * loadFrom3 = X3, * loadFrom4 = X4, * loadFrom5 = X5, * loadFrom6 = X6, * loadFrom7 = X7, * loadFrom8 = X8, * loadFrom9 = X9, * loadFrom10 = X10, * loadFrom11 = X11, * loadFrom12 = X12, * loadFrom13 = X13, * loadFrom14 = X14, * loadFrom15 = X15;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2, * opposite3 = Y3, * opposite4 = Y4, * opposite5 = Y5, * opposite6 = Y6, * opposite7 = Y7, * opposite8 = Y8, * opposite9 = Y9, * opposite10 = Y10, * opposite11 = Y11, * opposite12 = Y12, * opposite13 = Y13, * opposite14 = Y14, * opposite15 = Y15;

		origami_utils::transpose<Item, Reg>(a1, a3, a5, a7, a9, a11, a13, a15, a17, a19, a21, a23, a25, a27, a29, a31);

		while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2 && loadFrom3 != endX3 && loadFrom3 != endY3
			&& loadFrom4 != endX4 && loadFrom4 != endY4 && loadFrom5 != endX5 && loadFrom5 != endY5 && loadFrom6 != endX6 && loadFrom6 != endY6 && loadFrom7 != endX7 && loadFrom7 != endY7
			&& loadFrom8 != endX8 && loadFrom8 != endY8 && loadFrom9 != endX9 && loadFrom9 != endY9 && loadFrom10 != endX10 && loadFrom10 != endY10 && loadFrom11 != endX11 && loadFrom11 != endY11 
			&& loadFrom12 != endX12 && loadFrom12 != endY12 && loadFrom13 != endX13 && loadFrom13 != endY13 && loadFrom14 != endX14 && loadFrom14 != endY14 && loadFrom15 != endX15 && loadFrom15 != endY15)
		{
			bool first = *loadFrom0 <= *opposite0;
			Item* tmp = first ? loadFrom0 : opposite0;
			opposite0 = first ? opposite0 : loadFrom0;
			loadFrom0 = tmp;

			first = *loadFrom1 <= *opposite1;
			tmp = first ? loadFrom1 : opposite1;
			opposite1 = first ? opposite1 : loadFrom1;
			loadFrom1 = tmp;

			first = *loadFrom2 <= *opposite2;
			tmp = first ? loadFrom2 : opposite2;
			opposite2 = first ? opposite2 : loadFrom2;
			loadFrom2 = tmp;

			first = *loadFrom3 <= *opposite3;
			tmp = first ? loadFrom3 : opposite3;
			opposite3 = first ? opposite3 : loadFrom3;
			loadFrom3 = tmp;

			first = *loadFrom4 <= *opposite4;
			tmp = first ? loadFrom4 : opposite4;
			opposite4 = first ? opposite4 : loadFrom4;
			loadFrom4 = tmp;

			first = *loadFrom5 <= *opposite5;
			tmp = first ? loadFrom5 : opposite5;
			opposite5 = first ? opposite5 : loadFrom5;
			loadFrom5 = tmp;

			first = *loadFrom6 <= *opposite6;
			tmp = first ? loadFrom6 : opposite6;
			opposite6 = first ? opposite6 : loadFrom6;
			loadFrom6 = tmp;

			first = *loadFrom7 <= *opposite7;
			tmp = first ? loadFrom7 : opposite7;
			opposite7 = first ? opposite7 : loadFrom7;
			loadFrom7 = tmp;

			first = *loadFrom8 <= *opposite8;
			tmp = first ? loadFrom8 : opposite8;
			opposite8 = first ? opposite8 : loadFrom8;
			loadFrom8 = tmp;

			first = *loadFrom9 <= *opposite9;
			tmp = first ? loadFrom9 : opposite9;
			opposite9 = first ? opposite9 : loadFrom9;
			loadFrom9 = tmp;

			first = *loadFrom10 <= *opposite10;
			tmp = first ? loadFrom10 : opposite10;
			opposite10 = first ? opposite10 : loadFrom10;
			loadFrom10 = tmp;

			first = *loadFrom11 <= *opposite11;
			tmp = first ? loadFrom11 : opposite11;
			opposite11 = first ? opposite11 : loadFrom11;
			loadFrom11 = tmp;

			first = *loadFrom12 <= *opposite12;
			tmp = first ? loadFrom12 : opposite12;
			opposite12 = first ? opposite12 : loadFrom12;
			loadFrom12 = tmp;

			first = *loadFrom13 <= *opposite13;
			tmp = first ? loadFrom13 : opposite13;
			opposite13 = first ? opposite13 : loadFrom13;
			loadFrom13 = tmp;

			first = *loadFrom14 <= *opposite14;
			tmp = first ? loadFrom14 : opposite14;
			opposite14 = first ? opposite14 : loadFrom14;
			loadFrom14 = tmp;

			first = *loadFrom15 <= *opposite15;
			tmp = first ? loadFrom15 : opposite15;
			opposite15 = first ? opposite15 : loadFrom15;
			loadFrom15 = tmp;


			if constexpr (nreg == 1) {
				origami_utils::transpose<Item, Reg>(a0, a2, a4, a6, a8, a10, a12, a14, a16, a18, a20, a22, a24, a26, a28, a30);
				origami_utils::cswap<Reg, Item>(a0, a2, a4, a6, a8, a10, a12, a14, a16, a18, a20, a22, a24, a26, a28, a30, a1, a3, a5, a7, a9, a11, a13, a15, a17, a19, a21, a23, a25, a27, a29, a31);
				origami_utils::transpose<Item, Reg>(a0, a2, a4, a6, a8, a10, a12, a14, a16, a18, a20, a22, a24, a26, a28, a30);

				origami_utils::store<Reg, stream>(a0, (Reg*)C0);
				origami_utils::store<Reg, stream>(a2, (Reg*)C1);
				origami_utils::store<Reg, stream>(a4, (Reg*)C2);
				origami_utils::store<Reg, stream>(a6, (Reg*)C3);
				origami_utils::store<Reg, stream>(a8, (Reg*)C4);
				origami_utils::store<Reg, stream>(a10, (Reg*)C5);
				origami_utils::store<Reg, stream>(a12, (Reg*)C6);
				origami_utils::store<Reg, stream>(a14, (Reg*)C7);
				origami_utils::store<Reg, stream>(a16, (Reg*)C8);
				origami_utils::store<Reg, stream>(a18, (Reg*)C9);
				origami_utils::store<Reg, stream>(a20, (Reg*)C10);
				origami_utils::store<Reg, stream>(a22, (Reg*)C11);
				origami_utils::store<Reg, stream>(a24, (Reg*)C12);
				origami_utils::store<Reg, stream>(a26, (Reg*)C13);
				origami_utils::store<Reg, stream>(a28, (Reg*)C14);
				origami_utils::store<Reg, stream>(a30, (Reg*)C15);

				if constexpr (sizeof(Reg) == 64) {
					_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom4 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom5 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom6 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom7 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom8 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom9 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom10 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom11 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom12 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom13 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom14 + 512), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom15 + 512), _MM_HINT_T2);
				}
				else {
					_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
					_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom3 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom4 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom5 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom6 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom7 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom8 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom9 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom10 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom11 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom12 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom13 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom14 + 64), _MM_HINT_T2);
					_mm_prefetch((char*)(loadFrom15 + 64), _MM_HINT_T2);
				}

				origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				origami_utils::load<Reg>(a4, (Reg*)loadFrom2); origami_utils::load<Reg>(a6, (Reg*)loadFrom3);
				origami_utils::load<Reg>(a8, (Reg*)loadFrom4); origami_utils::load<Reg>(a10, (Reg*)loadFrom5);
				origami_utils::load<Reg>(a12, (Reg*)loadFrom6); origami_utils::load<Reg>(a14, (Reg*)loadFrom7);
				origami_utils::load<Reg>(a16, (Reg*)loadFrom8); origami_utils::load<Reg>(a18, (Reg*)loadFrom9);
				origami_utils::load<Reg>(a20, (Reg*)loadFrom10); origami_utils::load<Reg>(a22, (Reg*)loadFrom11);
				origami_utils::load<Reg>(a24, (Reg*)loadFrom12); origami_utils::load<Reg>(a26, (Reg*)loadFrom13);
				origami_utils::load<Reg>(a28, (Reg*)loadFrom14); origami_utils::load<Reg>(a30, (Reg*)loadFrom15);
			}
			loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC; loadFrom3 += INC;
			loadFrom4 += INC; loadFrom5 += INC; loadFrom6 += INC; loadFrom7 += INC;
			loadFrom8 += INC; loadFrom9 += INC; loadFrom10 += INC; loadFrom11 += INC;
			loadFrom12 += INC; loadFrom13 += INC; loadFrom14 += INC; loadFrom15 += INC;
			C0 += INC; C1 += INC; C2 += INC; C3 += INC; C4 += INC; C5 += INC; C6 += INC; C7 += INC;
			C8 += INC; C9 += INC; C10 += INC; C11 += INC; C12 += INC; C13 += INC; C14 += INC; C15 += INC;
		}
	}

	
	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_1x_2(std::queue<origami_utils::MergeStream>* Q, std::queue<origami_utils::MergeStream>* Q_next, char* output) {

		origami_utils::MergeStream j0 = Q->front(); Q->pop();
		origami_utils::MergeStream j1 = Q->front(); Q->pop();

		Item* __restrict X0 = (Item*)(j0.A), * __restrict Y0 = (Item*)(j1.A), * __restrict C0 = (Item*)output, * __C0 = C0;
		Item* __restrict endX0 = (Item*)(j0.A + j0.len), * __restrict endY0 = (Item*)(j1.A + j1.len);
		output += j0.len + j1.len;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		Item SENTINEL = get_max<Item>();
		Item* SENTINEL_PTR = &SENTINEL;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
		}
		X0 += INC; Y0 += INC;

		Item* loadFrom0 = X0, * opposite0 = Y0;
		ui done0 = 0;

		while (1) {
			while (loadFrom0 != endX0 && loadFrom0 != endY0)
			{
				bool first = *loadFrom0 <= *opposite0;
				Item* tmp = first ? loadFrom0 : opposite0;
				opposite0 = first ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				if constexpr (nreg == 1) {
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
				}
				else if constexpr (nreg == 2) {
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
				}

				_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);

				loadFrom0 += INC;

				C0 += INC;
			}
			loadFrom0 = opposite0;
			opposite0 = SENTINEL_PTR; //endOp - 1;
			done0++;
			if (done0 == 2) {	// both streams exhausted, replace with new stream
				// dump register contents
				if constexpr (nreg == 1) {
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
				}
				else if constexpr (nreg == 2) {
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3); origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5); origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 6); origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 7);
				}
				// push to next queue
				origami_utils::MergeStream job((char*)__C0, j0.len + j1.len);
				Q_next->push(job);

				// get next job
				done0 = 0;
				if (!Q->empty()) {
					j0 = Q->front(); Q->pop();
					j1 = Q->front(); Q->pop();

					X0 = (Item*)(j0.A), Y0 = (Item*)(j1.A), C0 = (Item*)output, __C0 = C0;
					endX0 = (Item*)(j0.A + j0.len), endY0 = (Item*)(j1.A + j1.len);
					output += j0.len + j1.len;

					if constexpr (nreg == 1) {
						origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
					}
					else if constexpr (nreg == 2) {
						origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
						origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
					}
					else if constexpr (nreg == 3) {
						origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
						origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
					}
					else if constexpr (nreg == 4) {
						origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
						origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
					}

					X0 += INC; Y0 += INC;

					loadFrom0 = X0, opposite0 = Y0;
				}
				else
					break;
			}
		}
	}

	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_2x_2(std::queue<origami_utils::MergeStream>* Q, std::queue<origami_utils::MergeStream>* Q_next, char* output) {

		origami_utils::MergeStream j0 = Q->front(); Q->pop();
		origami_utils::MergeStream j1 = Q->front(); Q->pop();
		origami_utils::MergeStream j2 = Q->front(); Q->pop();
		origami_utils::MergeStream j3 = Q->front(); Q->pop();

		Item* __restrict X0 = (Item*)(j0.A), * __restrict Y0 = (Item*)(j1.A), * __restrict C0 = (Item*)output, * __C0 = C0;
		Item* __restrict endX0 = (Item*)(j0.A + j0.len), * __restrict endY0 = (Item*)(j1.A + j1.len);
		output += j0.len + j1.len;
		Item* __restrict X1 = (Item*)(j2.A), * __restrict Y1 = (Item*)(j3.A), * __restrict C1 = (Item*)output, * __C1 = C1;
		Item* __restrict endX1 = (Item*)(j2.A + j2.len), * __restrict endY1 = (Item*)(j3.A + j3.len);
		output += j2.len + j3.len;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		Item SENTINEL = get_max<Item>();
		Item* SENTINEL_PTR = &SENTINEL;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1); origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1); origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1); origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);
			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1); origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1;
		Item* opposite0 = Y0, * opposite1 = Y1;

		ui done0 = 0, done1 = 0;

		while (1) {
			//ui64 out = 0;
			while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1)
			{
				//out += 2 * sizeof(Reg) / sizeof(Item);
				bool first = *loadFrom0 <= *opposite0;
				Item* tmp = first ? loadFrom0 : opposite0;
				opposite0 = first ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				first = *loadFrom1 <= *opposite1;
				tmp = first ? loadFrom1 : opposite1;
				opposite1 = first ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				if constexpr (nreg == 1) {
					/*origami_utils::print<Reg, Item>(a0);
					origami_utils::print<Reg, Item>(a1);*/

					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_merge_network::merge_network2<Reg, Item>(a2, a3);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);

					/*origami_utils::print<Reg, Item>(a0);
					origami_utils::print<Reg, Item>(a1);
					PRINT_DASH(20);*/

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				}
				else if constexpr (nreg == 2) {
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a4, (Reg*)loadFrom1);	origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
					origami_utils::load<Reg>(a6, (Reg*)loadFrom1);	origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom1);	origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);	origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);
				}

				_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
				_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);

				loadFrom0 += INC; loadFrom1 += INC;

				C0 += INC; C1 += INC;
			}
			//printf("%llu\n", out);
			// at least one stream exhausted
			if (loadFrom0 == endX0 || loadFrom0 == endY0) {
				//Item* endOp = (loadFrom0 == endX0) ? endY0 : endX0;
				loadFrom0 = opposite0;
				opposite0 = SENTINEL_PTR; //endOp - 1;
				done0++;
				if (done0 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);

						/*origami_utils::print<Reg, Item>(a0);
						origami_utils::print<Reg, Item>(a1);*/
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
						origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
						origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3); origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
						origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5); origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 6); origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 7);
					}
					// push to next queue
					origami_utils::MergeStream job((char*)__C0, j0.len + j1.len);
					Q_next->push(job);

					// get next job
					done0 = 0;
					if (!Q->empty()) {
						j0 = Q->front(); Q->pop();
						j1 = Q->front(); Q->pop();

						X0 = (Item*)(j0.A), Y0 = (Item*)(j1.A), C0 = (Item*)output, __C0 = C0;
						endX0 = (Item*)(j0.A + j0.len), endY0 = (Item*)(j1.A + j1.len);
						output += j0.len + j1.len;

						if constexpr (nreg == 1) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
						}
						else if constexpr (nreg == 2) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
							origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
							origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
							origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
						}

						X0 += INC; Y0 += INC;

						loadFrom0 = X0, opposite0 = Y0;
					}
					else {
						// no more jobs; finish the other one
						while (loadFrom1 != endX1 && loadFrom1 != endY1)
						{
							bool first = *loadFrom1 <= *opposite1;
							Item* tmp = first ? loadFrom1 : opposite1;
							opposite1 = first ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							if constexpr (nreg == 1) {
								origami_merge_network::merge_network2<Reg, Item>(a2, a3);
								origami_utils::store<Reg, stream>(a2, (Reg*)C1);
								origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
							}
							else if constexpr (nreg == 2) {
								origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
								origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
								origami_utils::load<Reg>(a4, (Reg*)loadFrom1);	origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
							}
							else if constexpr (nreg == 3) {
								origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
								origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
								origami_utils::load<Reg>(a6, (Reg*)loadFrom1);	origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);
							}
							else if constexpr (nreg == 4) {
								origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
								origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
								origami_utils::load<Reg>(a8, (Reg*)loadFrom1);	origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);	origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);
							}
							_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
							loadFrom1 += INC;
							C1 += INC;
						}
						if constexpr (nreg == 1) {
							origami_merge_network::merge_network2<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						}
						else if constexpr (nreg == 2) {
							origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
							origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
							origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
							origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
							origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
						}
						C1 += INC;
						if (opposite1 != SENTINEL_PTR) {
							Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
							while (opposite1 != endOpposite1) {
								_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
								if constexpr (nreg == 1) {
									origami_utils::load<Reg>(a2, (Reg*)opposite1);
									origami_merge_network::merge_network2<Reg, Item>(a2, a3);
									origami_utils::store<Reg, stream>(a2, (Reg*)C1);
								}
								else if constexpr (nreg == 2) {
									origami_utils::load<Reg>(a4, (Reg*)opposite1); origami_utils::load<Reg>(a5, (Reg*)opposite1 + 1);
									origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
									origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
								}
								else if constexpr (nreg == 3) {
									origami_utils::load<Reg>(a6, (Reg*)opposite1); origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a8, (Reg*)opposite1 + 2);
									origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
									origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
								}
								else if constexpr (nreg == 4) {
									origami_utils::load<Reg>(a8, (Reg*)opposite1); origami_utils::load<Reg>(a9, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a10, (Reg*)opposite1 + 2); 	origami_utils::load<Reg>(a11, (Reg*)opposite1 + 3);

									origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
									origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
									origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
								}
								opposite1 += INC;
								C1 += INC;
							}
						}
						if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a3, (Reg*)C1);
						else if constexpr (nreg == 2) {
							origami_utils::store<Reg, stream>(a6, (Reg*)C1);
							origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::store<Reg, stream>(a9, (Reg*)C1);
							origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 1);
							origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_merge_network::merge_network4<Reg, Item>(a12, a13, a14, a15);
							origami_utils::store<Reg, stream>(a12, (Reg*)C1);
							origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 1);
							origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 2);
							origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 3);
						}
						C1 += INC;
						origami_utils::MergeStream job((char*)__C1, j2.len + j3.len);
						Q_next->push(job);
						break;
					}
				}
			}
			if (loadFrom1 == endX1 || loadFrom1 == endY1) {
				//Item* endOp = (loadFrom1 == endX1) ? endY1 : endX1;
				loadFrom1 = opposite1;
				opposite1 = SENTINEL_PTR;	// endOp - 1
				done1++;
				if (done1 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::store<Reg, stream>(a3, (Reg*)C1 + 1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
						origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 3); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
						origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
						origami_utils::store<Reg, stream>(a12, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 5);
						origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 6); origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 7);
					}

					// push to next queue
					origami_utils::MergeStream job((char*)__C1, j2.len + j3.len);
					Q_next->push(job);

					// get next job
					done1 = 0;
					if (!Q->empty()) {
						j2 = Q->front(); Q->pop();
						j3 = Q->front(); Q->pop();

						X1 = (Item*)(j2.A), Y1 = (Item*)(j3.A), C1 = (Item*)output, __C1 = C1;
						endX1 = (Item*)(j2.A + j2.len), endY1 = (Item*)(j3.A + j3.len);
						output += j2.len + j3.len;

						if constexpr (nreg == 1) {
							origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
						}
						else if constexpr (nreg == 2) {
							origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
							origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::load<Reg>(a6, (Reg*)X1); origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
							origami_utils::load<Reg>(a9, (Reg*)Y1); origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1); origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);
							origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1); origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);
						}

						X1 += INC; Y1 += INC;
						loadFrom1 = X1, opposite1 = Y1;
					}
					else {
						// no more jobs; finish the other one
						while (loadFrom0 != endX0 && loadFrom0 != endY0)
						{
							bool first = *loadFrom0 <= *opposite0;
							Item* tmp = first ? loadFrom0 : opposite0;
							opposite0 = first ? opposite0 : loadFrom0;
							loadFrom0 = tmp;

							if constexpr (nreg == 1) {
								origami_merge_network::merge_network2<Reg, Item>(a0, a1);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0);
								origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
							}
							else if constexpr (nreg == 2) {
								origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
								origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
							}
							else if constexpr (nreg == 3) {
								origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
								origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
							}
							else if constexpr (nreg == 4) {
								origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
								origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
							}

							_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
							loadFrom0 += INC; C0 += INC;
						}
						if constexpr (nreg == 1) {
							origami_merge_network::merge_network2<Reg, Item>(a0, a1);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						}
						else if constexpr (nreg == 2) {
							origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
						}
						C0 += INC;

						if (opposite0 != SENTINEL_PTR) {
							Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
							while (opposite0 != endOpposite0) {
								_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
								if constexpr (nreg == 1) {
									origami_utils::load<Reg>(a0, (Reg*)opposite0);
									origami_merge_network::merge_network2<Reg, Item>(a0, a1);
									origami_utils::store<Reg, stream>(a0, (Reg*)C0);
								}
								else if constexpr (nreg == 2) {
									origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1);
									origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
									origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
								}
								else if constexpr (nreg == 3) {
									origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2);
									origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
									origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
								}
								else if constexpr (nreg == 4) {
									origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)opposite0 + 3);
									origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
									origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
								}
								opposite0 += INC;
								C0 += INC;
							}
						}
						if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a1, (Reg*)C0);
						else if constexpr (nreg == 2) {
							origami_utils::store<Reg, stream>(a2, (Reg*)C0);
							origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::store<Reg, stream>(a3, (Reg*)C0);
							origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 1);
							origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
							origami_utils::store<Reg, stream>(a4, (Reg*)C0);
							origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 1);
							origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 2);
							origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 3);
						}
						C0 += INC;
						origami_utils::MergeStream job((char*)__C0, j0.len + j1.len);
						Q_next->push(job);
						break;
					}
				}

			}
		}
	}

	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_3x_2(std::queue<origami_utils::MergeStream>* Q, std::queue<origami_utils::MergeStream>* Q_next, char* output) {

		origami_utils::MergeStream j0 = Q->front(); Q->pop();
		origami_utils::MergeStream j1 = Q->front(); Q->pop();
		origami_utils::MergeStream j2 = Q->front(); Q->pop();
		origami_utils::MergeStream j3 = Q->front(); Q->pop();
		origami_utils::MergeStream j4 = Q->front(); Q->pop();
		origami_utils::MergeStream j5 = Q->front(); Q->pop();

		Item* __restrict X0 = (Item*)(j0.A), * __restrict Y0 = (Item*)(j1.A), * __restrict C0 = (Item*)output, * __C0 = C0;
		Item* __restrict endX0 = (Item*)(j0.A + j0.len), * __restrict endY0 = (Item*)(j1.A + j1.len);
		output += j0.len + j1.len;
		Item* __restrict X1 = (Item*)(j2.A), * __restrict Y1 = (Item*)(j3.A), * __restrict C1 = (Item*)output, * __C1 = C1;
		Item* __restrict endX1 = (Item*)(j2.A + j2.len), * __restrict endY1 = (Item*)(j3.A + j3.len);
		output += j2.len + j3.len;
		Item* __restrict X2 = (Item*)(j4.A), * __restrict Y2 = (Item*)(j5.A), * __restrict C2 = (Item*)output, * __C2 = C2;
		Item* __restrict endX2 = (Item*)(j4.A + j4.len), * __restrict endY2 = (Item*)(j5.A + j5.len);
		output += j4.len + j5.len;

		Item SENTINEL = get_max<Item>();
		Item* SENTINEL_PTR = &SENTINEL;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;

		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);
		origami_utils::init_reg<Reg>(a20); origami_utils::init_reg<Reg>(a21); origami_utils::init_reg<Reg>(a22); origami_utils::init_reg<Reg>(a23);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
			origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a8, (Reg*)X2); origami_utils::load<Reg>(a9, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a10, (Reg*)Y2); origami_utils::load<Reg>(a11, (Reg*)Y2 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0);		origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0);		origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1);		origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1);		origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
			origami_utils::load<Reg>(a12, (Reg*)X2);	origami_utils::load<Reg>(a13, (Reg*)X2 + 1);	origami_utils::load<Reg>(a14, (Reg*)X2 + 2);
			origami_utils::load<Reg>(a15, (Reg*)Y2);	origami_utils::load<Reg>(a16, (Reg*)Y2 + 1);	origami_utils::load<Reg>(a17, (Reg*)Y2 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);

			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);

			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);

			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);

			origami_utils::load<Reg>(a16, (Reg*)X2); origami_utils::load<Reg>(a17, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a18, (Reg*)X2 + 2); origami_utils::load<Reg>(a19, (Reg*)X2 + 3);

			origami_utils::load<Reg>(a20, (Reg*)Y2); origami_utils::load<Reg>(a21, (Reg*)Y2 + 1);
			origami_utils::load<Reg>(a22, (Reg*)Y2 + 2); origami_utils::load<Reg>(a23, (Reg*)Y2 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2;

		ui done0 = 0, done1 = 0, done2 = 0;

		while (1) {
			while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2)
			{
				bool first = *loadFrom0 <= *opposite0;
				Item* tmp = first ? loadFrom0 : opposite0;
				opposite0 = first ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				first = *loadFrom1 <= *opposite1;
				tmp = first ? loadFrom1 : opposite1;
				opposite1 = first ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				first = *loadFrom2 <= *opposite2;
				tmp = first ? loadFrom2 : opposite2;
				opposite2 = first ? opposite2 : loadFrom2;
				loadFrom2 = tmp;

				if constexpr (nreg == 1) {
					//origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5);
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_merge_network::merge_network2<Reg, Item>(a2, a3);
					origami_merge_network::merge_network2<Reg, Item>(a4, a5);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);
					origami_utils::store<Reg, stream>(a4, (Reg*)C2);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a2, (Reg*)loadFrom1); origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
				}
				else if constexpr (nreg == 2) {
					/*origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
					origami_utils::rswap<Reg, Item>(a1, a2);

					origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a5, a7);
					origami_utils::rswap<Reg, Item>(a5, a6);

					origami_utils::rswap<Reg, Item>(a8, a10); origami_utils::rswap<Reg, Item>(a9, a11);
					origami_utils::rswap<Reg, Item>(a9, a10);*/

					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
					origami_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);


					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1);
					origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a8, (Reg*)C2);
					origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a4, (Reg*)loadFrom1);
					origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom2);
					origami_utils::load<Reg>(a9, (Reg*)loadFrom2 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
					origami_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);

					origami_utils::store<Reg, stream>(a6, (Reg*)C1);
					origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

					origami_utils::store<Reg, stream>(a12, (Reg*)C2);
					origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1);
					origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);

					origami_utils::load<Reg>(a6, (Reg*)loadFrom1);
					origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);

					origami_utils::load<Reg>(a12, (Reg*)loadFrom2);
					origami_utils::load<Reg>(a13, (Reg*)loadFrom2 + 1);
					origami_utils::load<Reg>(a14, (Reg*)loadFrom2 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);


					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);

					origami_utils::store<Reg, stream>(a8, (Reg*)C1);
					origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2);
					origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

					origami_utils::store<Reg, stream>(a16, (Reg*)C2);
					origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
					origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2);
					origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
					origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);

					origami_utils::load<Reg>(a8, (Reg*)loadFrom1);
					origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1);
					origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);
					origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);

					origami_utils::load<Reg>(a16, (Reg*)loadFrom2);
					origami_utils::load<Reg>(a17, (Reg*)loadFrom2 + 1);
					origami_utils::load<Reg>(a18, (Reg*)loadFrom2 + 2);
					origami_utils::load<Reg>(a19, (Reg*)loadFrom2 + 3);
				}

				loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC;
				C0 += INC; C1 += INC; C2 += INC;
			}

			// at least one stream exhausted
			if (loadFrom0 == endX0 || loadFrom0 == endY0) {
				//Item* endOp = (loadFrom0 == endX0) ? endY0 : endX0;
				loadFrom0 = opposite0;
				opposite0 = SENTINEL_PTR; //endOp - 1;
				done0++;
				if (done0 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);

						/*origami_utils::print<Reg, Item>(a0);
						origami_utils::print<Reg, Item>(a1);*/
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
						origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
						origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3); origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
						origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5); origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 6); origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 7);
					}
					// push to next queue
					origami_utils::MergeStream job((char*)__C0, j0.len + j1.len);
					Q_next->push(job);

					// get next job
					done0 = 0;
					if (!Q->empty()) {
						j0 = Q->front(); Q->pop();
						j1 = Q->front(); Q->pop();

						X0 = (Item*)(j0.A), Y0 = (Item*)(j1.A), C0 = (Item*)output, __C0 = C0;
						endX0 = (Item*)(j0.A + j0.len), endY0 = (Item*)(j1.A + j1.len);
						output += j0.len + j1.len;

						if constexpr (nreg == 1) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
						}
						else if constexpr (nreg == 2) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
							origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
							origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
							origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
						}

						X0 += INC; Y0 += INC;

						loadFrom0 = X0, opposite0 = Y0;
					}
					else {
						// no more jobs; finish the other two
						while (loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2) {
							Item comp1 = *loadFrom1;
							Item comp2 = *opposite1;
							Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
							opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							comp1 = *loadFrom2;
							comp2 = *opposite2;
							tmp = comp1 < comp2 ? loadFrom2 : opposite2;
							opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
							loadFrom2 = tmp;

							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::rswap<Reg, Item>(a4, a5);

							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2);

							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

							_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

							loadFrom1 += INC; loadFrom2 += INC;

							C1 += INC; C2 += INC;
						}

						while (loadFrom1 != endX1 && loadFrom1 != endY1) {
							Item comp1 = *loadFrom1;
							Item comp2 = *opposite1;
							Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
							opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
							loadFrom1 += INC;
							C1 += INC;
						}
						origami_utils::rswap<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
						if (opposite1 != SENTINEL_PTR) {
							Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
							while (opposite1 != endOpposite1) {
								_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a2, (Reg*)opposite1);
								opposite1 += INC;
								origami_utils::rswap<Reg, Item>(a2, a3);
								origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;

						while (loadFrom2 != endX2 && loadFrom2 != endY2) {
							Item comp1 = *loadFrom2;
							Item comp2 = *opposite2;
							Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
							opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
							loadFrom2 = tmp;

							origami_utils::rswap<Reg, Item>(a4, a5);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
							loadFrom2 += INC;
							C2 += INC;
						}
						origami_utils::rswap<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;

						if (opposite2 != SENTINEL_PTR) {
							Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;
							while (opposite2 != endOpposite2) {
								_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a4, (Reg*)opposite2);
								opposite2 += INC;
								origami_utils::rswap<Reg, Item>(a4, a5);
								origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;




						origami_utils::MergeStream job1((char*)__C1, j2.len + j3.len);
						origami_utils::MergeStream job2((char*)__C2, j4.len + j5.len);
						Q_next->push(job1);
						Q_next->push(job2);
						break;
					}
				}
			}
			if (loadFrom1 == endX1 || loadFrom1 == endY1) {
				//Item* endOp = (loadFrom1 == endX1) ? endY1 : endX1;
				loadFrom1 = opposite1;
				opposite1 = SENTINEL_PTR;	// endOp - 1
				done1++;
				if (done1 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::store<Reg, stream>(a3, (Reg*)C1 + 1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
						origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 3); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
						origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
						origami_utils::store<Reg, stream>(a12, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 5);
						origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 6); origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 7);
					}

					// push to next queue
					origami_utils::MergeStream job((char*)__C1, j2.len + j3.len);
					Q_next->push(job);

					// get next job
					done1 = 0;
					if (!Q->empty()) {
						j2 = Q->front(); Q->pop();
						j3 = Q->front(); Q->pop();

						X1 = (Item*)(j2.A), Y1 = (Item*)(j3.A), C1 = (Item*)output, __C1 = C1;
						endX1 = (Item*)(j2.A + j2.len), endY1 = (Item*)(j3.A + j3.len);
						output += j2.len + j3.len;

						if constexpr (nreg == 1) {
							origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
						}
						else if constexpr (nreg == 2) {
							origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
							origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::load<Reg>(a6, (Reg*)X1); origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
							origami_utils::load<Reg>(a9, (Reg*)Y1); origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1); origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);
							origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1); origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);
						}

						X1 += INC; Y1 += INC;
						loadFrom1 = X1, opposite1 = Y1;
					}
					else {
						// no more jobs; finish the other two
						while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom2 != endX2 && loadFrom2 != endY2) {
							Item comp1 = *loadFrom0;
							Item comp2 = *opposite0;
							Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
							opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
							loadFrom0 = tmp;

							comp1 = *loadFrom2;
							comp2 = *opposite2;
							tmp = comp1 < comp2 ? loadFrom2 : opposite2;
							opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
							loadFrom2 = tmp;

							origami_utils::rswap<Reg, Item>(a0, a1);
							origami_utils::rswap<Reg, Item>(a4, a5);

							origami_utils::store<Reg, stream>(a0, (Reg*)C0);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2);

							origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

							_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

							loadFrom0 += INC; loadFrom2 += INC;

							C0 += INC; C2 += INC;
						}

						while (loadFrom0 != endX0 && loadFrom0 != endY0) {
							Item comp1 = *loadFrom0;
							Item comp2 = *opposite0;
							Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
							opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
							loadFrom0 = tmp;

							origami_utils::rswap<Reg, Item>(a0, a1);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0);
							origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
							loadFrom0 += INC;
							C0 += INC;
						}
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;

						if (opposite0 != SENTINEL_PTR) {
							Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
							while (opposite0 != endOpposite0) {
								_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a0, (Reg*)opposite0);
								opposite0 += INC;
								origami_utils::rswap<Reg, Item>(a0, a1);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;

						while (loadFrom2 != endX2 && loadFrom2 != endY2) {
							Item comp1 = *loadFrom2;
							Item comp2 = *opposite2;
							Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
							opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
							loadFrom2 = tmp;

							origami_utils::rswap<Reg, Item>(a4, a5);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
							loadFrom2 += INC;
							C2 += INC;
						}
						origami_utils::rswap<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;

						if (opposite2 != SENTINEL_PTR) {
							Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;
							while (opposite2 != endOpposite2) {
								_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a4, (Reg*)opposite2);
								opposite2 += INC;
								origami_utils::rswap<Reg, Item>(a4, a5);
								origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;


						origami_utils::MergeStream job1((char*)__C0, j0.len + j1.len);
						origami_utils::MergeStream job2((char*)__C2, j4.len + j5.len);
						Q_next->push(job1);
						Q_next->push(job2);
						break;
					}
				}
			}
			if (loadFrom2 == endX2 || loadFrom2 == endY2) {
				loadFrom2 = opposite2;
				opposite2 = SENTINEL_PTR;	// endOp - 1
				done2++;
				if (done2 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2);
						origami_utils::store<Reg, stream>(a5, (Reg*)C2 + 1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a8, (Reg*)C2); origami_utils::store<Reg, stream>(a10, (Reg*)C2 + 1);
						origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C2 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);
						origami_utils::store<Reg, stream>(a12, (Reg*)C2); origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1); origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);
						origami_utils::store<Reg, stream>(a15, (Reg*)C2 + 3); origami_utils::store<Reg, stream>(a16, (Reg*)C2 + 4); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);
						origami_utils::store<Reg, stream>(a16, (Reg*)C2); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
						origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);
						origami_utils::store<Reg, stream>(a20, (Reg*)C2 + 4); origami_utils::store<Reg, stream>(a21, (Reg*)C2 + 5);
						origami_utils::store<Reg, stream>(a22, (Reg*)C2 + 6); origami_utils::store<Reg, stream>(a23, (Reg*)C2 + 7);
					}

					// push to next queue
					origami_utils::MergeStream job((char*)__C2, j4.len + j5.len);
					Q_next->push(job);

					// get next job
					done2 = 0;
					if (!Q->empty()) {
						j4 = Q->front(); Q->pop();
						j5 = Q->front(); Q->pop();

						X2 = (Item*)(j4.A), Y2 = (Item*)(j5.A), C2 = (Item*)output, __C2 = C2;
						endX2 = (Item*)(j4.A + j4.len), endY2 = (Item*)(j5.A + j5.len);
						output += j4.len + j5.len;

						if constexpr (nreg == 1) {
							origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
						}
						else if constexpr (nreg == 2) {
							origami_utils::load<Reg>(a8, (Reg*)X2); origami_utils::load<Reg>(a9, (Reg*)X2 + 1);
							origami_utils::load<Reg>(a10, (Reg*)Y2); origami_utils::load<Reg>(a11, (Reg*)Y2 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_utils::load<Reg>(a12, (Reg*)X2); origami_utils::load<Reg>(a13, (Reg*)X2 + 1);		origami_utils::load<Reg>(a14, (Reg*)X2 + 2);
							origami_utils::load<Reg>(a15, (Reg*)Y2); origami_utils::load<Reg>(a16, (Reg*)Y2 + 1);	origami_utils::load<Reg>(a17, (Reg*)Y2 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_utils::load<Reg>(a16, (Reg*)X2); origami_utils::load<Reg>(a17, (Reg*)X2 + 1); origami_utils::load<Reg>(a18, (Reg*)X2 + 2); origami_utils::load<Reg>(a19, (Reg*)X2 + 3);
							origami_utils::load<Reg>(a20, (Reg*)Y2); origami_utils::load<Reg>(a21, (Reg*)Y2 + 1); origami_utils::load<Reg>(a22, (Reg*)Y2 + 2); origami_utils::load<Reg>(a23, (Reg*)Y2 + 3);
						}

						X2 += INC; Y2 += INC;
						loadFrom2 = X2, opposite2 = Y2;
					}
					else {
						// no more jobs; finish the other two
						while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1) {
							Item comp1 = *loadFrom0;
							Item comp2 = *opposite0;
							Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
							opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
							loadFrom0 = tmp;

							comp1 = *loadFrom1;
							comp2 = *opposite1;
							tmp = comp1 < comp2 ? loadFrom1 : opposite1;
							opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							origami_utils::rswap<Reg, Item>(a0, a1);
							origami_utils::rswap<Reg, Item>(a2, a3);

							origami_utils::store<Reg, stream>(a0, (Reg*)C0);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1);

							origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);

							_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);

							loadFrom0 += INC;
							loadFrom1 += INC;

							C0 += INC;
							C1 += INC;
						}
						while (loadFrom0 != endX0 && loadFrom0 != endY0) {
							Item comp1 = *loadFrom0;
							Item comp2 = *opposite0;
							Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
							opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
							loadFrom0 = tmp;

							origami_utils::rswap<Reg, Item>(a0, a1);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0);
							origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
							loadFrom0 += INC;
							C0 += INC;
						}
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;

						if (opposite0 != SENTINEL_PTR) {
							Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
							while (opposite0 != endOpposite0) {
								_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a0, (Reg*)opposite0);
								opposite0 += INC;
								origami_utils::rswap<Reg, Item>(a0, a1);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;

						while (loadFrom1 != endX1 && loadFrom1 != endY1) {
							Item comp1 = *loadFrom1;
							Item comp2 = *opposite1;
							Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
							opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
							loadFrom1 += INC;
							C1 += INC;
						}
						origami_utils::rswap<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;

						if (opposite1 != SENTINEL_PTR) {
							Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
							while (opposite1 != endOpposite1) {
								_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a2, (Reg*)opposite1);
								opposite1 += INC;
								origami_utils::rswap<Reg, Item>(a2, a3);
								origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;

						origami_utils::MergeStream job((char*)__C0, j0.len + j1.len);
						Q_next->push(job);
						break;
					}
				}
			}
		}
	}

	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_2x_3(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD, Item* E, Item* F) {

		// ** NOTE: tail handling not added for nreg = 4

		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = E;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = F;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;

		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;
		Item SENTINEL = get_max<Item>();
		Item* SENTINEL_PTR = &SENTINEL;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0); origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1); origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1); origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1); origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);
			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1); origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);
			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1); origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);
			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1); origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1;
		Item* opposite0 = Y0, * opposite1 = Y1;

		ui done0 = 0, done1 = 0;

		while (1) {

			while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1)
			{
				bool first = *loadFrom0 <= *opposite0;
				Item* tmp = first ? loadFrom0 : opposite0;
				opposite0 = first ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				first = *loadFrom1 <= *opposite1;
				tmp = first ? loadFrom1 : opposite1;
				opposite1 = first ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				if constexpr (nreg == 1) {
					origami_merge_network::merge_network2<Reg, Item>(a0, a1);
					origami_merge_network::merge_network2<Reg, Item>(a2, a3);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
				}
				else if constexpr (nreg == 2) {
					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a4, (Reg*)loadFrom1);	origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
					origami_utils::load<Reg>(a6, (Reg*)loadFrom1);	origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom1);	origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);	origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);
				}

				_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
				_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);

				loadFrom0 += INC; loadFrom1 += INC;

				C0 += INC; C1 += INC;
			}

			// at least one stream exhausted
			if (loadFrom0 == endX0 || loadFrom0 == endY0) {
				//Item* endOp = (loadFrom0 == endX0) ? endY0 : endX0;
				loadFrom0 = opposite0;
				opposite0 = SENTINEL_PTR; //endOp - 1;
				done0++;
				if (done0 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);

						/*origami_utils::print<Reg, Item>(a0);
						origami_utils::print<Reg, Item>(a1);*/
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
						origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
						origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3); origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
						origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5); origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 6); origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 7);
					}


					// no more jobs; finish the other one
					while (loadFrom1 != endX1 && loadFrom1 != endY1)
					{
						bool first = *loadFrom1 <= *opposite1;
						Item* tmp = first ? loadFrom1 : opposite1;
						opposite1 = first ? opposite1 : loadFrom1;
						loadFrom1 = tmp;

						if constexpr (nreg == 1) {
							origami_merge_network::merge_network2<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
						}
						else if constexpr (nreg == 2) {
							origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
							origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom1);	origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
							origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
							origami_utils::load<Reg>(a6, (Reg*)loadFrom1);	origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
							origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
							origami_utils::load<Reg>(a8, (Reg*)loadFrom1);	origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1); origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);	origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);
						}
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						loadFrom1 += INC;
						C1 += INC;
					}
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
						origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
					}
					C1 += INC;
					if (opposite1 != SENTINEL_PTR) {
						Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
						while (opposite1 != endOpposite1) {
							_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
							if constexpr (nreg == 1) {
								origami_utils::load<Reg>(a2, (Reg*)opposite1);
								origami_merge_network::merge_network2<Reg, Item>(a2, a3);
								origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							}
							else if constexpr (nreg == 2) {
								origami_utils::load<Reg>(a4, (Reg*)opposite1); origami_utils::load<Reg>(a5, (Reg*)opposite1 + 1);
								origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
								origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
							}
							else if constexpr (nreg == 3) {
								origami_utils::load<Reg>(a6, (Reg*)opposite1); origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a8, (Reg*)opposite1 + 2);
								origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
								origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
							}
							else if constexpr (nreg == 4) {
								origami_utils::load<Reg>(a8, (Reg*)opposite1); origami_utils::load<Reg>(a9, (Reg*)opposite1 + 1); origami_utils::load<Reg>(a10, (Reg*)opposite1 + 2); 	origami_utils::load<Reg>(a11, (Reg*)opposite1 + 3);

								origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
								origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
								origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
							}
							opposite1 += INC;
							C1 += INC;
						}
					}
					if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a3, (Reg*)C1);
					else if constexpr (nreg == 2) {
						origami_utils::store<Reg, stream>(a6, (Reg*)C1);
						origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
					}
					else if constexpr (nreg == 3) {
						origami_utils::store<Reg, stream>(a9, (Reg*)C1);
						origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 2);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network4<Reg, Item>(a12, a13, a14, a15);
						origami_utils::store<Reg, stream>(a12, (Reg*)C1);
						origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 2);
						origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 3);
					}
					C1 += INC;
					break;
				}
			}
			if (loadFrom1 == endX1 || loadFrom1 == endY1) {
				//Item* endOp = (loadFrom1 == endX1) ? endY1 : endX1;
				loadFrom1 = opposite1;
				opposite1 = SENTINEL_PTR;	// endOp - 1
				done1++;
				if (done1 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::store<Reg, stream>(a3, (Reg*)C1 + 1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
						origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 3); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
						origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
						origami_utils::store<Reg, stream>(a12, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 5);
						origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 6); origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 7);
					}

					// finish the other one
					while (loadFrom0 != endX0 && loadFrom0 != endY0)
					{
						bool first = *loadFrom0 <= *opposite0;
						Item* tmp = first ? loadFrom0 : opposite0;
						opposite0 = first ? opposite0 : loadFrom0;
						loadFrom0 = tmp;

						if constexpr (nreg == 1) {
							origami_merge_network::merge_network2<Reg, Item>(a0, a1);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0);
							origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
						}
						else if constexpr (nreg == 2) {
							origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
							origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
						}
						else if constexpr (nreg == 3) {
							origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
							origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 	origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
						}
						else if constexpr (nreg == 4) {
							origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
							origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);
						}

						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						loadFrom0 += INC; C0 += INC;
					}
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					}
					C0 += INC;

					if (opposite0 != SENTINEL_PTR) {
						Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
						while (opposite0 != endOpposite0) {
							_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
							if constexpr (nreg == 1) {
								origami_utils::load<Reg>(a0, (Reg*)opposite0);
								origami_merge_network::merge_network2<Reg, Item>(a0, a1);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0);
							}
							else if constexpr (nreg == 2) {
								origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1);
								origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
							}
							else if constexpr (nreg == 3) {
								origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2);
								origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
							}
							else if constexpr (nreg == 4) {
								origami_utils::load<Reg>(a0, (Reg*)opposite0); origami_utils::load<Reg>(a1, (Reg*)opposite0 + 1); origami_utils::load<Reg>(a2, (Reg*)opposite0 + 2); 	origami_utils::load<Reg>(a3, (Reg*)opposite0 + 3);
								origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
								origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
							}
							opposite0 += INC;
							C0 += INC;
						}
					}
					if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a1, (Reg*)C0);
					else if constexpr (nreg == 2) {
						origami_utils::store<Reg, stream>(a2, (Reg*)C0);
						origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 1);
					}
					else if constexpr (nreg == 3) {
						origami_utils::store<Reg, stream>(a3, (Reg*)C0);
						origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 1);
						origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 2);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a4, (Reg*)C0);
						origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 1);
						origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 2);
						origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 3);
					}
					C0 += INC;
					break;
				}
			}

		}


	}

	template <typename Reg, bool stream, ui nreg, typename Item>
	FORCEINLINE void mergebl2_vectorized_3x_3(Item* A, ui64 lenA, Item* B, ui64 lenB, Item* C, ui64 lenC, Item* D, ui64 lenD,
		Item* E, ui64 lenE, Item* F, ui64 lenF, Item* G, Item* H, Item* I) {
		Item* __restrict X0 = A, * __restrict Y0 = B, * __restrict C0 = G;
		Item* __restrict endX0 = A + lenA, * __restrict endY0 = B + lenB;
		Item* __restrict X1 = C, * __restrict Y1 = D, * __restrict C1 = H;
		Item* __restrict endX1 = C + lenC, * __restrict endY1 = D + lenD;
		Item* __restrict X2 = E, * __restrict Y2 = F, * __restrict C2 = I;
		Item* __restrict endX2 = E + lenE, * __restrict endY2 = F + lenF;

		register Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23;
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * nreg;
		Item SENTINEL = get_max<Item>();
		Item* SENTINEL_PTR = &SENTINEL;

		origami_utils::init_reg<Reg>(a0); origami_utils::init_reg<Reg>(a1); origami_utils::init_reg<Reg>(a2); origami_utils::init_reg<Reg>(a3);
		origami_utils::init_reg<Reg>(a4); origami_utils::init_reg<Reg>(a5); origami_utils::init_reg<Reg>(a6); origami_utils::init_reg<Reg>(a7);
		origami_utils::init_reg<Reg>(a8); origami_utils::init_reg<Reg>(a9); origami_utils::init_reg<Reg>(a10); origami_utils::init_reg<Reg>(a11);
		origami_utils::init_reg<Reg>(a12); origami_utils::init_reg<Reg>(a13); origami_utils::init_reg<Reg>(a14); origami_utils::init_reg<Reg>(a15);
		origami_utils::init_reg<Reg>(a16); origami_utils::init_reg<Reg>(a17); origami_utils::init_reg<Reg>(a18); origami_utils::init_reg<Reg>(a19);
		origami_utils::init_reg<Reg>(a20); origami_utils::init_reg<Reg>(a21); origami_utils::init_reg<Reg>(a22); origami_utils::init_reg<Reg>(a23);

		if constexpr (nreg == 1) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)Y0);
			origami_utils::load<Reg>(a2, (Reg*)X1); origami_utils::load<Reg>(a3, (Reg*)Y1);
			origami_utils::load<Reg>(a4, (Reg*)X2); origami_utils::load<Reg>(a5, (Reg*)Y2);
		}
		else if constexpr (nreg == 2) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)Y0); origami_utils::load<Reg>(a3, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a4, (Reg*)X1); origami_utils::load<Reg>(a5, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y1); origami_utils::load<Reg>(a7, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a8, (Reg*)X2); origami_utils::load<Reg>(a9, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a10, (Reg*)Y2); origami_utils::load<Reg>(a11, (Reg*)Y2 + 1);
		}
		else if constexpr (nreg == 3) {
			origami_utils::load<Reg>(a0, (Reg*)X0);		origami_utils::load<Reg>(a1, (Reg*)X0 + 1);		origami_utils::load<Reg>(a2, (Reg*)X0 + 2);
			origami_utils::load<Reg>(a3, (Reg*)Y0);		origami_utils::load<Reg>(a4, (Reg*)Y0 + 1);		origami_utils::load<Reg>(a5, (Reg*)Y0 + 2);
			origami_utils::load<Reg>(a6, (Reg*)X1);		origami_utils::load<Reg>(a7, (Reg*)X1 + 1);		origami_utils::load<Reg>(a8, (Reg*)X1 + 2);
			origami_utils::load<Reg>(a9, (Reg*)Y1);		origami_utils::load<Reg>(a10, (Reg*)Y1 + 1);	origami_utils::load<Reg>(a11, (Reg*)Y1 + 2);
			origami_utils::load<Reg>(a12, (Reg*)X2);	origami_utils::load<Reg>(a13, (Reg*)X2 + 1);	origami_utils::load<Reg>(a14, (Reg*)X2 + 2);
			origami_utils::load<Reg>(a15, (Reg*)Y2);	origami_utils::load<Reg>(a16, (Reg*)Y2 + 1);	origami_utils::load<Reg>(a17, (Reg*)Y2 + 2);
		}
		else if constexpr (nreg == 4) {
			origami_utils::load<Reg>(a0, (Reg*)X0); origami_utils::load<Reg>(a1, (Reg*)X0 + 1);
			origami_utils::load<Reg>(a2, (Reg*)X0 + 2); origami_utils::load<Reg>(a3, (Reg*)X0 + 3);

			origami_utils::load<Reg>(a4, (Reg*)Y0); origami_utils::load<Reg>(a5, (Reg*)Y0 + 1);
			origami_utils::load<Reg>(a6, (Reg*)Y0 + 2); origami_utils::load<Reg>(a7, (Reg*)Y0 + 3);

			origami_utils::load<Reg>(a8, (Reg*)X1); origami_utils::load<Reg>(a9, (Reg*)X1 + 1);
			origami_utils::load<Reg>(a10, (Reg*)X1 + 2); origami_utils::load<Reg>(a11, (Reg*)X1 + 3);

			origami_utils::load<Reg>(a12, (Reg*)Y1); origami_utils::load<Reg>(a13, (Reg*)Y1 + 1);
			origami_utils::load<Reg>(a14, (Reg*)Y1 + 2); origami_utils::load<Reg>(a15, (Reg*)Y1 + 3);

			origami_utils::load<Reg>(a16, (Reg*)X2); origami_utils::load<Reg>(a17, (Reg*)X2 + 1);
			origami_utils::load<Reg>(a18, (Reg*)X2 + 2); origami_utils::load<Reg>(a19, (Reg*)X2 + 3);

			origami_utils::load<Reg>(a20, (Reg*)Y2); origami_utils::load<Reg>(a21, (Reg*)Y2 + 1);
			origami_utils::load<Reg>(a22, (Reg*)Y2 + 2); origami_utils::load<Reg>(a23, (Reg*)Y2 + 3);
		}

		X0 += INC; Y0 += INC; X1 += INC; Y1 += INC; X2 += INC; Y2 += INC;

		Item* loadFrom0 = X0, * loadFrom1 = X1, * loadFrom2 = X2;
		Item* opposite0 = Y0, * opposite1 = Y1, * opposite2 = Y2;

		ui done0 = 0, done1 = 0, done2 = 0;


		while (1) {
			while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2)
			{
				bool first = *loadFrom0 <= *opposite0;
				Item* tmp = first ? loadFrom0 : opposite0;
				opposite0 = first ? opposite0 : loadFrom0;
				loadFrom0 = tmp;

				first = *loadFrom1 <= *opposite1;
				tmp = first ? loadFrom1 : opposite1;
				opposite1 = first ? opposite1 : loadFrom1;
				loadFrom1 = tmp;

				first = *loadFrom2 <= *opposite2;
				tmp = first ? loadFrom2 : opposite2;
				opposite2 = first ? opposite2 : loadFrom2;
				loadFrom2 = tmp;

				if constexpr (nreg == 1) {
					origami_utils::rswap<Reg, Item>(a0, a1); 
					origami_utils::rswap<Reg, Item>(a2, a3); 
					origami_utils::rswap<Reg, Item>(a4, a5);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1);
					origami_utils::store<Reg, stream>(a4, (Reg*)C2);

					if constexpr (stream) {
						if constexpr (sizeof(Reg) == 64) {							// AVX-512
							_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
						}
						else {
							_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
							_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
						}
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0); origami_utils::load<Reg>(a2, (Reg*)loadFrom1); origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
				}
				else if constexpr (nreg == 2) {
					/*origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
					origami_utils::rswap<Reg, Item>(a1, a2);

					origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a5, a7);
					origami_utils::rswap<Reg, Item>(a5, a6);

					origami_utils::rswap<Reg, Item>(a8, a10); origami_utils::rswap<Reg, Item>(a9, a11);
					origami_utils::rswap<Reg, Item>(a9, a10);*/

					origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
					origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
					origami_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);


					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a4, (Reg*)C1);
					origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a8, (Reg*)C2);
					origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a4, (Reg*)loadFrom1);
					origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom2);
					origami_utils::load<Reg>(a9, (Reg*)loadFrom2 + 1);
				}
				else if constexpr (nreg == 3) {
					origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
					origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
					origami_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);

					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);

					origami_utils::store<Reg, stream>(a6, (Reg*)C1);
					origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);

					origami_utils::store<Reg, stream>(a12, (Reg*)C2);
					origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1);
					origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);

					origami_utils::load<Reg>(a6, (Reg*)loadFrom1);
					origami_utils::load<Reg>(a7, (Reg*)loadFrom1 + 1);
					origami_utils::load<Reg>(a8, (Reg*)loadFrom1 + 2);

					origami_utils::load<Reg>(a12, (Reg*)loadFrom2);
					origami_utils::load<Reg>(a13, (Reg*)loadFrom2 + 1);
					origami_utils::load<Reg>(a14, (Reg*)loadFrom2 + 2);
				}
				else if constexpr (nreg == 4) {
					origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);


					origami_utils::store<Reg, stream>(a0, (Reg*)C0);
					origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
					origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
					origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);

					origami_utils::store<Reg, stream>(a8, (Reg*)C1);
					origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
					origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2);
					origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);

					origami_utils::store<Reg, stream>(a16, (Reg*)C2);
					origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
					origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2);
					origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);

					if constexpr (sizeof(Reg) == 64) {
						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
					}
					else {
						_mm_prefetch((char*)(loadFrom0 + 64), _MM_HINT_T2);		// prefetch 512 is slower
						_mm_prefetch((char*)(loadFrom1 + 64), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 64), _MM_HINT_T2);
					}

					origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
					origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
					origami_utils::load<Reg>(a2, (Reg*)loadFrom0 + 2);
					origami_utils::load<Reg>(a3, (Reg*)loadFrom0 + 3);

					origami_utils::load<Reg>(a8, (Reg*)loadFrom1);
					origami_utils::load<Reg>(a9, (Reg*)loadFrom1 + 1);
					origami_utils::load<Reg>(a10, (Reg*)loadFrom1 + 2);
					origami_utils::load<Reg>(a11, (Reg*)loadFrom1 + 3);

					origami_utils::load<Reg>(a16, (Reg*)loadFrom2);
					origami_utils::load<Reg>(a17, (Reg*)loadFrom2 + 1);
					origami_utils::load<Reg>(a18, (Reg*)loadFrom2 + 2);
					origami_utils::load<Reg>(a19, (Reg*)loadFrom2 + 3);
				}

				loadFrom0 += INC; loadFrom1 += INC; loadFrom2 += INC;
				C0 += INC; C1 += INC; C2 += INC;
			}
			// at least one stream exhausted
			if (loadFrom0 == endX0 || loadFrom0 == endY0) {
				//Item* endOp = (loadFrom0 == endX0) ? endY0 : endX0;
				loadFrom0 = opposite0;
				opposite0 = SENTINEL_PTR; //endOp - 1;
				done0++;
				if (done0 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);

						/*origami_utils::print<Reg, Item>(a0);
						origami_utils::print<Reg, Item>(a1);*/
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a0, a1, a2, a3);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1);
						origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a0, a1, a2, a3, a4, a5);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2);
						origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3); origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0); origami_utils::store<Reg, stream>(a1, (Reg*)C0 + 1); origami_utils::store<Reg, stream>(a2, (Reg*)C0 + 2); origami_utils::store<Reg, stream>(a3, (Reg*)C0 + 3);
						origami_utils::store<Reg, stream>(a4, (Reg*)C0 + 4); origami_utils::store<Reg, stream>(a5, (Reg*)C0 + 5); origami_utils::store<Reg, stream>(a6, (Reg*)C0 + 6); origami_utils::store<Reg, stream>(a7, (Reg*)C0 + 7);
					}

					// finish the other two
					/*
					ui done10 = 0, done20 = 0;
					while (1) {
						while (loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2) {
							Item comp1 = *loadFrom1;
							Item comp2 = *opposite1;
							Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
							opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							comp1 = *loadFrom2;
							comp2 = *opposite2;
							tmp = comp1 < comp2 ? loadFrom2 : opposite2;
							opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
							loadFrom2 = tmp;

							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::rswap<Reg, Item>(a4, a5);

							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2);

							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

							_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
							_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

							loadFrom1 += INC; loadFrom2 += INC;

							C1 += INC; C2 += INC;
						}

						if (loadFrom1 == endX1 && loadFrom1 == endY1) {
							loadFrom1 = opposite1;
							opposite1 = SENTINEL_PTR;
							done10++;
							if (done10 == 2) {
								if constexpr (nreg == 1) {
									irl_merge_network::merge_network2<Reg, Item>(a2, a3);
									origami_utils::store<Reg, stream>(a2, (Reg*)C1);
									origami_utils::store<Reg, stream>(a3, (Reg*)C1 + 1);
								}
								else if constexpr (nreg == 2) {
									irl_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
									origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
									origami_utils::store<Reg, stream>(a6, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 3);
								}
								else if constexpr (nreg == 3) {
									irl_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
									origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
									origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 3); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 5);
								}
								else if constexpr (nreg == 4) {
									irl_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
									origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
									origami_utils::store<Reg, stream>(a12, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 5); origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 6); origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 7);
								}


								// no more jobs; finish the other one
								while (loadFrom2 != endX2 && loadFrom2 != endY2)
								{
									bool first = *loadFrom2 <= *opposite2;
									Item* tmp = first ? loadFrom2 : opposite2;
									opposite2 = first ? opposite2 : loadFrom2;
									loadFrom2 = tmp;

									if constexpr (nreg == 1) {
										irl_merge_network::merge_network2<Reg, Item>(a4, a5);
										origami_utils::store<Reg, stream>(a4, (Reg*)C2);
										origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
									}
									else if constexpr (nreg == 2) {
										irl_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);
										origami_utils::store<Reg, stream>(a8, (Reg*)C2); origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);
										origami_utils::load<Reg>(a8, (Reg*)loadFrom2);	origami_utils::load<Reg>(a9, (Reg*)loadFrom2 + 1);
									}
									else if constexpr (nreg == 3) {
										irl_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);
										origami_utils::store<Reg, stream>(a12, (Reg*)C2); origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1); origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);
										origami_utils::load<Reg>(a12, (Reg*)loadFrom2);	origami_utils::load<Reg>(a13, (Reg*)loadFrom2 + 1); origami_utils::load<Reg>(a14, (Reg*)loadFrom2 + 2);
									}
									else if constexpr (nreg == 4) {
										irl_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);
										origami_utils::store<Reg, stream>(a16, (Reg*)C2); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1); origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);
										origami_utils::load<Reg>(a16, (Reg*)loadFrom2);	origami_utils::load<Reg>(a17, (Reg*)loadFrom2 + 1); origami_utils::load<Reg>(a18, (Reg*)loadFrom2 + 2);	origami_utils::load<Reg>(a19, (Reg*)loadFrom2 + 3);
									}
									_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);
									loadFrom2 += INC;
									C2 += INC;
								}
								if constexpr (nreg == 1) {
									irl_merge_network::merge_network2<Reg, Item>(a4, a5);
									origami_utils::store<Reg, stream>(a4, (Reg*)C2);
								}
								else if constexpr (nreg == 2) {
									irl_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);
									origami_utils::store<Reg, stream>(a8, (Reg*)C2); origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);
								}
								else if constexpr (nreg == 3) {
									irl_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);
									origami_utils::store<Reg, stream>(a12, (Reg*)C2); origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1); origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);
								}
								else if constexpr (nreg == 4) {
									irl_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);
									origami_utils::store<Reg, stream>(a16, (Reg*)C2); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
									origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);
								}
								C2 += INC;
								if (opposite2 != SENTINEL_PTR) {
									Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;
									while (opposite2 != endOpposite2) {
										_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
										if constexpr (nreg == 1) {
											origami_utils::load<Reg>(a4, (Reg*)opposite2);
											irl_merge_network::merge_network2<Reg, Item>(a4, a5);
											origami_utils::store<Reg, stream>(a4, (Reg*)C2);
										}
										else if constexpr (nreg == 2) {
											origami_utils::load<Reg>(a8, (Reg*)opposite2); origami_utils::load<Reg>(a9, (Reg*)opposite2 + 1);
											irl_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);
											origami_utils::store<Reg, stream>(a8, (Reg*)C2); origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 1);
										}
										else if constexpr (nreg == 3) {
											origami_utils::load<Reg>(a12, (Reg*)opposite2); origami_utils::load<Reg>(a13, (Reg*)opposite2 + 1); origami_utils::load<Reg>(a14, (Reg*)opposite2 + 2);
											irl_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);
											origami_utils::store<Reg, stream>(a12, (Reg*)C2); origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1); origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);
										}
										else if constexpr (nreg == 4) {
											origami_utils::load<Reg>(a16, (Reg*)opposite2); origami_utils::load<Reg>(a17, (Reg*)opposite2 + 1); origami_utils::load<Reg>(a18, (Reg*)opposite2 + 2); 	origami_utils::load<Reg>(a19, (Reg*)opposite2 + 3);
											irl_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);
											origami_utils::store<Reg, stream>(a16, (Reg*)C2); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
											origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);
										}
										opposite2 += INC;
										C2 += INC;
									}
								}
								if constexpr (nreg == 1) origami_utils::store<Reg, stream>(a5, (Reg*)C2);
								else if constexpr (nreg == 2) {
									origami_utils::store<Reg, stream>(a10, (Reg*)C2);
									origami_utils::store<Reg, stream>(a11, (Reg*)C2 + 1);
								}
								else if constexpr (nreg == 3) {
									origami_utils::store<Reg, stream>(a15, (Reg*)C2);
									origami_utils::store<Reg, stream>(a16, (Reg*)C2 + 1);
									origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 2);
								}
								else if constexpr (nreg == 4) {
									irl_merge_network::merge_network4<Reg, Item>(a20, a21, a22, a23);
									origami_utils::store<Reg, stream>(a20, (Reg*)C2);
									origami_utils::store<Reg, stream>(a21, (Reg*)C2 + 1);
									origami_utils::store<Reg, stream>(a22, (Reg*)C2 + 2);
									origami_utils::store<Reg, stream>(a23, (Reg*)C2 + 3);
								}
								C2 += INC;
								break;
							}
						}
						if (loadFrom2 == endX2 && loadFrom2 == endY2) {
							loadFrom2 = opposite2;
							opposite2 = SENTINEL_PTR;
							done20++;
							if (done20 == 2) {

							}
						}

						while (loadFrom1 != endX1 && loadFrom1 != endY1) {
							Item comp1 = *loadFrom1;
							Item comp2 = *opposite1;
							Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
							opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
							loadFrom1 = tmp;

							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1);
							origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
							loadFrom1 += INC;
							C1 += INC;
						}
						origami_utils::rswap<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
						if (opposite1 != SENTINEL_PTR) {
							Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
							while (opposite1 != endOpposite1) {
								_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a2, (Reg*)opposite1);
								opposite1 += INC;
								origami_utils::rswap<Reg, Item>(a2, a3);
								origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;

						while (loadFrom2 != endX2 && loadFrom2 != endY2) {
							Item comp1 = *loadFrom2;
							Item comp2 = *opposite2;
							Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
							opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
							loadFrom2 = tmp;

							origami_utils::rswap<Reg, Item>(a4, a5);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2);
							origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
							loadFrom2 += INC;
							C2 += INC;
						}
						origami_utils::rswap<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;

						if (opposite2 != SENTINEL_PTR) {
							Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;
							while (opposite2 != endOpposite2) {
								_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
								origami_utils::load<Reg>(a4, (Reg*)opposite2);
								opposite2 += INC;
								origami_utils::rswap<Reg, Item>(a4, a5);
								origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
							}
						}
						origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;
						break;
					}*/
					
					while (loadFrom1 != endX1 && loadFrom1 != endY1 && loadFrom2 != endX2 && loadFrom2 != endY2) {
						Item comp1 = *loadFrom1;
						Item comp2 = *opposite1;
						Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
						opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
						loadFrom1 = tmp;

						comp1 = *loadFrom2;
						comp2 = *opposite2;
						tmp = comp1 < comp2 ? loadFrom2 : opposite2;
						opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
						loadFrom2 = tmp;

						origami_utils::rswap<Reg, Item>(a2, a3);
						origami_utils::rswap<Reg, Item>(a4, a5);

						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2);

						origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
						origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

						loadFrom1 += INC;
						loadFrom2 += INC;

						C1 += INC;
						C2 += INC;
					}

					while (loadFrom1 != endX1 && loadFrom1 != endY1) {
						Item comp1 = *loadFrom1;
						Item comp2 = *opposite1;
						Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
						opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
						loadFrom1 = tmp;

						origami_utils::rswap<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
						loadFrom1 += INC;
						C1 += INC;
					}
					origami_utils::rswap<Reg, Item>(a2, a3);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;

					if (opposite1 != SENTINEL_PTR) {
						Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
						while (opposite1 != endOpposite1) {
							_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
							origami_utils::load<Reg>(a2, (Reg*)opposite1);
							opposite1 += INC;
							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
						}
					}
					
					origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;
					
					while (loadFrom2 != endX2 && loadFrom2 != endY2) {
						Item comp1 = *loadFrom2;
						Item comp2 = *opposite2;
						Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
						opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
						loadFrom2 = tmp;

						origami_utils::rswap<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2);
						origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
						loadFrom2 += INC;
						C2 += INC;
					}
					origami_utils::rswap<Reg, Item>(a4, a5);
					origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;

					if (opposite2 != SENTINEL_PTR) {
						Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;
						while (opposite2 != endOpposite2) {
							_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
							origami_utils::load<Reg>(a4, (Reg*)opposite2);
							opposite2 += INC;
							origami_utils::rswap<Reg, Item>(a4, a5);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
						}
					}
					origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;
					break;
				}

			}
			if (loadFrom1 == endX1 || loadFrom1 == endY1) {
				//Item* endOp = (loadFrom1 == endX1) ? endY1 : endX1;
				loadFrom1 = opposite1;
				opposite1 = SENTINEL_PTR;	// endOp - 1
				done1++;
				if (done1 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::store<Reg, stream>(a3, (Reg*)C1 + 1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a4, a5, a6, a7);
						origami_utils::store<Reg, stream>(a4, (Reg*)C1); origami_utils::store<Reg, stream>(a5, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a6, a7, a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a6, (Reg*)C1); origami_utils::store<Reg, stream>(a7, (Reg*)C1 + 1); origami_utils::store<Reg, stream>(a8, (Reg*)C1 + 2);
						origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 3); origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a8, a9, a10, a11, a12, a13, a14, a15);
						origami_utils::store<Reg, stream>(a8, (Reg*)C1); origami_utils::store<Reg, stream>(a9, (Reg*)C1 + 1);
						origami_utils::store<Reg, stream>(a10, (Reg*)C1 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C1 + 3);
						origami_utils::store<Reg, stream>(a12, (Reg*)C1 + 4); origami_utils::store<Reg, stream>(a13, (Reg*)C1 + 5);
						origami_utils::store<Reg, stream>(a14, (Reg*)C1 + 6); origami_utils::store<Reg, stream>(a15, (Reg*)C1 + 7);
					}


					// no more jobs; finish the other two
					while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom2 != endX2 && loadFrom2 != endY2) {
						Item comp1 = *loadFrom0;
						Item comp2 = *opposite0;
						Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
						opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
						loadFrom0 = tmp;

						comp1 = *loadFrom2;
						comp2 = *opposite2;
						tmp = comp1 < comp2 ? loadFrom2 : opposite2;
						opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
						loadFrom2 = tmp;

						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::rswap<Reg, Item>(a4, a5);

						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2);

						origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
						origami_utils::load<Reg>(a4, (Reg*)loadFrom2);

						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom2 + 512), _MM_HINT_T2);

						loadFrom0 += INC; loadFrom2 += INC;

						C0 += INC; C2 += INC;
					}

					while (loadFrom0 != endX0 && loadFrom0 != endY0) {
						Item comp1 = *loadFrom0;
						Item comp2 = *opposite0;
						Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
						opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
						loadFrom0 = tmp;

						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
						loadFrom0 += INC;
						C0 += INC;
					}
					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;

					if (opposite0 != SENTINEL_PTR) {
						Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
						while (opposite0 != endOpposite0) {
							_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
							origami_utils::load<Reg>(a0, (Reg*)opposite0);
							opposite0 += INC;
							origami_utils::rswap<Reg, Item>(a0, a1);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
						}
					}
					origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;

					while (loadFrom2 != endX2 && loadFrom2 != endY2) {
						Item comp1 = *loadFrom2;
						Item comp2 = *opposite2;
						Item* tmp = comp1 < comp2 ? loadFrom2 : opposite2;
						opposite2 = comp1 < comp2 ? opposite2 : loadFrom2;
						loadFrom2 = tmp;

						origami_utils::rswap<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2);
						origami_utils::load<Reg>(a4, (Reg*)loadFrom2);
						loadFrom2 += INC;
						C2 += INC;
					}
					origami_utils::rswap<Reg, Item>(a4, a5);
					origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;

					if (opposite2 != SENTINEL_PTR) {
						Item* endOpposite2 = (loadFrom2 == endX2) ? endY2 : endX2;
						while (opposite2 != endOpposite2) {
							_mm_prefetch((char*)(opposite2 + 64), _MM_HINT_T2);
							origami_utils::load<Reg>(a4, (Reg*)opposite2);
							opposite2 += INC;
							origami_utils::rswap<Reg, Item>(a4, a5);
							origami_utils::store<Reg, stream>(a4, (Reg*)C2); C2 += INC;
						}
					}
					origami_utils::store<Reg, stream>(a5, (Reg*)C2); C2 += INC;
					break;
				}
			}
			if (loadFrom2 == endX2 || loadFrom2 == endY2) {
				loadFrom2 = opposite2;
				opposite2 = SENTINEL_PTR;	// endOp - 1
				done2++;
				if (done2 == 2) {	// both streams exhausted, replace with new stream
					// dump register contents
					if constexpr (nreg == 1) {
						origami_merge_network::merge_network2<Reg, Item>(a4, a5);
						origami_utils::store<Reg, stream>(a4, (Reg*)C2);
						origami_utils::store<Reg, stream>(a5, (Reg*)C2 + 1);
					}
					else if constexpr (nreg == 2) {
						origami_merge_network::merge_network4<Reg, Item>(a8, a9, a10, a11);
						origami_utils::store<Reg, stream>(a8, (Reg*)C2); origami_utils::store<Reg, stream>(a10, (Reg*)C2 + 1);
						origami_utils::store<Reg, stream>(a9, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a11, (Reg*)C2 + 3);
					}
					else if constexpr (nreg == 3) {
						origami_merge_network::merge_network6<Reg, Item>(a12, a13, a14, a15, a16, a17);
						origami_utils::store<Reg, stream>(a12, (Reg*)C2); origami_utils::store<Reg, stream>(a13, (Reg*)C2 + 1); origami_utils::store<Reg, stream>(a14, (Reg*)C2 + 2);
						origami_utils::store<Reg, stream>(a15, (Reg*)C2 + 3); origami_utils::store<Reg, stream>(a16, (Reg*)C2 + 4); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 5);
					}
					else if constexpr (nreg == 4) {
						origami_merge_network::merge_network8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23);
						origami_utils::store<Reg, stream>(a16, (Reg*)C2); origami_utils::store<Reg, stream>(a17, (Reg*)C2 + 1);
						origami_utils::store<Reg, stream>(a18, (Reg*)C2 + 2); origami_utils::store<Reg, stream>(a19, (Reg*)C2 + 3);
						origami_utils::store<Reg, stream>(a20, (Reg*)C2 + 4); origami_utils::store<Reg, stream>(a21, (Reg*)C2 + 5);
						origami_utils::store<Reg, stream>(a22, (Reg*)C2 + 6); origami_utils::store<Reg, stream>(a23, (Reg*)C2 + 7);
					}

					// no more jobs; finish the other two
					while (loadFrom0 != endX0 && loadFrom0 != endY0 && loadFrom1 != endX1 && loadFrom1 != endY1) {
						Item comp1 = *loadFrom0;
						Item comp2 = *opposite0;
						Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
						opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
						loadFrom0 = tmp;

						comp1 = *loadFrom1;
						comp2 = *opposite1;
						tmp = comp1 < comp2 ? loadFrom1 : opposite1;
						opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
						loadFrom1 = tmp;

						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::rswap<Reg, Item>(a2, a3);

						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);

						origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
						origami_utils::load<Reg>(a2, (Reg*)loadFrom1);

						_mm_prefetch((char*)(loadFrom0 + 512), _MM_HINT_T2);
						_mm_prefetch((char*)(loadFrom1 + 512), _MM_HINT_T2);

						loadFrom0 += INC;
						loadFrom1 += INC;

						C0 += INC;
						C1 += INC;
					}
					while (loadFrom0 != endX0 && loadFrom0 != endY0) {
						Item comp1 = *loadFrom0;
						Item comp2 = *opposite0;
						Item* tmp = comp1 < comp2 ? loadFrom0 : opposite0;
						opposite0 = comp1 < comp2 ? opposite0 : loadFrom0;
						loadFrom0 = tmp;

						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, stream>(a0, (Reg*)C0);
						origami_utils::load<Reg>(a0, (Reg*)loadFrom0);
						loadFrom0 += INC;
						C0 += INC;
					}
					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;

					if (opposite0 != SENTINEL_PTR) {
						Item* endOpposite0 = (loadFrom0 == endX0) ? endY0 : endX0;
						while (opposite0 != endOpposite0) {
							_mm_prefetch((char*)(opposite0 + 64), _MM_HINT_T2);
							origami_utils::load<Reg>(a0, (Reg*)opposite0);
							opposite0 += INC;
							origami_utils::rswap<Reg, Item>(a0, a1);
							origami_utils::store<Reg, stream>(a0, (Reg*)C0); C0 += INC;
						}
					}
					origami_utils::store<Reg, stream>(a1, (Reg*)C0); C0 += INC;

					while (loadFrom1 != endX1 && loadFrom1 != endY1) {
						Item comp1 = *loadFrom1;
						Item comp2 = *opposite1;
						Item* tmp = comp1 < comp2 ? loadFrom1 : opposite1;
						opposite1 = comp1 < comp2 ? opposite1 : loadFrom1;
						loadFrom1 = tmp;

						origami_utils::rswap<Reg, Item>(a2, a3);
						origami_utils::store<Reg, stream>(a2, (Reg*)C1);
						origami_utils::load<Reg>(a2, (Reg*)loadFrom1);
						loadFrom1 += INC;
						C1 += INC;
					}
					origami_utils::rswap<Reg, Item>(a2, a3);
					origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;

					if (opposite1 != SENTINEL_PTR) {
						Item* endOpposite1 = (loadFrom1 == endX1) ? endY1 : endX1;
						while (opposite1 != endOpposite1) {
							_mm_prefetch((char*)(opposite1 + 64), _MM_HINT_T2);
							origami_utils::load<Reg>(a2, (Reg*)opposite1);
							opposite1 += INC;
							origami_utils::rswap<Reg, Item>(a2, a3);
							origami_utils::store<Reg, stream>(a2, (Reg*)C1); C1 += INC;
						}
					}
					origami_utils::store<Reg, stream>(a3, (Reg*)C1); C1 += INC;
					break;
				}
			}
		}
	}

	template <typename Reg, typename Item, bool STREAM, ui UNROLL, ui NREG_1x, ui NREG_2x, ui NREG_3x>
	FORCEINLINE void merge(Item* src, Item* dst, ui64 n, ui64 merge_start) {
		ui64 merge_next = merge_start << 1;
		ui64 n_merge = n / merge_next;

		if constexpr (UNROLL == 1) {
			Item* p1 = src, * p2 = dst;
			FOR(i, n_merge, 1) {
				if constexpr (std::is_same<Reg, Item>::value) {
					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
						mergebl2_scalar_kv<Item, NREG_1x>(p1, merge_start, p1 + merge_start, merge_start, p2);
					else
						mergebl2_scalar<Item, NREG_1x>(p1, merge_start, p1 + merge_start, merge_start, p2);
				}
				else
					mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(p1, merge_start, p1 + merge_start, merge_start, p2);
				p1 += merge_next; p2 += merge_next;
			}
				
		}
		else if constexpr (UNROLL == 2) {
			if (n_merge >= 2) {
				ui64 remaining = n_merge & 1;
				ui64 n_merge_duo = n_merge >> 1;
				Item* p1 = src, * p2 = dst;
				ui64 i = 0;
				while (i < n_merge_duo) {
					if constexpr (std::is_same<Reg, Item>::value) {
						if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
							mergebl2_scalar_kv<Item, NREG_2x>(p1, merge_start, p1 + merge_start, merge_start, p2);
						else
							mergebl2_scalar_2x<Item, NREG_2x>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p2, p2 + merge_next);
					}
					else
						mergebl2_vectorized_2x_3<Reg, STREAM, NREG_2x, Item>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p2, p2 + merge_next);
					p1 += 2 * merge_next; p2 += 2 * merge_next;
					i++;
				}
				if (remaining) {
					if constexpr (std::is_same<Reg, Item>::value) {
						if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
							mergebl2_scalar_kv<Item, NREG_1x>(p1, merge_start, p1 + merge_start, merge_start, p2);
						else
							mergebl2_scalar<Item, NREG_1x>(p1, merge_start, p1 + merge_start, merge_start, p2);
					}
					else
						mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(p1, merge_start, p1 + merge_start, merge_start, p2);
				}
			}
			else {
				if constexpr (std::is_same<Reg, Item>::value) {
					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
						mergebl2_scalar_kv<Item, NREG_1x>(src, merge_start, src + merge_start, merge_start, dst);
					else
						mergebl2_scalar<Item, NREG_1x>(src, merge_start, src + merge_start, merge_start, dst);
				}
				else
					mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(src, merge_start, src + merge_start, merge_start, dst);
			}
		}
		else if constexpr (UNROLL == 3) {
			if (n_merge >= 3) {
				ui64 remaining = n_merge % 3;
				ui64 n_merge_trio = n_merge / 3;
				Item* p1 = src, * p2 = dst;
				ui64 i = 0;
				while (i < n_merge_trio) {
					if constexpr (std::is_same<Reg, Item>::value) {
						if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
							mergebl2_scalar_kv<Item, NREG_3x>(p1, merge_start, p1 + merge_start, merge_start, p2);
						else
							mergebl2_scalar_3x<Item, NREG_3x>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p1 + 4 * merge_start, merge_start, p1 + 5 * merge_start, merge_start, p2, p2 + merge_next, p2 + 2 * merge_next);
					}
					else
						mergebl2_vectorized_3x_3<Reg, STREAM, NREG_3x, Item>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p1 + 4 * merge_start, merge_start, p1 + 5 * merge_start, merge_start, p2, p2 + merge_next, p2 + 2 * merge_next);
					p1 += 3 * merge_next; p2 += 3 * merge_next;
					i++;
				}
				if (remaining == 2) {
					if constexpr (std::is_same<Reg, Item>::value) {
						if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
							mergebl2_scalar_kv<Item, NREG_2x>(p1, merge_start, p1 + merge_start, merge_start, p2);
						else
							mergebl2_scalar_2x<Item, NREG_2x>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p2, p2 + merge_next);
					}
					else
						mergebl2_vectorized_2x_3<Reg, STREAM, NREG_2x, Item>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p2, p2 + merge_next);
				}					
				else if (remaining == 1)
					mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(p1, merge_start, p1 + merge_start, merge_start, p2);
			}
			else if (n_merge == 2) {
				if constexpr (std::is_same<Reg, Item>::value) {
					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
						mergebl2_scalar_kv<Item, NREG_2x>(src, merge_start, src + merge_start, merge_start, dst);
					else
						mergebl2_scalar_2x<Item, NREG_2x>(src, merge_start, src + merge_start, merge_start, src + 2 * merge_start, merge_start, src + 3 * merge_start, merge_start, dst, dst + merge_next);
				}
				else
					mergebl2_vectorized_2x_3<Reg, STREAM, NREG_2x, Item>(src, merge_start, src + merge_start, merge_start, src + 2 * merge_start, merge_start, src + 3 * merge_start, merge_start, dst, dst + merge_next);
			}
			else if (n_merge == 1) {
				if constexpr (std::is_same<Reg, Item>::value) {
					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
						mergebl2_scalar_kv<Item, NREG_1x>(src, merge_start, src + merge_start, merge_start, dst);
					else
						mergebl2_scalar<Item, NREG_1x>(src, merge_start, src + merge_start, merge_start, dst, dst + merge_next);
				}
				else
					mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(src, merge_start, src + merge_start, merge_start, dst);
			}
		}
		else if constexpr (UNROLL == 4) {
		/*
			if (n_merge >= 4) {
				ui64 remaining = n_merge & 3;
				ui64 n_merge_quad = n_merge >> 2;
				Item* p1 = src, * p2 = dst;
				ui64 i = 0;
				while (i < n_merge_quad) {
					mergebl2_vectorized_4x<Reg, STREAM, NREG_1x, Item>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p1 + 4 * merge_start, merge_start, p1 + 5 * merge_start, merge_start, p2, p2 + merge_next, p2 + 2 * merge_next);
					p1 += 4 * merge_next; p2 += 4 * merge_next;
					i++;
				}
				if (remaining == 3)
					mergebl2_vectorized_3x<Reg, STREAM, NREG_3x, Item>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p1 + 4 * merge_start, merge_start, p1 + 5 * merge_start, merge_start, p1 + 6 * merge_start, merge_start, p1 + 7 * merge_start, merge_start, p2, p2 + merge_next, p2 + 2 * merge_next, p2 + 4 * merge_next);
				else if (remaining == 2)
					mergebl2_vectorized_2x<Reg, STREAM, NREG_2x, Item>(p1, merge_start, p1 + merge_start, merge_start, p1 + 2 * merge_start, merge_start, p1 + 3 * merge_start, merge_start, p2, p2 + merge_next);
				else if (remaining == 1)
					mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(p1, merge_start, p1 + merge_start, merge_start, p2);
			}
			else if (n_merge == 3)
				mergebl2_vectorized_3x<Reg, STREAM, NREG_3x, Item>(src, merge_start, src + merge_start, merge_start, src + 2 * merge_start, merge_start, src + 3 * merge_start, merge_start, src + 4 * merge_start, merge_start, src + 5 * merge_start, merge_start, src + 6 * merge_start, merge_start, src + 7 * merge_start, merge_start, dst, dst + merge_next, dst + 2 * merge_next, dst + 4 * merge_next);
			else if (n_merge == 2)
				mergebl2_vectorized_2x<Reg, STREAM, NREG_2x, Item>(src, merge_start, src + merge_start, merge_start, src + 2 * merge_start, merge_start, src + 3 * merge_start, merge_start, dst, dst + merge_next);
			else if (n_merge == 1)
				mergebl2_vectorized<Reg, STREAM, NREG_1x, Item>(src, merge_start, src + merge_start, merge_start, dst);
		*/
		}
	}

	template <typename Reg, typename Item, bool STREAM, ui UNROLL, ui NREG_1x, ui NREG_2x, ui NREG_3x>
	FORCEINLINE Item* merge_series(Item* src, Item* dst, ui64 n, ui64 merge_start, ui64 sort_n) {
		while (merge_start < sort_n) {
			merge<Reg, Item, STREAM, UNROLL, NREG_1x, NREG_2x, NREG_3x>(src, dst, n, merge_start);
			merge_start <<= 1;
			Item* tmp = dst; dst = src; src = tmp;
		}
		return src;
	}
	
}
