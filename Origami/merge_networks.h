#pragma once
#include "commons.h"
#include "merge_utils.h"

namespace origami_merge_network {
	template <typename Reg, typename Item>
	FORCEINLINE void merge_network2(Reg& a0, Reg& a1) {
		origami_utils::rswap<Reg, Item>(a0, a1);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_network4(Reg& a0, Reg& a1, Reg& a2, Reg& a3) {
		origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
		origami_utils::rswap<Reg, Item>(a1, a2);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_network6(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5) {
		origami_utils::rswap<Reg, Item>(a0, a3); origami_utils::rswap<Reg, Item>(a1, a4); origami_utils::rswap<Reg, Item>(a2, a5);
		origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a2, a3);

		/*origami_utils::rswap<Reg, Item>(a0, a4); origami_utils::rswap<Reg, Item>(a1, a5);
		origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a2, a4);
		origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5); */
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_network8(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7) {

		if constexpr (sizeof(Reg) == sizeof(Item)) {
			// scalar or SSE holding 64 + 64
			origami_utils::rswap<Reg, Item>(a0, a4);	origami_utils::rswap<Reg, Item>(a1, a5);	origami_utils::rswap<Reg, Item>(a2, a6);	origami_utils::rswap<Reg, Item>(a3, a7);
			origami_utils::rswap<Reg, Item>(a2, a4);	origami_utils::rswap<Reg, Item>(a3, a5);
			origami_utils::rswap<Reg, Item>(a1, a2);	origami_utils::rswap<Reg, Item>(a3, a4);	//origami_utils::rswap<Reg, Item>(a5, a6);
		}
		else {
			origami_utils::rswap<Reg, Item>(a0, a4);	origami_utils::rswap<Reg, Item>(a1, a5);	origami_utils::rswap<Reg, Item>(a2, a6);	origami_utils::rswap<Reg, Item>(a3, a7);
			origami_utils::rswap<Reg, Item>(a2, a4);	origami_utils::rswap<Reg, Item>(a3, a5);
			origami_utils::rswap<Reg, Item>(a1, a2);	origami_utils::rswap<Reg, Item>(a3, a4);	//origami_utils::rswap<Reg, Item>(a5, a6);
		}
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_network10(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9) {
		// best from ripco
		/*origami_utils::rswap<Reg, Item>(a0, a5); origami_utils::rswap<Reg, Item>(a1, a6); origami_utils::rswap<Reg, Item>(a2, a7); origami_utils::rswap<Reg, Item>(a3, a8); origami_utils::rswap<Reg, Item>(a4, a9);
		origami_utils::rswap<Reg, Item>(a3, a6);
		origami_utils::rswap<Reg, Item>(a4, a6);
		origami_utils::rswap<Reg, Item>(a3, a5);
		origami_utils::rswap<Reg, Item>(a2, a5); origami_utils::rswap<Reg, Item>(a6, a8); origami_utils::rswap<Reg, Item>(a1, a3); origami_utils::rswap<Reg, Item>(a4, a7);
		origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a6, a7); origami_utils::rswap<Reg, Item>(a4, a5);*/

		// Batcher's
		origami_utils::rswap<Reg, Item>(a0, a8); origami_utils::rswap<Reg, Item>(a1, a9); origami_utils::rswap<Reg, Item>(a2, a6); origami_utils::rswap<Reg, Item>(a3, a7);
		origami_utils::rswap<Reg, Item>(a1, a5); origami_utils::rswap<Reg, Item>(a4, a8); origami_utils::rswap<Reg, Item>(a0, a2);
		origami_utils::rswap<Reg, Item>(a4, a6); origami_utils::rswap<Reg, Item>(a3, a5); origami_utils::rswap<Reg, Item>(a7, a9);
		origami_utils::rswap<Reg, Item>(a0, a1); origami_utils::rswap<Reg, Item>(a2, a3); origami_utils::rswap<Reg, Item>(a4, a5); origami_utils::rswap<Reg, Item>(a6, a7);
		origami_utils::rswap<Reg, Item>(a8, a9);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_network16(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		origami_utils::rswap<Reg, Item>(a0, a8);
		origami_utils::rswap<Reg, Item>(a1, a9);
		origami_utils::rswap<Reg, Item>(a2, a10);
		origami_utils::rswap<Reg, Item>(a3, a11);
		origami_utils::rswap<Reg, Item>(a4, a12);
		origami_utils::rswap<Reg, Item>(a5, a13);
		origami_utils::rswap<Reg, Item>(a6, a14);
		origami_utils::rswap<Reg, Item>(a7, a15);

		origami_utils::rswap<Reg, Item>(a4, a8);
		origami_utils::rswap<Reg, Item>(a5, a9);
		origami_utils::rswap<Reg, Item>(a6, a10);
		origami_utils::rswap<Reg, Item>(a7, a11);

		origami_utils::rswap<Reg, Item>(a2, a4);
		origami_utils::rswap<Reg, Item>(a3, a5);
		origami_utils::rswap<Reg, Item>(a6, a8);
		origami_utils::rswap<Reg, Item>(a7, a9);
		origami_utils::rswap<Reg, Item>(a10, a12);
		origami_utils::rswap<Reg, Item>(a11, a13);

		origami_utils::rswap<Reg, Item>(a1, a2);
		origami_utils::rswap<Reg, Item>(a3, a4);
		origami_utils::rswap<Reg, Item>(a5, a6);
		origami_utils::rswap<Reg, Item>(a7, a8);
		origami_utils::rswap<Reg, Item>(a9, a10);
		origami_utils::rswap<Reg, Item>(a11, a12);
		origami_utils::rswap<Reg, Item>(a13, a14);
	}


	// template instantiation
	template FORCEINLINE void merge_network2<sse, ui>(sse&, sse&);
	template FORCEINLINE void merge_network2<ssef, float>(ssef&, ssef&);
	template FORCEINLINE void merge_network2<sse, i64>(sse&, sse&);
	template FORCEINLINE void merge_network2<ssed, double>(ssed&, ssed&);
	template FORCEINLINE void merge_network2<sse, KeyValue<i64, i64>>(sse&, sse&);

	template FORCEINLINE void merge_network2<avx2, ui>(avx2&, avx2&);
	template FORCEINLINE void merge_network2<avx2f, float>(avx2f&, avx2f&);
	template FORCEINLINE void merge_network2<avx2, i64>(avx2&, avx2&);
	template FORCEINLINE void merge_network2<avx2d, double>(avx2d&, avx2d&);
	template FORCEINLINE void merge_network2<avx2, KeyValue<i64, i64>>(avx2&, avx2&);

	template FORCEINLINE void merge_network2<avx512, ui>(avx512&, avx512&);
	template FORCEINLINE void merge_network2<avx512f, float>(avx512f&, avx512f&);
	template FORCEINLINE void merge_network2<avx512, i64>(avx512&, avx512&);
	template FORCEINLINE void merge_network2<avx512d, double>(avx512d&, avx512d&);
	template FORCEINLINE void merge_network2<avx512, KeyValue<i64, i64>>(avx512&, avx512&);

	template FORCEINLINE void merge_network4<sse, ui>(sse&, sse&, sse&, sse&);
	template FORCEINLINE void merge_network4<ssef, float>(ssef&, ssef&, ssef&, ssef&);
	template FORCEINLINE void merge_network4<sse, i64>(sse&, sse&, sse&, sse&);
	template FORCEINLINE void merge_network4<ssed, double>(ssed&, ssed&, ssed&, ssed&);
	template FORCEINLINE void merge_network4<sse, KeyValue<i64, i64>>(sse&, sse&, sse&, sse&);

	template FORCEINLINE void merge_network4<avx2, ui>(avx2&, avx2&, avx2&, avx2&);
	template FORCEINLINE void merge_network4<avx2f, float>(avx2f&, avx2f&, avx2f&, avx2f&);
	template FORCEINLINE void merge_network4<avx2, i64>(avx2&, avx2&, avx2&, avx2&);
	template FORCEINLINE void merge_network4<avx2d, double>(avx2d&, avx2d&, avx2d&, avx2d&);
	template FORCEINLINE void merge_network4<avx2, KeyValue<i64, i64>>(avx2&, avx2&, avx2&, avx2&);

	template FORCEINLINE void merge_network4<avx512, ui>(avx512&, avx512&, avx512&, avx512&);
	template FORCEINLINE void merge_network4<avx512f, float>(avx512f&, avx512f&, avx512f&, avx512f&);
	template FORCEINLINE void merge_network4<avx512, i64>(avx512&, avx512&, avx512&, avx512&);
	template FORCEINLINE void merge_network4<avx512d, double>(avx512d&, avx512d&, avx512d&, avx512d&);
	template FORCEINLINE void merge_network4<avx512, KeyValue<i64, i64>>(avx512&, avx512&, avx512&, avx512&);

	template FORCEINLINE void merge_network8<sse, ui>(sse&, sse&, sse&, sse&, sse&, sse&, sse&, sse&);
	template FORCEINLINE void merge_network8<ssef, float>(ssef&, ssef&, ssef&, ssef&, ssef&, ssef&, ssef&, ssef&);
	template FORCEINLINE void merge_network8<sse, i64>(sse&, sse&, sse&, sse&, sse&, sse&, sse&, sse&);
	template FORCEINLINE void merge_network8<ssed, double>(ssed&, ssed&, ssed&, ssed&, ssed&, ssed&, ssed&, ssed&);
	template FORCEINLINE void merge_network8<sse, KeyValue<i64, i64>>(sse&, sse&, sse&, sse&, sse&, sse&, sse&, sse&);

	template FORCEINLINE void merge_network8<avx2, ui>(avx2&, avx2&, avx2&, avx2&, avx2&, avx2&, avx2&, avx2&);
	template FORCEINLINE void merge_network8<avx2f, float>(avx2f&, avx2f&, avx2f&, avx2f&, avx2f&, avx2f&, avx2f&, avx2f&);
	template FORCEINLINE void merge_network8<avx2, i64>(avx2&, avx2&, avx2&, avx2&, avx2&, avx2&, avx2&, avx2&);
	template FORCEINLINE void merge_network8<avx2d, double>(avx2d&, avx2d&, avx2d&, avx2d&, avx2d&, avx2d&, avx2d&, avx2d&);
	template FORCEINLINE void merge_network8<avx2, KeyValue<i64, i64>>(avx2&, avx2&, avx2&, avx2&, avx2&, avx2&, avx2&, avx2&);

	template FORCEINLINE void merge_network8<avx512, ui>(avx512&, avx512&, avx512&, avx512&, avx512&, avx512&, avx512&, avx512&);
	template FORCEINLINE void merge_network8<avx512f, float>(avx512f&, avx512f&, avx512f&, avx512f&, avx512f&, avx512f&, avx512f&, avx512f&);
	template FORCEINLINE void merge_network8<avx512, i64>(avx512&, avx512&, avx512&, avx512&, avx512&, avx512&, avx512&, avx512&);
	template FORCEINLINE void merge_network8<avx512d, double>(avx512d&, avx512d&, avx512d&, avx512d&, avx512d&, avx512d&, avx512d&, avx512d&);
	template FORCEINLINE void merge_network8<avx512, KeyValue<i64, i64>>(avx512&, avx512&, avx512&, avx512&, avx512&, avx512&, avx512&, avx512&);
}