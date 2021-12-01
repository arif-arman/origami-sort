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
}