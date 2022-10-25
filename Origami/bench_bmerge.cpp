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
#include "merger.h"
#include "writer.h"

template <typename Reg, bool stream, typename Item = ui, ui NREG = 1, ui UNROLL = 1>
void bmerge(ui writer_type = 1) {
//#define STD_CORRECTNESS
	SetThreadAffinityMask(GetCurrentThread(), 1 << 4);
	ui64 init_sz = stream ? MB(128) : (L2_BYTES >> 1); 
	ui Itemsize = sizeof(Item);
	ui64 n = init_sz / Itemsize;
	
	constexpr int repeat = stream ? 100 : 1e5;

	// create chunks, align boundary
	ui64 chunk = (n / (UNROLL << 1));
	chunk = (chunk / (sizeof(Reg) * NREG / Itemsize) * (sizeof(Reg) * NREG / Itemsize));
	n = chunk * (UNROLL << 1);

	ui64 sz = n * Itemsize;
	Item* A = (Item*)VALLOC(sz);
	Item* C = (Item*)VALLOC(sz);

	datagen::Writer<Item>  writer;
	writer.generate(A, n, writer_type);
	memset(C, 0, sz);
	sort_every(A, n, chunk);	

#ifdef STD_CORRECTNESS
	Item* S = (Item*)VALLOC(sz);
	memcpy(S, A, sz);
	sort_every<Item>(S, n, chunk << 1);
#endif 

	printf("Reg: %u, Key: %u, NREG: %u, UNROLL: %u\n", sizeof(Reg) << 3, Itemsize << 3, NREG, UNROLL);
	printf("Merging ... ");
	hrc::time_point st = hrc::now();
	FOR(i, repeat, 1) {
		if constexpr (std::is_same<Item, Reg>::value) {
			if constexpr (UNROLL == 1) {
				if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)
					origami_merger::mergebl2_scalar_kv<Item, NREG>(A, chunk, A + chunk, chunk, C);
				else 
					origami_merger::mergebl2_scalar<Item, NREG>(A, chunk, A + chunk, chunk, C);
			}
			else if constexpr (UNROLL == 2) {
				if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)
					origami_merger::mergebl2_scalar_kv_2x<Item, NREG>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, C, C + 2 * chunk);
				else 
					origami_merger::mergebl2_scalar_2x<Item, NREG>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, C, C + 2 * chunk);
			}
				
			else if constexpr (UNROLL == 3)
				origami_merger::mergebl2_scalar_3x<Item, NREG>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, A + 4 * chunk, chunk, A + 5 * chunk, chunk, C, C + 2 * chunk, C + 4 * chunk);
			else if constexpr (UNROLL == 4)
				origami_merger::mergebl2_scalar_4x<Item, NREG>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, A + 4 * chunk, chunk, A + 5 * chunk, chunk, A + 6 * chunk, chunk, A + 7 * chunk, chunk,
					C, C + 2 * chunk, C + 4 * chunk, C + 6 * chunk);
		}
		else {
			if constexpr (UNROLL == 1)
				origami_merger::mergebl2_vectorized<Reg, stream, NREG, Item>(A, chunk, A + chunk, chunk, C);
			else if constexpr (UNROLL == 2)
				origami_merger::mergebl2_vectorized_2x<Reg, stream, NREG, Item>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, C, C + 2 * chunk);
			else if constexpr (UNROLL == 3) 
				origami_merger::mergebl2_vectorized_3x<Reg, stream, NREG, Item>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, A + 4 * chunk, chunk, A + 5 * chunk, chunk,
					C, C + 2 * chunk, C + 4 * chunk);
			else if constexpr (UNROLL == 4)
				origami_merger::mergebl2_vectorized_4x<Reg, stream, NREG, Item>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, A + 4 * chunk, chunk, A + 5 * chunk, chunk, A + 6 * chunk, chunk, A + 7 * chunk, chunk,
					C, C + 2 * chunk, C + 4 * chunk, C + 6 * chunk);
			else if constexpr (UNROLL == 8)
				origami_merger::mergebl2_vectorized_8x<Reg, stream, NREG, Item>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, A + 4 * chunk, chunk, A + 5 * chunk, chunk, A + 6 * chunk, chunk, A + 7 * chunk, chunk, A + 8 * chunk, chunk, A + 9 * chunk, chunk, A + 10 * chunk, chunk, A + 11 * chunk, chunk, A + 12 * chunk, chunk, A + 13 * chunk, chunk, A + 14 * chunk, chunk, A + 15 * chunk, chunk,
					C, C + 2 * chunk, C + 4 * chunk, C + 6 * chunk, C + 8 * chunk, C + 10 * chunk, C + 12 * chunk, C + 14 * chunk);
			else if constexpr (UNROLL == 16)
				origami_merger::mergebl2_vectorized_16x<Reg, stream, NREG, Item>(A, chunk, A + chunk, chunk, A + 2 * chunk, chunk, A + 3 * chunk, chunk, A + 4 * chunk, chunk, A + 5 * chunk, chunk, A + 6 * chunk, chunk, A + 7 * chunk, chunk, A + 8 * chunk, chunk, A + 9 * chunk, chunk, A + 10 * chunk, chunk, A + 11 * chunk, chunk, A + 12 * chunk, chunk, A + 13 * chunk, chunk, A + 14 * chunk, chunk, A + 15 * chunk, chunk,
					A + 16 * chunk, chunk, A + 17 * chunk, chunk, A + 18 * chunk, chunk, A + 19 * chunk, chunk, A + 20 * chunk, chunk, A + 21 * chunk, chunk, A + 22 * chunk, chunk, A + 23 * chunk, chunk, A + 24 * chunk, chunk, A + 25 * chunk, chunk, A + 26 * chunk, chunk, A + 27 * chunk, chunk, A + 28 * chunk, chunk, A + 29 * chunk, chunk, A + 30 * chunk, chunk, A + 31 * chunk, chunk,
					C, C + 2 * chunk, C + 4 * chunk, C + 6 * chunk, C + 8 * chunk, C + 10 * chunk, C + 12 * chunk, C + 14 * chunk, C + 16 * chunk, C + 18 * chunk, C + 20 * chunk, C + 22 * chunk, C + 24 * chunk, C + 26 * chunk, C + 28 * chunk, C + 30 * chunk);
		}
	}

	hrc::time_point en = hrc::now();
	double el = ELAPSED_MS(st, en);
	double sp = n * repeat / el / 1e3;
	printf("done, elapsed: %.2f ms, Speed: %.2f M/s\n", el, sp);		

	printf("Checking correcntess ... ");
