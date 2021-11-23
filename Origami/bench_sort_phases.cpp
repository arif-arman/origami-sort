#include "commons.h"
#include "writer.h"
#include "utils.h"		
#include "merge_utils.h"
#include "sorter.h"
#include <iostream>

template <typename Item, typename Reg>
class InRegisterTest {
public:
	static constexpr ui Itemsize = sizeof(Item);
	static constexpr ui ItemsPerReg = sizeof(Reg) / Itemsize;

	InRegisterTest() {}

	// possible values of NREG: 4, 8, 16, 32
	// possible values of UNROLL: 1, 2, 4
	template <int NREG, int UNROLL>
	void in_register_sort() {
		constexpr ui64 n = NREG * ItemsPerReg * UNROLL;
		constexpr ui64 iter = 1e9;
		Item* d = new Item[n];
		Reg* d2 = (Reg*)d;
		datagen::random_writer<Item>(d, n);

		printf("Sorting in-register: reg %u, unroll %u ... ", NREG, UNROLL);

		hrc::time_point st, en;
		if constexpr (UNROLL == 1) {
			if constexpr (NREG == 4) {
				Reg a0, a1, a2, a3;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_sorter::in_register_sort<Item, Reg>(a0, a1, a2, a3);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
				origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
			}
			else if constexpr (NREG == 16) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			else if constexpr (NREG == 32) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort32<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
		}
		else if constexpr (UNROLL == 2) {
			if constexpr (NREG == 4) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort<Item, Reg>(a0, a1, a2, a3);
					origami_sorter::in_register_sort<Item, Reg>(a4, a5, a6, a7);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
				origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			else if constexpr (NREG == 16) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::in_register_sort16<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
			else if constexpr (NREG == 32) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31); origami_utils::load<Reg>(a32, d2 + 32); origami_utils::load<Reg>(a33, d2 + 33); origami_utils::load<Reg>(a34, d2 + 34); origami_utils::load<Reg>(a35, d2 + 35); origami_utils::load<Reg>(a36, d2 + 36); origami_utils::load<Reg>(a37, d2 + 37); origami_utils::load<Reg>(a38, d2 + 38); origami_utils::load<Reg>(a39, d2 + 39); origami_utils::load<Reg>(a40, d2 + 40); origami_utils::load<Reg>(a41, d2 + 41); origami_utils::load<Reg>(a42, d2 + 42); origami_utils::load<Reg>(a43, d2 + 43); origami_utils::load<Reg>(a44, d2 + 44); origami_utils::load<Reg>(a45, d2 + 45); origami_utils::load<Reg>(a46, d2 + 46); origami_utils::load<Reg>(a47, d2 + 47); origami_utils::load<Reg>(a48, d2 + 48); origami_utils::load<Reg>(a49, d2 + 49); origami_utils::load<Reg>(a50, d2 + 50); origami_utils::load<Reg>(a51, d2 + 51); origami_utils::load<Reg>(a52, d2 + 52); origami_utils::load<Reg>(a53, d2 + 53); origami_utils::load<Reg>(a54, d2 + 54); origami_utils::load<Reg>(a55, d2 + 55); origami_utils::load<Reg>(a56, d2 + 56); origami_utils::load<Reg>(a57, d2 + 57); origami_utils::load<Reg>(a58, d2 + 58); origami_utils::load<Reg>(a59, d2 + 59); origami_utils::load<Reg>(a60, d2 + 60); origami_utils::load<Reg>(a61, d2 + 61); origami_utils::load<Reg>(a62, d2 + 62); origami_utils::load<Reg>(a63, d2 + 63);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort32<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					origami_sorter::in_register_sort32<Item, Reg>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31); origami_utils::store<Reg>(a32, d2 + 32); origami_utils::store<Reg>(a33, d2 + 33); origami_utils::store<Reg>(a34, d2 + 34); origami_utils::store<Reg>(a35, d2 + 35); origami_utils::store<Reg>(a36, d2 + 36); origami_utils::store<Reg>(a37, d2 + 37); origami_utils::store<Reg>(a38, d2 + 38); origami_utils::store<Reg>(a39, d2 + 39); origami_utils::store<Reg>(a40, d2 + 40); origami_utils::store<Reg>(a41, d2 + 41); origami_utils::store<Reg>(a42, d2 + 42); origami_utils::store<Reg>(a43, d2 + 43); origami_utils::store<Reg>(a44, d2 + 44); origami_utils::store<Reg>(a45, d2 + 45); origami_utils::store<Reg>(a46, d2 + 46); origami_utils::store<Reg>(a47, d2 + 47); origami_utils::store<Reg>(a48, d2 + 48); origami_utils::store<Reg>(a49, d2 + 49); origami_utils::store<Reg>(a50, d2 + 50); origami_utils::store<Reg>(a51, d2 + 51); origami_utils::store<Reg>(a52, d2 + 52); origami_utils::store<Reg>(a53, d2 + 53); origami_utils::store<Reg>(a54, d2 + 54); origami_utils::store<Reg>(a55, d2 + 55); origami_utils::store<Reg>(a56, d2 + 56); origami_utils::store<Reg>(a57, d2 + 57); origami_utils::store<Reg>(a58, d2 + 58); origami_utils::store<Reg>(a59, d2 + 59); origami_utils::store<Reg>(a60, d2 + 60); origami_utils::store<Reg>(a61, d2 + 61); origami_utils::store<Reg>(a62, d2 + 62); origami_utils::store<Reg>(a63, d2 + 63);
			}
		}
		else if constexpr (UNROLL == 4) {
			if constexpr (NREG == 4) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort<Item, Reg>(a0, a1, a2, a3);
					origami_sorter::in_register_sort<Item, Reg>(a4, a5, a6, a7);
					origami_sorter::in_register_sort<Item, Reg>(a8, a9, a10, a11);
					origami_sorter::in_register_sort<Item, Reg>(a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::in_register_sort8<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
					origami_sorter::in_register_sort8<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
			else if constexpr (NREG == 16) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31); origami_utils::load<Reg>(a32, d2 + 32); origami_utils::load<Reg>(a33, d2 + 33); origami_utils::load<Reg>(a34, d2 + 34); origami_utils::load<Reg>(a35, d2 + 35); origami_utils::load<Reg>(a36, d2 + 36); origami_utils::load<Reg>(a37, d2 + 37); origami_utils::load<Reg>(a38, d2 + 38); origami_utils::load<Reg>(a39, d2 + 39); origami_utils::load<Reg>(a40, d2 + 40); origami_utils::load<Reg>(a41, d2 + 41); origami_utils::load<Reg>(a42, d2 + 42); origami_utils::load<Reg>(a43, d2 + 43); origami_utils::load<Reg>(a44, d2 + 44); origami_utils::load<Reg>(a45, d2 + 45); origami_utils::load<Reg>(a46, d2 + 46); origami_utils::load<Reg>(a47, d2 + 47); origami_utils::load<Reg>(a48, d2 + 48); origami_utils::load<Reg>(a49, d2 + 49); origami_utils::load<Reg>(a50, d2 + 50); origami_utils::load<Reg>(a51, d2 + 51); origami_utils::load<Reg>(a52, d2 + 52); origami_utils::load<Reg>(a53, d2 + 53); origami_utils::load<Reg>(a54, d2 + 54); origami_utils::load<Reg>(a55, d2 + 55); origami_utils::load<Reg>(a56, d2 + 56); origami_utils::load<Reg>(a57, d2 + 57); origami_utils::load<Reg>(a58, d2 + 58); origami_utils::load<Reg>(a59, d2 + 59); origami_utils::load<Reg>(a60, d2 + 60); origami_utils::load<Reg>(a61, d2 + 61); origami_utils::load<Reg>(a62, d2 + 62); origami_utils::load<Reg>(a63, d2 + 63);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::in_register_sort16<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					origami_sorter::in_register_sort16<Item, Reg>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47);
					origami_sorter::in_register_sort16<Item, Reg>(a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31); origami_utils::store<Reg>(a32, d2 + 32); origami_utils::store<Reg>(a33, d2 + 33); origami_utils::store<Reg>(a34, d2 + 34); origami_utils::store<Reg>(a35, d2 + 35); origami_utils::store<Reg>(a36, d2 + 36); origami_utils::store<Reg>(a37, d2 + 37); origami_utils::store<Reg>(a38, d2 + 38); origami_utils::store<Reg>(a39, d2 + 39); origami_utils::store<Reg>(a40, d2 + 40); origami_utils::store<Reg>(a41, d2 + 41); origami_utils::store<Reg>(a42, d2 + 42); origami_utils::store<Reg>(a43, d2 + 43); origami_utils::store<Reg>(a44, d2 + 44); origami_utils::store<Reg>(a45, d2 + 45); origami_utils::store<Reg>(a46, d2 + 46); origami_utils::store<Reg>(a47, d2 + 47); origami_utils::store<Reg>(a48, d2 + 48); origami_utils::store<Reg>(a49, d2 + 49); origami_utils::store<Reg>(a50, d2 + 50); origami_utils::store<Reg>(a51, d2 + 51); origami_utils::store<Reg>(a52, d2 + 52); origami_utils::store<Reg>(a53, d2 + 53); origami_utils::store<Reg>(a54, d2 + 54); origami_utils::store<Reg>(a55, d2 + 55); origami_utils::store<Reg>(a56, d2 + 56); origami_utils::store<Reg>(a57, d2 + 57); origami_utils::store<Reg>(a58, d2 + 58); origami_utils::store<Reg>(a59, d2 + 59); origami_utils::store<Reg>(a60, d2 + 60); origami_utils::store<Reg>(a61, d2 + 61); origami_utils::store<Reg>(a62, d2 + 62); origami_utils::store<Reg>(a63, d2 + 63);
			}
		}
		printf("done, ");
		double el = ELAPSED(st, en);
		printf("elapsed %.2f s @ %.2f M/s\n", el, double(iter * n) / el / 1e6);

		if ((d[3] & 0x123) == 0) printf("%llu %llu\n", d[3], d[11]);
		delete[] d;
	}

	template <int NREG = 8, typename Item, typename KeyType, typename ValueType>
	void in_register_sort_kv_scalar() {
		constexpr ui KeySize = sizeof(KeyType);
		constexpr ui ValueSize = sizeof(ValueType);
		constexpr ui ItemSize = sizeof(Item);

		constexpr ui64 n = NREG;
		constexpr ui64 iter = 1e9;
		Item* d = new Item[n];
		datagen::random_writer<Item>(d, n);

		printf("Sorting in-register: reg %u ... ", NREG);

		hrc::time_point st, en;
		if constexpr (NREG == 8) {
			char* d2 = (char*)d;

			KeyType a0, a1, a2, a3, a4, a5, a6, a7;
			ValueType b0, b1, b2, b3, b4, b5, b6, b7;

			a0 = *(KeyType*)d2; b0 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a1 = *(KeyType*)d2; b1 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a2 = *(KeyType*)d2; b2 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a3 = *(KeyType*)d2; b3 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a4 = *(KeyType*)d2; b4 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a5 = *(KeyType*)d2; b5 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a6 = *(KeyType*)d2; b6 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
			a7 = *(KeyType*)d2; b7 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;

			st = hrc::now();
			FOR(i, iter, 1) {
				origami_sorter::in_register_sort8_kv_scalar<KeyType, ValueType>(a0, b0, a1, b1, a2, b2, a3, b3, a4, b4, a5, b5, a6, b6, a7, b7);
			}
			en = hrc::now();

			d2 = (char*)d;
			*(KeyType*)d2 = a0; *((ValueType*)(d2 + KeySize)) = b0; d2 += ItemSize;
			*(KeyType*)d2 = a1; *((ValueType*)(d2 + KeySize)) = b1; d2 += ItemSize;
			*(KeyType*)d2 = a2; *((ValueType*)(d2 + KeySize)) = b2; d2 += ItemSize;
			*(KeyType*)d2 = a3; *((ValueType*)(d2 + KeySize)) = b3; d2 += ItemSize;
			*(KeyType*)d2 = a4; *((ValueType*)(d2 + KeySize)) = b4; d2 += ItemSize;
			*(KeyType*)d2 = a5; *((ValueType*)(d2 + KeySize)) = b5; d2 += ItemSize;
			*(KeyType*)d2 = a6; *((ValueType*)(d2 + KeySize)) = b6; d2 += ItemSize;
			*(KeyType*)d2 = a7; *((ValueType*)(d2 + KeySize)) = b7; d2 += ItemSize;
		}

		printf("done, ");
		double el = ELAPSED(st, en);
		printf("elapsed %.2f @ %.2f M/s\n", el, double(iter * n) / el / 1e6);

		if (d[3] & 0x123 == 0) printf("%llu %llu\n", d[3], d[11]);
		delete[] d;
	}

	template <int UNROLL>
	void in_register_transpose() {
		constexpr ui64 nreg = ItemsPerReg;			// W x W matrix
		constexpr ui64 n = nreg * ItemsPerReg * UNROLL;
		constexpr ui64 iter = 1e9;
		Item* d = new Item[n];
		Reg* d2 = (Reg*)d;
		datagen::random_writer<Item>(d, n);

		printf("Transposing in-register: reg %u, unroll %u, matrix: %u x %u ... ", nreg, UNROLL, nreg, nreg);

		hrc::time_point st, en;

		if constexpr (UNROLL == 1) {
			if constexpr (nreg == 2) {
				Reg a0, a1;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_utils::transpose<Item, Reg>(a0, a1);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1);
			}
			else if constexpr (nreg == 4) {
				Reg a0, a1, a2, a3;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_utils::transpose<Item, Reg>(a0, a1, a2, a3);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
			}
			else if constexpr (nreg == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);

				st = hrc::now();
				FOR(i, iter, 1)
					origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
				origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);

			}
			else if constexpr (nreg == 16) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			else
				ReportError("Matrix dimension not supported");
		}
		else if constexpr (UNROLL == 2) {
			if constexpr (nreg == 2) {
				Reg a0, a1, a2, a3;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_utils::transpose<Item, Reg>(a0, a1);
					origami_utils::transpose<Item, Reg>(a2, a3);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
			}
			else if constexpr (nreg == 4) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_utils::transpose<Item, Reg>(a0, a1, a2, a3);
					origami_utils::transpose<Item, Reg>(a4, a5, a6, a7);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
			}
			else if constexpr (nreg == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::transpose<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
				origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			else if constexpr (nreg == 16) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				Reg a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					origami_utils::transpose<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
				origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
			else
				ReportError("Matrix dimension not supported");
		}

		printf("done, ");
		double el = ELAPSED_MS(st, en);
		printf("elapsed: %.2f ms, Speed: %.2f M/s\n", el, double(iter * n) / el / 1e3);

		if (*((char*)d) == 123) printf("%llu %llu\n", d[3], d[11]);
		delete[] d;
	}

	template <int BITS = 32, int UNROLL = 1>
	void diag_exchange() {
		constexpr ui n = (ItemsPerReg << 1) * UNROLL;
		Item* d = new Item[n];
		datagen::random_writer<Item>(d, n);
		Reg* d2 = (Reg*)d;

		hrc::time_point st, en;
		double el;
		constexpr ui64 repeat = 1;

		Reg a0, a1, a2, a3, a4, a5, a6, a7;
		if constexpr (UNROLL >= 1) {
			origami_utils::load<Reg>(a0, d2);
			origami_utils::load<Reg>(a1, d2 + 1);
		}
		if constexpr (UNROLL >= 2) {
			origami_utils::load<Reg>(a2, d2 + 2);
			origami_utils::load<Reg>(a3, d2 + 3);
		}
		if constexpr (UNROLL >= 4) {
			origami_utils::load<Reg>(a4, d2 + 4);
			origami_utils::load<Reg>(a5, d2 + 5);
			origami_utils::load<Reg>(a6, d2 + 6);
			origami_utils::load<Reg>(a7, d2 + 7);
		}

		printf("Running diagonal exchange for bits: %u ...\n", BITS);

		origami_utils::print<Reg, Item>(a0);
		origami_utils::print<Reg, Item>(a1);

		st = hrc::now();
		FOR(i, repeat, 1) {
			if constexpr (UNROLL >= 1)
				origami_utils::diag_exchange<Reg, Item, BITS>(a0, a1);
			if constexpr (UNROLL >= 2)
				origami_utils::diag_exchange<Reg, Item, BITS>(a2, a3);
			if constexpr (UNROLL >= 4) {
				origami_utils::diag_exchange<Reg, Item, BITS>(a4, a5);
				origami_utils::diag_exchange<Reg, Item, BITS>(a6, a7);
			}
		}
		en = hrc::now();
		el = ELAPSED_MS(st, en);
		double sp = repeat * UNROLL / el / 1e6;		// in B exchanges / s
		printf("Elapsed: %.2f ms, Speed: %.2f B exchanges/s\n", el, sp);


		origami_utils::print<Reg, Item>(a0);
		origami_utils::print<Reg, Item>(a1);

		if constexpr (UNROLL >= 1) {
			origami_utils::store<Reg>(a0, d2);
			origami_utils::store<Reg>(a1, d2 + 1);
		}
		if constexpr (UNROLL >= 2) {
			origami_utils::store<Reg>(a2, d2 + 2);
			origami_utils::store<Reg>(a3, d2 + 3);
		}
		if constexpr (UNROLL >= 4) {
			origami_utils::store<Reg>(a4, d2 + 4);
			origami_utils::store<Reg>(a5, d2 + 5);
			origami_utils::store<Reg>(a6, d2 + 6);
			origami_utils::store<Reg>(a7, d2 + 7);
		}

		if (*((char*)d) == 123) printf("Dummy\n");
	}

	// RSWAP  
	template <int NREG, int UNROLL>
	void in_register_merge() {
		constexpr ui64 per_n = NREG * ItemsPerReg;
		constexpr ui64 per_n_half = per_n >> 1;
		constexpr ui64 n = per_n * UNROLL;
		constexpr ui64 iter = 1e9;
		Item* d = new Item[n];
		Reg* d2 = (Reg*)d;
		datagen::random_writer<Item>(d, n);
		SortEvery<Item>(d, n, per_n_half);

		printf("Merging in-register: reg %u, unroll %u, merge: %u x %u ...", NREG, UNROLL, per_n_half, per_n_half);

		hrc::time_point st, en;

		if constexpr (UNROLL == 1) {
			if constexpr (NREG == 2) {
				Reg a0, a1;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_sorter::rswap<Item, Reg>(a0, a1);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1);
			}
			else if constexpr (NREG == 4) {
				Reg a0, a1, a2, a3;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				st = hrc::now();
				FOR(i, iter, 1)
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
				origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
			}
		}
		else if constexpr (UNROLL == 2) {
			if constexpr (NREG == 2) {
				Reg a0, a1, a2, a3;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1);
					origami_sorter::rswap<Item, Reg>(a2, a3);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
			}
			else if constexpr (NREG == 4) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3);
					origami_sorter::rswap<Item, Reg>(a4, a5, a6, a7);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::rswap<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
				origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
		}
		else if constexpr (UNROLL == 3) {
			if constexpr (NREG == 2) {
				Reg a0, a1, a2, a3, a4, a5;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2);
				origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5);

				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1);
					origami_sorter::rswap<Item, Reg>(a2, a3);
					origami_sorter::rswap<Item, Reg>(a4, a5);
				}
				en = hrc::now();

				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2);
				origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				Reg a16, a17, a18, a19, a20, a21, a22, a23;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); 				origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23);

				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::rswap<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::rswap<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
				origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
				origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23);

			}
		}
		else if constexpr (UNROLL == 4) {
			if constexpr (NREG == 2) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3);
				origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);

				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1);
					origami_sorter::rswap<Item, Reg>(a2, a3);
					origami_sorter::rswap<Item, Reg>(a4, a5);
					origami_sorter::rswap<Item, Reg>(a6, a7);
				}
				en = hrc::now();

				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3);
				origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
			}
			else if constexpr (NREG == 4) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3);
					origami_sorter::rswap<Item, Reg>(a4, a5, a6, a7);
					origami_sorter::rswap<Item, Reg>(a8, a9, a10, a11);
					origami_sorter::rswap<Item, Reg>(a12, a13, a14, a15);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			else if constexpr (NREG == 8) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
				Reg a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
				origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
				origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); 				origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23);
				origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27);				origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);

				st = hrc::now();
				FOR(i, iter, 1) {
					origami_sorter::rswap<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::rswap<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::rswap<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
					origami_sorter::rswap<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);
				origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
				origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23);
				origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27);	origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
		}
		printf("done, ");
		double el = ELAPSED(st, en);
		printf("elapsed: %.2f s @ %.2f M/s\n", el, double(iter * n) / el / 1e6);

		FOR(i, n, per_n)
			SortCorrectnessChecker<Item>(d + i, per_n);

		if (d[3] & 0x123 == 0) printf("%llu %llu\n", d[3], d[11]);
		delete[] d;
	}

	// MCMERGE
	template <int NREG, int UNROLL>
	void mcmerge_test() {
		printf("Running mcmerge test with %d registers, %dx unroll ... \n", NREG, UNROLL);

		constexpr ui64 n = NREG * ItemsPerReg * UNROLL;
		Item* data = new Item[n];
		origami_utils::random_writer<Item>(data, n);

		constexpr ui64 repeat = 1e9;			// NOTE: if we want to check correctness, we should run one iteration only
		if constexpr (NREG == 16) {
			Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
			Reg a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
			Reg* d2 = (Reg*)data;

			if constexpr (UNROLL >= 1) {
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
			}
			if constexpr (UNROLL >= 2) {
				origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
			}

			// prepare for merge 
			if constexpr (UNROLL >= 1) {
				origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
				origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
			}
			if constexpr (UNROLL >= 2) {
				origami_sorter::in_register_sort8<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
				origami_sorter::in_register_sort8<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);
			}


			// now perform the merges
			hrc::time_point st, en;
			double el;

			// debug: print reg for correctness check

			ui times = NREG * ItemsPerReg / 16;
			if (times > 0) {
				printf("Merge8x8 (%dx) ... ", times);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mcmerge8x8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mcmerge8x8<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			times >>= 1;
			if (times > 0) {
				printf("Merge16x16 (%dx) ... ", sizeof(Reg) / sizeof(Item) / 2);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mcmerge16x16<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mcmerge16x16<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			times >>= 1;
			if (times > 0) {
				printf("Merge32x32 (%dx) ... ", sizeof(Reg) / sizeof(Item) / 4);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mcmerge32x32<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mcmerge32x32<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}


			times >>= 1;
			if (times > 0) {
				printf("Merge64x64 (%dx) ... ", sizeof(Reg) / sizeof(Item) / 8);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mcmerge64x64<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mcmerge64x64<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			if constexpr (UNROLL >= 1) {
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			if constexpr (UNROLL >= 2) {
				origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
		}

		else if constexpr (NREG == 32) {
			Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
			Reg a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
			Reg* d2 = (Reg*)data;

			origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
			origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);

			origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
			origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
			origami_sorter::in_register_sort8<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
			origami_sorter::in_register_sort8<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);

			// now perform the merges
			hrc::time_point st, en;
			double el;
	
			ui times = NREG * ItemsPerReg / 16;
			if (times > 0) {
				printf("Merge8x8 (%dx) ... ", times);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mcmerge8x8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			times >>= 1;
			if (times > 0) {
				printf("Merge16x16 (%dx) ... ", times);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mcmerge16x16<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);

			}

			times >>= 1;
			if (times > 0) {
				printf("Merge32x32 (%dx) ... ", times);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mcmerge32x32<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			times >>= 1;
			if (times > 0) {
				printf("Merge64x64 (%dx) ... ", times);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mcmerge64x64<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);

			if (*(char*)data == 123) printf("Dummy\n");
		}
		delete[] data;
	}

	//  MRMERGE
	template <int NREG, int UNROLL>
	void mrmerge_test() {
		printf("Running mrmerge test with %d registers, %dx unroll ... \n", NREG, UNROLL);

		constexpr ui64 n = NREG * ItemsPerReg * UNROLL;
		Item* data = new Item[n];
		origami_utils::random_writer<Item>(data, n);
		// prepare for merge --> sort every W (SIMD width) keys
		SortEvery<Item>(data, n, ItemsPerReg);
		constexpr ui64 repeat = 1e9;

		if constexpr (NREG == 16) {
			Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
			Reg a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;

			Reg* d2 = (Reg*)data;
			if constexpr (UNROLL >= 1) {
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
			}
			if constexpr (UNROLL >= 2) {
				origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
			}

			// now perform the merges
			hrc::time_point st, en;
			double el;

			ui merge_size = ItemsPerReg;
			ui reg_per_merge = 2;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ...  ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mrmerge2R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mrmerge2R<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mrmerge4R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mrmerge4R<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mrmerge8R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mrmerge8R<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					if (UNROLL >= 1) {
						origami_sorter::mrmerge16R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					}
					if (UNROLL >= 2) {
						origami_sorter::mrmerge16R<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					}
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			if constexpr (UNROLL >= 1) {
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
			}
			if constexpr (UNROLL >= 2) {
				origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
		}
		else if constexpr (NREG == 32) {
			Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
			Reg a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
			Reg* d2 = (Reg*)data;

			origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
			origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);

			// now perform the merges
			hrc::time_point st, en;
			double el;

			// debug: print reg for correctness check

			ui merge_size = ItemsPerReg;
			ui reg_per_merge = 2;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mrmerge2R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mrmerge4R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mrmerge8R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mrmerge16R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

			merge_size <<= 1;
			reg_per_merge <<= 1;
			if (reg_per_merge <= NREG) {
				printf("Merge%dx%d (%dx) ... ", merge_size, merge_size, NREG / reg_per_merge);
				st = hrc::now();
				FOR(i, repeat, 1) {
					origami_sorter::mrmerge32R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				en = hrc::now();
				el = ELAPSED_MS(st, en);

				printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el, (n * repeat) / el / 1e3);
			}

		}
		delete[] data;
	}

	template <int NREG, int N, int _SWITCH>
	void phase1_sort_test() {
		// using only for debugging
		printf("Running Origami phase1_sortN with NREG: %u, SWITCH: %d, N: %d ... \n", NREG, _SWITCH, N);
		PRINT_DASH(60);

		constexpr ui tot_n = NREG * ItemsPerReg;
		Item* data = new Item[tot_n];
		Item* back = new Item[tot_n];
		origami_utils::random_writer<Item>(data, tot_n);
		memcpy(back, data, tot_n * Itemsize);
		printf("Running std::sort on backup data ... ");
		std::sort(back, back + tot_n);
		printf("done\n");

		printf("Running Origami ... ");
		origami_sorter::phase1_sort<Item, Reg, NREG, N, _SWITCH>(data, data);
		printf("done\n");

		// check correctness of sort
		// NOTE: this is only valid if we are sorting the entire matrix
		printf("Checking sort correctness ... ");
		SortCorrectnessCheckerSTD<Item>(data, back, N);
		printf("done\n");

		delete[] data;
		delete[] back;
		PRINT_DASH(60); PRINT_DASH(60);
	}

	// bench phase1 sort -- Table 9 in paper
	template <int NREG, int N, int _SWITCH>
	void phase1_sort_sliding_test() {
		constexpr ui L2_CACHE = MB(1) / Itemsize;
		constexpr ui MAT_N = NREG * ItemsPerReg;
		constexpr ui TOT_N = L2_CACHE >> 1; //MAT_N; //
		constexpr bool full_mat = (MAT_N == N);

		printf("phase1_sortN with NREG: %u, SWITCH: %d, N: %d, TOT_N: %u ... \n", NREG, _SWITCH, N, TOT_N);

		Item* data = new Item[TOT_N];
		Item* back = new Item[TOT_N];
		Item* sorted = new Item[TOT_N];

		origami_utils::random_writer<Item>(data, TOT_N);
		memcpy(back, data, TOT_N * Itemsize);
		memcpy(sorted, data, TOT_N * Itemsize);

		//printf("Running std::sort on backup data ... ");
		SortEvery(sorted, TOT_N, N);
		//printf("done\n");

		hrc::time_point st, en;
		double el = 0;
		constexpr ui repeat = 1e4;
		printf("Running sort ... ");
		FOR(i, repeat, 1) {
			memcpy(data, back, TOT_N * Itemsize);
			st = hrc::now();
			FOR(j, TOT_N, MAT_N)
				origami_sorter::phase1_sort<Item, Reg, NREG, N, _SWITCH>(data + j);
			en = hrc::now();
			el += ELAPSED_MS(st, en);
		}

		printf("done, ");

		printf("avg. elapsed: %.2f ms, Speed: %.2f M/s\n", el, (double)TOT_N * repeat / el / 1e3);

		// check correctness of sort
		// NOTE: this is only valid if we are sorting the entire matrix
		printf("Checking sort correctness ... ");
		FOR(i, TOT_N, N) {
			if constexpr (full_mat) {
				if (SortCorrectnessCheckerSTD<Item>(data + i, sorted + i, N) == false) {
					ReportError("Correctness error");
					break;
				}
			}
			else {
				if (SortCorrectnessChecker<Item>(data + i, N) == false) {
					ReportError("Correctness error");
					break;
				}
			}
		}
		printf("done\n");

		delete[] data;
		delete[] back;
		delete[] sorted;
		PRINT_DASH(60); 
	}
};

template <typename Reg, typename Item>
void phase1_in_register_sort_test() {
	print_size<Reg, Item>();
	// if scalar and key-value
	if constexpr (std::is_same<Reg, Item>::value) {
		InRegisterTest<Item, Reg> irt;
		if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			irt.in_register_sort_kv_scalar<8, Item, i64, i64>();
		}
	}
	// vector (keys, key-value) and scalar keys
	else {
		InRegisterTest<Item, Reg> irt;
		irt.in_register_sort<4, 1>();
		irt.in_register_sort<4, 2>();
		irt.in_register_sort<4, 4>();

		irt.in_register_sort<8, 1>();
		irt.in_register_sort<8, 2>();
		irt.in_register_sort<8, 4>();

		irt.in_register_sort<16, 1>();
		irt.in_register_sort<16, 2>();
		irt.in_register_sort<16, 4>();

	}
}

template <typename Reg, typename Item>
void phase1_in_register_transpose_test() {
	print_size<Reg, Item>();

	// if vectorized
	if constexpr (std::is_same<Reg, Item>::value) {
		ReportError("Transpose only for vectorized");
	}
	else {
		InRegisterTest<Item, Reg> irt;
		irt.in_register_transpose<1>();
		irt.in_register_transpose<2>();
	}
}

// bench mcmerge and mrmerge -- Table 2 in paper
template <typename Reg, typename Item>
void phase1_matrix_merge_test() {
	print_size<Reg, Item>();

	if constexpr (std::is_same<Reg, Item>::value) {
		ReportError("Only for vectorized");
	}
	else {

		InRegisterTest<Item, Reg> irt;
		// col SWAP / mcmerge
		//irt.mcmerge_test<16, 2>();				// 16 registers
		irt.mcmerge_test<32, 1>();					// 32 x W matrix

		//  SWAP / mrmerge
		//irt.mrmerge_test<16, 2>();
		irt.mrmerge_test<32, 1>();
	}
}

// bench phase1 sort -- Table 9 in paper
template <typename Reg, typename Item>
void phase1_sort_test() {
	// scalar
	if constexpr (std::is_same<Reg, Item>::value) {
		InRegisterTest<Item, Reg> irt;
		irt.phase1_sort_sliding_test<8, 8, 32>();
		irt.phase1_sort_sliding_test<16, 16, 32>();
		irt.phase1_sort_sliding_test<32, 32, 32>();
	}
	// vectorized
	else {
#if REG_TYPE == 1
		using Item1 = ui;
		print_size<Reg, Item1>();
		InRegisterTest<Item1, Reg> irt1;
		irt1.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 128, min(128, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<64, 256, min(256, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<128, 512, min(512, _P1_SWITCH)>();

		using Item2 = i64;
		print_size<Reg, Item2>();
		InRegisterTest<Item2, Reg> irt2;
		irt2.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<64, 128, min(128, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<128, 256, min(256, _P1_SWITCH)>();

		using Item3 = KeyValue<i64, i64>;
		print_size<Reg, Item3>();
		InRegisterTest<Item3, Reg> irt3;
		irt3.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<64, 64, min(64, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<128, 128, min(128, _P1_SWITCH)>();
#endif

#if REG_TYPE == 2
#if KEY_TYPE == 0
		using Item1 = ui;
		print_size<Reg, Item1>();
		InRegisterTest<Item1, Reg> irt1;

		irt1.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 128, min(128, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 256, min(256, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<64, 512, min(512, _P1_SWITCH)>();
#elif KEY_TYPE == 1
		using Item2 = i64;
		print_size<Reg, Item2>();
		InRegisterTest<Item2, Reg> irt2;
		irt2.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 128, min(128, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<64, 256, min(256, _P1_SWITCH)>();
#else 
		using Item3 = KeyValue<i64, i64>;
		print_size<Reg, Item3>();
		InRegisterTest<Item3, Reg> irt3;
		irt3.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<64, 128, min(128, _P1_SWITCH)>();
#endif
#endif

#if REG_TYPE == 3
		using Item1 = ui;
		print_size<Reg, Item1>();
		InRegisterTest<Item1, Reg> irt1;
		irt1.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 128, min(128, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 256, min(256, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<32, 512, min(512, _P1_SWITCH)>();
		irt1.phase1_sort_sliding_test<64, 1024, min(1024, _P1_SWITCH)>();
		//irt1.phase1_sort_sliding_test<128, 2048, 64>();
		//irt1.phase1_sort_sliding_test<256, 4096, 64>();

		using Item2 = i64;
		print_size<Reg, Item2>();
		InRegisterTest<Item2, Reg> irt2;
		irt2.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 128, min(128, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<32, 256, min(256, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<64, 512, min(512, _P1_SWITCH)>();
		irt2.phase1_sort_sliding_test<128, 1024, min(1024, _P1_SWITCH)>();

		using Item3 = KeyValue<i64, i64>;
		print_size<Reg, Item3>();
		InRegisterTest<Item3, Reg> irt3;
		irt3.phase1_sort_sliding_test<32, 8, min(8, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 16, min(16, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 32, min(32, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 64, min(64, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<32, 128, min(128, _P1_SWITCH)>();
		irt3.phase1_sort_sliding_test<64, 256, min(256, _P1_SWITCH)>();
#endif
	}
}

// bench RSWAP -- Table 3, 4 in paper
template <typename Reg, typename Item>
void phase2_in_register_merge_test() {
	print_size<Reg, Item>();

	if constexpr (std::is_same<Reg, Item>::value) {
		ReportError("Only for vectorized");
	}
	else {
		InRegisterTest<Item, Reg> irt;
		irt.in_register_merge<2, 1>();
		irt.in_register_merge<2, 2>();
		irt.in_register_merge<2, 3>();
		irt.in_register_merge<2, 4>();
	}
}

template<typename Reg, typename Item, bool IN_CACHE, ui P1_NREG = _P1_NREG, ui P1_N = _P1_N, ui P1_SWITCH = _P1_SWITCH, ui P2_MERGE_UNROLL = _P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x = _P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x = _P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x = _P2_MERGE_NREG_3x, ui P3_MERGE_UNROLL = _P3_MERGE_UNROLL, ui P3_MERGE_NREG_1x = _P3_MERGE_NREG_1x, ui P3_MERGE_NREG_2x = _P3_MERGE_NREG_2x, ui P3_MERGE_NREG_3x = _P3_MERGE_NREG_3x>
void sort_sliding_test(ui64 sort_n = 64, ui writer_type = 1) {
//#define STD_CORRECTNESS
	print_size<Reg, Item>();

	const ui Itemsize = sizeof(Item);
	const ui P2_N = (L2_BYTES >> 1) / Itemsize;
	ui64 _GB = 1;
	ui64 size = GB(_GB);
	size = max(size, sort_n * sizeof(Item));
	int repeat = 3;
	if constexpr (IN_CACHE) {
		size = P2_N * Itemsize;
		repeat = 1e4;
	}

	ui64 n_items = size / Itemsize;
	if (sort_n > n_items) {
		n_items = sort_n;
		size = n_items * Itemsize;
	}

	printf("Running sort-kernel test sliding bench --> n: %llu, sort_n: %llu ...\n", n_items, sort_n);

	Item* data = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	//Item* data_back = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
	Item* end = data + n_items;
	Item* output = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

	datagen::Writer<Item> writer;
	writer.generate(data, n_items, writer_type);

	memset(output, 0, size);
	//memcpy(data_back, data, size);


#ifdef STD_CORRECTNESS
	printf("Sorting with std::sort ... ");
	Item* sorted = (Item*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE); //nullptr; // 
	memcpy(sorted, data, size);
	hrc::time_point st1 = hrc::now();
	SortEvery(sorted, n_items, sort_n);
	//SortEvery(data, n_items, sort_n);
	hrc::time_point en1 = hrc::now();
	printf("done\n");
	double el1 = ELAPSED_MS(st1, en1);
	printf("Elapsed: %.2f ms, Speed: %.2f M/s\n", el1, (n_items / el1 / 1e3));
	//VFREE(data);	// large std::sort bench
	//return;
#endif	

	double avgS = 0;
	FOR(i, repeat, 1) {
		//memcpy(data, data_back, size);
		//PRINT_ARR(data, n_items);
		Item* data2 = data;
		Item* end2 = data2 + n_items;
		Item* output2 = output;
		Item* o = data;

		hrc::time_point st1 = hrc::now();
		if constexpr (IN_CACHE)
			o = origami_sorter::phase2_sort<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x>(data2, output2, end2, sort_n);
		else
			o = origami_sorter::sort_single_thread<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_N, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x, P3_MERGE_UNROLL, P3_MERGE_NREG_1x, P3_MERGE_NREG_2x, P3_MERGE_NREG_3x>(data2, output2, end2, sort_n);

		hrc::time_point en1 = hrc::now();

		printf("\r                               \r");

		// fix output ptr
		double el = ELAPSED(st1, en1);
		double sp = double(n_items) / el / 1e6;
		avgS += sp;


		//PRINT_ARR(o, n_items);
		//PRINT_ARR64(o, n_items);

#ifdef STD_CORRECTNESS
		printf("Iter %3lu done, checking correctness w/ std::sort ... ", i);
		Item* p = o; Item* pEnd = o + n_items; Item* p_sorted = sorted;
		while (p < pEnd) {
			if (!SortCorrectnessCheckerSTD(p, p_sorted, sort_n)) {
				printf("Correctness error @ %llu\n", i);
				exit(1);
			}
			p += sort_n; p_sorted += sort_n;
		}
		printf("done\r                                                                    \r");
#else 
		Item* p = o;
		Item* pEnd = p + n_items;
		while (p < pEnd) {
			if (!SortCorrectnessChecker(p, sort_n)) {
				printf("Correctness error @ %llu\n", i);
				break;
				//system("pause");
				//exit(1);
			}
			p += sort_n;
		}
#endif
		printf("\rIter: %llu / %llu", i + 1, repeat);
	}
	avgS /= repeat;
	printf("\nSpeed: %.2f M keys/sec\n", avgS);
	// prints to prevent compiler optimizations
	if (*((char*)data + 13) & 0x123 == *(char*)output) printf("%u %u\n", data[13], output[13]);
	PRINT_DASH(50);

	VirtualFree(data, 0, MEM_RELEASE);
	//VirtualFree(data_back, 0, MEM_RELEASE);			// comment out for large sorts
	VirtualFree(output, 0, MEM_RELEASE);
#ifdef STD_CORRECTNESS
	VirtualFree(sorted, 0, MEM_RELEASE);
#endif 
#undef STD_CORRECTNESS
}

template<typename Reg, typename Item>
void phase2_switch_point_test() {
	if constexpr (std::is_same<Reg, Item>::value) {
		// determine the switching point
		sort_sliding_test<Reg, Item, true, 8, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 16, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 64, 64, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
	}
	else {
#if REG_TYPE == 1
#if KEY_TYPE == 0
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 32, 128, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
		sort_sliding_test<Reg, Item, true, 64, 256, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(512);

#elif KEY_TYPE == 1
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 64, 128, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);

#elif KEY_TYPE == 2
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 64, 64, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
#endif


#elif REG_TYPE == 2
#if KEY_TYPE == 0
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 32, 128, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
		sort_sliding_test<Reg, Item, true, 32, 256, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(512);
		sort_sliding_test<Reg, Item, true, 64, 512, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(1024);

#elif KEY_TYPE == 1
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 32, 128, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
		sort_sliding_test<Reg, Item, true, 64, 256, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(512);

#elif KEY_TYPE == 2
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 64, 128, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
#endif 


#elif REG_TYPE == 3
#if KEY_TYPE == 0
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 32, 128, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
		sort_sliding_test<Reg, Item, true, 32, 256, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(512);
		sort_sliding_test<Reg, Item, true, 32, 512, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(1024);
		sort_sliding_test<Reg, Item, true, 64, 1024, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(2048);
		sort_sliding_test<Reg, Item, true, 128, 2048, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(4096);

#elif KEY_TYPE == 1
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 32, 128, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
		sort_sliding_test<Reg, Item, true, 32, 256, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(512);
		sort_sliding_test<Reg, Item, true, 64, 512, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(1024);
		sort_sliding_test<Reg, Item, true, 128, 1024, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(2048);

#elif KEY_TYPE == 2
		sort_sliding_test<Reg, Item, true, 32, 8, 8, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(16);
		sort_sliding_test<Reg, Item, true, 32, 16, 16, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(32);
		sort_sliding_test<Reg, Item, true, 32, 32, 32, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(64);
		sort_sliding_test<Reg, Item, true, 32, 64, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(128);
		sort_sliding_test<Reg, Item, true, 32, 128, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(256);
		sort_sliding_test<Reg, Item, true, 64, 256, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(512);
		sort_sliding_test<Reg, Item, true, 128, 512, 64, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(1024);
#endif 
#endif 
	}
}

// bench phase2 sort -- Table 10 in paper
template <typename Reg, typename Item>
void phase2_sort_test() {
	ui sort_pow = log2(_P1_N) + 1;
	const ui end_pow = log2((L2_BYTES >> 1) / sizeof(Item));
	while (sort_pow <= end_pow) {
		sort_sliding_test<Reg, Item, true, _P1_NREG, _P1_N, _P1_SWITCH, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x>(1LLU << sort_pow);
		++sort_pow;
	}
}

// bench phase3 sort -- Table 11 in paper
template <typename Reg, typename Item>
void phase3_sort_single_thread_test(ui writer_type = 1) {
	ui sort_pow = log2((L2_BYTES >> 1) / sizeof(Item));
	const ui end_pow = log2(GB(1) / sizeof(Item));
	while (sort_pow <= end_pow) {
		sort_sliding_test<Reg, Item, false, _P1_NREG, _P1_N, _P1_SWITCH, _P2_MERGE_UNROLL, _P2_MERGE_NREG_1x, _P2_MERGE_NREG_2x, _P2_MERGE_NREG_3x, _P3_MERGE_UNROLL, _P3_MERGE_NREG_1x, _P3_MERGE_NREG_2x, _P3_MERGE_NREG_3x>(1LLU << sort_pow, writer_type);
		++sort_pow;
	}
}

template <typename Reg, typename Item>
void phase4_sortN_test(int argc, char** argv) {
//#define STD_CORRECTNESS
	constexpr ui P2_N = (L2_BYTES >> 1) / sizeof(Item);
	ui n_threads = 16;		if (argc > 1) n_threads = atoi(argv[1]);
	ui n_cores = 8;			if (argc > 2) n_cores = atoi(argv[2]);
	ui64 size = GB(1LLU);	if (argc > 3) size = (1LLU << atoi(argv[3])) * sizeof(Item);
	ui min_k = 4;			if (argc > 4) min_k = atoi(argv[4]);
	ui64 n_items = size / sizeof(Item);

	hrc::time_point st, en;

	Item* d = (Item*)VALLOC(size);
	//Item* d_back = (Item*)VALLOC(size);

	Item* tmp = (Item*)VALLOC(size);
	ui64 kway_buf_size = GB(1LLU);
	Item* kway_buf = (Item*)VALLOC(kway_buf_size);
	Item** o = new Item * [n_threads];
	Item* output = tmp;

	//Writer(d, n_items);
	datagen::Writer<Item> writer;
	writer.generate(d, n_items, MT);
	//memcpy(d_back, d, size);

#ifdef STD_CORRECTNESS
	Item* d_sorted = (Item*)VALLOC(size);
	memcpy(d_sorted, d, size);
	printf("Sorting with std::sort for correctness check ... ");
	hrc::time_point st1 = hrc::now();
	std::sort(d_sorted, d_sorted + n_items);
	hrc::time_point en1 = hrc::now();
	double el1 = ELAPSED_MS(st1, en1);
	printf("done in %.2f ms, Speed: %.2f M/s\n", el1, n_items / el1 / 1e3);
#endif

	memset(tmp, 0, size);
	memset(kway_buf, 0, kway_buf_size);

	printf("Sorting %llu items using %d threads, %d cores ...\n", n_items, n_threads, n_cores);

	Item* out = origami_sorter::sort_multi_thread<Item, Reg>(d, tmp, n_items, n_threads, n_cores, min_k, kway_buf);

	printf("Checking correctness ... ");
	// -- upto Phase 2
	//FOR(i, n_items, P2_N) SortCorrectnessChecker<Item>(out + i, P2_N);

	// -- upto Phase 3
	/*ui64 n_per_thread = n_items / n_threads;
	FOR(i, n_items, n_per_thread) SortCorrectnessChecker<Item>(out + i, n_per_thread);*/

#ifdef STD_CORRECTNESS
	SortCorrectnessCheckerSTD<Item>(out, d_sorted, n_items);
#else 
	SortCorrectnessChecker<Item>(out, n_items);
#endif 
	printf("done\n");

	delete[] o;
	VirtualFree(d, 0, MEM_RELEASE);
	//VirtualFree(d_back, 0, MEM_RELEASE);
#ifdef STD_CORRECTNESS
	VirtualFree(d_sorted, 0, MEM_RELEASE);
#endif
	VirtualFree(tmp, 0, MEM_RELEASE);
	VirtualFree(kway_buf, 0, MEM_RELEASE);
#undef STD_CORRECTNESS
}


int main(int argc, char** argv) {

	SetThreadAffinityMask(GetCurrentThread(), 1 << 4);

//#define PHASE1_IN_REG_SORT
//#define PHASE1_IN_REG_TRANSPOSE
//#define PHASE1_MATRIX_MERGES
//#define PHASE1_SORT

//#define PHASE2_IN_REG_MERGE
//#define PHASE2_SWITCH_POINT
#define PHASE2_SORT

//#define PHASE3_SORT
//#define PHASE4_SORT

#ifdef PHASE1_IN_REG_SORT
	phase1_in_register_sort_test<Regtype, Itemtype>();
#endif 

#ifdef PHASE1_IN_REG_TRANSPOSE
	phase1_in_register_transpose_test<Regtype, Itemtype>();
#endif 

#ifdef PHASE1_MATRIX_MERGES
	// bench mcmerge and mrmerge -- Table 2 in paper
	phase1_matrix_merge_test<Regtype, Itemtype>();
#endif 

#ifdef PHASE1_SORT
	// bench phase1 sort -- Table 9 in paper
	phase1_sort_test<Regtype, Itemtype>();
#endif 

	// Phase 2
#ifdef PHASE2_IN_REG_MERGE 
	// bench RSWAP -- Table 3, 4 in paper
	phase2_in_register_merge_test<Regtype, Itemtype>();
#endif 

#ifdef PHASE2_SWITCH_POINT
	// get switch point from P1 to P2
	phase2_switch_point_test<Regtype, Itemtype>();
#endif 

#ifdef PHASE2_SORT
	// bench phase 2 sort -- Table 10 in paper
	phase2_sort_test<Regtype, Itemtype>();
#endif 

	// Phase 3
#ifdef PHASE3_SORT
	// bench phase 3 sort -- Table 11 in paper
	phase3_sort_single_thread_test<Regtype, Itemtype>(1);

	// for testing different dists
	//FOR_INIT(i, 1, 10, 1)
		//phase3_sort_single_thread_test<Regtype, Itemtype>(i);
#endif

	// Phase 4
#ifdef PHASE4_SORT
	// bench phase 4 sort -- Table 13, 14 in paper
	phase4_sortN_test<Regtype, Itemtype>(argc, argv);
#endif 

	system("pause");

	return 0;
}