#ifdef STD_CORRECTNESS
		sort_correctness_checker_std<Item>(C, S, n);
#else 
	FOR(i, UNROLL, 1)
		sort_correctness_checker(C + i * (chunk << 1), (chunk << 1));
#endif
	printf("done\n");

	VFREE(A);
	VFREE(C);

#ifdef STD_CORRECTNESS
	VFREE(S);
#undef STD_CORRECTNESS
#endif

	PRINT_DASH(50);
}

template <typename Reg, typename Item, bool STREAM = false>
void bmerge_test() {
	SetThreadAffinityMask(GetCurrentThread(), 1 << 4);

	// scalar
	if constexpr (std::is_same<Reg, Item>::value) {
		bmerge<Reg, STREAM, Item, 1, 1>();
		bmerge<Reg, STREAM, Item, 2, 1>();
		bmerge<Reg, STREAM, Item, 3, 1>();
		bmerge<Reg, STREAM, Item, 4, 1>();
		bmerge<Reg, STREAM, Item, 1, 2>();
		bmerge<Reg, STREAM, Item, 2, 2>();
		bmerge<Reg, STREAM, Item, 3, 2>();
		bmerge<Reg, STREAM, Item, 4, 2>();
	}
	// vectorized
	else {
		bmerge<Reg, STREAM, Item, 1, 1>();
		bmerge<Reg, STREAM, Item, 1, 2>();
		bmerge<Reg, STREAM, Item, 1, 3>();
		bmerge<Reg, STREAM, Item, 1, 4>();

		bmerge<Reg, STREAM, Item, 2, 1>();
		bmerge<Reg, STREAM, Item, 2, 2>();
		bmerge<Reg, STREAM, Item, 2, 3>();
		bmerge<Reg, STREAM, Item, 2, 4>();

		bmerge<Reg, STREAM, Item, 3, 1>();
		bmerge<Reg, STREAM, Item, 3, 2>();
		bmerge<Reg, STREAM, Item, 3, 3>();
		bmerge<Reg, STREAM, Item, 3, 4>();

		bmerge<Reg, STREAM, Item, 4, 1>();
		bmerge<Reg, STREAM, Item, 4, 2>();
		bmerge<Reg, STREAM, Item, 4, 3>();
		bmerge<Reg, STREAM, Item, 4, 4>();
	}
}

int main() {
	SetThreadAffinityMask(GetCurrentThread(), 1 << 4);
	
	// find UNROLL and NREG -- Table 5 in paper
	bmerge_test<Regtype, Itemtype>();

	system("pause");
	return 0;
}