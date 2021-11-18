#pragma once

#include "commons.h"
#include "merge_utils.h"
#include "merge_tree.h"
#include "merger.h"
#include "Partition.h"
#include <fstream>


namespace origami_sorter {

	// -------- the basics: S, T
	template <typename Item, typename Reg>
	FORCEINLINE void in_register_sort(Reg& a0, Reg& a1, Reg& a2, Reg& a3) {
		origami_utils::swap<Reg, Item>(a0, a2); origami_utils::swap<Reg, Item>(a1, a3);
		origami_utils::swap<Reg, Item>(a0, a1); origami_utils::swap<Reg, Item>(a2, a3);
		origami_utils::swap<Reg, Item>(a1, a2);
	}
	
	template <typename Item, typename Reg>
	FORCEINLINE void in_register_sort8(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7) {
		origami_utils::swap<Reg, Item>(a0, a4);	origami_utils::swap<Reg, Item>(a1, a5);	origami_utils::swap<Reg, Item>(a2, a6);	origami_utils::swap<Reg, Item>(a3, a7);
		origami_utils::swap<Reg, Item>(a0, a2);	origami_utils::swap<Reg, Item>(a1, a3);	origami_utils::swap<Reg, Item>(a4, a6);	origami_utils::swap<Reg, Item>(a5, a7);
		origami_utils::swap<Reg, Item>(a2, a4);	origami_utils::swap<Reg, Item>(a3, a5);	origami_utils::swap<Reg, Item>(a0, a1);	origami_utils::swap<Reg, Item>(a6, a7);
		origami_utils::swap<Reg, Item>(a2, a3);	origami_utils::swap<Reg, Item>(a4, a5);
		origami_utils::swap<Reg, Item>(a1, a4);	origami_utils::swap<Reg, Item>(a3, a6);
		origami_utils::swap<Reg, Item>(a1, a2);	origami_utils::swap<Reg, Item>(a3, a4);	origami_utils::swap<Reg, Item>(a5, a6);
	}

	template <typename KeyType, typename ValueType>
	FORCEINLINE void in_register_sort8_kv_scalar(KeyType& a0, ValueType& b0, KeyType& a1, ValueType& b1, KeyType& a2, ValueType& b2, KeyType& a3, ValueType& b3, KeyType& a4, ValueType& b4, KeyType& a5, ValueType& b5, KeyType& a6, ValueType& b6, KeyType& a7, ValueType& b7) {
		SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
		SWAPKV2(0, 2);	SWAPKV2(1, 3);	SWAPKV2(4, 6);	SWAPKV2(5, 7);
		SWAPKV2(2, 4);	SWAPKV2(3, 5);	SWAPKV2(0, 1);	SWAPKV2(6, 7);
		SWAPKV2(2, 3);	SWAPKV2(4, 5);
		SWAPKV2(1, 4);	SWAPKV2(3, 6);
		SWAPKV2(1, 2);	SWAPKV2(3, 4);	SWAPKV2(5, 6);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_sort16(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, 
		Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		origami_utils::swap<Reg, Item>(a0, a1);	origami_utils::swap<Reg, Item>(a2, a3);	origami_utils::swap<Reg, Item>(a4, a5);	origami_utils::swap<Reg, Item>(a6, a7);	origami_utils::swap<Reg, Item>(a8, a9);	origami_utils::swap<Reg, Item>(a10, a11);	origami_utils::swap<Reg, Item>(a12, a13);	origami_utils::swap<Reg, Item>(a14, a15);	origami_utils::swap<Reg, Item>(a0, a2);	origami_utils::swap<Reg, Item>(a4, a6);	origami_utils::swap<Reg, Item>(a8, a10);	origami_utils::swap<Reg, Item>(a12, a14);	origami_utils::swap<Reg, Item>(a1, a3);	origami_utils::swap<Reg, Item>(a5, a7);	origami_utils::swap<Reg, Item>(a9, a11);	origami_utils::swap<Reg, Item>(a13, a15);	origami_utils::swap<Reg, Item>(a0, a4);	origami_utils::swap<Reg, Item>(a8, a12);	origami_utils::swap<Reg, Item>(a1, a5);	origami_utils::swap<Reg, Item>(a9, a13);	origami_utils::swap<Reg, Item>(a2, a6);	origami_utils::swap<Reg, Item>(a10, a14);	origami_utils::swap<Reg, Item>(a3, a7);	origami_utils::swap<Reg, Item>(a11, a15);	origami_utils::swap<Reg, Item>(a0, a8);	origami_utils::swap<Reg, Item>(a1, a9);	origami_utils::swap<Reg, Item>(a2, a10);	origami_utils::swap<Reg, Item>(a3, a11);	origami_utils::swap<Reg, Item>(a4, a12);	origami_utils::swap<Reg, Item>(a5, a13);	origami_utils::swap<Reg, Item>(a6, a14);	origami_utils::swap<Reg, Item>(a7, a15);	origami_utils::swap<Reg, Item>(a5, a10);	origami_utils::swap<Reg, Item>(a6, a9);	origami_utils::swap<Reg, Item>(a3, a12);	origami_utils::swap<Reg, Item>(a13, a14);	origami_utils::swap<Reg, Item>(a7, a11);	origami_utils::swap<Reg, Item>(a1, a2);	origami_utils::swap<Reg, Item>(a4, a8);	origami_utils::swap<Reg, Item>(a1, a4);	origami_utils::swap<Reg, Item>(a7, a13);	origami_utils::swap<Reg, Item>(a2, a8);	origami_utils::swap<Reg, Item>(a11, a14);	origami_utils::swap<Reg, Item>(a2, a4);	origami_utils::swap<Reg, Item>(a5, a6);	origami_utils::swap<Reg, Item>(a9, a10);	origami_utils::swap<Reg, Item>(a11, a13);	origami_utils::swap<Reg, Item>(a3, a8);	origami_utils::swap<Reg, Item>(a7, a12);
		origami_utils::swap<Reg, Item>(a6, a8);	origami_utils::swap<Reg, Item>(a10, a12);	origami_utils::swap<Reg, Item>(a3, a5);	origami_utils::swap<Reg, Item>(a7, a9);	origami_utils::swap<Reg, Item>(a3, a4);	origami_utils::swap<Reg, Item>(a5, a6);	origami_utils::swap<Reg, Item>(a7, a8);	origami_utils::swap<Reg, Item>(a9, a10);	origami_utils::swap<Reg, Item>(a11, a12);	origami_utils::swap<Reg, Item>(a6, a7);	origami_utils::swap<Reg, Item>(a8, a9);
	}

	template <typename KeyType, typename ValueType>
	FORCEINLINE void in_register_sort16_kv_scalar(KeyType& a0, ValueType& b0, KeyType& a1, ValueType& b1, KeyType& a2, ValueType& b2, KeyType& a3, ValueType& b3, KeyType& a4, ValueType& b4, KeyType& a5, ValueType& b5, KeyType& a6, ValueType& b6, KeyType& a7, ValueType& b7, KeyType& a8, ValueType& b8, KeyType& a9, ValueType& b9, KeyType& a10, ValueType& b10, KeyType& a11, ValueType& b11, KeyType& a12, ValueType& b12, KeyType& a13, ValueType& b13, KeyType& a14, ValueType& b14, KeyType& a15, ValueType& b15) {
		SWAPKV2(0, 1);	SWAPKV2(2, 3);	SWAPKV2(4, 5);	SWAPKV2(6, 7);	SWAPKV2(8, 9);	SWAPKV2(10, 11);	SWAPKV2(12, 13);	SWAPKV2(14, 15);	SWAPKV2(0, 2);	SWAPKV2(4, 6);	SWAPKV2(8, 10);	SWAPKV2(12, 14);	SWAPKV2(1, 3);	SWAPKV2(5, 7);	SWAPKV2(9, 11);	SWAPKV2(13, 15);	SWAPKV2(0, 4);	SWAPKV2(8, 12);	SWAPKV2(1, 5);	SWAPKV2(9, 13);	SWAPKV2(2, 6);	SWAPKV2(10, 14);	SWAPKV2(3, 7);	SWAPKV2(11, 15);	SWAPKV2(0, 8);	SWAPKV2(1, 9);	SWAPKV2(2, 10);	SWAPKV2(3, 11);	SWAPKV2(4, 12);	SWAPKV2(5, 13);	SWAPKV2(6, 14);	SWAPKV2(7, 15);	SWAPKV2(5, 10);	SWAPKV2(6, 9);	SWAPKV2(3, 12);	SWAPKV2(13, 14);	SWAPKV2(7, 11);	SWAPKV2(1, 2);	SWAPKV2(4, 8);	SWAPKV2(1, 4);	SWAPKV2(7, 13);	SWAPKV2(2, 8);	SWAPKV2(11, 14);	SWAPKV2(2, 4);	SWAPKV2(5, 6);	SWAPKV2(9, 10);	SWAPKV2(11, 13);	SWAPKV2(3, 8);	SWAPKV2(7, 12);
		SWAPKV2(6, 8);	SWAPKV2(10, 12);	SWAPKV2(3, 5);	SWAPKV2(7, 9);	SWAPKV2(3, 4);	SWAPKV2(5, 6);	SWAPKV2(7, 8);	SWAPKV2(9, 10);	SWAPKV2(11, 12);	SWAPKV2(6, 7);	SWAPKV2(8, 9);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_sort32(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15, Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		origami_utils::swap<Reg, Item>(a0, a16);
		origami_utils::swap<Reg, Item>(a1, a17);
		origami_utils::swap<Reg, Item>(a2, a18);
		origami_utils::swap<Reg, Item>(a3, a19);
		origami_utils::swap<Reg, Item>(a4, a20);
		origami_utils::swap<Reg, Item>(a5, a21);
		origami_utils::swap<Reg, Item>(a6, a22);
		origami_utils::swap<Reg, Item>(a7, a23);
		origami_utils::swap<Reg, Item>(a8, a24);
		origami_utils::swap<Reg, Item>(a9, a25);
		origami_utils::swap<Reg, Item>(a10, a26);
		origami_utils::swap<Reg, Item>(a11, a27);
		origami_utils::swap<Reg, Item>(a12, a28);
		origami_utils::swap<Reg, Item>(a13, a29);
		origami_utils::swap<Reg, Item>(a14, a30);
		origami_utils::swap<Reg, Item>(a15, a31);
		origami_utils::swap<Reg, Item>(a0, a8);
		origami_utils::swap<Reg, Item>(a1, a9);
		origami_utils::swap<Reg, Item>(a2, a10);
		origami_utils::swap<Reg, Item>(a3, a11);
		origami_utils::swap<Reg, Item>(a4, a12);
		origami_utils::swap<Reg, Item>(a5, a13);
		origami_utils::swap<Reg, Item>(a6, a14);
		origami_utils::swap<Reg, Item>(a7, a15);
		origami_utils::swap<Reg, Item>(a16, a24);
		origami_utils::swap<Reg, Item>(a17, a25);
		origami_utils::swap<Reg, Item>(a18, a26);
		origami_utils::swap<Reg, Item>(a19, a27);
		origami_utils::swap<Reg, Item>(a20, a28);
		origami_utils::swap<Reg, Item>(a21, a29);
		origami_utils::swap<Reg, Item>(a22, a30);
		origami_utils::swap<Reg, Item>(a23, a31);
		origami_utils::swap<Reg, Item>(a8, a16);
		origami_utils::swap<Reg, Item>(a9, a17);
		origami_utils::swap<Reg, Item>(a10, a18);
		origami_utils::swap<Reg, Item>(a11, a19);
		origami_utils::swap<Reg, Item>(a12, a20);
		origami_utils::swap<Reg, Item>(a13, a21);
		origami_utils::swap<Reg, Item>(a14, a22);
		origami_utils::swap<Reg, Item>(a15, a23);
		origami_utils::swap<Reg, Item>(a0, a4);
		origami_utils::swap<Reg, Item>(a1, a5);
		origami_utils::swap<Reg, Item>(a2, a6);
		origami_utils::swap<Reg, Item>(a3, a7);
		origami_utils::swap<Reg, Item>(a8, a12);
		origami_utils::swap<Reg, Item>(a9, a13);
		origami_utils::swap<Reg, Item>(a10, a14);
		origami_utils::swap<Reg, Item>(a11, a15);
		origami_utils::swap<Reg, Item>(a16, a20);
		origami_utils::swap<Reg, Item>(a17, a21);
		origami_utils::swap<Reg, Item>(a18, a22);
		origami_utils::swap<Reg, Item>(a19, a23);
		origami_utils::swap<Reg, Item>(a24, a28);
		origami_utils::swap<Reg, Item>(a25, a29);
		origami_utils::swap<Reg, Item>(a26, a30);
		origami_utils::swap<Reg, Item>(a27, a31);
		origami_utils::swap<Reg, Item>(a4, a16);
		origami_utils::swap<Reg, Item>(a5, a17);
		origami_utils::swap<Reg, Item>(a6, a18);
		origami_utils::swap<Reg, Item>(a7, a19);
		origami_utils::swap<Reg, Item>(a12, a24);
		origami_utils::swap<Reg, Item>(a13, a25);
		origami_utils::swap<Reg, Item>(a14, a26);
		origami_utils::swap<Reg, Item>(a15, a27);
		origami_utils::swap<Reg, Item>(a4, a8);
		origami_utils::swap<Reg, Item>(a5, a9);
		origami_utils::swap<Reg, Item>(a6, a10);
		origami_utils::swap<Reg, Item>(a7, a11);
		origami_utils::swap<Reg, Item>(a12, a16);
		origami_utils::swap<Reg, Item>(a13, a17);
		origami_utils::swap<Reg, Item>(a14, a18);
		origami_utils::swap<Reg, Item>(a15, a19);
		origami_utils::swap<Reg, Item>(a20, a24);
		origami_utils::swap<Reg, Item>(a21, a25);
		origami_utils::swap<Reg, Item>(a22, a26);
		origami_utils::swap<Reg, Item>(a23, a27);
		origami_utils::swap<Reg, Item>(a0, a2);
		origami_utils::swap<Reg, Item>(a1, a3);
		origami_utils::swap<Reg, Item>(a4, a6);
		origami_utils::swap<Reg, Item>(a5, a7);
		origami_utils::swap<Reg, Item>(a8, a10);
		origami_utils::swap<Reg, Item>(a9, a11);
		origami_utils::swap<Reg, Item>(a12, a14);
		origami_utils::swap<Reg, Item>(a13, a15);
		origami_utils::swap<Reg, Item>(a16, a18);
		origami_utils::swap<Reg, Item>(a17, a19);
		origami_utils::swap<Reg, Item>(a20, a22);
		origami_utils::swap<Reg, Item>(a21, a23);
		origami_utils::swap<Reg, Item>(a24, a26);
		origami_utils::swap<Reg, Item>(a25, a27);
		origami_utils::swap<Reg, Item>(a28, a30);
		origami_utils::swap<Reg, Item>(a29, a31);
		origami_utils::swap<Reg, Item>(a2, a16);
		origami_utils::swap<Reg, Item>(a3, a17);
		origami_utils::swap<Reg, Item>(a6, a20);
		origami_utils::swap<Reg, Item>(a7, a21);
		origami_utils::swap<Reg, Item>(a10, a24);
		origami_utils::swap<Reg, Item>(a11, a25);
		origami_utils::swap<Reg, Item>(a14, a28);
		origami_utils::swap<Reg, Item>(a15, a29);
		origami_utils::swap<Reg, Item>(a2, a8);
		origami_utils::swap<Reg, Item>(a3, a9);
		origami_utils::swap<Reg, Item>(a6, a12);
		origami_utils::swap<Reg, Item>(a7, a13);
		origami_utils::swap<Reg, Item>(a10, a16);
		origami_utils::swap<Reg, Item>(a11, a17);
		origami_utils::swap<Reg, Item>(a14, a20);
		origami_utils::swap<Reg, Item>(a15, a21);
		origami_utils::swap<Reg, Item>(a18, a24);
		origami_utils::swap<Reg, Item>(a19, a25);
		origami_utils::swap<Reg, Item>(a22, a28);
		origami_utils::swap<Reg, Item>(a23, a29);
		origami_utils::swap<Reg, Item>(a2, a4);
		origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a6, a8);
		origami_utils::swap<Reg, Item>(a7, a9);
		origami_utils::swap<Reg, Item>(a10, a12);
		origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a14, a16);
		origami_utils::swap<Reg, Item>(a15, a17);
		origami_utils::swap<Reg, Item>(a18, a20);
		origami_utils::swap<Reg, Item>(a19, a21);
		origami_utils::swap<Reg, Item>(a22, a24);
		origami_utils::swap<Reg, Item>(a23, a25);
		origami_utils::swap<Reg, Item>(a26, a28);
		origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a0, a1);
		origami_utils::swap<Reg, Item>(a2, a3);
		origami_utils::swap<Reg, Item>(a4, a5);
		origami_utils::swap<Reg, Item>(a6, a7);
		origami_utils::swap<Reg, Item>(a8, a9);
		origami_utils::swap<Reg, Item>(a10, a11);
		origami_utils::swap<Reg, Item>(a12, a13);
		origami_utils::swap<Reg, Item>(a14, a15);
		origami_utils::swap<Reg, Item>(a16, a17);
		origami_utils::swap<Reg, Item>(a18, a19);
		origami_utils::swap<Reg, Item>(a20, a21);
		origami_utils::swap<Reg, Item>(a22, a23);
		origami_utils::swap<Reg, Item>(a24, a25);
		origami_utils::swap<Reg, Item>(a26, a27);
		origami_utils::swap<Reg, Item>(a28, a29);
		origami_utils::swap<Reg, Item>(a30, a31);
		origami_utils::swap<Reg, Item>(a1, a16);
		origami_utils::swap<Reg, Item>(a3, a18);
		origami_utils::swap<Reg, Item>(a5, a20);
		origami_utils::swap<Reg, Item>(a7, a22);
		origami_utils::swap<Reg, Item>(a9, a24);
		origami_utils::swap<Reg, Item>(a11, a26);
		origami_utils::swap<Reg, Item>(a13, a28);
		origami_utils::swap<Reg, Item>(a15, a30);
		origami_utils::swap<Reg, Item>(a1, a8);
		origami_utils::swap<Reg, Item>(a3, a10);
		origami_utils::swap<Reg, Item>(a5, a12);
		origami_utils::swap<Reg, Item>(a7, a14);
		origami_utils::swap<Reg, Item>(a9, a16);
		origami_utils::swap<Reg, Item>(a11, a18);
		origami_utils::swap<Reg, Item>(a13, a20);
		origami_utils::swap<Reg, Item>(a15, a22);
		origami_utils::swap<Reg, Item>(a17, a24);
		origami_utils::swap<Reg, Item>(a19, a26);
		origami_utils::swap<Reg, Item>(a21, a28);
		origami_utils::swap<Reg, Item>(a23, a30);
		origami_utils::swap<Reg, Item>(a1, a4);
		origami_utils::swap<Reg, Item>(a3, a6);
		origami_utils::swap<Reg, Item>(a5, a8);
		origami_utils::swap<Reg, Item>(a7, a10);
		origami_utils::swap<Reg, Item>(a9, a12);
		origami_utils::swap<Reg, Item>(a11, a14);
		origami_utils::swap<Reg, Item>(a13, a16);
		origami_utils::swap<Reg, Item>(a15, a18);
		origami_utils::swap<Reg, Item>(a17, a20);
		origami_utils::swap<Reg, Item>(a19, a22);
		origami_utils::swap<Reg, Item>(a21, a24);
		origami_utils::swap<Reg, Item>(a23, a26);
		origami_utils::swap<Reg, Item>(a25, a28);
		origami_utils::swap<Reg, Item>(a27, a30);
		origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a3, a4);
		origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a7, a8);
		origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a11, a12);
		origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a15, a16);
		origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a19, a20);
		origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a23, a24);
		origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a27, a28);
		origami_utils::swap<Reg, Item>(a29, a30);
	}

	template <typename KeyType, typename ValueType>
	FORCEINLINE void in_register_sort32_kv_scalar(KeyType& a0, ValueType& b0, KeyType& a1, ValueType& b1, KeyType& a2, ValueType& b2, KeyType& a3, ValueType& b3, KeyType& a4, ValueType& b4, KeyType& a5, ValueType& b5, KeyType& a6, ValueType& b6, KeyType& a7, ValueType& b7, KeyType& a8, ValueType& b8, KeyType& a9, ValueType& b9, KeyType& a10, ValueType& b10, KeyType& a11, ValueType& b11, KeyType& a12, ValueType& b12, KeyType& a13, ValueType& b13, KeyType& a14, ValueType& b14, KeyType& a15, ValueType& b15, KeyType& a16, ValueType& b16, KeyType& a17, ValueType& b17, KeyType& a18, ValueType& b18, KeyType& a19, ValueType& b19, KeyType& a20, ValueType& b20, KeyType& a21, ValueType& b21, KeyType& a22, ValueType& b22, KeyType& a23, ValueType& b23, KeyType& a24, ValueType& b24, KeyType& a25, ValueType& b25, KeyType& a26, ValueType& b26, KeyType& a27, ValueType& b27, KeyType& a28, ValueType& b28, KeyType& a29, ValueType& b29, KeyType& a30, ValueType& b30, KeyType& a31, ValueType& b31) {
		SWAPKV2(0, 16);
		SWAPKV2(1, 17);
		SWAPKV2(2, 18);
		SWAPKV2(3, 19);
		SWAPKV2(4, 20);
		SWAPKV2(5, 21);
		SWAPKV2(6, 22);
		SWAPKV2(7, 23);
		SWAPKV2(8, 24);
		SWAPKV2(9, 25);
		SWAPKV2(10, 26);
		SWAPKV2(11, 27);
		SWAPKV2(12, 28);
		SWAPKV2(13, 29);
		SWAPKV2(14, 30);
		SWAPKV2(15, 31);
		SWAPKV2(0, 8);
		SWAPKV2(1, 9);
		SWAPKV2(2, 10);
		SWAPKV2(3, 11);
		SWAPKV2(4, 12);
		SWAPKV2(5, 13);
		SWAPKV2(6, 14);
		SWAPKV2(7, 15);
		SWAPKV2(16, 24);
		SWAPKV2(17, 25);
		SWAPKV2(18, 26);
		SWAPKV2(19, 27);
		SWAPKV2(20, 28);
		SWAPKV2(21, 29);
		SWAPKV2(22, 30);
		SWAPKV2(23, 31);
		SWAPKV2(8, 16);
		SWAPKV2(9, 17);
		SWAPKV2(10, 18);
		SWAPKV2(11, 19);
		SWAPKV2(12, 20);
		SWAPKV2(13, 21);
		SWAPKV2(14, 22);
		SWAPKV2(15, 23);
		SWAPKV2(0, 4);
		SWAPKV2(1, 5);
		SWAPKV2(2, 6);
		SWAPKV2(3, 7);
		SWAPKV2(8, 12);
		SWAPKV2(9, 13);
		SWAPKV2(10, 14);
		SWAPKV2(11, 15);
		SWAPKV2(16, 20);
		SWAPKV2(17, 21);
		SWAPKV2(18, 22);
		SWAPKV2(19, 23);
		SWAPKV2(24, 28);
		SWAPKV2(25, 29);
		SWAPKV2(26, 30);
		SWAPKV2(27, 31);
		SWAPKV2(4, 16);
		SWAPKV2(5, 17);
		SWAPKV2(6, 18);
		SWAPKV2(7, 19);
		SWAPKV2(12, 24);
		SWAPKV2(13, 25);
		SWAPKV2(14, 26);
		SWAPKV2(15, 27);
		SWAPKV2(4, 8);
		SWAPKV2(5, 9);
		SWAPKV2(6, 10);
		SWAPKV2(7, 11);
		SWAPKV2(12, 16);
		SWAPKV2(13, 17);
		SWAPKV2(14, 18);
		SWAPKV2(15, 19);
		SWAPKV2(20, 24);
		SWAPKV2(21, 25);
		SWAPKV2(22, 26);
		SWAPKV2(23, 27);
		SWAPKV2(0, 2);
		SWAPKV2(1, 3);
		SWAPKV2(4, 6);
		SWAPKV2(5, 7);
		SWAPKV2(8, 10);
		SWAPKV2(9, 11);
		SWAPKV2(12, 14);
		SWAPKV2(13, 15);
		SWAPKV2(16, 18);
		SWAPKV2(17, 19);
		SWAPKV2(20, 22);
		SWAPKV2(21, 23);
		SWAPKV2(24, 26);
		SWAPKV2(25, 27);
		SWAPKV2(28, 30);
		SWAPKV2(29, 31);
		SWAPKV2(2, 16);
		SWAPKV2(3, 17);
		SWAPKV2(6, 20);
		SWAPKV2(7, 21);
		SWAPKV2(10, 24);
		SWAPKV2(11, 25);
		SWAPKV2(14, 28);
		SWAPKV2(15, 29);
		SWAPKV2(2, 8);
		SWAPKV2(3, 9);
		SWAPKV2(6, 12);
		SWAPKV2(7, 13);
		SWAPKV2(10, 16);
		SWAPKV2(11, 17);
		SWAPKV2(14, 20);
		SWAPKV2(15, 21);
		SWAPKV2(18, 24);
		SWAPKV2(19, 25);
		SWAPKV2(22, 28);
		SWAPKV2(23, 29);
		SWAPKV2(2, 4);
		SWAPKV2(3, 5);
		SWAPKV2(6, 8);
		SWAPKV2(7, 9);
		SWAPKV2(10, 12);
		SWAPKV2(11, 13);
		SWAPKV2(14, 16);
		SWAPKV2(15, 17);
		SWAPKV2(18, 20);
		SWAPKV2(19, 21);
		SWAPKV2(22, 24);
		SWAPKV2(23, 25);
		SWAPKV2(26, 28);
		SWAPKV2(27, 29);
		SWAPKV2(0, 1);
		SWAPKV2(2, 3);
		SWAPKV2(4, 5);
		SWAPKV2(6, 7);
		SWAPKV2(8, 9);
		SWAPKV2(10, 11);
		SWAPKV2(12, 13);
		SWAPKV2(14, 15);
		SWAPKV2(16, 17);
		SWAPKV2(18, 19);
		SWAPKV2(20, 21);
		SWAPKV2(22, 23);
		SWAPKV2(24, 25);
		SWAPKV2(26, 27);
		SWAPKV2(28, 29);
		SWAPKV2(30, 31);
		SWAPKV2(1, 16);
		SWAPKV2(3, 18);
		SWAPKV2(5, 20);
		SWAPKV2(7, 22);
		SWAPKV2(9, 24);
		SWAPKV2(11, 26);
		SWAPKV2(13, 28);
		SWAPKV2(15, 30);
		SWAPKV2(1, 8);
		SWAPKV2(3, 10);
		SWAPKV2(5, 12);
		SWAPKV2(7, 14);
		SWAPKV2(9, 16);
		SWAPKV2(11, 18);
		SWAPKV2(13, 20);
		SWAPKV2(15, 22);
		SWAPKV2(17, 24);
		SWAPKV2(19, 26);
		SWAPKV2(21, 28);
		SWAPKV2(23, 30);
		SWAPKV2(1, 4);
		SWAPKV2(3, 6);
		SWAPKV2(5, 8);
		SWAPKV2(7, 10);
		SWAPKV2(9, 12);
		SWAPKV2(11, 14);
		SWAPKV2(13, 16);
		SWAPKV2(15, 18);
		SWAPKV2(17, 20);
		SWAPKV2(19, 22);
		SWAPKV2(21, 24);
		SWAPKV2(23, 26);
		SWAPKV2(25, 28);
		SWAPKV2(27, 30);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(15, 16);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_sort64(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15, Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31, Reg& a32, Reg& a33, Reg& a34, Reg& a35, Reg& a36, Reg& a37, Reg& a38, Reg& a39, Reg& a40, Reg& a41, Reg& a42, Reg& a43, Reg& a44, Reg& a45, Reg& a46, Reg& a47, Reg& a48, Reg& a49, Reg& a50, Reg& a51, Reg& a52, Reg& a53, Reg& a54, Reg& a55, Reg& a56, Reg& a57, Reg& a58, Reg& a59, Reg& a60, Reg& a61, Reg& a62, Reg& a63) {
		origami_utils::swap<Reg, Item>(a0, a1);
		origami_utils::swap<Reg, Item>(a2, a3);
		origami_utils::swap<Reg, Item>(a0, a2);
		origami_utils::swap<Reg, Item>(a1, a3);
		origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a4, a5);
		origami_utils::swap<Reg, Item>(a6, a7);
		origami_utils::swap<Reg, Item>(a4, a6);
		origami_utils::swap<Reg, Item>(a5, a7);
		origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a0, a4);
		origami_utils::swap<Reg, Item>(a2, a6);
		origami_utils::swap<Reg, Item>(a2, a4);
		origami_utils::swap<Reg, Item>(a1, a5);
		origami_utils::swap<Reg, Item>(a3, a7);
		origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a3, a4);
		origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a8, a9);
		origami_utils::swap<Reg, Item>(a10, a11);
		origami_utils::swap<Reg, Item>(a8, a10);
		origami_utils::swap<Reg, Item>(a9, a11);
		origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a12, a13);
		origami_utils::swap<Reg, Item>(a14, a15);
		origami_utils::swap<Reg, Item>(a12, a14);
		origami_utils::swap<Reg, Item>(a13, a15);
		origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a8, a12);
		origami_utils::swap<Reg, Item>(a10, a14);
		origami_utils::swap<Reg, Item>(a10, a12);
		origami_utils::swap<Reg, Item>(a9, a13);
		origami_utils::swap<Reg, Item>(a11, a15);
		origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a11, a12);
		origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a0, a8);
		origami_utils::swap<Reg, Item>(a4, a12);
		origami_utils::swap<Reg, Item>(a4, a8);
		origami_utils::swap<Reg, Item>(a2, a10);
		origami_utils::swap<Reg, Item>(a6, a14);
		origami_utils::swap<Reg, Item>(a6, a10);
		origami_utils::swap<Reg, Item>(a2, a4);
		origami_utils::swap<Reg, Item>(a6, a8);
		origami_utils::swap<Reg, Item>(a10, a12);
		origami_utils::swap<Reg, Item>(a1, a9);
		origami_utils::swap<Reg, Item>(a5, a13);
		origami_utils::swap<Reg, Item>(a5, a9);
		origami_utils::swap<Reg, Item>(a3, a11);
		origami_utils::swap<Reg, Item>(a7, a15);
		origami_utils::swap<Reg, Item>(a7, a11);
		origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a7, a9);
		origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a3, a4);
		origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a7, a8);
		origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a11, a12);
		origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a16, a17);
		origami_utils::swap<Reg, Item>(a18, a19);
		origami_utils::swap<Reg, Item>(a16, a18);
		origami_utils::swap<Reg, Item>(a17, a19);
		origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a20, a21);
		origami_utils::swap<Reg, Item>(a22, a23);
		origami_utils::swap<Reg, Item>(a20, a22);
		origami_utils::swap<Reg, Item>(a21, a23);
		origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a16, a20);
		origami_utils::swap<Reg, Item>(a18, a22);
		origami_utils::swap<Reg, Item>(a18, a20);
		origami_utils::swap<Reg, Item>(a17, a21);
		origami_utils::swap<Reg, Item>(a19, a23);
		origami_utils::swap<Reg, Item>(a19, a21);
		origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a19, a20);
		origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a24, a25);
		origami_utils::swap<Reg, Item>(a26, a27);
		origami_utils::swap<Reg, Item>(a24, a26);
		origami_utils::swap<Reg, Item>(a25, a27);
		origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a28, a29);
		origami_utils::swap<Reg, Item>(a30, a31);
		origami_utils::swap<Reg, Item>(a28, a30);
		origami_utils::swap<Reg, Item>(a29, a31);
		origami_utils::swap<Reg, Item>(a29, a30);
		origami_utils::swap<Reg, Item>(a24, a28);
		origami_utils::swap<Reg, Item>(a26, a30);
		origami_utils::swap<Reg, Item>(a26, a28);
		origami_utils::swap<Reg, Item>(a25, a29);
		origami_utils::swap<Reg, Item>(a27, a31);
		origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a27, a28);
		origami_utils::swap<Reg, Item>(a29, a30);
		origami_utils::swap<Reg, Item>(a16, a24);
		origami_utils::swap<Reg, Item>(a20, a28);
		origami_utils::swap<Reg, Item>(a20, a24);
		origami_utils::swap<Reg, Item>(a18, a26);
		origami_utils::swap<Reg, Item>(a22, a30);
		origami_utils::swap<Reg, Item>(a22, a26);
		origami_utils::swap<Reg, Item>(a18, a20);
		origami_utils::swap<Reg, Item>(a22, a24);
		origami_utils::swap<Reg, Item>(a26, a28);
		origami_utils::swap<Reg, Item>(a17, a25);
		origami_utils::swap<Reg, Item>(a21, a29);
		origami_utils::swap<Reg, Item>(a21, a25);
		origami_utils::swap<Reg, Item>(a19, a27);
		origami_utils::swap<Reg, Item>(a23, a31);
		origami_utils::swap<Reg, Item>(a23, a27);
		origami_utils::swap<Reg, Item>(a19, a21);
		origami_utils::swap<Reg, Item>(a23, a25);
		origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a19, a20);
		origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a23, a24);
		origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a27, a28);
		origami_utils::swap<Reg, Item>(a29, a30);
		origami_utils::swap<Reg, Item>(a0, a16);
		origami_utils::swap<Reg, Item>(a8, a24);
		origami_utils::swap<Reg, Item>(a8, a16);
		origami_utils::swap<Reg, Item>(a4, a20);
		origami_utils::swap<Reg, Item>(a12, a28);
		origami_utils::swap<Reg, Item>(a12, a20);
		origami_utils::swap<Reg, Item>(a4, a8);
		origami_utils::swap<Reg, Item>(a12, a16);
		origami_utils::swap<Reg, Item>(a20, a24);
		origami_utils::swap<Reg, Item>(a2, a18);
		origami_utils::swap<Reg, Item>(a10, a26);
		origami_utils::swap<Reg, Item>(a10, a18);
		origami_utils::swap<Reg, Item>(a6, a22);
		origami_utils::swap<Reg, Item>(a14, a30);
		origami_utils::swap<Reg, Item>(a14, a22);
		origami_utils::swap<Reg, Item>(a6, a10);
		origami_utils::swap<Reg, Item>(a14, a18);
		origami_utils::swap<Reg, Item>(a22, a26);
		origami_utils::swap<Reg, Item>(a2, a4);
		origami_utils::swap<Reg, Item>(a6, a8);
		origami_utils::swap<Reg, Item>(a10, a12);
		origami_utils::swap<Reg, Item>(a14, a16);
		origami_utils::swap<Reg, Item>(a18, a20);
		origami_utils::swap<Reg, Item>(a22, a24);
		origami_utils::swap<Reg, Item>(a26, a28);
		origami_utils::swap<Reg, Item>(a1, a17);
		origami_utils::swap<Reg, Item>(a9, a25);
		origami_utils::swap<Reg, Item>(a9, a17);
		origami_utils::swap<Reg, Item>(a5, a21);
		origami_utils::swap<Reg, Item>(a13, a29);
		origami_utils::swap<Reg, Item>(a13, a21);
		origami_utils::swap<Reg, Item>(a5, a9);
		origami_utils::swap<Reg, Item>(a13, a17);
		origami_utils::swap<Reg, Item>(a21, a25);
		origami_utils::swap<Reg, Item>(a3, a19);
		origami_utils::swap<Reg, Item>(a11, a27);
		origami_utils::swap<Reg, Item>(a11, a19);
		origami_utils::swap<Reg, Item>(a7, a23);
		origami_utils::swap<Reg, Item>(a15, a31);
		origami_utils::swap<Reg, Item>(a15, a23);
		origami_utils::swap<Reg, Item>(a7, a11);
		origami_utils::swap<Reg, Item>(a15, a19);
		origami_utils::swap<Reg, Item>(a23, a27);
		origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a7, a9);
		origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a15, a17);
		origami_utils::swap<Reg, Item>(a19, a21);
		origami_utils::swap<Reg, Item>(a23, a25);
		origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a3, a4);
		origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a7, a8);
		origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a11, a12);
		origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a15, a16);
		origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a19, a20);
		origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a23, a24);
		origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a27, a28);
		origami_utils::swap<Reg, Item>(a29, a30);
		origami_utils::swap<Reg, Item>(a32, a33);
		origami_utils::swap<Reg, Item>(a34, a35);
		origami_utils::swap<Reg, Item>(a32, a34);
		origami_utils::swap<Reg, Item>(a33, a35);
		origami_utils::swap<Reg, Item>(a33, a34);
		origami_utils::swap<Reg, Item>(a36, a37);
		origami_utils::swap<Reg, Item>(a38, a39);
		origami_utils::swap<Reg, Item>(a36, a38);
		origami_utils::swap<Reg, Item>(a37, a39);
		origami_utils::swap<Reg, Item>(a37, a38);
		origami_utils::swap<Reg, Item>(a32, a36);
		origami_utils::swap<Reg, Item>(a34, a38);
		origami_utils::swap<Reg, Item>(a34, a36);
		origami_utils::swap<Reg, Item>(a33, a37);
		origami_utils::swap<Reg, Item>(a35, a39);
		origami_utils::swap<Reg, Item>(a35, a37);
		origami_utils::swap<Reg, Item>(a33, a34);
		origami_utils::swap<Reg, Item>(a35, a36);
		origami_utils::swap<Reg, Item>(a37, a38);
		origami_utils::swap<Reg, Item>(a40, a41);
		origami_utils::swap<Reg, Item>(a42, a43);
		origami_utils::swap<Reg, Item>(a40, a42);
		origami_utils::swap<Reg, Item>(a41, a43);
		origami_utils::swap<Reg, Item>(a41, a42);
		origami_utils::swap<Reg, Item>(a44, a45);
		origami_utils::swap<Reg, Item>(a46, a47);
		origami_utils::swap<Reg, Item>(a44, a46);
		origami_utils::swap<Reg, Item>(a45, a47);
		origami_utils::swap<Reg, Item>(a45, a46);
		origami_utils::swap<Reg, Item>(a40, a44);
		origami_utils::swap<Reg, Item>(a42, a46);
		origami_utils::swap<Reg, Item>(a42, a44);
		origami_utils::swap<Reg, Item>(a41, a45);
		origami_utils::swap<Reg, Item>(a43, a47);
		origami_utils::swap<Reg, Item>(a43, a45);
		origami_utils::swap<Reg, Item>(a41, a42);
		origami_utils::swap<Reg, Item>(a43, a44);
		origami_utils::swap<Reg, Item>(a45, a46);
		origami_utils::swap<Reg, Item>(a32, a40);
		origami_utils::swap<Reg, Item>(a36, a44);
		origami_utils::swap<Reg, Item>(a36, a40);
		origami_utils::swap<Reg, Item>(a34, a42);
		origami_utils::swap<Reg, Item>(a38, a46);
		origami_utils::swap<Reg, Item>(a38, a42);
		origami_utils::swap<Reg, Item>(a34, a36);
		origami_utils::swap<Reg, Item>(a38, a40);
		origami_utils::swap<Reg, Item>(a42, a44);
		origami_utils::swap<Reg, Item>(a33, a41);
		origami_utils::swap<Reg, Item>(a37, a45);
		origami_utils::swap<Reg, Item>(a37, a41);
		origami_utils::swap<Reg, Item>(a35, a43);
		origami_utils::swap<Reg, Item>(a39, a47);
		origami_utils::swap<Reg, Item>(a39, a43);
		origami_utils::swap<Reg, Item>(a35, a37);
		origami_utils::swap<Reg, Item>(a39, a41);
		origami_utils::swap<Reg, Item>(a43, a45);
		origami_utils::swap<Reg, Item>(a33, a34);
		origami_utils::swap<Reg, Item>(a35, a36);
		origami_utils::swap<Reg, Item>(a37, a38);
		origami_utils::swap<Reg, Item>(a39, a40);
		origami_utils::swap<Reg, Item>(a41, a42);
		origami_utils::swap<Reg, Item>(a43, a44);
		origami_utils::swap<Reg, Item>(a45, a46);
		origami_utils::swap<Reg, Item>(a48, a49);
		origami_utils::swap<Reg, Item>(a50, a51);
		origami_utils::swap<Reg, Item>(a48, a50);
		origami_utils::swap<Reg, Item>(a49, a51);
		origami_utils::swap<Reg, Item>(a49, a50);
		origami_utils::swap<Reg, Item>(a52, a53);
		origami_utils::swap<Reg, Item>(a54, a55);
		origami_utils::swap<Reg, Item>(a52, a54);
		origami_utils::swap<Reg, Item>(a53, a55);
		origami_utils::swap<Reg, Item>(a53, a54);
		origami_utils::swap<Reg, Item>(a48, a52);
		origami_utils::swap<Reg, Item>(a50, a54);
		origami_utils::swap<Reg, Item>(a50, a52);
		origami_utils::swap<Reg, Item>(a49, a53);
		origami_utils::swap<Reg, Item>(a51, a55);
		origami_utils::swap<Reg, Item>(a51, a53);
		origami_utils::swap<Reg, Item>(a49, a50);
		origami_utils::swap<Reg, Item>(a51, a52);
		origami_utils::swap<Reg, Item>(a53, a54);
		origami_utils::swap<Reg, Item>(a56, a57);
		origami_utils::swap<Reg, Item>(a58, a59);
		origami_utils::swap<Reg, Item>(a56, a58);
		origami_utils::swap<Reg, Item>(a57, a59);
		origami_utils::swap<Reg, Item>(a57, a58);
		origami_utils::swap<Reg, Item>(a60, a61);
		origami_utils::swap<Reg, Item>(a62, a63);
		origami_utils::swap<Reg, Item>(a60, a62);
		origami_utils::swap<Reg, Item>(a61, a63);
		origami_utils::swap<Reg, Item>(a61, a62);
		origami_utils::swap<Reg, Item>(a56, a60);
		origami_utils::swap<Reg, Item>(a58, a62);
		origami_utils::swap<Reg, Item>(a58, a60);
		origami_utils::swap<Reg, Item>(a57, a61);
		origami_utils::swap<Reg, Item>(a59, a63);
		origami_utils::swap<Reg, Item>(a59, a61);
		origami_utils::swap<Reg, Item>(a57, a58);
		origami_utils::swap<Reg, Item>(a59, a60);
		origami_utils::swap<Reg, Item>(a61, a62);
		origami_utils::swap<Reg, Item>(a48, a56);
		origami_utils::swap<Reg, Item>(a52, a60);
		origami_utils::swap<Reg, Item>(a52, a56);
		origami_utils::swap<Reg, Item>(a50, a58);
		origami_utils::swap<Reg, Item>(a54, a62);
		origami_utils::swap<Reg, Item>(a54, a58);
		origami_utils::swap<Reg, Item>(a50, a52);
		origami_utils::swap<Reg, Item>(a54, a56);
		origami_utils::swap<Reg, Item>(a58, a60);
		origami_utils::swap<Reg, Item>(a49, a57);
		origami_utils::swap<Reg, Item>(a53, a61);
		origami_utils::swap<Reg, Item>(a53, a57);
		origami_utils::swap<Reg, Item>(a51, a59);
		origami_utils::swap<Reg, Item>(a55, a63);
		origami_utils::swap<Reg, Item>(a55, a59);
		origami_utils::swap<Reg, Item>(a51, a53);
		origami_utils::swap<Reg, Item>(a55, a57);
		origami_utils::swap<Reg, Item>(a59, a61);
		origami_utils::swap<Reg, Item>(a49, a50);
		origami_utils::swap<Reg, Item>(a51, a52);
		origami_utils::swap<Reg, Item>(a53, a54);
		origami_utils::swap<Reg, Item>(a55, a56);
		origami_utils::swap<Reg, Item>(a57, a58);
		origami_utils::swap<Reg, Item>(a59, a60);
		origami_utils::swap<Reg, Item>(a61, a62);
		origami_utils::swap<Reg, Item>(a32, a48);
		origami_utils::swap<Reg, Item>(a40, a56);
		origami_utils::swap<Reg, Item>(a40, a48);
		origami_utils::swap<Reg, Item>(a36, a52);
		origami_utils::swap<Reg, Item>(a44, a60);
		origami_utils::swap<Reg, Item>(a44, a52);
		origami_utils::swap<Reg, Item>(a36, a40);
		origami_utils::swap<Reg, Item>(a44, a48);
		origami_utils::swap<Reg, Item>(a52, a56);
		origami_utils::swap<Reg, Item>(a34, a50);
		origami_utils::swap<Reg, Item>(a42, a58);
		origami_utils::swap<Reg, Item>(a42, a50);
		origami_utils::swap<Reg, Item>(a38, a54);
		origami_utils::swap<Reg, Item>(a46, a62);
		origami_utils::swap<Reg, Item>(a46, a54);
		origami_utils::swap<Reg, Item>(a38, a42);
		origami_utils::swap<Reg, Item>(a46, a50);
		origami_utils::swap<Reg, Item>(a54, a58);
		origami_utils::swap<Reg, Item>(a34, a36);
		origami_utils::swap<Reg, Item>(a38, a40);
		origami_utils::swap<Reg, Item>(a42, a44);
		origami_utils::swap<Reg, Item>(a46, a48);
		origami_utils::swap<Reg, Item>(a50, a52);
		origami_utils::swap<Reg, Item>(a54, a56);
		origami_utils::swap<Reg, Item>(a58, a60);
		origami_utils::swap<Reg, Item>(a33, a49);
		origami_utils::swap<Reg, Item>(a41, a57);
		origami_utils::swap<Reg, Item>(a41, a49);
		origami_utils::swap<Reg, Item>(a37, a53);
		origami_utils::swap<Reg, Item>(a45, a61);
		origami_utils::swap<Reg, Item>(a45, a53);
		origami_utils::swap<Reg, Item>(a37, a41);
		origami_utils::swap<Reg, Item>(a45, a49);
		origami_utils::swap<Reg, Item>(a53, a57);
		origami_utils::swap<Reg, Item>(a35, a51);
		origami_utils::swap<Reg, Item>(a43, a59);
		origami_utils::swap<Reg, Item>(a43, a51);
		origami_utils::swap<Reg, Item>(a39, a55);
		origami_utils::swap<Reg, Item>(a47, a63);
		origami_utils::swap<Reg, Item>(a47, a55);
		origami_utils::swap<Reg, Item>(a39, a43);
		origami_utils::swap<Reg, Item>(a47, a51);
		origami_utils::swap<Reg, Item>(a55, a59);
		origami_utils::swap<Reg, Item>(a35, a37);
		origami_utils::swap<Reg, Item>(a39, a41);
		origami_utils::swap<Reg, Item>(a43, a45);
		origami_utils::swap<Reg, Item>(a47, a49);
		origami_utils::swap<Reg, Item>(a51, a53);
		origami_utils::swap<Reg, Item>(a55, a57);
		origami_utils::swap<Reg, Item>(a59, a61);
		origami_utils::swap<Reg, Item>(a33, a34);
		origami_utils::swap<Reg, Item>(a35, a36);
		origami_utils::swap<Reg, Item>(a37, a38);
		origami_utils::swap<Reg, Item>(a39, a40);
		origami_utils::swap<Reg, Item>(a41, a42);
		origami_utils::swap<Reg, Item>(a43, a44);
		origami_utils::swap<Reg, Item>(a45, a46);
		origami_utils::swap<Reg, Item>(a47, a48);
		origami_utils::swap<Reg, Item>(a49, a50);
		origami_utils::swap<Reg, Item>(a51, a52);
		origami_utils::swap<Reg, Item>(a53, a54);
		origami_utils::swap<Reg, Item>(a55, a56);
		origami_utils::swap<Reg, Item>(a57, a58);
		origami_utils::swap<Reg, Item>(a59, a60);
		origami_utils::swap<Reg, Item>(a61, a62);
		origami_utils::swap<Reg, Item>(a0, a32);
		origami_utils::swap<Reg, Item>(a16, a48);
		origami_utils::swap<Reg, Item>(a16, a32);
		origami_utils::swap<Reg, Item>(a8, a40);
		origami_utils::swap<Reg, Item>(a24, a56);
		origami_utils::swap<Reg, Item>(a24, a40);
		origami_utils::swap<Reg, Item>(a8, a16);
		origami_utils::swap<Reg, Item>(a24, a32);
		origami_utils::swap<Reg, Item>(a40, a48);
		origami_utils::swap<Reg, Item>(a4, a36);
		origami_utils::swap<Reg, Item>(a20, a52);
		origami_utils::swap<Reg, Item>(a20, a36);
		origami_utils::swap<Reg, Item>(a12, a44);
		origami_utils::swap<Reg, Item>(a28, a60);
		origami_utils::swap<Reg, Item>(a28, a44);
		origami_utils::swap<Reg, Item>(a12, a20);
		origami_utils::swap<Reg, Item>(a28, a36);
		origami_utils::swap<Reg, Item>(a44, a52);
		origami_utils::swap<Reg, Item>(a4, a8);
		origami_utils::swap<Reg, Item>(a12, a16);
		origami_utils::swap<Reg, Item>(a20, a24);
		origami_utils::swap<Reg, Item>(a28, a32);
		origami_utils::swap<Reg, Item>(a36, a40);
		origami_utils::swap<Reg, Item>(a44, a48);
		origami_utils::swap<Reg, Item>(a52, a56);
		origami_utils::swap<Reg, Item>(a2, a34);
		origami_utils::swap<Reg, Item>(a18, a50);
		origami_utils::swap<Reg, Item>(a18, a34);
		origami_utils::swap<Reg, Item>(a10, a42);
		origami_utils::swap<Reg, Item>(a26, a58);
		origami_utils::swap<Reg, Item>(a26, a42);
		origami_utils::swap<Reg, Item>(a10, a18);
		origami_utils::swap<Reg, Item>(a26, a34);
		origami_utils::swap<Reg, Item>(a42, a50);
		origami_utils::swap<Reg, Item>(a6, a38);
		origami_utils::swap<Reg, Item>(a22, a54);
		origami_utils::swap<Reg, Item>(a22, a38);
		origami_utils::swap<Reg, Item>(a14, a46);
		origami_utils::swap<Reg, Item>(a30, a62);
		origami_utils::swap<Reg, Item>(a30, a46);
		origami_utils::swap<Reg, Item>(a14, a22);
		origami_utils::swap<Reg, Item>(a30, a38);
		origami_utils::swap<Reg, Item>(a46, a54);
		origami_utils::swap<Reg, Item>(a6, a10);
		origami_utils::swap<Reg, Item>(a14, a18);
		origami_utils::swap<Reg, Item>(a22, a26);
		origami_utils::swap<Reg, Item>(a30, a34);
		origami_utils::swap<Reg, Item>(a38, a42);
		origami_utils::swap<Reg, Item>(a46, a50);
		origami_utils::swap<Reg, Item>(a54, a58);
		origami_utils::swap<Reg, Item>(a2, a4);
		origami_utils::swap<Reg, Item>(a6, a8);
		origami_utils::swap<Reg, Item>(a10, a12);
		origami_utils::swap<Reg, Item>(a14, a16);
		origami_utils::swap<Reg, Item>(a18, a20);
		origami_utils::swap<Reg, Item>(a22, a24);
		origami_utils::swap<Reg, Item>(a26, a28);
		origami_utils::swap<Reg, Item>(a30, a32);
		origami_utils::swap<Reg, Item>(a34, a36);
		origami_utils::swap<Reg, Item>(a38, a40);
		origami_utils::swap<Reg, Item>(a42, a44);
		origami_utils::swap<Reg, Item>(a46, a48);
		origami_utils::swap<Reg, Item>(a50, a52);
		origami_utils::swap<Reg, Item>(a54, a56);
		origami_utils::swap<Reg, Item>(a58, a60);
		origami_utils::swap<Reg, Item>(a1, a33);
		origami_utils::swap<Reg, Item>(a17, a49);
		origami_utils::swap<Reg, Item>(a17, a33);
		origami_utils::swap<Reg, Item>(a9, a41);
		origami_utils::swap<Reg, Item>(a25, a57);
		origami_utils::swap<Reg, Item>(a25, a41);
		origami_utils::swap<Reg, Item>(a9, a17);
		origami_utils::swap<Reg, Item>(a25, a33);
		origami_utils::swap<Reg, Item>(a41, a49);
		origami_utils::swap<Reg, Item>(a5, a37);
		origami_utils::swap<Reg, Item>(a21, a53);
		origami_utils::swap<Reg, Item>(a21, a37);
		origami_utils::swap<Reg, Item>(a13, a45);
		origami_utils::swap<Reg, Item>(a29, a61);
		origami_utils::swap<Reg, Item>(a29, a45);
		origami_utils::swap<Reg, Item>(a13, a21);
		origami_utils::swap<Reg, Item>(a29, a37);
		origami_utils::swap<Reg, Item>(a45, a53);
		origami_utils::swap<Reg, Item>(a5, a9);
		origami_utils::swap<Reg, Item>(a13, a17);
		origami_utils::swap<Reg, Item>(a21, a25);
		origami_utils::swap<Reg, Item>(a29, a33);
		origami_utils::swap<Reg, Item>(a37, a41);
		origami_utils::swap<Reg, Item>(a45, a49);
		origami_utils::swap<Reg, Item>(a53, a57);
		origami_utils::swap<Reg, Item>(a3, a35);
		origami_utils::swap<Reg, Item>(a19, a51);
		origami_utils::swap<Reg, Item>(a19, a35);
		origami_utils::swap<Reg, Item>(a11, a43);
		origami_utils::swap<Reg, Item>(a27, a59);
		origami_utils::swap<Reg, Item>(a27, a43);
		origami_utils::swap<Reg, Item>(a11, a19);
		origami_utils::swap<Reg, Item>(a27, a35);
		origami_utils::swap<Reg, Item>(a43, a51);
		origami_utils::swap<Reg, Item>(a7, a39);
		origami_utils::swap<Reg, Item>(a23, a55);
		origami_utils::swap<Reg, Item>(a23, a39);
		origami_utils::swap<Reg, Item>(a15, a47);
		origami_utils::swap<Reg, Item>(a31, a63);
		origami_utils::swap<Reg, Item>(a31, a47);
		origami_utils::swap<Reg, Item>(a15, a23);
		origami_utils::swap<Reg, Item>(a31, a39);
		origami_utils::swap<Reg, Item>(a47, a55);
		origami_utils::swap<Reg, Item>(a7, a11);
		origami_utils::swap<Reg, Item>(a15, a19);
		origami_utils::swap<Reg, Item>(a23, a27);
		origami_utils::swap<Reg, Item>(a31, a35);
		origami_utils::swap<Reg, Item>(a39, a43);
		origami_utils::swap<Reg, Item>(a47, a51);
		origami_utils::swap<Reg, Item>(a55, a59);
		origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a7, a9);
		origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a15, a17);
		origami_utils::swap<Reg, Item>(a19, a21);
		origami_utils::swap<Reg, Item>(a23, a25);
		origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a31, a33);
		origami_utils::swap<Reg, Item>(a35, a37);
		origami_utils::swap<Reg, Item>(a39, a41);
		origami_utils::swap<Reg, Item>(a43, a45);
		origami_utils::swap<Reg, Item>(a47, a49);
		origami_utils::swap<Reg, Item>(a51, a53);
		origami_utils::swap<Reg, Item>(a55, a57);
		origami_utils::swap<Reg, Item>(a59, a61);
		origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a3, a4);
		origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a7, a8);
		origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a11, a12);
		origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a15, a16);
		origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a19, a20);
		origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a23, a24);
		origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a27, a28);
		origami_utils::swap<Reg, Item>(a29, a30);
		origami_utils::swap<Reg, Item>(a31, a32);
		origami_utils::swap<Reg, Item>(a33, a34);
		origami_utils::swap<Reg, Item>(a35, a36);
		origami_utils::swap<Reg, Item>(a37, a38);
		origami_utils::swap<Reg, Item>(a39, a40);
		origami_utils::swap<Reg, Item>(a41, a42);
		origami_utils::swap<Reg, Item>(a43, a44);
		origami_utils::swap<Reg, Item>(a45, a46);
		origami_utils::swap<Reg, Item>(a47, a48);
		origami_utils::swap<Reg, Item>(a49, a50);
		origami_utils::swap<Reg, Item>(a51, a52);
		origami_utils::swap<Reg, Item>(a53, a54);
		origami_utils::swap<Reg, Item>(a55, a56);
		origami_utils::swap<Reg, Item>(a57, a58);
		origami_utils::swap<Reg, Item>(a59, a60);
		origami_utils::swap<Reg, Item>(a61, a62);

	}

	template <typename KeyType, typename ValueType>
	FORCEINLINE void in_register_sort64_kv_scalar(KeyType& a0, ValueType& b0, KeyType& a1, ValueType& b1, KeyType& a2, ValueType& b2, KeyType& a3, ValueType& b3, KeyType& a4, ValueType& b4, KeyType& a5, ValueType& b5, KeyType& a6, ValueType& b6, KeyType& a7, ValueType& b7, KeyType& a8, ValueType& b8, KeyType& a9, ValueType& b9, KeyType& a10, ValueType& b10, KeyType& a11, ValueType& b11, KeyType& a12, ValueType& b12, KeyType& a13, ValueType& b13, KeyType& a14, ValueType& b14, KeyType& a15, ValueType& b15, KeyType& a16, ValueType& b16, KeyType& a17, ValueType& b17, KeyType& a18, ValueType& b18, KeyType& a19, ValueType& b19, KeyType& a20, ValueType& b20, KeyType& a21, ValueType& b21, KeyType& a22, ValueType& b22, KeyType& a23, ValueType& b23, KeyType& a24, ValueType& b24, KeyType& a25, ValueType& b25, KeyType& a26, ValueType& b26, KeyType& a27, ValueType& b27, KeyType& a28, ValueType& b28, KeyType& a29, ValueType& b29, KeyType& a30, ValueType& b30, KeyType& a31, ValueType& b31, KeyType& a32, ValueType& b32, KeyType& a33, ValueType& b33, KeyType& a34, ValueType& b34, KeyType& a35, ValueType& b35, KeyType& a36, ValueType& b36, KeyType& a37, ValueType& b37, KeyType& a38, ValueType& b38, KeyType& a39, ValueType& b39, KeyType& a40, ValueType& b40, KeyType& a41, ValueType& b41, KeyType& a42, ValueType& b42, KeyType& a43, ValueType& b43, KeyType& a44, ValueType& b44, KeyType& a45, ValueType& b45, KeyType& a46, ValueType& b46, KeyType& a47, ValueType& b47, KeyType& a48, ValueType& b48, KeyType& a49, ValueType& b49, KeyType& a50, ValueType& b50, KeyType& a51, ValueType& b51, KeyType& a52, ValueType& b52, KeyType& a53, ValueType& b53, KeyType& a54, ValueType& b54, KeyType& a55, ValueType& b55, KeyType& a56, ValueType& b56, KeyType& a57, ValueType& b57, KeyType& a58, ValueType& b58, KeyType& a59, ValueType& b59, KeyType& a60, ValueType& b60, KeyType& a61, ValueType& b61, KeyType& a62, ValueType& b62, KeyType& a63, ValueType& b63) {
		SWAPKV2(0, 1);
		SWAPKV2(2, 3);
		SWAPKV2(0, 2);
		SWAPKV2(1, 3);
		SWAPKV2(1, 2);
		SWAPKV2(4, 5);
		SWAPKV2(6, 7);
		SWAPKV2(4, 6);
		SWAPKV2(5, 7);
		SWAPKV2(5, 6);
		SWAPKV2(0, 4);
		SWAPKV2(2, 6);
		SWAPKV2(2, 4);
		SWAPKV2(1, 5);
		SWAPKV2(3, 7);
		SWAPKV2(3, 5);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(8, 9);
		SWAPKV2(10, 11);
		SWAPKV2(8, 10);
		SWAPKV2(9, 11);
		SWAPKV2(9, 10);
		SWAPKV2(12, 13);
		SWAPKV2(14, 15);
		SWAPKV2(12, 14);
		SWAPKV2(13, 15);
		SWAPKV2(13, 14);
		SWAPKV2(8, 12);
		SWAPKV2(10, 14);
		SWAPKV2(10, 12);
		SWAPKV2(9, 13);
		SWAPKV2(11, 15);
		SWAPKV2(11, 13);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(0, 8);
		SWAPKV2(4, 12);
		SWAPKV2(4, 8);
		SWAPKV2(2, 10);
		SWAPKV2(6, 14);
		SWAPKV2(6, 10);
		SWAPKV2(2, 4);
		SWAPKV2(6, 8);
		SWAPKV2(10, 12);
		SWAPKV2(1, 9);
		SWAPKV2(5, 13);
		SWAPKV2(5, 9);
		SWAPKV2(3, 11);
		SWAPKV2(7, 15);
		SWAPKV2(7, 11);
		SWAPKV2(3, 5);
		SWAPKV2(7, 9);
		SWAPKV2(11, 13);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(16, 17);
		SWAPKV2(18, 19);
		SWAPKV2(16, 18);
		SWAPKV2(17, 19);
		SWAPKV2(17, 18);
		SWAPKV2(20, 21);
		SWAPKV2(22, 23);
		SWAPKV2(20, 22);
		SWAPKV2(21, 23);
		SWAPKV2(21, 22);
		SWAPKV2(16, 20);
		SWAPKV2(18, 22);
		SWAPKV2(18, 20);
		SWAPKV2(17, 21);
		SWAPKV2(19, 23);
		SWAPKV2(19, 21);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(24, 25);
		SWAPKV2(26, 27);
		SWAPKV2(24, 26);
		SWAPKV2(25, 27);
		SWAPKV2(25, 26);
		SWAPKV2(28, 29);
		SWAPKV2(30, 31);
		SWAPKV2(28, 30);
		SWAPKV2(29, 31);
		SWAPKV2(29, 30);
		SWAPKV2(24, 28);
		SWAPKV2(26, 30);
		SWAPKV2(26, 28);
		SWAPKV2(25, 29);
		SWAPKV2(27, 31);
		SWAPKV2(27, 29);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(16, 24);
		SWAPKV2(20, 28);
		SWAPKV2(20, 24);
		SWAPKV2(18, 26);
		SWAPKV2(22, 30);
		SWAPKV2(22, 26);
		SWAPKV2(18, 20);
		SWAPKV2(22, 24);
		SWAPKV2(26, 28);
		SWAPKV2(17, 25);
		SWAPKV2(21, 29);
		SWAPKV2(21, 25);
		SWAPKV2(19, 27);
		SWAPKV2(23, 31);
		SWAPKV2(23, 27);
		SWAPKV2(19, 21);
		SWAPKV2(23, 25);
		SWAPKV2(27, 29);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(0, 16);
		SWAPKV2(8, 24);
		SWAPKV2(8, 16);
		SWAPKV2(4, 20);
		SWAPKV2(12, 28);
		SWAPKV2(12, 20);
		SWAPKV2(4, 8);
		SWAPKV2(12, 16);
		SWAPKV2(20, 24);
		SWAPKV2(2, 18);
		SWAPKV2(10, 26);
		SWAPKV2(10, 18);
		SWAPKV2(6, 22);
		SWAPKV2(14, 30);
		SWAPKV2(14, 22);
		SWAPKV2(6, 10);
		SWAPKV2(14, 18);
		SWAPKV2(22, 26);
		SWAPKV2(2, 4);
		SWAPKV2(6, 8);
		SWAPKV2(10, 12);
		SWAPKV2(14, 16);
		SWAPKV2(18, 20);
		SWAPKV2(22, 24);
		SWAPKV2(26, 28);
		SWAPKV2(1, 17);
		SWAPKV2(9, 25);
		SWAPKV2(9, 17);
		SWAPKV2(5, 21);
		SWAPKV2(13, 29);
		SWAPKV2(13, 21);
		SWAPKV2(5, 9);
		SWAPKV2(13, 17);
		SWAPKV2(21, 25);
		SWAPKV2(3, 19);
		SWAPKV2(11, 27);
		SWAPKV2(11, 19);
		SWAPKV2(7, 23);
		SWAPKV2(15, 31);
		SWAPKV2(15, 23);
		SWAPKV2(7, 11);
		SWAPKV2(15, 19);
		SWAPKV2(23, 27);
		SWAPKV2(3, 5);
		SWAPKV2(7, 9);
		SWAPKV2(11, 13);
		SWAPKV2(15, 17);
		SWAPKV2(19, 21);
		SWAPKV2(23, 25);
		SWAPKV2(27, 29);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(15, 16);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(32, 33);
		SWAPKV2(34, 35);
		SWAPKV2(32, 34);
		SWAPKV2(33, 35);
		SWAPKV2(33, 34);
		SWAPKV2(36, 37);
		SWAPKV2(38, 39);
		SWAPKV2(36, 38);
		SWAPKV2(37, 39);
		SWAPKV2(37, 38);
		SWAPKV2(32, 36);
		SWAPKV2(34, 38);
		SWAPKV2(34, 36);
		SWAPKV2(33, 37);
		SWAPKV2(35, 39);
		SWAPKV2(35, 37);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(40, 41);
		SWAPKV2(42, 43);
		SWAPKV2(40, 42);
		SWAPKV2(41, 43);
		SWAPKV2(41, 42);
		SWAPKV2(44, 45);
		SWAPKV2(46, 47);
		SWAPKV2(44, 46);
		SWAPKV2(45, 47);
		SWAPKV2(45, 46);
		SWAPKV2(40, 44);
		SWAPKV2(42, 46);
		SWAPKV2(42, 44);
		SWAPKV2(41, 45);
		SWAPKV2(43, 47);
		SWAPKV2(43, 45);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(32, 40);
		SWAPKV2(36, 44);
		SWAPKV2(36, 40);
		SWAPKV2(34, 42);
		SWAPKV2(38, 46);
		SWAPKV2(38, 42);
		SWAPKV2(34, 36);
		SWAPKV2(38, 40);
		SWAPKV2(42, 44);
		SWAPKV2(33, 41);
		SWAPKV2(37, 45);
		SWAPKV2(37, 41);
		SWAPKV2(35, 43);
		SWAPKV2(39, 47);
		SWAPKV2(39, 43);
		SWAPKV2(35, 37);
		SWAPKV2(39, 41);
		SWAPKV2(43, 45);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(39, 40);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(48, 49);
		SWAPKV2(50, 51);
		SWAPKV2(48, 50);
		SWAPKV2(49, 51);
		SWAPKV2(49, 50);
		SWAPKV2(52, 53);
		SWAPKV2(54, 55);
		SWAPKV2(52, 54);
		SWAPKV2(53, 55);
		SWAPKV2(53, 54);
		SWAPKV2(48, 52);
		SWAPKV2(50, 54);
		SWAPKV2(50, 52);
		SWAPKV2(49, 53);
		SWAPKV2(51, 55);
		SWAPKV2(51, 53);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(56, 57);
		SWAPKV2(58, 59);
		SWAPKV2(56, 58);
		SWAPKV2(57, 59);
		SWAPKV2(57, 58);
		SWAPKV2(60, 61);
		SWAPKV2(62, 63);
		SWAPKV2(60, 62);
		SWAPKV2(61, 63);
		SWAPKV2(61, 62);
		SWAPKV2(56, 60);
		SWAPKV2(58, 62);
		SWAPKV2(58, 60);
		SWAPKV2(57, 61);
		SWAPKV2(59, 63);
		SWAPKV2(59, 61);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
		SWAPKV2(48, 56);
		SWAPKV2(52, 60);
		SWAPKV2(52, 56);
		SWAPKV2(50, 58);
		SWAPKV2(54, 62);
		SWAPKV2(54, 58);
		SWAPKV2(50, 52);
		SWAPKV2(54, 56);
		SWAPKV2(58, 60);
		SWAPKV2(49, 57);
		SWAPKV2(53, 61);
		SWAPKV2(53, 57);
		SWAPKV2(51, 59);
		SWAPKV2(55, 63);
		SWAPKV2(55, 59);
		SWAPKV2(51, 53);
		SWAPKV2(55, 57);
		SWAPKV2(59, 61);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(55, 56);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
		SWAPKV2(32, 48);
		SWAPKV2(40, 56);
		SWAPKV2(40, 48);
		SWAPKV2(36, 52);
		SWAPKV2(44, 60);
		SWAPKV2(44, 52);
		SWAPKV2(36, 40);
		SWAPKV2(44, 48);
		SWAPKV2(52, 56);
		SWAPKV2(34, 50);
		SWAPKV2(42, 58);
		SWAPKV2(42, 50);
		SWAPKV2(38, 54);
		SWAPKV2(46, 62);
		SWAPKV2(46, 54);
		SWAPKV2(38, 42);
		SWAPKV2(46, 50);
		SWAPKV2(54, 58);
		SWAPKV2(34, 36);
		SWAPKV2(38, 40);
		SWAPKV2(42, 44);
		SWAPKV2(46, 48);
		SWAPKV2(50, 52);
		SWAPKV2(54, 56);
		SWAPKV2(58, 60);
		SWAPKV2(33, 49);
		SWAPKV2(41, 57);
		SWAPKV2(41, 49);
		SWAPKV2(37, 53);
		SWAPKV2(45, 61);
		SWAPKV2(45, 53);
		SWAPKV2(37, 41);
		SWAPKV2(45, 49);
		SWAPKV2(53, 57);
		SWAPKV2(35, 51);
		SWAPKV2(43, 59);
		SWAPKV2(43, 51);
		SWAPKV2(39, 55);
		SWAPKV2(47, 63);
		SWAPKV2(47, 55);
		SWAPKV2(39, 43);
		SWAPKV2(47, 51);
		SWAPKV2(55, 59);
		SWAPKV2(35, 37);
		SWAPKV2(39, 41);
		SWAPKV2(43, 45);
		SWAPKV2(47, 49);
		SWAPKV2(51, 53);
		SWAPKV2(55, 57);
		SWAPKV2(59, 61);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(39, 40);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(47, 48);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(55, 56);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
		SWAPKV2(0, 32);
		SWAPKV2(16, 48);
		SWAPKV2(16, 32);
		SWAPKV2(8, 40);
		SWAPKV2(24, 56);
		SWAPKV2(24, 40);
		SWAPKV2(8, 16);
		SWAPKV2(24, 32);
		SWAPKV2(40, 48);
		SWAPKV2(4, 36);
		SWAPKV2(20, 52);
		SWAPKV2(20, 36);
		SWAPKV2(12, 44);
		SWAPKV2(28, 60);
		SWAPKV2(28, 44);
		SWAPKV2(12, 20);
		SWAPKV2(28, 36);
		SWAPKV2(44, 52);
		SWAPKV2(4, 8);
		SWAPKV2(12, 16);
		SWAPKV2(20, 24);
		SWAPKV2(28, 32);
		SWAPKV2(36, 40);
		SWAPKV2(44, 48);
		SWAPKV2(52, 56);
		SWAPKV2(2, 34);
		SWAPKV2(18, 50);
		SWAPKV2(18, 34);
		SWAPKV2(10, 42);
		SWAPKV2(26, 58);
		SWAPKV2(26, 42);
		SWAPKV2(10, 18);
		SWAPKV2(26, 34);
		SWAPKV2(42, 50);
		SWAPKV2(6, 38);
		SWAPKV2(22, 54);
		SWAPKV2(22, 38);
		SWAPKV2(14, 46);
		SWAPKV2(30, 62);
		SWAPKV2(30, 46);
		SWAPKV2(14, 22);
		SWAPKV2(30, 38);
		SWAPKV2(46, 54);
		SWAPKV2(6, 10);
		SWAPKV2(14, 18);
		SWAPKV2(22, 26);
		SWAPKV2(30, 34);
		SWAPKV2(38, 42);
		SWAPKV2(46, 50);
		SWAPKV2(54, 58);
		SWAPKV2(2, 4);
		SWAPKV2(6, 8);
		SWAPKV2(10, 12);
		SWAPKV2(14, 16);
		SWAPKV2(18, 20);
		SWAPKV2(22, 24);
		SWAPKV2(26, 28);
		SWAPKV2(30, 32);
		SWAPKV2(34, 36);
		SWAPKV2(38, 40);
		SWAPKV2(42, 44);
		SWAPKV2(46, 48);
		SWAPKV2(50, 52);
		SWAPKV2(54, 56);
		SWAPKV2(58, 60);
		SWAPKV2(1, 33);
		SWAPKV2(17, 49);
		SWAPKV2(17, 33);
		SWAPKV2(9, 41);
		SWAPKV2(25, 57);
		SWAPKV2(25, 41);
		SWAPKV2(9, 17);
		SWAPKV2(25, 33);
		SWAPKV2(41, 49);
		SWAPKV2(5, 37);
		SWAPKV2(21, 53);
		SWAPKV2(21, 37);
		SWAPKV2(13, 45);
		SWAPKV2(29, 61);
		SWAPKV2(29, 45);
		SWAPKV2(13, 21);
		SWAPKV2(29, 37);
		SWAPKV2(45, 53);
		SWAPKV2(5, 9);
		SWAPKV2(13, 17);
		SWAPKV2(21, 25);
		SWAPKV2(29, 33);
		SWAPKV2(37, 41);
		SWAPKV2(45, 49);
		SWAPKV2(53, 57);
		SWAPKV2(3, 35);
		SWAPKV2(19, 51);
		SWAPKV2(19, 35);
		SWAPKV2(11, 43);
		SWAPKV2(27, 59);
		SWAPKV2(27, 43);
		SWAPKV2(11, 19);
		SWAPKV2(27, 35);
		SWAPKV2(43, 51);
		SWAPKV2(7, 39);
		SWAPKV2(23, 55);
		SWAPKV2(23, 39);
		SWAPKV2(15, 47);
		SWAPKV2(31, 63);
		SWAPKV2(31, 47);
		SWAPKV2(15, 23);
		SWAPKV2(31, 39);
		SWAPKV2(47, 55);
		SWAPKV2(7, 11);
		SWAPKV2(15, 19);
		SWAPKV2(23, 27);
		SWAPKV2(31, 35);
		SWAPKV2(39, 43);
		SWAPKV2(47, 51);
		SWAPKV2(55, 59);
		SWAPKV2(3, 5);
		SWAPKV2(7, 9);
		SWAPKV2(11, 13);
		SWAPKV2(15, 17);
		SWAPKV2(19, 21);
		SWAPKV2(23, 25);
		SWAPKV2(27, 29);
		SWAPKV2(31, 33);
		SWAPKV2(35, 37);
		SWAPKV2(39, 41);
		SWAPKV2(43, 45);
		SWAPKV2(47, 49);
		SWAPKV2(51, 53);
		SWAPKV2(55, 57);
		SWAPKV2(59, 61);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(15, 16);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(31, 32);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(39, 40);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(47, 48);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(55, 56);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_transpose2(Reg& a0, Reg& a1) {
		ReportError("Not implemented");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_transpose4(Reg& a0, Reg& a1, Reg& a2, Reg& a3) {
		origami_utils::transpose<Reg, Item>(a0, a1, a2, a3);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_transpose8(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7) {
		origami_utils::transpose<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void in_register_transpose16(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		ReportError("Not implemented");
	}


	// ----------------- In-register merge with rswap 
	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1) {
		origami_utils::rswap<Reg, Item>(a0, a1);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3) {
		origami_utils::rswap<Reg, Item>(a0, a2); origami_utils::rswap<Reg, Item>(a1, a3);
		origami_utils::rswap<Reg, Item>(a1, a2);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7) {
		origami_utils::rswap<Reg, Item>(a0, a4); origami_utils::rswap<Reg, Item>(a1, a5); origami_utils::rswap<Reg, Item>(a2, a6); origami_utils::rswap<Reg, Item>(a3, a7);
		origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a3, a5);
		origami_utils::rswap<Reg, Item>(a1, a2); origami_utils::rswap<Reg, Item>(a3, a4); origami_utils::rswap<Reg, Item>(a5, a6);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		origami_utils::rswap<Reg, Item>(a0, a8); origami_utils::rswap<Reg, Item>(a1, a9); origami_utils::rswap<Reg, Item>(a2, a10); origami_utils::rswap<Reg, Item>(a3, a11); origami_utils::rswap<Reg, Item>(a4, a12); origami_utils::rswap<Reg, Item>(a5, a13); origami_utils::rswap<Reg, Item>(a6, a14); origami_utils::rswap<Reg, Item>(a7, a15);
		origami_utils::rswap<Reg, Item>(a4, a8); origami_utils::rswap<Reg, Item>(a5, a9); origami_utils::rswap<Reg, Item>(a6, a10); origami_utils::rswap<Reg, Item>(a7, a11);
		origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a3, a5); origami_utils::rswap<Reg, Item>(a6, a8); origami_utils::rswap<Reg, Item>(a7, a9); origami_utils::rswap<Reg, Item>(a10, a12); origami_utils::rswap<Reg, Item>(a11, a13);
		origami_utils::rswap<Reg, Item>(a1, a2); origami_utils::rswap<Reg, Item>(a3, a4); origami_utils::rswap<Reg, Item>(a5, a6); origami_utils::rswap<Reg, Item>(a7, a8); origami_utils::rswap<Reg, Item>(a9, a10); origami_utils::rswap<Reg, Item>(a11, a12); origami_utils::rswap<Reg, Item>(a13, a14);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {

		origami_utils::rswap<Reg, Item>(a0, a16); origami_utils::rswap<Reg, Item>(a1, a17); origami_utils::rswap<Reg, Item>(a2, a18); origami_utils::rswap<Reg, Item>(a3, a19); origami_utils::rswap<Reg, Item>(a4, a20); origami_utils::rswap<Reg, Item>(a5, a21); origami_utils::rswap<Reg, Item>(a6, a22); origami_utils::rswap<Reg, Item>(a7, a23); origami_utils::rswap<Reg, Item>(a8, a24); origami_utils::rswap<Reg, Item>(a9, a25); origami_utils::rswap<Reg, Item>(a10, a26); origami_utils::rswap<Reg, Item>(a11, a27); origami_utils::rswap<Reg, Item>(a12, a28); origami_utils::rswap<Reg, Item>(a13, a29); origami_utils::rswap<Reg, Item>(a14, a30); origami_utils::rswap<Reg, Item>(a15, a31);

		origami_utils::rswap<Reg, Item>(a8, a16); origami_utils::rswap<Reg, Item>(a9, a17); origami_utils::rswap<Reg, Item>(a10, a18); origami_utils::rswap<Reg, Item>(a11, a19); origami_utils::rswap<Reg, Item>(a12, a20); origami_utils::rswap<Reg, Item>(a13, a21); origami_utils::rswap<Reg, Item>(a14, a22); origami_utils::rswap<Reg, Item>(a15, a23);

		origami_utils::rswap<Reg, Item>(a4, a8); origami_utils::rswap<Reg, Item>(a5, a9); origami_utils::rswap<Reg, Item>(a6, a10); origami_utils::rswap<Reg, Item>(a7, a11); origami_utils::rswap<Reg, Item>(a12, a16); origami_utils::rswap<Reg, Item>(a13, a17); origami_utils::rswap<Reg, Item>(a14, a18); origami_utils::rswap<Reg, Item>(a15, a19); origami_utils::rswap<Reg, Item>(a20, a24); origami_utils::rswap<Reg, Item>(a21, a25); origami_utils::rswap<Reg, Item>(a22, a26); origami_utils::rswap<Reg, Item>(a23, a27);

		origami_utils::rswap<Reg, Item>(a2, a4); origami_utils::rswap<Reg, Item>(a3, a5); origami_utils::rswap<Reg, Item>(a6, a8); origami_utils::rswap<Reg, Item>(a7, a9); origami_utils::rswap<Reg, Item>(a10, a12); origami_utils::rswap<Reg, Item>(a11, a13); origami_utils::rswap<Reg, Item>(a14, a16); origami_utils::rswap<Reg, Item>(a15, a17); origami_utils::rswap<Reg, Item>(a18, a20); origami_utils::rswap<Reg, Item>(a19, a21); origami_utils::rswap<Reg, Item>(a22, a24); origami_utils::rswap<Reg, Item>(a23, a25); origami_utils::rswap<Reg, Item>(a26, a28); origami_utils::rswap<Reg, Item>(a27, a29);

		origami_utils::rswap<Reg, Item>(a1, a2); origami_utils::rswap<Reg, Item>(a3, a4); origami_utils::rswap<Reg, Item>(a5, a6); origami_utils::rswap<Reg, Item>(a7, a8); origami_utils::rswap<Reg, Item>(a9, a10); origami_utils::rswap<Reg, Item>(a11, a12); origami_utils::rswap<Reg, Item>(a13, a14); origami_utils::rswap<Reg, Item>(a15, a16); origami_utils::rswap<Reg, Item>(a17, a18); origami_utils::rswap<Reg, Item>(a19, a20); origami_utils::rswap<Reg, Item>(a21, a22);  origami_utils::rswap<Reg, Item>(a23, a24); origami_utils::rswap<Reg, Item>(a25, a26); origami_utils::rswap<Reg, Item>(a27, a28); origami_utils::rswap<Reg, Item>(a29, a30);
	}


	
	// ----------------------
	// in-register merges

	// in register merge with mcmerge
	// 16 registers
	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge8x8(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge8x8(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		origami_utils::cswap<Reg, Item>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge16x16(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		constexpr ui Itemsize = sizeof(Item) << 3;
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a0, a8);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a1, a9);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a2, a10);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a3, a11);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a4, a12);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a5, a13);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a6, a14);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a7, a15);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

		origami_utils::shuffle<Reg, Itemsize>(a8);	origami_utils::shuffle<Reg, Itemsize>(a9);
		origami_utils::shuffle<Reg, Itemsize>(a10);	origami_utils::shuffle<Reg, Itemsize>(a11);
		origami_utils::shuffle<Reg, Itemsize>(a12);	origami_utils::shuffle<Reg, Itemsize>(a13);
		origami_utils::shuffle<Reg, Itemsize>(a14);	origami_utils::shuffle<Reg, Itemsize>(a15);
		
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

		origami_utils::shuffle<Reg, Itemsize>(a8);	origami_utils::shuffle<Reg, Itemsize>(a9);
		origami_utils::shuffle<Reg, Itemsize>(a10);	origami_utils::shuffle<Reg, Itemsize>(a11);
		origami_utils::shuffle<Reg, Itemsize>(a12);	origami_utils::shuffle<Reg, Itemsize>(a13);
		origami_utils::shuffle<Reg, Itemsize>(a14);	origami_utils::shuffle<Reg, Itemsize>(a15);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge16x16(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge32x32(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		constexpr ui Itemsize = sizeof(Item) << 3;

		constexpr ui l1_bits = Itemsize << 1;
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a0, a8);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a1, a9);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a2, a10);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a3, a11);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a4, a12);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a5, a13);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a6, a14);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a7, a15);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

		origami_utils::shuffle<Reg, l1_bits>(a8);	origami_utils::shuffle<Reg, l1_bits>(a9);
		origami_utils::shuffle<Reg, l1_bits>(a10);	origami_utils::shuffle<Reg, l1_bits>(a11);
		origami_utils::shuffle<Reg, l1_bits>(a12);	origami_utils::shuffle<Reg, l1_bits>(a13);
		origami_utils::shuffle<Reg, l1_bits>(a14);	origami_utils::shuffle<Reg, l1_bits>(a15);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

		origami_utils::mn8_level2_shuffle<Reg, Item>(a0, a8);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a1, a9);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a2, a10);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a3, a11);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a4, a12);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a5, a13);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a6, a14);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a7, a15);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge32x32(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		constexpr ui Itemsize = sizeof(Item) << 3;
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a0, a16);	origami_utils::diag_exchange<Reg, Item, Itemsize>(a1, a17);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a2, a18);	origami_utils::diag_exchange<Reg, Item, Itemsize>(a3, a19);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a4, a20);	origami_utils::diag_exchange<Reg, Item, Itemsize>(a5, a21);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a6, a22);	origami_utils::diag_exchange<Reg, Item, Itemsize>(a7, a23);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a8, a24);	origami_utils::diag_exchange<Reg, Item, Itemsize>(a9, a25);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a10, a26); origami_utils::diag_exchange<Reg, Item, Itemsize>(a11, a27);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a12, a28); origami_utils::diag_exchange<Reg, Item, Itemsize>(a13, a29);
		origami_utils::diag_exchange<Reg, Item, Itemsize>(a14, a30); origami_utils::diag_exchange<Reg, Item, Itemsize>(a15, a31);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		origami_utils::shuffle<Reg, Itemsize>(a16);	origami_utils::shuffle<Reg, Itemsize>(a17);
		origami_utils::shuffle<Reg, Itemsize>(a18);	origami_utils::shuffle<Reg, Itemsize>(a19);
		origami_utils::shuffle<Reg, Itemsize>(a20);	origami_utils::shuffle<Reg, Itemsize>(a21);
		origami_utils::shuffle<Reg, Itemsize>(a22);	origami_utils::shuffle<Reg, Itemsize>(a23);
		origami_utils::shuffle<Reg, Itemsize>(a24);	origami_utils::shuffle<Reg, Itemsize>(a25);
		origami_utils::shuffle<Reg, Itemsize>(a26);	origami_utils::shuffle<Reg, Itemsize>(a27);
		origami_utils::shuffle<Reg, Itemsize>(a28);	origami_utils::shuffle<Reg, Itemsize>(a29);
		origami_utils::shuffle<Reg, Itemsize>(a30);	origami_utils::shuffle<Reg, Itemsize>(a31);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		origami_utils::shuffle<Reg, Itemsize>(a16);	origami_utils::shuffle<Reg, Itemsize>(a17);
		origami_utils::shuffle<Reg, Itemsize>(a18);	origami_utils::shuffle<Reg, Itemsize>(a19);
		origami_utils::shuffle<Reg, Itemsize>(a20);	origami_utils::shuffle<Reg, Itemsize>(a21);
		origami_utils::shuffle<Reg, Itemsize>(a22);	origami_utils::shuffle<Reg, Itemsize>(a23);
		origami_utils::shuffle<Reg, Itemsize>(a24);	origami_utils::shuffle<Reg, Itemsize>(a25);
		origami_utils::shuffle<Reg, Itemsize>(a26);	origami_utils::shuffle<Reg, Itemsize>(a27);
		origami_utils::shuffle<Reg, Itemsize>(a28);	origami_utils::shuffle<Reg, Itemsize>(a29);
		origami_utils::shuffle<Reg, Itemsize>(a30);	origami_utils::shuffle<Reg, Itemsize>(a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge64x64(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		if constexpr (sizeof(Reg) * 16 < sizeof(Item) * 128)
			ReportError("mcmerge64x64: invalid register and item type");
		
		// NOTE: valid only for AVX2 + 32, AVX512 + 64 + 128

		constexpr ui Itemsize = sizeof(Item) << 3;	// in bits
		
		/*
		Network:
		(0, 8); (1, 9); (2, 10); (3, 11); (4, 12); (5, 13); (6, 14); (7, 15);
		(4, 8); (5, 9); (6, 10); (7, 11);
		(2, 4); (3, 5); (6, 8); (7, 9); (10, 12); (11, 13);
		(1, 2); (3, 4); (5, 6); (7, 8); (9, 10); (11, 12); (13, 14);

		After mcmerge32x32; our matrix is in the form:
		a1 b1 d1 f1 a2 b2 d2 f2
		h1 c1 e1 g1 h2 c2 e2 g2
		For the first level of MN16 we want it to be:
		a1 b1 c1 d1 e1 f1 g1 h1
		a2 b2 c2 d2 e2 f2 g2 h2
		or any combination where 1s can be compared with 2s
		*/

		// Step 1: permute to form the following matrix
		// a1 b1 d1 f1 h1 c1 e1 g1
		// a2 b2 d2 f2 h2 c2 e2 g2
		constexpr ui l1_bits = Itemsize << 2;
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a0, a8);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a1, a9);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a2, a10);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a3, a11);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a4, a12);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a5, a13);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a6, a14);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a7, a15);

		// Step 2: first level of MN16
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		
		// Step 3: permute for 2nd level of SN16
		// we want to compare: (4, 8), (5, 9), (6, 10), (7, 11)
		// in this case: (e1, a2), (f1, b2), (g1, c2), (h1, d2)
		// we have:
		// a1 b1 d1 f1 h1 c1 e1 g1
		// a2 b2 d2 f2 h2 c2 e2 g2
		// we want:
		// a1 b1 d1 f1 h1 c1 e1 g1
		// f2 h2 e2 b2 d2 g2 a2 c2 
		// we could permute a1 b1 .. g1 h1; but that would require 8 more permutes
		origami_utils::mn16_level1_shuffle<Reg, Item>(a8);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a9);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a10);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a11);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a12);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a13);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a14);
		origami_utils::mn16_level1_shuffle<Reg, Item>(a15);

		// Step 4: second level of MN16
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

		// Step 5: permute for 3rd level of SN16
		/*
		(2, 4); (3, 5); (6, 8); (7, 9); (10, 12); (11, 13);
		We have:
		a1 b1 d1 f1 h1 c1 e1 g1
		f2 h2 e2 b2 d2 g2 a2 c2
		We want:
		a1 b1 g1 h1 c1 d1 c2 d2
		g2 h2 a2 b2 e1 f1 e2 f2
		--> we can get there by a combination of permute and shuffle
		*/
		origami_utils::mn16_level2_shuffle<Reg, Item>(a0, a8);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a1, a9);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a2, a10);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a3, a11);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a4, a12);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a5, a13);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a6, a14);
		origami_utils::mn16_level2_shuffle<Reg, Item>(a7, a15);

		// Step 6: third level of MN16
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

		// Step 7: permute for 4th level of SN16 (MN16)
		/*
		(1, 2); (3, 4); (5, 6); (7, 8); (9, 10); (11, 12); (13, 14);
		In this case: (b1, c1), (d1, e1), (f1, g1), (h1, a2), (b2, c2), (d2, e2), (f2, g2)
		We have:
		a1 b1 g1 h1 c1 d1 c2 d2
		g2 h2 a2 b2 e1 f1 e2 f2
		We want:
		a1 b1 d1 f1 h1 b2 d2 f2
		h2 c1 e1 g1 a2 c2 e2 g2
		*/
		// 6 instr -> 4 permutes, 2 blends
		origami_utils::mn16_level3_shuffle<Reg, Item>(a0, a8);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a1, a9);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a2, a10);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a3, a11);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a4, a12);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a5, a13);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a6, a14);
		origami_utils::mn16_level3_shuffle<Reg, Item>(a7, a15);

		// Step 8: fourth and last level of MN16
		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);

	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge64x64(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		constexpr ui Itemsize = sizeof(Item) << 3;

		constexpr ui l1_bits = Itemsize << 1;
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a0, a16); origami_utils::diag_exchange<Reg, Item, l1_bits>(a1, a17);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a2, a18); origami_utils::diag_exchange<Reg, Item, l1_bits>(a3, a19);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a4, a20); origami_utils::diag_exchange<Reg, Item, l1_bits>(a5, a21);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a6, a22); origami_utils::diag_exchange<Reg, Item, l1_bits>(a7, a23);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a8, a24); origami_utils::diag_exchange<Reg, Item, l1_bits>(a9, a25);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a10, a26); origami_utils::diag_exchange<Reg, Item, l1_bits>(a11, a27);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a12, a28); origami_utils::diag_exchange<Reg, Item, l1_bits>(a13, a29);
		origami_utils::diag_exchange<Reg, Item, l1_bits>(a14, a30); origami_utils::diag_exchange<Reg, Item, l1_bits>(a15, a31);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		origami_utils::shuffle<Reg, l1_bits>(a16); origami_utils::shuffle<Reg, l1_bits>(a17);
		origami_utils::shuffle<Reg, l1_bits>(a18); origami_utils::shuffle<Reg, l1_bits>(a19);
		origami_utils::shuffle<Reg, l1_bits>(a20); origami_utils::shuffle<Reg, l1_bits>(a21);
		origami_utils::shuffle<Reg, l1_bits>(a22); origami_utils::shuffle<Reg, l1_bits>(a23);
		origami_utils::shuffle<Reg, l1_bits>(a24); origami_utils::shuffle<Reg, l1_bits>(a25);
		origami_utils::shuffle<Reg, l1_bits>(a26); origami_utils::shuffle<Reg, l1_bits>(a27);
		origami_utils::shuffle<Reg, l1_bits>(a28); origami_utils::shuffle<Reg, l1_bits>(a29);
		origami_utils::shuffle<Reg, l1_bits>(a30); origami_utils::shuffle<Reg, l1_bits>(a31);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		origami_utils::mn8_level2_shuffle<Reg, Item>(a0, a16); origami_utils::mn8_level2_shuffle<Reg, Item>(a1, a17);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a2, a18); origami_utils::mn8_level2_shuffle<Reg, Item>(a3, a19);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a4, a20); origami_utils::mn8_level2_shuffle<Reg, Item>(a5, a21);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a6, a22); origami_utils::mn8_level2_shuffle<Reg, Item>(a7, a23);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a8, a24); origami_utils::mn8_level2_shuffle<Reg, Item>(a9, a25);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a10, a26); origami_utils::mn8_level2_shuffle<Reg, Item>(a11, a27);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a12, a28); origami_utils::mn8_level2_shuffle<Reg, Item>(a13, a29);
		origami_utils::mn8_level2_shuffle<Reg, Item>(a14, a30); origami_utils::mn8_level2_shuffle<Reg, Item>(a15, a31);

		origami_utils::cswap<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mcmerge128x128(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		ReportError("mcmerge128x128: not implemented");
		
		if constexpr (sizeof(Reg) * 16 < sizeof(Item) * 256)
			ReportError("mcmerge128x128: invalid register and item type");
	}


	// in register merge with mrmerge
	// 16 registers

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge_sort_reg(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		// sort registers
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			if constexpr (std::is_same<Reg, avx512>::value) {
				avx512_utils::sort_reg<Item, Reg>(a0); avx512_utils::sort_reg<Item, Reg>(a1);
				avx512_utils::sort_reg<Item, Reg>(a2); avx512_utils::sort_reg<Item, Reg>(a3);
				avx512_utils::sort_reg<Item, Reg>(a4); avx512_utils::sort_reg<Item, Reg>(a5);
				avx512_utils::sort_reg<Item, Reg>(a6); avx512_utils::sort_reg<Item, Reg>(a7);
				avx512_utils::sort_reg<Item, Reg>(a8); avx512_utils::sort_reg<Item, Reg>(a9);
				avx512_utils::sort_reg<Item, Reg>(a10); avx512_utils::sort_reg<Item, Reg>(a11);
				avx512_utils::sort_reg<Item, Reg>(a12); avx512_utils::sort_reg<Item, Reg>(a13);
				avx512_utils::sort_reg<Item, Reg>(a14); avx512_utils::sort_reg<Item, Reg>(a15);
			}
			else {
				// transpose and csort --> note transpose depends on the Items per register since we use it to sort each register
				constexpr ui items_per_reg = sizeof(Reg) / sizeof(Item);
				origami_utils::transpose_large<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
				if constexpr (items_per_reg == 2) {

					/*origami_utils::transpose<Item, Reg>(a0, a1);
					origami_utils::transpose<Item, Reg>(a2, a3);
					origami_utils::transpose<Item, Reg>(a4, a5);
					origami_utils::transpose<Item, Reg>(a6, a7);
					origami_utils::transpose<Item, Reg>(a8, a9);
					origami_utils::transpose<Item, Reg>(a10, a11);
					origami_utils::transpose<Item, Reg>(a12, a13);
					origami_utils::transpose<Item, Reg>(a14, a15);*/
					// sort column --> just one swap
					origami_utils::swap<Reg, Item>(a0, a1);
					origami_utils::swap<Reg, Item>(a2, a3);
					origami_utils::swap<Reg, Item>(a4, a5);
					origami_utils::swap<Reg, Item>(a6, a7);
					origami_utils::swap<Reg, Item>(a8, a9);
					origami_utils::swap<Reg, Item>(a10, a11);
					origami_utils::swap<Reg, Item>(a12, a13);
					origami_utils::swap<Reg, Item>(a14, a15);
				}
				else if constexpr (items_per_reg == 4) {
					origami_sorter::in_register_sort<Item, Reg>(a0, a1, a2, a3);
					origami_sorter::in_register_sort<Item, Reg>(a4, a5, a6, a7);
					origami_sorter::in_register_sort<Item, Reg>(a8, a9, a10, a11);
					origami_sorter::in_register_sort<Item, Reg>(a12, a13, a14, a15);
				}
				else if constexpr (items_per_reg == 8) {
					origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
				}
				else if constexpr (items_per_reg == 16) {
					origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
				}
				else ReportError("mrmerge_sort_reg: items per reg not supported");
				origami_utils::transpose_large<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
			}
		}
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge_sort_reg(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		// sort registers
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			if constexpr (std::is_same<Reg, avx512>::value) {
				avx512_utils::sort_reg<Item, Reg>(a0); avx512_utils::sort_reg<Item, Reg>(a1);
				avx512_utils::sort_reg<Item, Reg>(a2); avx512_utils::sort_reg<Item, Reg>(a3);
				avx512_utils::sort_reg<Item, Reg>(a4); avx512_utils::sort_reg<Item, Reg>(a5);
				avx512_utils::sort_reg<Item, Reg>(a6); avx512_utils::sort_reg<Item, Reg>(a7);
				avx512_utils::sort_reg<Item, Reg>(a8); avx512_utils::sort_reg<Item, Reg>(a9);
				avx512_utils::sort_reg<Item, Reg>(a10); avx512_utils::sort_reg<Item, Reg>(a11);
				avx512_utils::sort_reg<Item, Reg>(a12); avx512_utils::sort_reg<Item, Reg>(a13);
				avx512_utils::sort_reg<Item, Reg>(a14); avx512_utils::sort_reg<Item, Reg>(a15);
				avx512_utils::sort_reg<Item, Reg>(a16); avx512_utils::sort_reg<Item, Reg>(a17);
				avx512_utils::sort_reg<Item, Reg>(a18); avx512_utils::sort_reg<Item, Reg>(a19);
				avx512_utils::sort_reg<Item, Reg>(a20); avx512_utils::sort_reg<Item, Reg>(a21);
				avx512_utils::sort_reg<Item, Reg>(a22); avx512_utils::sort_reg<Item, Reg>(a23);
				avx512_utils::sort_reg<Item, Reg>(a24); avx512_utils::sort_reg<Item, Reg>(a25);
				avx512_utils::sort_reg<Item, Reg>(a26); avx512_utils::sort_reg<Item, Reg>(a27);
				avx512_utils::sort_reg<Item, Reg>(a28); avx512_utils::sort_reg<Item, Reg>(a29);
				avx512_utils::sort_reg<Item, Reg>(a30); avx512_utils::sort_reg<Item, Reg>(a31);
			}
			else {
				// transpose and csort --> note transpose depends on the Items per register since we use it to sort each register
				constexpr ui items_per_reg = sizeof(Reg) / sizeof(Item);
				origami_utils::transpose_large<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				if constexpr (items_per_reg == 2) {
					// sort column --> just one swap
					origami_utils::swap<Reg, Item>(a0, a1); origami_utils::swap<Reg, Item>(a2, a3);
					origami_utils::swap<Reg, Item>(a4, a5); origami_utils::swap<Reg, Item>(a6, a7);
					origami_utils::swap<Reg, Item>(a8, a9); origami_utils::swap<Reg, Item>(a10, a11);
					origami_utils::swap<Reg, Item>(a12, a13); origami_utils::swap<Reg, Item>(a14, a15);
					origami_utils::swap<Reg, Item>(a16, a17); origami_utils::swap<Reg, Item>(a18, a19);
					origami_utils::swap<Reg, Item>(a20, a21); origami_utils::swap<Reg, Item>(a22, a23);
					origami_utils::swap<Reg, Item>(a24, a25); origami_utils::swap<Reg, Item>(a26, a27);
					origami_utils::swap<Reg, Item>(a28, a29); origami_utils::swap<Reg, Item>(a30, a31);
				}
				else if constexpr (items_per_reg == 4) {
					origami_sorter::in_register_sort<Item, Reg>(a0, a1, a2, a3);	origami_sorter::in_register_sort<Item, Reg>(a4, a5, a6, a7);
					origami_sorter::in_register_sort<Item, Reg>(a8, a9, a10, a11);	origami_sorter::in_register_sort<Item, Reg>(a12, a13, a14, a15);
					origami_sorter::in_register_sort<Item, Reg>(a16, a17, a18, a19);	origami_sorter::in_register_sort<Item, Reg>(a20, a21, a22, a23);
					origami_sorter::in_register_sort<Item, Reg>(a24, a25, a26, a27);	origami_sorter::in_register_sort<Item, Reg>(a28, a29, a30, a31);
				}
				else if constexpr (items_per_reg == 8) {
					origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::in_register_sort8<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
					origami_sorter::in_register_sort8<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);
				}
				else if constexpr (items_per_reg == 16) {
					origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					origami_sorter::in_register_sort16<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				}
				else ReportError("mrmerge_sort_reg: items per reg not supported");
				origami_utils::transpose_large<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
			}
		}
	}


	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge2R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a1); origami_utils::reverse<Reg, Item>(a3);
			origami_utils::reverse<Reg, Item>(a5); origami_utils::reverse<Reg, Item>(a7);
			origami_utils::reverse<Reg, Item>(a9); origami_utils::reverse<Reg, Item>(a11);
			origami_utils::reverse<Reg, Item>(a13); origami_utils::reverse<Reg, Item>(a15);
		}
		
		// run swaps
		origami_utils::swap<Reg, Item>(a0, a1);
		origami_utils::swap<Reg, Item>(a2, a3);
		origami_utils::swap<Reg, Item>(a4, a5);
		origami_utils::swap<Reg, Item>(a6, a7);
		origami_utils::swap<Reg, Item>(a8, a9);
		origami_utils::swap<Reg, Item>(a10, a11);
		origami_utils::swap<Reg, Item>(a12, a13);
		origami_utils::swap<Reg, Item>(a14, a15);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge2R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15, 
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a1); origami_utils::reverse<Reg, Item>(a3);
			origami_utils::reverse<Reg, Item>(a5); origami_utils::reverse<Reg, Item>(a7);
			origami_utils::reverse<Reg, Item>(a9); origami_utils::reverse<Reg, Item>(a11);
			origami_utils::reverse<Reg, Item>(a13); origami_utils::reverse<Reg, Item>(a15);
			origami_utils::reverse<Reg, Item>(a17); origami_utils::reverse<Reg, Item>(a19);
			origami_utils::reverse<Reg, Item>(a21); origami_utils::reverse<Reg, Item>(a23);
			origami_utils::reverse<Reg, Item>(a25); origami_utils::reverse<Reg, Item>(a27);
			origami_utils::reverse<Reg, Item>(a29); origami_utils::reverse<Reg, Item>(a31);
		}

		// run swaps
		origami_utils::swap<Reg, Item>(a0, a1); origami_utils::swap<Reg, Item>(a2, a3);
		origami_utils::swap<Reg, Item>(a4, a5); origami_utils::swap<Reg, Item>(a6, a7);
		origami_utils::swap<Reg, Item>(a8, a9); origami_utils::swap<Reg, Item>(a10, a11);
		origami_utils::swap<Reg, Item>(a12, a13); origami_utils::swap<Reg, Item>(a14, a15);
		origami_utils::swap<Reg, Item>(a16, a17); origami_utils::swap<Reg, Item>(a18, a19);
		origami_utils::swap<Reg, Item>(a20, a21); origami_utils::swap<Reg, Item>(a22, a23);
		origami_utils::swap<Reg, Item>(a24, a25); origami_utils::swap<Reg, Item>(a26, a27);
		origami_utils::swap<Reg, Item>(a28, a29); origami_utils::swap<Reg, Item>(a30, a31);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge4R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a2); origami_utils::reverse<Reg, Item>(a3);
			origami_utils::reverse<Reg, Item>(a6); origami_utils::reverse<Reg, Item>(a7);
			origami_utils::reverse<Reg, Item>(a10); origami_utils::reverse<Reg, Item>(a11);
			origami_utils::reverse<Reg, Item>(a14); origami_utils::reverse<Reg, Item>(a15);
		}

		// run swaps 
		origami_utils::swap<Reg, Item>(a0, a2);  origami_utils::swap<Reg, Item>(a1, a3);  origami_utils::swap<Reg, Item>(a1, a2); 
		origami_utils::swap<Reg, Item>(a4, a6);  origami_utils::swap<Reg, Item>(a5, a7);  origami_utils::swap<Reg, Item>(a5, a6); 
		origami_utils::swap<Reg, Item>(a8, a10);  origami_utils::swap<Reg, Item>(a9, a11);  origami_utils::swap<Reg, Item>(a9, a10); 
		origami_utils::swap<Reg, Item>(a12, a14);  origami_utils::swap<Reg, Item>(a13, a15);  origami_utils::swap<Reg, Item>(a13, a14); 
		
		// sort registers
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge4R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a2); origami_utils::reverse<Reg, Item>(a3);
			origami_utils::reverse<Reg, Item>(a6); origami_utils::reverse<Reg, Item>(a7);
			origami_utils::reverse<Reg, Item>(a10); origami_utils::reverse<Reg, Item>(a11);
			origami_utils::reverse<Reg, Item>(a14); origami_utils::reverse<Reg, Item>(a15);
			origami_utils::reverse<Reg, Item>(a18); origami_utils::reverse<Reg, Item>(a19);
			origami_utils::reverse<Reg, Item>(a22); origami_utils::reverse<Reg, Item>(a23);
			origami_utils::reverse<Reg, Item>(a26); origami_utils::reverse<Reg, Item>(a27);
			origami_utils::reverse<Reg, Item>(a30); origami_utils::reverse<Reg, Item>(a31);
		}

		// run swaps
		origami_utils::swap<Reg, Item>(a0, a2);  origami_utils::swap<Reg, Item>(a1, a3);  origami_utils::swap<Reg, Item>(a1, a2);
		origami_utils::swap<Reg, Item>(a4, a6);  origami_utils::swap<Reg, Item>(a5, a7);  origami_utils::swap<Reg, Item>(a5, a6);
		origami_utils::swap<Reg, Item>(a8, a10);  origami_utils::swap<Reg, Item>(a9, a11);  origami_utils::swap<Reg, Item>(a9, a10);
		origami_utils::swap<Reg, Item>(a12, a14);  origami_utils::swap<Reg, Item>(a13, a15);  origami_utils::swap<Reg, Item>(a13, a14);
		origami_utils::swap<Reg, Item>(a16, a18);  origami_utils::swap<Reg, Item>(a17, a19);  origami_utils::swap<Reg, Item>(a17, a18);
		origami_utils::swap<Reg, Item>(a20, a22);  origami_utils::swap<Reg, Item>(a21, a23);  origami_utils::swap<Reg, Item>(a21, a22);
		origami_utils::swap<Reg, Item>(a24, a26);  origami_utils::swap<Reg, Item>(a25, a27);  origami_utils::swap<Reg, Item>(a25, a26);
		origami_utils::swap<Reg, Item>(a28, a30);  origami_utils::swap<Reg, Item>(a29, a31);  origami_utils::swap<Reg, Item>(a29, a30);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

	}


	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge8R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a4); origami_utils::reverse<Reg, Item>(a5);
			origami_utils::reverse<Reg, Item>(a6); origami_utils::reverse<Reg, Item>(a7);
			origami_utils::reverse<Reg, Item>(a12); origami_utils::reverse<Reg, Item>(a13);
			origami_utils::reverse<Reg, Item>(a14); origami_utils::reverse<Reg, Item>(a15);
		}

		// run swaps 
		origami_utils::swap<Reg, Item>(a0, a4);  origami_utils::swap<Reg, Item>(a1, a5);  origami_utils::swap<Reg, Item>(a2, a6); origami_utils::swap<Reg, Item>(a3, a7);
		origami_utils::swap<Reg, Item>(a2, a4);  origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a1, a2);  origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6);

		origami_utils::swap<Reg, Item>(a8, a12);  origami_utils::swap<Reg, Item>(a9, a13);  origami_utils::swap<Reg, Item>(a10, a14); origami_utils::swap<Reg, Item>(a11, a15);
		origami_utils::swap<Reg, Item>(a10, a12);  origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a9, a10);  origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14);

		// sort registers
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}
	
	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge8R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a4); origami_utils::reverse<Reg, Item>(a5);
			origami_utils::reverse<Reg, Item>(a6); origami_utils::reverse<Reg, Item>(a7);
			origami_utils::reverse<Reg, Item>(a12); origami_utils::reverse<Reg, Item>(a13);
			origami_utils::reverse<Reg, Item>(a14); origami_utils::reverse<Reg, Item>(a15);
			origami_utils::reverse<Reg, Item>(a20); origami_utils::reverse<Reg, Item>(a21);
			origami_utils::reverse<Reg, Item>(a22); origami_utils::reverse<Reg, Item>(a23);
			origami_utils::reverse<Reg, Item>(a28); origami_utils::reverse<Reg, Item>(a29);
			origami_utils::reverse<Reg, Item>(a30); origami_utils::reverse<Reg, Item>(a31);
		}

		// run swaps
		origami_utils::swap<Reg, Item>(a0, a4);  origami_utils::swap<Reg, Item>(a1, a5);  origami_utils::swap<Reg, Item>(a2, a6); origami_utils::swap<Reg, Item>(a3, a7);
		origami_utils::swap<Reg, Item>(a2, a4);  origami_utils::swap<Reg, Item>(a3, a5);
		origami_utils::swap<Reg, Item>(a1, a2);  origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6);

		origami_utils::swap<Reg, Item>(a8, a12);  origami_utils::swap<Reg, Item>(a9, a13);  origami_utils::swap<Reg, Item>(a10, a14); origami_utils::swap<Reg, Item>(a11, a15);
		origami_utils::swap<Reg, Item>(a10, a12);  origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a9, a10);  origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14);

		origami_utils::swap<Reg, Item>(a16, a20);  origami_utils::swap<Reg, Item>(a17, a21);  origami_utils::swap<Reg, Item>(a18, a22); origami_utils::swap<Reg, Item>(a19, a23);
		origami_utils::swap<Reg, Item>(a18, a20);  origami_utils::swap<Reg, Item>(a19, a21);
		origami_utils::swap<Reg, Item>(a17, a18);  origami_utils::swap<Reg, Item>(a19, a20); origami_utils::swap<Reg, Item>(a21, a22);

		origami_utils::swap<Reg, Item>(a24, a28);  origami_utils::swap<Reg, Item>(a25, a29);  origami_utils::swap<Reg, Item>(a26, a30); origami_utils::swap<Reg, Item>(a27, a31);
		origami_utils::swap<Reg, Item>(a26, a28);  origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a25, a26);  origami_utils::swap<Reg, Item>(a27, a28); origami_utils::swap<Reg, Item>(a29, a30);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge16R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a8); origami_utils::reverse<Reg, Item>(a9);
			origami_utils::reverse<Reg, Item>(a10); origami_utils::reverse<Reg, Item>(a11);
			origami_utils::reverse<Reg, Item>(a12); origami_utils::reverse<Reg, Item>(a13);
			origami_utils::reverse<Reg, Item>(a14); origami_utils::reverse<Reg, Item>(a15);
		}

		// run swaps
		origami_utils::swap<Reg, Item>(a0, a8);  origami_utils::swap<Reg, Item>(a1, a9);  origami_utils::swap<Reg, Item>(a2, a10); origami_utils::swap<Reg, Item>(a3, a11);
		origami_utils::swap<Reg, Item>(a4, a12);  origami_utils::swap<Reg, Item>(a5, a13); origami_utils::swap<Reg, Item>(a6, a14);  origami_utils::swap<Reg, Item>(a7, a15); 
		origami_utils::swap<Reg, Item>(a4, a8);  origami_utils::swap<Reg, Item>(a5, a9);  origami_utils::swap<Reg, Item>(a6, a10); origami_utils::swap<Reg, Item>(a7, a11);
		origami_utils::swap<Reg, Item>(a2, a4);  origami_utils::swap<Reg, Item>(a3, a5); origami_utils::swap<Reg, Item>(a6, a8);  origami_utils::swap<Reg, Item>(a7, a9); origami_utils::swap<Reg, Item>(a10, a12);  origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a1, a2);  origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6);  origami_utils::swap<Reg, Item>(a7, a8); origami_utils::swap<Reg, Item>(a9, a10);  origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14);

		// sort registers
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge16R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a8); origami_utils::reverse<Reg, Item>(a9);
			origami_utils::reverse<Reg, Item>(a10); origami_utils::reverse<Reg, Item>(a11);
			origami_utils::reverse<Reg, Item>(a12); origami_utils::reverse<Reg, Item>(a13);
			origami_utils::reverse<Reg, Item>(a14); origami_utils::reverse<Reg, Item>(a15);
			origami_utils::reverse<Reg, Item>(a24); origami_utils::reverse<Reg, Item>(a25);
			origami_utils::reverse<Reg, Item>(a26); origami_utils::reverse<Reg, Item>(a27);
			origami_utils::reverse<Reg, Item>(a28); origami_utils::reverse<Reg, Item>(a29);
			origami_utils::reverse<Reg, Item>(a30); origami_utils::reverse<Reg, Item>(a31);
		}

		// run swaps
		origami_utils::swap<Reg, Item>(a0, a8);  origami_utils::swap<Reg, Item>(a1, a9);  origami_utils::swap<Reg, Item>(a2, a10); origami_utils::swap<Reg, Item>(a3, a11);
		origami_utils::swap<Reg, Item>(a4, a12);  origami_utils::swap<Reg, Item>(a5, a13); origami_utils::swap<Reg, Item>(a6, a14);  origami_utils::swap<Reg, Item>(a7, a15);
		origami_utils::swap<Reg, Item>(a4, a8);  origami_utils::swap<Reg, Item>(a5, a9);  origami_utils::swap<Reg, Item>(a6, a10); origami_utils::swap<Reg, Item>(a7, a11);
		origami_utils::swap<Reg, Item>(a2, a4);  origami_utils::swap<Reg, Item>(a3, a5); origami_utils::swap<Reg, Item>(a6, a8);  origami_utils::swap<Reg, Item>(a7, a9); origami_utils::swap<Reg, Item>(a10, a12);  origami_utils::swap<Reg, Item>(a11, a13);
		origami_utils::swap<Reg, Item>(a1, a2);  origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6);  origami_utils::swap<Reg, Item>(a7, a8); origami_utils::swap<Reg, Item>(a9, a10);  origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14);

		origami_utils::swap<Reg, Item>(a16, a24);  origami_utils::swap<Reg, Item>(a17, a25); origami_utils::swap<Reg, Item>(a18, a26); origami_utils::swap<Reg, Item>(a19, a27);
		origami_utils::swap<Reg, Item>(a20, a28);  origami_utils::swap<Reg, Item>(a21, a29); origami_utils::swap<Reg, Item>(a22, a30);  origami_utils::swap<Reg, Item>(a23, a31);
		origami_utils::swap<Reg, Item>(a20, a24);  origami_utils::swap<Reg, Item>(a21, a25); origami_utils::swap<Reg, Item>(a22, a26); origami_utils::swap<Reg, Item>(a23, a27);
		origami_utils::swap<Reg, Item>(a18, a20);  origami_utils::swap<Reg, Item>(a19, a21); origami_utils::swap<Reg, Item>(a22, a24);  origami_utils::swap<Reg, Item>(a23, a25); origami_utils::swap<Reg, Item>(a26, a28);  origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a17, a18);  origami_utils::swap<Reg, Item>(a19, a20); origami_utils::swap<Reg, Item>(a21, a22);  origami_utils::swap<Reg, Item>(a23, a24); origami_utils::swap<Reg, Item>(a25, a26);  origami_utils::swap<Reg, Item>(a27, a28); origami_utils::swap<Reg, Item>(a29, a30);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge32R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a16); origami_utils::reverse<Reg, Item>(a17);
			origami_utils::reverse<Reg, Item>(a18); origami_utils::reverse<Reg, Item>(a19);
			origami_utils::reverse<Reg, Item>(a20); origami_utils::reverse<Reg, Item>(a21);
			origami_utils::reverse<Reg, Item>(a22); origami_utils::reverse<Reg, Item>(a23);
			origami_utils::reverse<Reg, Item>(a24); origami_utils::reverse<Reg, Item>(a25);
			origami_utils::reverse<Reg, Item>(a26); origami_utils::reverse<Reg, Item>(a27);
			origami_utils::reverse<Reg, Item>(a28); origami_utils::reverse<Reg, Item>(a29);
			origami_utils::reverse<Reg, Item>(a30); origami_utils::reverse<Reg, Item>(a31);
		}

		// run swaps
		origami_utils::swap<Reg, Item>(a0, a16);	origami_utils::swap<Reg, Item>(a1, a17);	origami_utils::swap<Reg, Item>(a2, a18);	origami_utils::swap<Reg, Item>(a3, a19);	origami_utils::swap<Reg, Item>(a4, a20);	origami_utils::swap<Reg, Item>(a5, a21);	origami_utils::swap<Reg, Item>(a6, a22);	origami_utils::swap<Reg, Item>(a7, a23);	origami_utils::swap<Reg, Item>(a8, a24);	origami_utils::swap<Reg, Item>(a9, a25);		origami_utils::swap<Reg, Item>(a10, a26); origami_utils::swap<Reg, Item>(a11, a27); origami_utils::swap<Reg, Item>(a12, a28); origami_utils::swap<Reg, Item>(a13, a29); origami_utils::swap<Reg, Item>(a14, a30); origami_utils::swap<Reg, Item>(a15, a31);
		origami_utils::swap<Reg, Item>(a8, a16);	origami_utils::swap<Reg, Item>(a9, a17);	origami_utils::swap<Reg, Item>(a10, a18);	origami_utils::swap<Reg, Item>(a11, a19);	origami_utils::swap<Reg, Item>(a12, a20);	origami_utils::swap<Reg, Item>(a13, a21);	origami_utils::swap<Reg, Item>(a14, a22);	origami_utils::swap<Reg, Item>(a15, a23);
		origami_utils::swap<Reg, Item>(a4, a8);		origami_utils::swap<Reg, Item>(a5, a9);		origami_utils::swap<Reg, Item>(a6, a10);	origami_utils::swap<Reg, Item>(a7, a11);	origami_utils::swap<Reg, Item>(a12, a16);	origami_utils::swap<Reg, Item>(a13, a17);	origami_utils::swap<Reg, Item>(a14, a18);	origami_utils::swap<Reg, Item>(a15, a19);	origami_utils::swap<Reg, Item>(a20, a24);	origami_utils::swap<Reg, Item>(a21, a25);		origami_utils::swap<Reg, Item>(a22, a26); origami_utils::swap<Reg, Item>(a23, a27);
		origami_utils::swap<Reg, Item>(a2, a4);		origami_utils::swap<Reg, Item>(a3, a5);		origami_utils::swap<Reg, Item>(a6, a8);		origami_utils::swap<Reg, Item>(a7, a9);		origami_utils::swap<Reg, Item>(a10, a12);	origami_utils::swap<Reg, Item>(a11, a13);	origami_utils::swap<Reg, Item>(a14, a16);	origami_utils::swap<Reg, Item>(a15, a17);	origami_utils::swap<Reg, Item>(a18, a20);	origami_utils::swap<Reg, Item>(a19, a21);		origami_utils::swap<Reg, Item>(a22, a24); origami_utils::swap<Reg, Item>(a23, a25); origami_utils::swap<Reg, Item>(a26, a28); origami_utils::swap<Reg, Item>(a27, a29);
		origami_utils::swap<Reg, Item>(a1, a2);		origami_utils::swap<Reg, Item>(a3, a4);		origami_utils::swap<Reg, Item>(a5, a6);		origami_utils::swap<Reg, Item>(a7, a8);		origami_utils::swap<Reg, Item>(a9, a10);	origami_utils::swap<Reg, Item>(a11, a12);	origami_utils::swap<Reg, Item>(a13, a14);	origami_utils::swap<Reg, Item>(a15, a16);	origami_utils::swap<Reg, Item>(a17, a18);	origami_utils::swap<Reg, Item>(a19, a20);		origami_utils::swap<Reg, Item>(a21, a22); origami_utils::swap<Reg, Item>(a23, a24); origami_utils::swap<Reg, Item>(a25, a26); origami_utils::swap<Reg, Item>(a27, a28); origami_utils::swap<Reg, Item>(a29, a30);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge64R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31,
		Reg& a32, Reg& a33, Reg& a34, Reg& a35, Reg& a36, Reg& a37, Reg& a38, Reg& a39, Reg& a40, Reg& a41, Reg& a42, Reg& a43, Reg& a44, Reg& a45, Reg& a46, Reg& a47, 
		Reg& a48, Reg& a49, Reg& a50, Reg& a51, Reg& a52, Reg& a53, Reg& a54, Reg& a55, Reg& a56, Reg& a57, Reg& a58, Reg& a59, Reg& a60, Reg& a61, Reg& a62, Reg& a63) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a32); origami_utils::reverse<Reg, Item>(a33); origami_utils::reverse<Reg, Item>(a34); origami_utils::reverse<Reg, Item>(a35); origami_utils::reverse<Reg, Item>(a36); origami_utils::reverse<Reg, Item>(a37); origami_utils::reverse<Reg, Item>(a38); origami_utils::reverse<Reg, Item>(a39); origami_utils::reverse<Reg, Item>(a40); origami_utils::reverse<Reg, Item>(a41); origami_utils::reverse<Reg, Item>(a42); origami_utils::reverse<Reg, Item>(a43); origami_utils::reverse<Reg, Item>(a44); origami_utils::reverse<Reg, Item>(a45); origami_utils::reverse<Reg, Item>(a46); origami_utils::reverse<Reg, Item>(a47); origami_utils::reverse<Reg, Item>(a48); origami_utils::reverse<Reg, Item>(a49); origami_utils::reverse<Reg, Item>(a50); origami_utils::reverse<Reg, Item>(a51); origami_utils::reverse<Reg, Item>(a52); origami_utils::reverse<Reg, Item>(a53); origami_utils::reverse<Reg, Item>(a54); origami_utils::reverse<Reg, Item>(a55); origami_utils::reverse<Reg, Item>(a56); origami_utils::reverse<Reg, Item>(a57); origami_utils::reverse<Reg, Item>(a58); origami_utils::reverse<Reg, Item>(a59); origami_utils::reverse<Reg, Item>(a60); origami_utils::reverse<Reg, Item>(a61); origami_utils::reverse<Reg, Item>(a62); origami_utils::reverse<Reg, Item>(a63);
		}
		// run swaps
		origami_utils::swap<Reg, Item>(a0, a32); origami_utils::swap<Reg, Item>(a16, a48); origami_utils::swap<Reg, Item>(a16, a32); origami_utils::swap<Reg, Item>(a8, a40); origami_utils::swap<Reg, Item>(a24, a56); origami_utils::swap<Reg, Item>(a24, a40); origami_utils::swap<Reg, Item>(a8, a16); origami_utils::swap<Reg, Item>(a24, a32); origami_utils::swap<Reg, Item>(a40, a48); origami_utils::swap<Reg, Item>(a4, a36); origami_utils::swap<Reg, Item>(a20, a52); origami_utils::swap<Reg, Item>(a20, a36); origami_utils::swap<Reg, Item>(a12, a44); origami_utils::swap<Reg, Item>(a28, a60); origami_utils::swap<Reg, Item>(a28, a44); origami_utils::swap<Reg, Item>(a12, a20); origami_utils::swap<Reg, Item>(a28, a36); origami_utils::swap<Reg, Item>(a44, a52); origami_utils::swap<Reg, Item>(a4, a8); origami_utils::swap<Reg, Item>(a12, a16); origami_utils::swap<Reg, Item>(a20, a24); origami_utils::swap<Reg, Item>(a28, a32); origami_utils::swap<Reg, Item>(a36, a40); origami_utils::swap<Reg, Item>(a44, a48); origami_utils::swap<Reg, Item>(a52, a56); origami_utils::swap<Reg, Item>(a2, a34); origami_utils::swap<Reg, Item>(a18, a50); origami_utils::swap<Reg, Item>(a18, a34); origami_utils::swap<Reg, Item>(a10, a42); origami_utils::swap<Reg, Item>(a26, a58); origami_utils::swap<Reg, Item>(a26, a42); origami_utils::swap<Reg, Item>(a10, a18); origami_utils::swap<Reg, Item>(a26, a34); origami_utils::swap<Reg, Item>(a42, a50); origami_utils::swap<Reg, Item>(a6, a38); origami_utils::swap<Reg, Item>(a22, a54); origami_utils::swap<Reg, Item>(a22, a38); origami_utils::swap<Reg, Item>(a14, a46); origami_utils::swap<Reg, Item>(a30, a62); origami_utils::swap<Reg, Item>(a30, a46); origami_utils::swap<Reg, Item>(a14, a22); origami_utils::swap<Reg, Item>(a30, a38); origami_utils::swap<Reg, Item>(a46, a54); origami_utils::swap<Reg, Item>(a6, a10); origami_utils::swap<Reg, Item>(a14, a18); origami_utils::swap<Reg, Item>(a22, a26); origami_utils::swap<Reg, Item>(a30, a34); origami_utils::swap<Reg, Item>(a38, a42); origami_utils::swap<Reg, Item>(a46, a50); origami_utils::swap<Reg, Item>(a54, a58); origami_utils::swap<Reg, Item>(a2, a4); origami_utils::swap<Reg, Item>(a6, a8); origami_utils::swap<Reg, Item>(a10, a12); origami_utils::swap<Reg, Item>(a14, a16); origami_utils::swap<Reg, Item>(a18, a20); origami_utils::swap<Reg, Item>(a22, a24); origami_utils::swap<Reg, Item>(a26, a28); origami_utils::swap<Reg, Item>(a30, a32); origami_utils::swap<Reg, Item>(a34, a36); origami_utils::swap<Reg, Item>(a38, a40); origami_utils::swap<Reg, Item>(a42, a44); origami_utils::swap<Reg, Item>(a46, a48); origami_utils::swap<Reg, Item>(a50, a52); origami_utils::swap<Reg, Item>(a54, a56); origami_utils::swap<Reg, Item>(a58, a60); origami_utils::swap<Reg, Item>(a1, a33); origami_utils::swap<Reg, Item>(a17, a49); origami_utils::swap<Reg, Item>(a17, a33); origami_utils::swap<Reg, Item>(a9, a41); origami_utils::swap<Reg, Item>(a25, a57); origami_utils::swap<Reg, Item>(a25, a41); origami_utils::swap<Reg, Item>(a9, a17); origami_utils::swap<Reg, Item>(a25, a33); origami_utils::swap<Reg, Item>(a41, a49); origami_utils::swap<Reg, Item>(a5, a37); origami_utils::swap<Reg, Item>(a21, a53); origami_utils::swap<Reg, Item>(a21, a37); origami_utils::swap<Reg, Item>(a13, a45); origami_utils::swap<Reg, Item>(a29, a61); origami_utils::swap<Reg, Item>(a29, a45); origami_utils::swap<Reg, Item>(a13, a21); origami_utils::swap<Reg, Item>(a29, a37); origami_utils::swap<Reg, Item>(a45, a53); origami_utils::swap<Reg, Item>(a5, a9); origami_utils::swap<Reg, Item>(a13, a17); origami_utils::swap<Reg, Item>(a21, a25); origami_utils::swap<Reg, Item>(a29, a33); origami_utils::swap<Reg, Item>(a37, a41); origami_utils::swap<Reg, Item>(a45, a49); origami_utils::swap<Reg, Item>(a53, a57); origami_utils::swap<Reg, Item>(a3, a35); origami_utils::swap<Reg, Item>(a19, a51); origami_utils::swap<Reg, Item>(a19, a35); origami_utils::swap<Reg, Item>(a11, a43); origami_utils::swap<Reg, Item>(a27, a59); origami_utils::swap<Reg, Item>(a27, a43); origami_utils::swap<Reg, Item>(a11, a19); origami_utils::swap<Reg, Item>(a27, a35); origami_utils::swap<Reg, Item>(a43, a51); origami_utils::swap<Reg, Item>(a7, a39); origami_utils::swap<Reg, Item>(a23, a55); origami_utils::swap<Reg, Item>(a23, a39); origami_utils::swap<Reg, Item>(a15, a47); origami_utils::swap<Reg, Item>(a31, a63); origami_utils::swap<Reg, Item>(a31, a47); origami_utils::swap<Reg, Item>(a15, a23); origami_utils::swap<Reg, Item>(a31, a39); origami_utils::swap<Reg, Item>(a47, a55); origami_utils::swap<Reg, Item>(a7, a11); origami_utils::swap<Reg, Item>(a15, a19); origami_utils::swap<Reg, Item>(a23, a27); origami_utils::swap<Reg, Item>(a31, a35); origami_utils::swap<Reg, Item>(a39, a43); origami_utils::swap<Reg, Item>(a47, a51); origami_utils::swap<Reg, Item>(a55, a59); origami_utils::swap<Reg, Item>(a3, a5); origami_utils::swap<Reg, Item>(a7, a9); origami_utils::swap<Reg, Item>(a11, a13); origami_utils::swap<Reg, Item>(a15, a17); origami_utils::swap<Reg, Item>(a19, a21); origami_utils::swap<Reg, Item>(a23, a25); origami_utils::swap<Reg, Item>(a27, a29); origami_utils::swap<Reg, Item>(a31, a33); origami_utils::swap<Reg, Item>(a35, a37); origami_utils::swap<Reg, Item>(a39, a41); origami_utils::swap<Reg, Item>(a43, a45); origami_utils::swap<Reg, Item>(a47, a49); origami_utils::swap<Reg, Item>(a51, a53); origami_utils::swap<Reg, Item>(a55, a57); origami_utils::swap<Reg, Item>(a59, a61); origami_utils::swap<Reg, Item>(a1, a2); origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6); origami_utils::swap<Reg, Item>(a7, a8); origami_utils::swap<Reg, Item>(a9, a10); origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14); origami_utils::swap<Reg, Item>(a15, a16); origami_utils::swap<Reg, Item>(a17, a18); origami_utils::swap<Reg, Item>(a19, a20); origami_utils::swap<Reg, Item>(a21, a22); origami_utils::swap<Reg, Item>(a23, a24); origami_utils::swap<Reg, Item>(a25, a26); origami_utils::swap<Reg, Item>(a27, a28); origami_utils::swap<Reg, Item>(a29, a30); origami_utils::swap<Reg, Item>(a31, a32); origami_utils::swap<Reg, Item>(a33, a34); origami_utils::swap<Reg, Item>(a35, a36); origami_utils::swap<Reg, Item>(a37, a38); origami_utils::swap<Reg, Item>(a39, a40); origami_utils::swap<Reg, Item>(a41, a42); origami_utils::swap<Reg, Item>(a43, a44); origami_utils::swap<Reg, Item>(a45, a46); origami_utils::swap<Reg, Item>(a47, a48); origami_utils::swap<Reg, Item>(a49, a50); origami_utils::swap<Reg, Item>(a51, a52); origami_utils::swap<Reg, Item>(a53, a54); origami_utils::swap<Reg, Item>(a55, a56); origami_utils::swap<Reg, Item>(a57, a58); origami_utils::swap<Reg, Item>(a59, a60); origami_utils::swap<Reg, Item>(a61, a62);

		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		mrmerge_sort_reg<Reg, Item>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge128R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15, Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31, Reg& a32, Reg& a33, Reg& a34, Reg& a35, Reg& a36, Reg& a37, Reg& a38, Reg& a39, Reg& a40, Reg& a41, Reg& a42, Reg& a43, Reg& a44, Reg& a45, Reg& a46, Reg& a47, Reg& a48, Reg& a49, Reg& a50, Reg& a51, Reg& a52, Reg& a53, Reg& a54, Reg& a55, Reg& a56, Reg& a57, Reg& a58, Reg& a59, Reg& a60, Reg& a61, Reg& a62, Reg& a63, Reg& a64, Reg& a65, Reg& a66, Reg& a67, Reg& a68, Reg& a69, Reg& a70, Reg& a71, Reg& a72, Reg& a73, Reg& a74, Reg& a75, Reg& a76, Reg& a77, Reg& a78, Reg& a79, Reg& a80, Reg& a81, Reg& a82, Reg& a83, Reg& a84, Reg& a85, Reg& a86, Reg& a87, Reg& a88, Reg& a89, Reg& a90, Reg& a91, Reg& a92, Reg& a93, Reg& a94, Reg& a95, Reg& a96, Reg& a97, Reg& a98, Reg& a99, Reg& a100, Reg& a101, Reg& a102, Reg& a103, Reg& a104, Reg& a105, Reg& a106, Reg& a107, Reg& a108, Reg& a109, Reg& a110, Reg& a111, Reg& a112, Reg& a113, Reg& a114, Reg& a115, Reg& a116, Reg& a117, Reg& a118, Reg& a119, Reg& a120, Reg& a121, Reg& a122, Reg& a123, Reg& a124, Reg& a125, Reg& a126, Reg& a127) {
		// reverse 2nd half
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a64); origami_utils::reverse<Reg, Item>(a65); origami_utils::reverse<Reg, Item>(a66); origami_utils::reverse<Reg, Item>(a67); origami_utils::reverse<Reg, Item>(a68); origami_utils::reverse<Reg, Item>(a69); origami_utils::reverse<Reg, Item>(a70); origami_utils::reverse<Reg, Item>(a71); origami_utils::reverse<Reg, Item>(a72); origami_utils::reverse<Reg, Item>(a73); origami_utils::reverse<Reg, Item>(a74); origami_utils::reverse<Reg, Item>(a75); origami_utils::reverse<Reg, Item>(a76); origami_utils::reverse<Reg, Item>(a77); origami_utils::reverse<Reg, Item>(a78); origami_utils::reverse<Reg, Item>(a79); origami_utils::reverse<Reg, Item>(a80); origami_utils::reverse<Reg, Item>(a81); origami_utils::reverse<Reg, Item>(a82); origami_utils::reverse<Reg, Item>(a83); origami_utils::reverse<Reg, Item>(a84); origami_utils::reverse<Reg, Item>(a85); origami_utils::reverse<Reg, Item>(a86); origami_utils::reverse<Reg, Item>(a87); origami_utils::reverse<Reg, Item>(a88); origami_utils::reverse<Reg, Item>(a89); origami_utils::reverse<Reg, Item>(a90); origami_utils::reverse<Reg, Item>(a91); origami_utils::reverse<Reg, Item>(a92); origami_utils::reverse<Reg, Item>(a93); origami_utils::reverse<Reg, Item>(a94); origami_utils::reverse<Reg, Item>(a95); origami_utils::reverse<Reg, Item>(a96); origami_utils::reverse<Reg, Item>(a97); origami_utils::reverse<Reg, Item>(a98); origami_utils::reverse<Reg, Item>(a99); origami_utils::reverse<Reg, Item>(a100); origami_utils::reverse<Reg, Item>(a101); origami_utils::reverse<Reg, Item>(a102); origami_utils::reverse<Reg, Item>(a103); origami_utils::reverse<Reg, Item>(a104); origami_utils::reverse<Reg, Item>(a105); origami_utils::reverse<Reg, Item>(a106); origami_utils::reverse<Reg, Item>(a107); origami_utils::reverse<Reg, Item>(a108); origami_utils::reverse<Reg, Item>(a109); origami_utils::reverse<Reg, Item>(a110); origami_utils::reverse<Reg, Item>(a111); origami_utils::reverse<Reg, Item>(a112); origami_utils::reverse<Reg, Item>(a113); origami_utils::reverse<Reg, Item>(a114); origami_utils::reverse<Reg, Item>(a115); origami_utils::reverse<Reg, Item>(a116); origami_utils::reverse<Reg, Item>(a117); origami_utils::reverse<Reg, Item>(a118); origami_utils::reverse<Reg, Item>(a119); origami_utils::reverse<Reg, Item>(a120); origami_utils::reverse<Reg, Item>(a121); origami_utils::reverse<Reg, Item>(a122); origami_utils::reverse<Reg, Item>(a123); origami_utils::reverse<Reg, Item>(a124); origami_utils::reverse<Reg, Item>(a125); origami_utils::reverse<Reg, Item>(a126); origami_utils::reverse<Reg, Item>(a127);
		}
		// run swaps
		origami_utils::swap<Reg, Item>(a0, a64); origami_utils::swap<Reg, Item>(a32, a96); origami_utils::swap<Reg, Item>(a32, a64); origami_utils::swap<Reg, Item>(a16, a80); origami_utils::swap<Reg, Item>(a48, a112); origami_utils::swap<Reg, Item>(a48, a80); origami_utils::swap<Reg, Item>(a16, a32); origami_utils::swap<Reg, Item>(a48, a64); origami_utils::swap<Reg, Item>(a80, a96); origami_utils::swap<Reg, Item>(a8, a72); origami_utils::swap<Reg, Item>(a40, a104); origami_utils::swap<Reg, Item>(a40, a72); origami_utils::swap<Reg, Item>(a24, a88); origami_utils::swap<Reg, Item>(a56, a120); origami_utils::swap<Reg, Item>(a56, a88); origami_utils::swap<Reg, Item>(a24, a40); origami_utils::swap<Reg, Item>(a56, a72); origami_utils::swap<Reg, Item>(a88, a104); origami_utils::swap<Reg, Item>(a8, a16); origami_utils::swap<Reg, Item>(a24, a32); origami_utils::swap<Reg, Item>(a40, a48); origami_utils::swap<Reg, Item>(a56, a64); origami_utils::swap<Reg, Item>(a72, a80); origami_utils::swap<Reg, Item>(a88, a96); origami_utils::swap<Reg, Item>(a104, a112); origami_utils::swap<Reg, Item>(a4, a68); origami_utils::swap<Reg, Item>(a36, a100); origami_utils::swap<Reg, Item>(a36, a68); origami_utils::swap<Reg, Item>(a20, a84); origami_utils::swap<Reg, Item>(a52, a116); origami_utils::swap<Reg, Item>(a52, a84); origami_utils::swap<Reg, Item>(a20, a36); origami_utils::swap<Reg, Item>(a52, a68); origami_utils::swap<Reg, Item>(a84, a100); origami_utils::swap<Reg, Item>(a12, a76); origami_utils::swap<Reg, Item>(a44, a108); origami_utils::swap<Reg, Item>(a44, a76); origami_utils::swap<Reg, Item>(a28, a92); origami_utils::swap<Reg, Item>(a60, a124); origami_utils::swap<Reg, Item>(a60, a92); origami_utils::swap<Reg, Item>(a28, a44); origami_utils::swap<Reg, Item>(a60, a76); origami_utils::swap<Reg, Item>(a92, a108); origami_utils::swap<Reg, Item>(a12, a20); origami_utils::swap<Reg, Item>(a28, a36); origami_utils::swap<Reg, Item>(a44, a52); origami_utils::swap<Reg, Item>(a60, a68); origami_utils::swap<Reg, Item>(a76, a84); origami_utils::swap<Reg, Item>(a92, a100); origami_utils::swap<Reg, Item>(a108, a116); origami_utils::swap<Reg, Item>(a4, a8); origami_utils::swap<Reg, Item>(a12, a16); origami_utils::swap<Reg, Item>(a20, a24); origami_utils::swap<Reg, Item>(a28, a32); origami_utils::swap<Reg, Item>(a36, a40); origami_utils::swap<Reg, Item>(a44, a48); origami_utils::swap<Reg, Item>(a52, a56); origami_utils::swap<Reg, Item>(a60, a64); origami_utils::swap<Reg, Item>(a68, a72); origami_utils::swap<Reg, Item>(a76, a80); origami_utils::swap<Reg, Item>(a84, a88); origami_utils::swap<Reg, Item>(a92, a96); origami_utils::swap<Reg, Item>(a100, a104); origami_utils::swap<Reg, Item>(a108, a112); origami_utils::swap<Reg, Item>(a116, a120); origami_utils::swap<Reg, Item>(a2, a66); origami_utils::swap<Reg, Item>(a34, a98); origami_utils::swap<Reg, Item>(a34, a66); origami_utils::swap<Reg, Item>(a18, a82); origami_utils::swap<Reg, Item>(a50, a114); origami_utils::swap<Reg, Item>(a50, a82); origami_utils::swap<Reg, Item>(a18, a34); origami_utils::swap<Reg, Item>(a50, a66); origami_utils::swap<Reg, Item>(a82, a98); origami_utils::swap<Reg, Item>(a10, a74); origami_utils::swap<Reg, Item>(a42, a106); origami_utils::swap<Reg, Item>(a42, a74); origami_utils::swap<Reg, Item>(a26, a90); origami_utils::swap<Reg, Item>(a58, a122); origami_utils::swap<Reg, Item>(a58, a90); origami_utils::swap<Reg, Item>(a26, a42); origami_utils::swap<Reg, Item>(a58, a74); origami_utils::swap<Reg, Item>(a90, a106); origami_utils::swap<Reg, Item>(a10, a18); origami_utils::swap<Reg, Item>(a26, a34); origami_utils::swap<Reg, Item>(a42, a50); origami_utils::swap<Reg, Item>(a58, a66); origami_utils::swap<Reg, Item>(a74, a82); origami_utils::swap<Reg, Item>(a90, a98); origami_utils::swap<Reg, Item>(a106, a114); origami_utils::swap<Reg, Item>(a6, a70); origami_utils::swap<Reg, Item>(a38, a102); origami_utils::swap<Reg, Item>(a38, a70); origami_utils::swap<Reg, Item>(a22, a86); origami_utils::swap<Reg, Item>(a54, a118); origami_utils::swap<Reg, Item>(a54, a86); origami_utils::swap<Reg, Item>(a22, a38); origami_utils::swap<Reg, Item>(a54, a70); origami_utils::swap<Reg, Item>(a86, a102); origami_utils::swap<Reg, Item>(a14, a78); origami_utils::swap<Reg, Item>(a46, a110); origami_utils::swap<Reg, Item>(a46, a78); origami_utils::swap<Reg, Item>(a30, a94); origami_utils::swap<Reg, Item>(a62, a126); origami_utils::swap<Reg, Item>(a62, a94); origami_utils::swap<Reg, Item>(a30, a46); origami_utils::swap<Reg, Item>(a62, a78); origami_utils::swap<Reg, Item>(a94, a110); origami_utils::swap<Reg, Item>(a14, a22); origami_utils::swap<Reg, Item>(a30, a38); origami_utils::swap<Reg, Item>(a46, a54); origami_utils::swap<Reg, Item>(a62, a70); origami_utils::swap<Reg, Item>(a78, a86); origami_utils::swap<Reg, Item>(a94, a102); origami_utils::swap<Reg, Item>(a110, a118); origami_utils::swap<Reg, Item>(a6, a10); origami_utils::swap<Reg, Item>(a14, a18); origami_utils::swap<Reg, Item>(a22, a26); origami_utils::swap<Reg, Item>(a30, a34); origami_utils::swap<Reg, Item>(a38, a42); origami_utils::swap<Reg, Item>(a46, a50); origami_utils::swap<Reg, Item>(a54, a58); origami_utils::swap<Reg, Item>(a62, a66); origami_utils::swap<Reg, Item>(a70, a74); origami_utils::swap<Reg, Item>(a78, a82); origami_utils::swap<Reg, Item>(a86, a90); origami_utils::swap<Reg, Item>(a94, a98); origami_utils::swap<Reg, Item>(a102, a106); origami_utils::swap<Reg, Item>(a110, a114); origami_utils::swap<Reg, Item>(a118, a122); origami_utils::swap<Reg, Item>(a2, a4); origami_utils::swap<Reg, Item>(a6, a8); origami_utils::swap<Reg, Item>(a10, a12); origami_utils::swap<Reg, Item>(a14, a16); origami_utils::swap<Reg, Item>(a18, a20); origami_utils::swap<Reg, Item>(a22, a24); origami_utils::swap<Reg, Item>(a26, a28); origami_utils::swap<Reg, Item>(a30, a32); origami_utils::swap<Reg, Item>(a34, a36); origami_utils::swap<Reg, Item>(a38, a40); origami_utils::swap<Reg, Item>(a42, a44); origami_utils::swap<Reg, Item>(a46, a48); origami_utils::swap<Reg, Item>(a50, a52); origami_utils::swap<Reg, Item>(a54, a56); origami_utils::swap<Reg, Item>(a58, a60); origami_utils::swap<Reg, Item>(a62, a64); origami_utils::swap<Reg, Item>(a66, a68); origami_utils::swap<Reg, Item>(a70, a72); origami_utils::swap<Reg, Item>(a74, a76); origami_utils::swap<Reg, Item>(a78, a80); origami_utils::swap<Reg, Item>(a82, a84); origami_utils::swap<Reg, Item>(a86, a88); origami_utils::swap<Reg, Item>(a90, a92); origami_utils::swap<Reg, Item>(a94, a96); origami_utils::swap<Reg, Item>(a98, a100); origami_utils::swap<Reg, Item>(a102, a104); origami_utils::swap<Reg, Item>(a106, a108); origami_utils::swap<Reg, Item>(a110, a112); origami_utils::swap<Reg, Item>(a114, a116); origami_utils::swap<Reg, Item>(a118, a120); origami_utils::swap<Reg, Item>(a122, a124); origami_utils::swap<Reg, Item>(a1, a65); origami_utils::swap<Reg, Item>(a33, a97); origami_utils::swap<Reg, Item>(a33, a65); origami_utils::swap<Reg, Item>(a17, a81); origami_utils::swap<Reg, Item>(a49, a113); origami_utils::swap<Reg, Item>(a49, a81); origami_utils::swap<Reg, Item>(a17, a33); origami_utils::swap<Reg, Item>(a49, a65); origami_utils::swap<Reg, Item>(a81, a97); origami_utils::swap<Reg, Item>(a9, a73); origami_utils::swap<Reg, Item>(a41, a105); origami_utils::swap<Reg, Item>(a41, a73); origami_utils::swap<Reg, Item>(a25, a89); origami_utils::swap<Reg, Item>(a57, a121); origami_utils::swap<Reg, Item>(a57, a89); origami_utils::swap<Reg, Item>(a25, a41); origami_utils::swap<Reg, Item>(a57, a73); origami_utils::swap<Reg, Item>(a89, a105); origami_utils::swap<Reg, Item>(a9, a17); origami_utils::swap<Reg, Item>(a25, a33); origami_utils::swap<Reg, Item>(a41, a49); origami_utils::swap<Reg, Item>(a57, a65); origami_utils::swap<Reg, Item>(a73, a81); origami_utils::swap<Reg, Item>(a89, a97); origami_utils::swap<Reg, Item>(a105, a113); origami_utils::swap<Reg, Item>(a5, a69); origami_utils::swap<Reg, Item>(a37, a101); origami_utils::swap<Reg, Item>(a37, a69); origami_utils::swap<Reg, Item>(a21, a85); origami_utils::swap<Reg, Item>(a53, a117); origami_utils::swap<Reg, Item>(a53, a85); origami_utils::swap<Reg, Item>(a21, a37); origami_utils::swap<Reg, Item>(a53, a69); origami_utils::swap<Reg, Item>(a85, a101); origami_utils::swap<Reg, Item>(a13, a77); origami_utils::swap<Reg, Item>(a45, a109); origami_utils::swap<Reg, Item>(a45, a77); origami_utils::swap<Reg, Item>(a29, a93); origami_utils::swap<Reg, Item>(a61, a125); origami_utils::swap<Reg, Item>(a61, a93); origami_utils::swap<Reg, Item>(a29, a45); origami_utils::swap<Reg, Item>(a61, a77); origami_utils::swap<Reg, Item>(a93, a109); origami_utils::swap<Reg, Item>(a13, a21); origami_utils::swap<Reg, Item>(a29, a37); origami_utils::swap<Reg, Item>(a45, a53); origami_utils::swap<Reg, Item>(a61, a69); origami_utils::swap<Reg, Item>(a77, a85); origami_utils::swap<Reg, Item>(a93, a101); origami_utils::swap<Reg, Item>(a109, a117); origami_utils::swap<Reg, Item>(a5, a9); origami_utils::swap<Reg, Item>(a13, a17); origami_utils::swap<Reg, Item>(a21, a25); origami_utils::swap<Reg, Item>(a29, a33); origami_utils::swap<Reg, Item>(a37, a41); origami_utils::swap<Reg, Item>(a45, a49); origami_utils::swap<Reg, Item>(a53, a57); origami_utils::swap<Reg, Item>(a61, a65); origami_utils::swap<Reg, Item>(a69, a73); origami_utils::swap<Reg, Item>(a77, a81); origami_utils::swap<Reg, Item>(a85, a89); origami_utils::swap<Reg, Item>(a93, a97); origami_utils::swap<Reg, Item>(a101, a105); origami_utils::swap<Reg, Item>(a109, a113); origami_utils::swap<Reg, Item>(a117, a121); origami_utils::swap<Reg, Item>(a3, a67); origami_utils::swap<Reg, Item>(a35, a99); origami_utils::swap<Reg, Item>(a35, a67); origami_utils::swap<Reg, Item>(a19, a83); origami_utils::swap<Reg, Item>(a51, a115); origami_utils::swap<Reg, Item>(a51, a83); origami_utils::swap<Reg, Item>(a19, a35); origami_utils::swap<Reg, Item>(a51, a67); origami_utils::swap<Reg, Item>(a83, a99); origami_utils::swap<Reg, Item>(a11, a75); origami_utils::swap<Reg, Item>(a43, a107); origami_utils::swap<Reg, Item>(a43, a75); origami_utils::swap<Reg, Item>(a27, a91); origami_utils::swap<Reg, Item>(a59, a123); origami_utils::swap<Reg, Item>(a59, a91); origami_utils::swap<Reg, Item>(a27, a43); origami_utils::swap<Reg, Item>(a59, a75); origami_utils::swap<Reg, Item>(a91, a107); origami_utils::swap<Reg, Item>(a11, a19); origami_utils::swap<Reg, Item>(a27, a35); origami_utils::swap<Reg, Item>(a43, a51); origami_utils::swap<Reg, Item>(a59, a67); origami_utils::swap<Reg, Item>(a75, a83); origami_utils::swap<Reg, Item>(a91, a99); origami_utils::swap<Reg, Item>(a107, a115); origami_utils::swap<Reg, Item>(a7, a71); origami_utils::swap<Reg, Item>(a39, a103); origami_utils::swap<Reg, Item>(a39, a71); origami_utils::swap<Reg, Item>(a23, a87); origami_utils::swap<Reg, Item>(a55, a119); origami_utils::swap<Reg, Item>(a55, a87); origami_utils::swap<Reg, Item>(a23, a39); origami_utils::swap<Reg, Item>(a55, a71); origami_utils::swap<Reg, Item>(a87, a103); origami_utils::swap<Reg, Item>(a15, a79); origami_utils::swap<Reg, Item>(a47, a111); origami_utils::swap<Reg, Item>(a47, a79); origami_utils::swap<Reg, Item>(a31, a95); origami_utils::swap<Reg, Item>(a63, a127); origami_utils::swap<Reg, Item>(a63, a95); origami_utils::swap<Reg, Item>(a31, a47); origami_utils::swap<Reg, Item>(a63, a79); origami_utils::swap<Reg, Item>(a95, a111); origami_utils::swap<Reg, Item>(a15, a23); origami_utils::swap<Reg, Item>(a31, a39); origami_utils::swap<Reg, Item>(a47, a55); origami_utils::swap<Reg, Item>(a63, a71); origami_utils::swap<Reg, Item>(a79, a87); origami_utils::swap<Reg, Item>(a95, a103); origami_utils::swap<Reg, Item>(a111, a119); origami_utils::swap<Reg, Item>(a7, a11); origami_utils::swap<Reg, Item>(a15, a19); origami_utils::swap<Reg, Item>(a23, a27); origami_utils::swap<Reg, Item>(a31, a35); origami_utils::swap<Reg, Item>(a39, a43); origami_utils::swap<Reg, Item>(a47, a51); origami_utils::swap<Reg, Item>(a55, a59); origami_utils::swap<Reg, Item>(a63, a67); origami_utils::swap<Reg, Item>(a71, a75); origami_utils::swap<Reg, Item>(a79, a83); origami_utils::swap<Reg, Item>(a87, a91); origami_utils::swap<Reg, Item>(a95, a99); origami_utils::swap<Reg, Item>(a103, a107); origami_utils::swap<Reg, Item>(a111, a115); origami_utils::swap<Reg, Item>(a119, a123); origami_utils::swap<Reg, Item>(a3, a5); origami_utils::swap<Reg, Item>(a7, a9); origami_utils::swap<Reg, Item>(a11, a13); origami_utils::swap<Reg, Item>(a15, a17); origami_utils::swap<Reg, Item>(a19, a21); origami_utils::swap<Reg, Item>(a23, a25); origami_utils::swap<Reg, Item>(a27, a29); origami_utils::swap<Reg, Item>(a31, a33); origami_utils::swap<Reg, Item>(a35, a37); origami_utils::swap<Reg, Item>(a39, a41); origami_utils::swap<Reg, Item>(a43, a45); origami_utils::swap<Reg, Item>(a47, a49); origami_utils::swap<Reg, Item>(a51, a53); origami_utils::swap<Reg, Item>(a55, a57); origami_utils::swap<Reg, Item>(a59, a61); origami_utils::swap<Reg, Item>(a63, a65); origami_utils::swap<Reg, Item>(a67, a69); origami_utils::swap<Reg, Item>(a71, a73); origami_utils::swap<Reg, Item>(a75, a77); origami_utils::swap<Reg, Item>(a79, a81); origami_utils::swap<Reg, Item>(a83, a85); origami_utils::swap<Reg, Item>(a87, a89); origami_utils::swap<Reg, Item>(a91, a93); origami_utils::swap<Reg, Item>(a95, a97); origami_utils::swap<Reg, Item>(a99, a101); origami_utils::swap<Reg, Item>(a103, a105); origami_utils::swap<Reg, Item>(a107, a109); origami_utils::swap<Reg, Item>(a111, a113); origami_utils::swap<Reg, Item>(a115, a117); origami_utils::swap<Reg, Item>(a119, a121); origami_utils::swap<Reg, Item>(a123, a125); origami_utils::swap<Reg, Item>(a1, a2); origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6); origami_utils::swap<Reg, Item>(a7, a8); origami_utils::swap<Reg, Item>(a9, a10); origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14); origami_utils::swap<Reg, Item>(a15, a16); origami_utils::swap<Reg, Item>(a17, a18); origami_utils::swap<Reg, Item>(a19, a20); origami_utils::swap<Reg, Item>(a21, a22); origami_utils::swap<Reg, Item>(a23, a24); origami_utils::swap<Reg, Item>(a25, a26); origami_utils::swap<Reg, Item>(a27, a28); origami_utils::swap<Reg, Item>(a29, a30); origami_utils::swap<Reg, Item>(a31, a32); origami_utils::swap<Reg, Item>(a33, a34); origami_utils::swap<Reg, Item>(a35, a36); origami_utils::swap<Reg, Item>(a37, a38); origami_utils::swap<Reg, Item>(a39, a40); origami_utils::swap<Reg, Item>(a41, a42); origami_utils::swap<Reg, Item>(a43, a44); origami_utils::swap<Reg, Item>(a45, a46); origami_utils::swap<Reg, Item>(a47, a48); origami_utils::swap<Reg, Item>(a49, a50); origami_utils::swap<Reg, Item>(a51, a52); origami_utils::swap<Reg, Item>(a53, a54); origami_utils::swap<Reg, Item>(a55, a56); origami_utils::swap<Reg, Item>(a57, a58); origami_utils::swap<Reg, Item>(a59, a60); origami_utils::swap<Reg, Item>(a61, a62); origami_utils::swap<Reg, Item>(a63, a64); origami_utils::swap<Reg, Item>(a65, a66); origami_utils::swap<Reg, Item>(a67, a68); origami_utils::swap<Reg, Item>(a69, a70); origami_utils::swap<Reg, Item>(a71, a72); origami_utils::swap<Reg, Item>(a73, a74); origami_utils::swap<Reg, Item>(a75, a76); origami_utils::swap<Reg, Item>(a77, a78); origami_utils::swap<Reg, Item>(a79, a80); origami_utils::swap<Reg, Item>(a81, a82); origami_utils::swap<Reg, Item>(a83, a84); origami_utils::swap<Reg, Item>(a85, a86); origami_utils::swap<Reg, Item>(a87, a88); origami_utils::swap<Reg, Item>(a89, a90); origami_utils::swap<Reg, Item>(a91, a92); origami_utils::swap<Reg, Item>(a93, a94); origami_utils::swap<Reg, Item>(a95, a96); origami_utils::swap<Reg, Item>(a97, a98); origami_utils::swap<Reg, Item>(a99, a100); origami_utils::swap<Reg, Item>(a101, a102); origami_utils::swap<Reg, Item>(a103, a104); origami_utils::swap<Reg, Item>(a105, a106); origami_utils::swap<Reg, Item>(a107, a108); origami_utils::swap<Reg, Item>(a109, a110); origami_utils::swap<Reg, Item>(a111, a112); origami_utils::swap<Reg, Item>(a113, a114); origami_utils::swap<Reg, Item>(a115, a116); origami_utils::swap<Reg, Item>(a117, a118); origami_utils::swap<Reg, Item>(a119, a120); origami_utils::swap<Reg, Item>(a121, a122); origami_utils::swap<Reg, Item>(a123, a124); origami_utils::swap<Reg, Item>(a125, a126);
		
		//
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		mrmerge_sort_reg<Reg, Item>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
		mrmerge_sort_reg<Reg, Item>(a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95);
		mrmerge_sort_reg<Reg, Item>(a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mrmerge256R(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15, Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31, Reg& a32, Reg& a33, Reg& a34, Reg& a35, Reg& a36, Reg& a37, Reg& a38, Reg& a39, Reg& a40, Reg& a41, Reg& a42, Reg& a43, Reg& a44, Reg& a45, Reg& a46, Reg& a47, Reg& a48, Reg& a49, Reg& a50, Reg& a51, Reg& a52, Reg& a53, Reg& a54, Reg& a55, Reg& a56, Reg& a57, Reg& a58, Reg& a59, Reg& a60, Reg& a61, Reg& a62, Reg& a63, Reg& a64, Reg& a65, Reg& a66, Reg& a67, Reg& a68, Reg& a69, Reg& a70, Reg& a71, Reg& a72, Reg& a73, Reg& a74, Reg& a75, Reg& a76, Reg& a77, Reg& a78, Reg& a79, Reg& a80, Reg& a81, Reg& a82, Reg& a83, Reg& a84, Reg& a85, Reg& a86, Reg& a87, Reg& a88, Reg& a89, Reg& a90, Reg& a91, Reg& a92, Reg& a93, Reg& a94, Reg& a95, Reg& a96, Reg& a97, Reg& a98, Reg& a99, Reg& a100, Reg& a101, Reg& a102, Reg& a103, Reg& a104, Reg& a105, Reg& a106, Reg& a107, Reg& a108, Reg& a109, Reg& a110, Reg& a111, Reg& a112, Reg& a113, Reg& a114, Reg& a115, Reg& a116, Reg& a117, Reg& a118, Reg& a119, Reg& a120, Reg& a121, Reg& a122, Reg& a123, Reg& a124, Reg& a125, Reg& a126, Reg& a127, Reg& a128, Reg& a129, Reg& a130, Reg& a131, Reg& a132, Reg& a133, Reg& a134, Reg& a135, Reg& a136, Reg& a137, Reg& a138, Reg& a139, Reg& a140, Reg& a141, Reg& a142, Reg& a143, Reg& a144, Reg& a145, Reg& a146, Reg& a147, Reg& a148, Reg& a149, Reg& a150, Reg& a151, Reg& a152, Reg& a153, Reg& a154, Reg& a155, Reg& a156, Reg& a157, Reg& a158, Reg& a159, Reg& a160, Reg& a161, Reg& a162, Reg& a163, Reg& a164, Reg& a165, Reg& a166, Reg& a167, Reg& a168, Reg& a169, Reg& a170, Reg& a171, Reg& a172, Reg& a173, Reg& a174, Reg& a175, Reg& a176, Reg& a177, Reg& a178, Reg& a179, Reg& a180, Reg& a181, Reg& a182, Reg& a183, Reg& a184, Reg& a185, Reg& a186, Reg& a187, Reg& a188, Reg& a189, Reg& a190, Reg& a191, Reg& a192, Reg& a193, Reg& a194, Reg& a195, Reg& a196, Reg& a197, Reg& a198, Reg& a199, Reg& a200, Reg& a201, Reg& a202, Reg& a203, Reg& a204, Reg& a205, Reg& a206, Reg& a207, Reg& a208, Reg& a209, Reg& a210, Reg& a211, Reg& a212, Reg& a213, Reg& a214, Reg& a215, Reg& a216, Reg& a217, Reg& a218, Reg& a219, Reg& a220, Reg& a221, Reg& a222, Reg& a223, Reg& a224, Reg& a225, Reg& a226, Reg& a227, Reg& a228, Reg& a229, Reg& a230, Reg& a231, Reg& a232, Reg& a233, Reg& a234, Reg& a235, Reg& a236, Reg& a237, Reg& a238, Reg& a239, Reg& a240, Reg& a241, Reg& a242, Reg& a243, Reg& a244, Reg& a245, Reg& a246, Reg& a247, Reg& a248, Reg& a249, Reg& a250, Reg& a251, Reg& a252, Reg& a253, Reg& a254, Reg& a255) {
		if constexpr (sizeof(Reg) / sizeof(Item) > 1) {
			origami_utils::reverse<Reg, Item>(a128); origami_utils::reverse<Reg, Item>(a129); origami_utils::reverse<Reg, Item>(a130); origami_utils::reverse<Reg, Item>(a131); origami_utils::reverse<Reg, Item>(a132); origami_utils::reverse<Reg, Item>(a133); origami_utils::reverse<Reg, Item>(a134); origami_utils::reverse<Reg, Item>(a135); origami_utils::reverse<Reg, Item>(a136); origami_utils::reverse<Reg, Item>(a137); origami_utils::reverse<Reg, Item>(a138); origami_utils::reverse<Reg, Item>(a139); origami_utils::reverse<Reg, Item>(a140); origami_utils::reverse<Reg, Item>(a141); origami_utils::reverse<Reg, Item>(a142); origami_utils::reverse<Reg, Item>(a143); origami_utils::reverse<Reg, Item>(a144); origami_utils::reverse<Reg, Item>(a145); origami_utils::reverse<Reg, Item>(a146); origami_utils::reverse<Reg, Item>(a147); origami_utils::reverse<Reg, Item>(a148); origami_utils::reverse<Reg, Item>(a149); origami_utils::reverse<Reg, Item>(a150); origami_utils::reverse<Reg, Item>(a151); origami_utils::reverse<Reg, Item>(a152); origami_utils::reverse<Reg, Item>(a153); origami_utils::reverse<Reg, Item>(a154); origami_utils::reverse<Reg, Item>(a155); origami_utils::reverse<Reg, Item>(a156); origami_utils::reverse<Reg, Item>(a157); origami_utils::reverse<Reg, Item>(a158); origami_utils::reverse<Reg, Item>(a159); origami_utils::reverse<Reg, Item>(a160); origami_utils::reverse<Reg, Item>(a161); origami_utils::reverse<Reg, Item>(a162); origami_utils::reverse<Reg, Item>(a163); origami_utils::reverse<Reg, Item>(a164); origami_utils::reverse<Reg, Item>(a165); origami_utils::reverse<Reg, Item>(a166); origami_utils::reverse<Reg, Item>(a167); origami_utils::reverse<Reg, Item>(a168); origami_utils::reverse<Reg, Item>(a169); origami_utils::reverse<Reg, Item>(a170); origami_utils::reverse<Reg, Item>(a171); origami_utils::reverse<Reg, Item>(a172); origami_utils::reverse<Reg, Item>(a173); origami_utils::reverse<Reg, Item>(a174); origami_utils::reverse<Reg, Item>(a175); origami_utils::reverse<Reg, Item>(a176); origami_utils::reverse<Reg, Item>(a177); origami_utils::reverse<Reg, Item>(a178); origami_utils::reverse<Reg, Item>(a179); origami_utils::reverse<Reg, Item>(a180); origami_utils::reverse<Reg, Item>(a181); origami_utils::reverse<Reg, Item>(a182); origami_utils::reverse<Reg, Item>(a183); origami_utils::reverse<Reg, Item>(a184); origami_utils::reverse<Reg, Item>(a185); origami_utils::reverse<Reg, Item>(a186); origami_utils::reverse<Reg, Item>(a187); origami_utils::reverse<Reg, Item>(a188); origami_utils::reverse<Reg, Item>(a189); origami_utils::reverse<Reg, Item>(a190); origami_utils::reverse<Reg, Item>(a191); origami_utils::reverse<Reg, Item>(a192); origami_utils::reverse<Reg, Item>(a193); origami_utils::reverse<Reg, Item>(a194); origami_utils::reverse<Reg, Item>(a195); origami_utils::reverse<Reg, Item>(a196); origami_utils::reverse<Reg, Item>(a197); origami_utils::reverse<Reg, Item>(a198); origami_utils::reverse<Reg, Item>(a199); origami_utils::reverse<Reg, Item>(a200); origami_utils::reverse<Reg, Item>(a201); origami_utils::reverse<Reg, Item>(a202); origami_utils::reverse<Reg, Item>(a203); origami_utils::reverse<Reg, Item>(a204); origami_utils::reverse<Reg, Item>(a205); origami_utils::reverse<Reg, Item>(a206); origami_utils::reverse<Reg, Item>(a207); origami_utils::reverse<Reg, Item>(a208); origami_utils::reverse<Reg, Item>(a209); origami_utils::reverse<Reg, Item>(a210); origami_utils::reverse<Reg, Item>(a211); origami_utils::reverse<Reg, Item>(a212); origami_utils::reverse<Reg, Item>(a213); origami_utils::reverse<Reg, Item>(a214); origami_utils::reverse<Reg, Item>(a215); origami_utils::reverse<Reg, Item>(a216); origami_utils::reverse<Reg, Item>(a217); origami_utils::reverse<Reg, Item>(a218); origami_utils::reverse<Reg, Item>(a219); origami_utils::reverse<Reg, Item>(a220); origami_utils::reverse<Reg, Item>(a221); origami_utils::reverse<Reg, Item>(a222); origami_utils::reverse<Reg, Item>(a223); origami_utils::reverse<Reg, Item>(a224); origami_utils::reverse<Reg, Item>(a225); origami_utils::reverse<Reg, Item>(a226); origami_utils::reverse<Reg, Item>(a227); origami_utils::reverse<Reg, Item>(a228); origami_utils::reverse<Reg, Item>(a229); origami_utils::reverse<Reg, Item>(a230); origami_utils::reverse<Reg, Item>(a231); origami_utils::reverse<Reg, Item>(a232); origami_utils::reverse<Reg, Item>(a233); origami_utils::reverse<Reg, Item>(a234); origami_utils::reverse<Reg, Item>(a235); origami_utils::reverse<Reg, Item>(a236); origami_utils::reverse<Reg, Item>(a237); origami_utils::reverse<Reg, Item>(a238); origami_utils::reverse<Reg, Item>(a239); origami_utils::reverse<Reg, Item>(a240); origami_utils::reverse<Reg, Item>(a241); origami_utils::reverse<Reg, Item>(a242); origami_utils::reverse<Reg, Item>(a243); origami_utils::reverse<Reg, Item>(a244); origami_utils::reverse<Reg, Item>(a245); origami_utils::reverse<Reg, Item>(a246); origami_utils::reverse<Reg, Item>(a247); origami_utils::reverse<Reg, Item>(a248); origami_utils::reverse<Reg, Item>(a249); origami_utils::reverse<Reg, Item>(a250); origami_utils::reverse<Reg, Item>(a251); origami_utils::reverse<Reg, Item>(a252); origami_utils::reverse<Reg, Item>(a253); origami_utils::reverse<Reg, Item>(a254); origami_utils::reverse<Reg, Item>(a255);
		}
		
		origami_utils::swap<Reg, Item>(a0, a128); origami_utils::swap<Reg, Item>(a64, a192); origami_utils::swap<Reg, Item>(a64, a128); origami_utils::swap<Reg, Item>(a32, a160); origami_utils::swap<Reg, Item>(a96, a224); origami_utils::swap<Reg, Item>(a96, a160); origami_utils::swap<Reg, Item>(a32, a64); origami_utils::swap<Reg, Item>(a96, a128); origami_utils::swap<Reg, Item>(a160, a192); origami_utils::swap<Reg, Item>(a16, a144); origami_utils::swap<Reg, Item>(a80, a208); origami_utils::swap<Reg, Item>(a80, a144); origami_utils::swap<Reg, Item>(a48, a176); origami_utils::swap<Reg, Item>(a112, a240); origami_utils::swap<Reg, Item>(a112, a176); origami_utils::swap<Reg, Item>(a48, a80); origami_utils::swap<Reg, Item>(a112, a144); origami_utils::swap<Reg, Item>(a176, a208); origami_utils::swap<Reg, Item>(a16, a32); origami_utils::swap<Reg, Item>(a48, a64); origami_utils::swap<Reg, Item>(a80, a96); origami_utils::swap<Reg, Item>(a112, a128); origami_utils::swap<Reg, Item>(a144, a160); origami_utils::swap<Reg, Item>(a176, a192); origami_utils::swap<Reg, Item>(a208, a224); origami_utils::swap<Reg, Item>(a8, a136); origami_utils::swap<Reg, Item>(a72, a200); origami_utils::swap<Reg, Item>(a72, a136); origami_utils::swap<Reg, Item>(a40, a168); origami_utils::swap<Reg, Item>(a104, a232); origami_utils::swap<Reg, Item>(a104, a168); origami_utils::swap<Reg, Item>(a40, a72); origami_utils::swap<Reg, Item>(a104, a136); origami_utils::swap<Reg, Item>(a168, a200); origami_utils::swap<Reg, Item>(a24, a152); origami_utils::swap<Reg, Item>(a88, a216); origami_utils::swap<Reg, Item>(a88, a152); origami_utils::swap<Reg, Item>(a56, a184); origami_utils::swap<Reg, Item>(a120, a248); origami_utils::swap<Reg, Item>(a120, a184); origami_utils::swap<Reg, Item>(a56, a88); origami_utils::swap<Reg, Item>(a120, a152); origami_utils::swap<Reg, Item>(a184, a216); origami_utils::swap<Reg, Item>(a24, a40); origami_utils::swap<Reg, Item>(a56, a72); origami_utils::swap<Reg, Item>(a88, a104); origami_utils::swap<Reg, Item>(a120, a136); origami_utils::swap<Reg, Item>(a152, a168); origami_utils::swap<Reg, Item>(a184, a200); origami_utils::swap<Reg, Item>(a216, a232); origami_utils::swap<Reg, Item>(a8, a16); origami_utils::swap<Reg, Item>(a24, a32); origami_utils::swap<Reg, Item>(a40, a48); origami_utils::swap<Reg, Item>(a56, a64); origami_utils::swap<Reg, Item>(a72, a80); origami_utils::swap<Reg, Item>(a88, a96); origami_utils::swap<Reg, Item>(a104, a112); origami_utils::swap<Reg, Item>(a120, a128); origami_utils::swap<Reg, Item>(a136, a144); origami_utils::swap<Reg, Item>(a152, a160); origami_utils::swap<Reg, Item>(a168, a176); origami_utils::swap<Reg, Item>(a184, a192); origami_utils::swap<Reg, Item>(a200, a208); origami_utils::swap<Reg, Item>(a216, a224); origami_utils::swap<Reg, Item>(a232, a240); origami_utils::swap<Reg, Item>(a4, a132); origami_utils::swap<Reg, Item>(a68, a196); origami_utils::swap<Reg, Item>(a68, a132); origami_utils::swap<Reg, Item>(a36, a164); origami_utils::swap<Reg, Item>(a100, a228); origami_utils::swap<Reg, Item>(a100, a164); origami_utils::swap<Reg, Item>(a36, a68); origami_utils::swap<Reg, Item>(a100, a132); origami_utils::swap<Reg, Item>(a164, a196); origami_utils::swap<Reg, Item>(a20, a148); origami_utils::swap<Reg, Item>(a84, a212); origami_utils::swap<Reg, Item>(a84, a148); origami_utils::swap<Reg, Item>(a52, a180); origami_utils::swap<Reg, Item>(a116, a244); origami_utils::swap<Reg, Item>(a116, a180); origami_utils::swap<Reg, Item>(a52, a84); origami_utils::swap<Reg, Item>(a116, a148); origami_utils::swap<Reg, Item>(a180, a212); origami_utils::swap<Reg, Item>(a20, a36); origami_utils::swap<Reg, Item>(a52, a68); origami_utils::swap<Reg, Item>(a84, a100); origami_utils::swap<Reg, Item>(a116, a132); origami_utils::swap<Reg, Item>(a148, a164); origami_utils::swap<Reg, Item>(a180, a196); origami_utils::swap<Reg, Item>(a212, a228); origami_utils::swap<Reg, Item>(a12, a140); origami_utils::swap<Reg, Item>(a76, a204); origami_utils::swap<Reg, Item>(a76, a140); origami_utils::swap<Reg, Item>(a44, a172); origami_utils::swap<Reg, Item>(a108, a236); origami_utils::swap<Reg, Item>(a108, a172); origami_utils::swap<Reg, Item>(a44, a76); origami_utils::swap<Reg, Item>(a108, a140); origami_utils::swap<Reg, Item>(a172, a204); origami_utils::swap<Reg, Item>(a28, a156); origami_utils::swap<Reg, Item>(a92, a220); origami_utils::swap<Reg, Item>(a92, a156); origami_utils::swap<Reg, Item>(a60, a188); origami_utils::swap<Reg, Item>(a124, a252); origami_utils::swap<Reg, Item>(a124, a188); origami_utils::swap<Reg, Item>(a60, a92); origami_utils::swap<Reg, Item>(a124, a156); origami_utils::swap<Reg, Item>(a188, a220); origami_utils::swap<Reg, Item>(a28, a44); origami_utils::swap<Reg, Item>(a60, a76); origami_utils::swap<Reg, Item>(a92, a108); origami_utils::swap<Reg, Item>(a124, a140); origami_utils::swap<Reg, Item>(a156, a172); origami_utils::swap<Reg, Item>(a188, a204); origami_utils::swap<Reg, Item>(a220, a236); origami_utils::swap<Reg, Item>(a12, a20); origami_utils::swap<Reg, Item>(a28, a36); origami_utils::swap<Reg, Item>(a44, a52); origami_utils::swap<Reg, Item>(a60, a68); origami_utils::swap<Reg, Item>(a76, a84); origami_utils::swap<Reg, Item>(a92, a100); origami_utils::swap<Reg, Item>(a108, a116); origami_utils::swap<Reg, Item>(a124, a132); origami_utils::swap<Reg, Item>(a140, a148); origami_utils::swap<Reg, Item>(a156, a164); origami_utils::swap<Reg, Item>(a172, a180); origami_utils::swap<Reg, Item>(a188, a196); origami_utils::swap<Reg, Item>(a204, a212); origami_utils::swap<Reg, Item>(a220, a228); origami_utils::swap<Reg, Item>(a236, a244); origami_utils::swap<Reg, Item>(a4, a8); origami_utils::swap<Reg, Item>(a12, a16); origami_utils::swap<Reg, Item>(a20, a24); origami_utils::swap<Reg, Item>(a28, a32); origami_utils::swap<Reg, Item>(a36, a40); origami_utils::swap<Reg, Item>(a44, a48); origami_utils::swap<Reg, Item>(a52, a56); origami_utils::swap<Reg, Item>(a60, a64); origami_utils::swap<Reg, Item>(a68, a72); origami_utils::swap<Reg, Item>(a76, a80); origami_utils::swap<Reg, Item>(a84, a88); origami_utils::swap<Reg, Item>(a92, a96); origami_utils::swap<Reg, Item>(a100, a104); origami_utils::swap<Reg, Item>(a108, a112); origami_utils::swap<Reg, Item>(a116, a120); origami_utils::swap<Reg, Item>(a124, a128); origami_utils::swap<Reg, Item>(a132, a136); origami_utils::swap<Reg, Item>(a140, a144); origami_utils::swap<Reg, Item>(a148, a152); origami_utils::swap<Reg, Item>(a156, a160); origami_utils::swap<Reg, Item>(a164, a168); origami_utils::swap<Reg, Item>(a172, a176); origami_utils::swap<Reg, Item>(a180, a184); origami_utils::swap<Reg, Item>(a188, a192); origami_utils::swap<Reg, Item>(a196, a200); origami_utils::swap<Reg, Item>(a204, a208); origami_utils::swap<Reg, Item>(a212, a216); origami_utils::swap<Reg, Item>(a220, a224); origami_utils::swap<Reg, Item>(a228, a232); origami_utils::swap<Reg, Item>(a236, a240); origami_utils::swap<Reg, Item>(a244, a248); origami_utils::swap<Reg, Item>(a2, a130); origami_utils::swap<Reg, Item>(a66, a194); origami_utils::swap<Reg, Item>(a66, a130); origami_utils::swap<Reg, Item>(a34, a162); origami_utils::swap<Reg, Item>(a98, a226); origami_utils::swap<Reg, Item>(a98, a162); origami_utils::swap<Reg, Item>(a34, a66); origami_utils::swap<Reg, Item>(a98, a130); origami_utils::swap<Reg, Item>(a162, a194); origami_utils::swap<Reg, Item>(a18, a146); origami_utils::swap<Reg, Item>(a82, a210); origami_utils::swap<Reg, Item>(a82, a146); origami_utils::swap<Reg, Item>(a50, a178); origami_utils::swap<Reg, Item>(a114, a242); origami_utils::swap<Reg, Item>(a114, a178); origami_utils::swap<Reg, Item>(a50, a82); origami_utils::swap<Reg, Item>(a114, a146); origami_utils::swap<Reg, Item>(a178, a210); origami_utils::swap<Reg, Item>(a18, a34); origami_utils::swap<Reg, Item>(a50, a66); origami_utils::swap<Reg, Item>(a82, a98); origami_utils::swap<Reg, Item>(a114, a130); origami_utils::swap<Reg, Item>(a146, a162); origami_utils::swap<Reg, Item>(a178, a194); origami_utils::swap<Reg, Item>(a210, a226); origami_utils::swap<Reg, Item>(a10, a138); origami_utils::swap<Reg, Item>(a74, a202); origami_utils::swap<Reg, Item>(a74, a138); origami_utils::swap<Reg, Item>(a42, a170); origami_utils::swap<Reg, Item>(a106, a234); origami_utils::swap<Reg, Item>(a106, a170); origami_utils::swap<Reg, Item>(a42, a74); origami_utils::swap<Reg, Item>(a106, a138); origami_utils::swap<Reg, Item>(a170, a202); origami_utils::swap<Reg, Item>(a26, a154); origami_utils::swap<Reg, Item>(a90, a218); origami_utils::swap<Reg, Item>(a90, a154); origami_utils::swap<Reg, Item>(a58, a186); origami_utils::swap<Reg, Item>(a122, a250); origami_utils::swap<Reg, Item>(a122, a186); origami_utils::swap<Reg, Item>(a58, a90); origami_utils::swap<Reg, Item>(a122, a154); origami_utils::swap<Reg, Item>(a186, a218); origami_utils::swap<Reg, Item>(a26, a42); origami_utils::swap<Reg, Item>(a58, a74); origami_utils::swap<Reg, Item>(a90, a106); origami_utils::swap<Reg, Item>(a122, a138); origami_utils::swap<Reg, Item>(a154, a170); origami_utils::swap<Reg, Item>(a186, a202); origami_utils::swap<Reg, Item>(a218, a234); origami_utils::swap<Reg, Item>(a10, a18); origami_utils::swap<Reg, Item>(a26, a34); origami_utils::swap<Reg, Item>(a42, a50); origami_utils::swap<Reg, Item>(a58, a66); origami_utils::swap<Reg, Item>(a74, a82); origami_utils::swap<Reg, Item>(a90, a98); origami_utils::swap<Reg, Item>(a106, a114); origami_utils::swap<Reg, Item>(a122, a130); origami_utils::swap<Reg, Item>(a138, a146); origami_utils::swap<Reg, Item>(a154, a162); origami_utils::swap<Reg, Item>(a170, a178); origami_utils::swap<Reg, Item>(a186, a194); origami_utils::swap<Reg, Item>(a202, a210); origami_utils::swap<Reg, Item>(a218, a226); origami_utils::swap<Reg, Item>(a234, a242); origami_utils::swap<Reg, Item>(a6, a134); origami_utils::swap<Reg, Item>(a70, a198); origami_utils::swap<Reg, Item>(a70, a134); origami_utils::swap<Reg, Item>(a38, a166); origami_utils::swap<Reg, Item>(a102, a230); origami_utils::swap<Reg, Item>(a102, a166); origami_utils::swap<Reg, Item>(a38, a70); origami_utils::swap<Reg, Item>(a102, a134); origami_utils::swap<Reg, Item>(a166, a198); origami_utils::swap<Reg, Item>(a22, a150); origami_utils::swap<Reg, Item>(a86, a214); origami_utils::swap<Reg, Item>(a86, a150); origami_utils::swap<Reg, Item>(a54, a182); origami_utils::swap<Reg, Item>(a118, a246); origami_utils::swap<Reg, Item>(a118, a182); origami_utils::swap<Reg, Item>(a54, a86); origami_utils::swap<Reg, Item>(a118, a150); origami_utils::swap<Reg, Item>(a182, a214); origami_utils::swap<Reg, Item>(a22, a38); origami_utils::swap<Reg, Item>(a54, a70); origami_utils::swap<Reg, Item>(a86, a102); origami_utils::swap<Reg, Item>(a118, a134); origami_utils::swap<Reg, Item>(a150, a166); origami_utils::swap<Reg, Item>(a182, a198); origami_utils::swap<Reg, Item>(a214, a230); origami_utils::swap<Reg, Item>(a14, a142); origami_utils::swap<Reg, Item>(a78, a206); origami_utils::swap<Reg, Item>(a78, a142); origami_utils::swap<Reg, Item>(a46, a174); origami_utils::swap<Reg, Item>(a110, a238); origami_utils::swap<Reg, Item>(a110, a174); origami_utils::swap<Reg, Item>(a46, a78); origami_utils::swap<Reg, Item>(a110, a142); origami_utils::swap<Reg, Item>(a174, a206); origami_utils::swap<Reg, Item>(a30, a158); origami_utils::swap<Reg, Item>(a94, a222); origami_utils::swap<Reg, Item>(a94, a158); origami_utils::swap<Reg, Item>(a62, a190); origami_utils::swap<Reg, Item>(a126, a254); origami_utils::swap<Reg, Item>(a126, a190); origami_utils::swap<Reg, Item>(a62, a94); origami_utils::swap<Reg, Item>(a126, a158); origami_utils::swap<Reg, Item>(a190, a222); origami_utils::swap<Reg, Item>(a30, a46); origami_utils::swap<Reg, Item>(a62, a78); origami_utils::swap<Reg, Item>(a94, a110); origami_utils::swap<Reg, Item>(a126, a142); origami_utils::swap<Reg, Item>(a158, a174); origami_utils::swap<Reg, Item>(a190, a206); origami_utils::swap<Reg, Item>(a222, a238); origami_utils::swap<Reg, Item>(a14, a22); origami_utils::swap<Reg, Item>(a30, a38); origami_utils::swap<Reg, Item>(a46, a54); origami_utils::swap<Reg, Item>(a62, a70); origami_utils::swap<Reg, Item>(a78, a86); origami_utils::swap<Reg, Item>(a94, a102); origami_utils::swap<Reg, Item>(a110, a118); origami_utils::swap<Reg, Item>(a126, a134); origami_utils::swap<Reg, Item>(a142, a150); origami_utils::swap<Reg, Item>(a158, a166); origami_utils::swap<Reg, Item>(a174, a182); origami_utils::swap<Reg, Item>(a190, a198); origami_utils::swap<Reg, Item>(a206, a214); origami_utils::swap<Reg, Item>(a222, a230); origami_utils::swap<Reg, Item>(a238, a246); origami_utils::swap<Reg, Item>(a6, a10); origami_utils::swap<Reg, Item>(a14, a18); origami_utils::swap<Reg, Item>(a22, a26); origami_utils::swap<Reg, Item>(a30, a34); origami_utils::swap<Reg, Item>(a38, a42); origami_utils::swap<Reg, Item>(a46, a50); origami_utils::swap<Reg, Item>(a54, a58); origami_utils::swap<Reg, Item>(a62, a66); origami_utils::swap<Reg, Item>(a70, a74); origami_utils::swap<Reg, Item>(a78, a82); origami_utils::swap<Reg, Item>(a86, a90); origami_utils::swap<Reg, Item>(a94, a98); origami_utils::swap<Reg, Item>(a102, a106); origami_utils::swap<Reg, Item>(a110, a114); origami_utils::swap<Reg, Item>(a118, a122); origami_utils::swap<Reg, Item>(a126, a130); origami_utils::swap<Reg, Item>(a134, a138); origami_utils::swap<Reg, Item>(a142, a146); origami_utils::swap<Reg, Item>(a150, a154); origami_utils::swap<Reg, Item>(a158, a162); origami_utils::swap<Reg, Item>(a166, a170); origami_utils::swap<Reg, Item>(a174, a178); origami_utils::swap<Reg, Item>(a182, a186); origami_utils::swap<Reg, Item>(a190, a194); origami_utils::swap<Reg, Item>(a198, a202); origami_utils::swap<Reg, Item>(a206, a210); origami_utils::swap<Reg, Item>(a214, a218); origami_utils::swap<Reg, Item>(a222, a226); origami_utils::swap<Reg, Item>(a230, a234); origami_utils::swap<Reg, Item>(a238, a242); origami_utils::swap<Reg, Item>(a246, a250); origami_utils::swap<Reg, Item>(a2, a4); origami_utils::swap<Reg, Item>(a6, a8); origami_utils::swap<Reg, Item>(a10, a12); origami_utils::swap<Reg, Item>(a14, a16); origami_utils::swap<Reg, Item>(a18, a20); origami_utils::swap<Reg, Item>(a22, a24); origami_utils::swap<Reg, Item>(a26, a28); origami_utils::swap<Reg, Item>(a30, a32); origami_utils::swap<Reg, Item>(a34, a36); origami_utils::swap<Reg, Item>(a38, a40); origami_utils::swap<Reg, Item>(a42, a44); origami_utils::swap<Reg, Item>(a46, a48); origami_utils::swap<Reg, Item>(a50, a52); origami_utils::swap<Reg, Item>(a54, a56); origami_utils::swap<Reg, Item>(a58, a60); origami_utils::swap<Reg, Item>(a62, a64); origami_utils::swap<Reg, Item>(a66, a68); origami_utils::swap<Reg, Item>(a70, a72); origami_utils::swap<Reg, Item>(a74, a76); origami_utils::swap<Reg, Item>(a78, a80); origami_utils::swap<Reg, Item>(a82, a84); origami_utils::swap<Reg, Item>(a86, a88); origami_utils::swap<Reg, Item>(a90, a92); origami_utils::swap<Reg, Item>(a94, a96); origami_utils::swap<Reg, Item>(a98, a100); origami_utils::swap<Reg, Item>(a102, a104); origami_utils::swap<Reg, Item>(a106, a108); origami_utils::swap<Reg, Item>(a110, a112); origami_utils::swap<Reg, Item>(a114, a116); origami_utils::swap<Reg, Item>(a118, a120); origami_utils::swap<Reg, Item>(a122, a124); origami_utils::swap<Reg, Item>(a126, a128); origami_utils::swap<Reg, Item>(a130, a132); origami_utils::swap<Reg, Item>(a134, a136); origami_utils::swap<Reg, Item>(a138, a140); origami_utils::swap<Reg, Item>(a142, a144); origami_utils::swap<Reg, Item>(a146, a148); origami_utils::swap<Reg, Item>(a150, a152); origami_utils::swap<Reg, Item>(a154, a156); origami_utils::swap<Reg, Item>(a158, a160); origami_utils::swap<Reg, Item>(a162, a164); origami_utils::swap<Reg, Item>(a166, a168); origami_utils::swap<Reg, Item>(a170, a172); origami_utils::swap<Reg, Item>(a174, a176); origami_utils::swap<Reg, Item>(a178, a180); origami_utils::swap<Reg, Item>(a182, a184); origami_utils::swap<Reg, Item>(a186, a188); origami_utils::swap<Reg, Item>(a190, a192); origami_utils::swap<Reg, Item>(a194, a196); origami_utils::swap<Reg, Item>(a198, a200); origami_utils::swap<Reg, Item>(a202, a204); origami_utils::swap<Reg, Item>(a206, a208); origami_utils::swap<Reg, Item>(a210, a212); origami_utils::swap<Reg, Item>(a214, a216); origami_utils::swap<Reg, Item>(a218, a220); origami_utils::swap<Reg, Item>(a222, a224); origami_utils::swap<Reg, Item>(a226, a228); origami_utils::swap<Reg, Item>(a230, a232); origami_utils::swap<Reg, Item>(a234, a236); origami_utils::swap<Reg, Item>(a238, a240); origami_utils::swap<Reg, Item>(a242, a244); origami_utils::swap<Reg, Item>(a246, a248); origami_utils::swap<Reg, Item>(a250, a252); origami_utils::swap<Reg, Item>(a1, a129); origami_utils::swap<Reg, Item>(a65, a193); origami_utils::swap<Reg, Item>(a65, a129); origami_utils::swap<Reg, Item>(a33, a161); origami_utils::swap<Reg, Item>(a97, a225); origami_utils::swap<Reg, Item>(a97, a161); origami_utils::swap<Reg, Item>(a33, a65); origami_utils::swap<Reg, Item>(a97, a129); origami_utils::swap<Reg, Item>(a161, a193); origami_utils::swap<Reg, Item>(a17, a145); origami_utils::swap<Reg, Item>(a81, a209); origami_utils::swap<Reg, Item>(a81, a145); origami_utils::swap<Reg, Item>(a49, a177); origami_utils::swap<Reg, Item>(a113, a241); origami_utils::swap<Reg, Item>(a113, a177); origami_utils::swap<Reg, Item>(a49, a81); origami_utils::swap<Reg, Item>(a113, a145); origami_utils::swap<Reg, Item>(a177, a209); origami_utils::swap<Reg, Item>(a17, a33); origami_utils::swap<Reg, Item>(a49, a65); origami_utils::swap<Reg, Item>(a81, a97); origami_utils::swap<Reg, Item>(a113, a129); origami_utils::swap<Reg, Item>(a145, a161); origami_utils::swap<Reg, Item>(a177, a193); origami_utils::swap<Reg, Item>(a209, a225); origami_utils::swap<Reg, Item>(a9, a137); origami_utils::swap<Reg, Item>(a73, a201); origami_utils::swap<Reg, Item>(a73, a137); origami_utils::swap<Reg, Item>(a41, a169); origami_utils::swap<Reg, Item>(a105, a233); origami_utils::swap<Reg, Item>(a105, a169); origami_utils::swap<Reg, Item>(a41, a73); origami_utils::swap<Reg, Item>(a105, a137); origami_utils::swap<Reg, Item>(a169, a201); origami_utils::swap<Reg, Item>(a25, a153); origami_utils::swap<Reg, Item>(a89, a217); origami_utils::swap<Reg, Item>(a89, a153); origami_utils::swap<Reg, Item>(a57, a185); origami_utils::swap<Reg, Item>(a121, a249); origami_utils::swap<Reg, Item>(a121, a185); origami_utils::swap<Reg, Item>(a57, a89); origami_utils::swap<Reg, Item>(a121, a153); origami_utils::swap<Reg, Item>(a185, a217); origami_utils::swap<Reg, Item>(a25, a41); origami_utils::swap<Reg, Item>(a57, a73); origami_utils::swap<Reg, Item>(a89, a105); origami_utils::swap<Reg, Item>(a121, a137); origami_utils::swap<Reg, Item>(a153, a169); origami_utils::swap<Reg, Item>(a185, a201); origami_utils::swap<Reg, Item>(a217, a233); origami_utils::swap<Reg, Item>(a9, a17); origami_utils::swap<Reg, Item>(a25, a33); origami_utils::swap<Reg, Item>(a41, a49); origami_utils::swap<Reg, Item>(a57, a65); origami_utils::swap<Reg, Item>(a73, a81); origami_utils::swap<Reg, Item>(a89, a97); origami_utils::swap<Reg, Item>(a105, a113); origami_utils::swap<Reg, Item>(a121, a129); origami_utils::swap<Reg, Item>(a137, a145); origami_utils::swap<Reg, Item>(a153, a161); origami_utils::swap<Reg, Item>(a169, a177); origami_utils::swap<Reg, Item>(a185, a193); origami_utils::swap<Reg, Item>(a201, a209); origami_utils::swap<Reg, Item>(a217, a225); origami_utils::swap<Reg, Item>(a233, a241); origami_utils::swap<Reg, Item>(a5, a133); origami_utils::swap<Reg, Item>(a69, a197); origami_utils::swap<Reg, Item>(a69, a133); origami_utils::swap<Reg, Item>(a37, a165); origami_utils::swap<Reg, Item>(a101, a229); origami_utils::swap<Reg, Item>(a101, a165); origami_utils::swap<Reg, Item>(a37, a69); origami_utils::swap<Reg, Item>(a101, a133); origami_utils::swap<Reg, Item>(a165, a197); origami_utils::swap<Reg, Item>(a21, a149); origami_utils::swap<Reg, Item>(a85, a213); origami_utils::swap<Reg, Item>(a85, a149); origami_utils::swap<Reg, Item>(a53, a181); origami_utils::swap<Reg, Item>(a117, a245); origami_utils::swap<Reg, Item>(a117, a181); origami_utils::swap<Reg, Item>(a53, a85); origami_utils::swap<Reg, Item>(a117, a149); origami_utils::swap<Reg, Item>(a181, a213); origami_utils::swap<Reg, Item>(a21, a37); origami_utils::swap<Reg, Item>(a53, a69); origami_utils::swap<Reg, Item>(a85, a101); origami_utils::swap<Reg, Item>(a117, a133); origami_utils::swap<Reg, Item>(a149, a165); origami_utils::swap<Reg, Item>(a181, a197); origami_utils::swap<Reg, Item>(a213, a229); origami_utils::swap<Reg, Item>(a13, a141); origami_utils::swap<Reg, Item>(a77, a205); origami_utils::swap<Reg, Item>(a77, a141); origami_utils::swap<Reg, Item>(a45, a173); origami_utils::swap<Reg, Item>(a109, a237); origami_utils::swap<Reg, Item>(a109, a173); origami_utils::swap<Reg, Item>(a45, a77); origami_utils::swap<Reg, Item>(a109, a141); origami_utils::swap<Reg, Item>(a173, a205); origami_utils::swap<Reg, Item>(a29, a157); origami_utils::swap<Reg, Item>(a93, a221); origami_utils::swap<Reg, Item>(a93, a157); origami_utils::swap<Reg, Item>(a61, a189); origami_utils::swap<Reg, Item>(a125, a253); origami_utils::swap<Reg, Item>(a125, a189); origami_utils::swap<Reg, Item>(a61, a93); origami_utils::swap<Reg, Item>(a125, a157); origami_utils::swap<Reg, Item>(a189, a221); origami_utils::swap<Reg, Item>(a29, a45); origami_utils::swap<Reg, Item>(a61, a77); origami_utils::swap<Reg, Item>(a93, a109); origami_utils::swap<Reg, Item>(a125, a141); origami_utils::swap<Reg, Item>(a157, a173); origami_utils::swap<Reg, Item>(a189, a205); origami_utils::swap<Reg, Item>(a221, a237); origami_utils::swap<Reg, Item>(a13, a21); origami_utils::swap<Reg, Item>(a29, a37); origami_utils::swap<Reg, Item>(a45, a53); origami_utils::swap<Reg, Item>(a61, a69); origami_utils::swap<Reg, Item>(a77, a85); origami_utils::swap<Reg, Item>(a93, a101); origami_utils::swap<Reg, Item>(a109, a117); origami_utils::swap<Reg, Item>(a125, a133); origami_utils::swap<Reg, Item>(a141, a149); origami_utils::swap<Reg, Item>(a157, a165); origami_utils::swap<Reg, Item>(a173, a181); origami_utils::swap<Reg, Item>(a189, a197); origami_utils::swap<Reg, Item>(a205, a213); origami_utils::swap<Reg, Item>(a221, a229); origami_utils::swap<Reg, Item>(a237, a245); origami_utils::swap<Reg, Item>(a5, a9); origami_utils::swap<Reg, Item>(a13, a17); origami_utils::swap<Reg, Item>(a21, a25); origami_utils::swap<Reg, Item>(a29, a33); origami_utils::swap<Reg, Item>(a37, a41); origami_utils::swap<Reg, Item>(a45, a49); origami_utils::swap<Reg, Item>(a53, a57); origami_utils::swap<Reg, Item>(a61, a65); origami_utils::swap<Reg, Item>(a69, a73); origami_utils::swap<Reg, Item>(a77, a81); origami_utils::swap<Reg, Item>(a85, a89); origami_utils::swap<Reg, Item>(a93, a97); origami_utils::swap<Reg, Item>(a101, a105); origami_utils::swap<Reg, Item>(a109, a113); origami_utils::swap<Reg, Item>(a117, a121); origami_utils::swap<Reg, Item>(a125, a129); origami_utils::swap<Reg, Item>(a133, a137); origami_utils::swap<Reg, Item>(a141, a145); origami_utils::swap<Reg, Item>(a149, a153); origami_utils::swap<Reg, Item>(a157, a161); origami_utils::swap<Reg, Item>(a165, a169); origami_utils::swap<Reg, Item>(a173, a177); origami_utils::swap<Reg, Item>(a181, a185); origami_utils::swap<Reg, Item>(a189, a193); origami_utils::swap<Reg, Item>(a197, a201); origami_utils::swap<Reg, Item>(a205, a209); origami_utils::swap<Reg, Item>(a213, a217); origami_utils::swap<Reg, Item>(a221, a225); origami_utils::swap<Reg, Item>(a229, a233); origami_utils::swap<Reg, Item>(a237, a241); origami_utils::swap<Reg, Item>(a245, a249); origami_utils::swap<Reg, Item>(a3, a131); origami_utils::swap<Reg, Item>(a67, a195); origami_utils::swap<Reg, Item>(a67, a131); origami_utils::swap<Reg, Item>(a35, a163); origami_utils::swap<Reg, Item>(a99, a227); origami_utils::swap<Reg, Item>(a99, a163); origami_utils::swap<Reg, Item>(a35, a67); origami_utils::swap<Reg, Item>(a99, a131); origami_utils::swap<Reg, Item>(a163, a195); origami_utils::swap<Reg, Item>(a19, a147); origami_utils::swap<Reg, Item>(a83, a211); origami_utils::swap<Reg, Item>(a83, a147); origami_utils::swap<Reg, Item>(a51, a179); origami_utils::swap<Reg, Item>(a115, a243); origami_utils::swap<Reg, Item>(a115, a179); origami_utils::swap<Reg, Item>(a51, a83); origami_utils::swap<Reg, Item>(a115, a147); origami_utils::swap<Reg, Item>(a179, a211); origami_utils::swap<Reg, Item>(a19, a35); origami_utils::swap<Reg, Item>(a51, a67); origami_utils::swap<Reg, Item>(a83, a99); origami_utils::swap<Reg, Item>(a115, a131); origami_utils::swap<Reg, Item>(a147, a163); origami_utils::swap<Reg, Item>(a179, a195); origami_utils::swap<Reg, Item>(a211, a227); origami_utils::swap<Reg, Item>(a11, a139); origami_utils::swap<Reg, Item>(a75, a203); origami_utils::swap<Reg, Item>(a75, a139); origami_utils::swap<Reg, Item>(a43, a171); origami_utils::swap<Reg, Item>(a107, a235); origami_utils::swap<Reg, Item>(a107, a171); origami_utils::swap<Reg, Item>(a43, a75); origami_utils::swap<Reg, Item>(a107, a139); origami_utils::swap<Reg, Item>(a171, a203); origami_utils::swap<Reg, Item>(a27, a155); origami_utils::swap<Reg, Item>(a91, a219); origami_utils::swap<Reg, Item>(a91, a155); origami_utils::swap<Reg, Item>(a59, a187); origami_utils::swap<Reg, Item>(a123, a251); origami_utils::swap<Reg, Item>(a123, a187); origami_utils::swap<Reg, Item>(a59, a91); origami_utils::swap<Reg, Item>(a123, a155); origami_utils::swap<Reg, Item>(a187, a219); origami_utils::swap<Reg, Item>(a27, a43); origami_utils::swap<Reg, Item>(a59, a75); origami_utils::swap<Reg, Item>(a91, a107); origami_utils::swap<Reg, Item>(a123, a139); origami_utils::swap<Reg, Item>(a155, a171); origami_utils::swap<Reg, Item>(a187, a203); origami_utils::swap<Reg, Item>(a219, a235); origami_utils::swap<Reg, Item>(a11, a19); origami_utils::swap<Reg, Item>(a27, a35); origami_utils::swap<Reg, Item>(a43, a51); origami_utils::swap<Reg, Item>(a59, a67); origami_utils::swap<Reg, Item>(a75, a83); origami_utils::swap<Reg, Item>(a91, a99); origami_utils::swap<Reg, Item>(a107, a115); origami_utils::swap<Reg, Item>(a123, a131); origami_utils::swap<Reg, Item>(a139, a147); origami_utils::swap<Reg, Item>(a155, a163); origami_utils::swap<Reg, Item>(a171, a179); origami_utils::swap<Reg, Item>(a187, a195); origami_utils::swap<Reg, Item>(a203, a211); origami_utils::swap<Reg, Item>(a219, a227); origami_utils::swap<Reg, Item>(a235, a243); origami_utils::swap<Reg, Item>(a7, a135); origami_utils::swap<Reg, Item>(a71, a199); origami_utils::swap<Reg, Item>(a71, a135); origami_utils::swap<Reg, Item>(a39, a167); origami_utils::swap<Reg, Item>(a103, a231); origami_utils::swap<Reg, Item>(a103, a167); origami_utils::swap<Reg, Item>(a39, a71); origami_utils::swap<Reg, Item>(a103, a135); origami_utils::swap<Reg, Item>(a167, a199); origami_utils::swap<Reg, Item>(a23, a151); origami_utils::swap<Reg, Item>(a87, a215); origami_utils::swap<Reg, Item>(a87, a151); origami_utils::swap<Reg, Item>(a55, a183); origami_utils::swap<Reg, Item>(a119, a247); origami_utils::swap<Reg, Item>(a119, a183); origami_utils::swap<Reg, Item>(a55, a87); origami_utils::swap<Reg, Item>(a119, a151); origami_utils::swap<Reg, Item>(a183, a215); origami_utils::swap<Reg, Item>(a23, a39); origami_utils::swap<Reg, Item>(a55, a71); origami_utils::swap<Reg, Item>(a87, a103); origami_utils::swap<Reg, Item>(a119, a135); origami_utils::swap<Reg, Item>(a151, a167); origami_utils::swap<Reg, Item>(a183, a199); origami_utils::swap<Reg, Item>(a215, a231); origami_utils::swap<Reg, Item>(a15, a143); origami_utils::swap<Reg, Item>(a79, a207); origami_utils::swap<Reg, Item>(a79, a143); origami_utils::swap<Reg, Item>(a47, a175); origami_utils::swap<Reg, Item>(a111, a239); origami_utils::swap<Reg, Item>(a111, a175); origami_utils::swap<Reg, Item>(a47, a79); origami_utils::swap<Reg, Item>(a111, a143); origami_utils::swap<Reg, Item>(a175, a207); origami_utils::swap<Reg, Item>(a31, a159); origami_utils::swap<Reg, Item>(a95, a223); origami_utils::swap<Reg, Item>(a95, a159); origami_utils::swap<Reg, Item>(a63, a191); origami_utils::swap<Reg, Item>(a127, a255); origami_utils::swap<Reg, Item>(a127, a191); origami_utils::swap<Reg, Item>(a63, a95); origami_utils::swap<Reg, Item>(a127, a159); origami_utils::swap<Reg, Item>(a191, a223); origami_utils::swap<Reg, Item>(a31, a47); origami_utils::swap<Reg, Item>(a63, a79); origami_utils::swap<Reg, Item>(a95, a111); origami_utils::swap<Reg, Item>(a127, a143); origami_utils::swap<Reg, Item>(a159, a175); origami_utils::swap<Reg, Item>(a191, a207); origami_utils::swap<Reg, Item>(a223, a239); origami_utils::swap<Reg, Item>(a15, a23); origami_utils::swap<Reg, Item>(a31, a39); origami_utils::swap<Reg, Item>(a47, a55); origami_utils::swap<Reg, Item>(a63, a71); origami_utils::swap<Reg, Item>(a79, a87); origami_utils::swap<Reg, Item>(a95, a103); origami_utils::swap<Reg, Item>(a111, a119); origami_utils::swap<Reg, Item>(a127, a135); origami_utils::swap<Reg, Item>(a143, a151); origami_utils::swap<Reg, Item>(a159, a167); origami_utils::swap<Reg, Item>(a175, a183); origami_utils::swap<Reg, Item>(a191, a199); origami_utils::swap<Reg, Item>(a207, a215); origami_utils::swap<Reg, Item>(a223, a231); origami_utils::swap<Reg, Item>(a239, a247); origami_utils::swap<Reg, Item>(a7, a11); origami_utils::swap<Reg, Item>(a15, a19); origami_utils::swap<Reg, Item>(a23, a27); origami_utils::swap<Reg, Item>(a31, a35); origami_utils::swap<Reg, Item>(a39, a43); origami_utils::swap<Reg, Item>(a47, a51); origami_utils::swap<Reg, Item>(a55, a59); origami_utils::swap<Reg, Item>(a63, a67); origami_utils::swap<Reg, Item>(a71, a75); origami_utils::swap<Reg, Item>(a79, a83); origami_utils::swap<Reg, Item>(a87, a91); origami_utils::swap<Reg, Item>(a95, a99); origami_utils::swap<Reg, Item>(a103, a107); origami_utils::swap<Reg, Item>(a111, a115); origami_utils::swap<Reg, Item>(a119, a123); origami_utils::swap<Reg, Item>(a127, a131); origami_utils::swap<Reg, Item>(a135, a139); origami_utils::swap<Reg, Item>(a143, a147); origami_utils::swap<Reg, Item>(a151, a155); origami_utils::swap<Reg, Item>(a159, a163); origami_utils::swap<Reg, Item>(a167, a171); origami_utils::swap<Reg, Item>(a175, a179); origami_utils::swap<Reg, Item>(a183, a187); origami_utils::swap<Reg, Item>(a191, a195); origami_utils::swap<Reg, Item>(a199, a203); origami_utils::swap<Reg, Item>(a207, a211); origami_utils::swap<Reg, Item>(a215, a219); origami_utils::swap<Reg, Item>(a223, a227); origami_utils::swap<Reg, Item>(a231, a235); origami_utils::swap<Reg, Item>(a239, a243); origami_utils::swap<Reg, Item>(a247, a251); origami_utils::swap<Reg, Item>(a3, a5); origami_utils::swap<Reg, Item>(a7, a9); origami_utils::swap<Reg, Item>(a11, a13); origami_utils::swap<Reg, Item>(a15, a17); origami_utils::swap<Reg, Item>(a19, a21); origami_utils::swap<Reg, Item>(a23, a25); origami_utils::swap<Reg, Item>(a27, a29); origami_utils::swap<Reg, Item>(a31, a33); origami_utils::swap<Reg, Item>(a35, a37); origami_utils::swap<Reg, Item>(a39, a41); origami_utils::swap<Reg, Item>(a43, a45); origami_utils::swap<Reg, Item>(a47, a49); origami_utils::swap<Reg, Item>(a51, a53); origami_utils::swap<Reg, Item>(a55, a57); origami_utils::swap<Reg, Item>(a59, a61); origami_utils::swap<Reg, Item>(a63, a65); origami_utils::swap<Reg, Item>(a67, a69); origami_utils::swap<Reg, Item>(a71, a73); origami_utils::swap<Reg, Item>(a75, a77); origami_utils::swap<Reg, Item>(a79, a81); origami_utils::swap<Reg, Item>(a83, a85); origami_utils::swap<Reg, Item>(a87, a89); origami_utils::swap<Reg, Item>(a91, a93); origami_utils::swap<Reg, Item>(a95, a97); origami_utils::swap<Reg, Item>(a99, a101); origami_utils::swap<Reg, Item>(a103, a105); origami_utils::swap<Reg, Item>(a107, a109); origami_utils::swap<Reg, Item>(a111, a113); origami_utils::swap<Reg, Item>(a115, a117); origami_utils::swap<Reg, Item>(a119, a121); origami_utils::swap<Reg, Item>(a123, a125); origami_utils::swap<Reg, Item>(a127, a129); origami_utils::swap<Reg, Item>(a131, a133); origami_utils::swap<Reg, Item>(a135, a137); origami_utils::swap<Reg, Item>(a139, a141); origami_utils::swap<Reg, Item>(a143, a145); origami_utils::swap<Reg, Item>(a147, a149); origami_utils::swap<Reg, Item>(a151, a153); origami_utils::swap<Reg, Item>(a155, a157); origami_utils::swap<Reg, Item>(a159, a161); origami_utils::swap<Reg, Item>(a163, a165); origami_utils::swap<Reg, Item>(a167, a169); origami_utils::swap<Reg, Item>(a171, a173); origami_utils::swap<Reg, Item>(a175, a177); origami_utils::swap<Reg, Item>(a179, a181); origami_utils::swap<Reg, Item>(a183, a185); origami_utils::swap<Reg, Item>(a187, a189); origami_utils::swap<Reg, Item>(a191, a193); origami_utils::swap<Reg, Item>(a195, a197); origami_utils::swap<Reg, Item>(a199, a201); origami_utils::swap<Reg, Item>(a203, a205); origami_utils::swap<Reg, Item>(a207, a209); origami_utils::swap<Reg, Item>(a211, a213); origami_utils::swap<Reg, Item>(a215, a217); origami_utils::swap<Reg, Item>(a219, a221); origami_utils::swap<Reg, Item>(a223, a225); origami_utils::swap<Reg, Item>(a227, a229); origami_utils::swap<Reg, Item>(a231, a233); origami_utils::swap<Reg, Item>(a235, a237); origami_utils::swap<Reg, Item>(a239, a241); origami_utils::swap<Reg, Item>(a243, a245); origami_utils::swap<Reg, Item>(a247, a249); origami_utils::swap<Reg, Item>(a251, a253); origami_utils::swap<Reg, Item>(a1, a2); origami_utils::swap<Reg, Item>(a3, a4); origami_utils::swap<Reg, Item>(a5, a6); origami_utils::swap<Reg, Item>(a7, a8); origami_utils::swap<Reg, Item>(a9, a10); origami_utils::swap<Reg, Item>(a11, a12); origami_utils::swap<Reg, Item>(a13, a14); origami_utils::swap<Reg, Item>(a15, a16); origami_utils::swap<Reg, Item>(a17, a18); origami_utils::swap<Reg, Item>(a19, a20); origami_utils::swap<Reg, Item>(a21, a22); origami_utils::swap<Reg, Item>(a23, a24); origami_utils::swap<Reg, Item>(a25, a26); origami_utils::swap<Reg, Item>(a27, a28); origami_utils::swap<Reg, Item>(a29, a30); origami_utils::swap<Reg, Item>(a31, a32); origami_utils::swap<Reg, Item>(a33, a34); origami_utils::swap<Reg, Item>(a35, a36); origami_utils::swap<Reg, Item>(a37, a38); origami_utils::swap<Reg, Item>(a39, a40); origami_utils::swap<Reg, Item>(a41, a42); origami_utils::swap<Reg, Item>(a43, a44); origami_utils::swap<Reg, Item>(a45, a46); origami_utils::swap<Reg, Item>(a47, a48); origami_utils::swap<Reg, Item>(a49, a50); origami_utils::swap<Reg, Item>(a51, a52); origami_utils::swap<Reg, Item>(a53, a54); origami_utils::swap<Reg, Item>(a55, a56); origami_utils::swap<Reg, Item>(a57, a58); origami_utils::swap<Reg, Item>(a59, a60); origami_utils::swap<Reg, Item>(a61, a62); origami_utils::swap<Reg, Item>(a63, a64); origami_utils::swap<Reg, Item>(a65, a66); origami_utils::swap<Reg, Item>(a67, a68); origami_utils::swap<Reg, Item>(a69, a70); origami_utils::swap<Reg, Item>(a71, a72); origami_utils::swap<Reg, Item>(a73, a74); origami_utils::swap<Reg, Item>(a75, a76); origami_utils::swap<Reg, Item>(a77, a78); origami_utils::swap<Reg, Item>(a79, a80); origami_utils::swap<Reg, Item>(a81, a82); origami_utils::swap<Reg, Item>(a83, a84); origami_utils::swap<Reg, Item>(a85, a86); origami_utils::swap<Reg, Item>(a87, a88); origami_utils::swap<Reg, Item>(a89, a90); origami_utils::swap<Reg, Item>(a91, a92); origami_utils::swap<Reg, Item>(a93, a94); origami_utils::swap<Reg, Item>(a95, a96); origami_utils::swap<Reg, Item>(a97, a98); origami_utils::swap<Reg, Item>(a99, a100); origami_utils::swap<Reg, Item>(a101, a102); origami_utils::swap<Reg, Item>(a103, a104); origami_utils::swap<Reg, Item>(a105, a106); origami_utils::swap<Reg, Item>(a107, a108); origami_utils::swap<Reg, Item>(a109, a110); origami_utils::swap<Reg, Item>(a111, a112); origami_utils::swap<Reg, Item>(a113, a114); origami_utils::swap<Reg, Item>(a115, a116); origami_utils::swap<Reg, Item>(a117, a118); origami_utils::swap<Reg, Item>(a119, a120); origami_utils::swap<Reg, Item>(a121, a122); origami_utils::swap<Reg, Item>(a123, a124); origami_utils::swap<Reg, Item>(a125, a126); origami_utils::swap<Reg, Item>(a127, a128); origami_utils::swap<Reg, Item>(a129, a130); origami_utils::swap<Reg, Item>(a131, a132); origami_utils::swap<Reg, Item>(a133, a134); origami_utils::swap<Reg, Item>(a135, a136); origami_utils::swap<Reg, Item>(a137, a138); origami_utils::swap<Reg, Item>(a139, a140); origami_utils::swap<Reg, Item>(a141, a142); origami_utils::swap<Reg, Item>(a143, a144); origami_utils::swap<Reg, Item>(a145, a146); origami_utils::swap<Reg, Item>(a147, a148); origami_utils::swap<Reg, Item>(a149, a150); origami_utils::swap<Reg, Item>(a151, a152); origami_utils::swap<Reg, Item>(a153, a154); origami_utils::swap<Reg, Item>(a155, a156); origami_utils::swap<Reg, Item>(a157, a158); origami_utils::swap<Reg, Item>(a159, a160); origami_utils::swap<Reg, Item>(a161, a162); origami_utils::swap<Reg, Item>(a163, a164); origami_utils::swap<Reg, Item>(a165, a166); origami_utils::swap<Reg, Item>(a167, a168); origami_utils::swap<Reg, Item>(a169, a170); origami_utils::swap<Reg, Item>(a171, a172); origami_utils::swap<Reg, Item>(a173, a174); origami_utils::swap<Reg, Item>(a175, a176); origami_utils::swap<Reg, Item>(a177, a178); origami_utils::swap<Reg, Item>(a179, a180); origami_utils::swap<Reg, Item>(a181, a182); origami_utils::swap<Reg, Item>(a183, a184); origami_utils::swap<Reg, Item>(a185, a186); origami_utils::swap<Reg, Item>(a187, a188); origami_utils::swap<Reg, Item>(a189, a190); origami_utils::swap<Reg, Item>(a191, a192); origami_utils::swap<Reg, Item>(a193, a194); origami_utils::swap<Reg, Item>(a195, a196); origami_utils::swap<Reg, Item>(a197, a198); origami_utils::swap<Reg, Item>(a199, a200); origami_utils::swap<Reg, Item>(a201, a202); origami_utils::swap<Reg, Item>(a203, a204); origami_utils::swap<Reg, Item>(a205, a206); origami_utils::swap<Reg, Item>(a207, a208); origami_utils::swap<Reg, Item>(a209, a210); origami_utils::swap<Reg, Item>(a211, a212); origami_utils::swap<Reg, Item>(a213, a214); origami_utils::swap<Reg, Item>(a215, a216); origami_utils::swap<Reg, Item>(a217, a218); origami_utils::swap<Reg, Item>(a219, a220); origami_utils::swap<Reg, Item>(a221, a222); origami_utils::swap<Reg, Item>(a223, a224); origami_utils::swap<Reg, Item>(a225, a226); origami_utils::swap<Reg, Item>(a227, a228); origami_utils::swap<Reg, Item>(a229, a230); origami_utils::swap<Reg, Item>(a231, a232); origami_utils::swap<Reg, Item>(a233, a234); origami_utils::swap<Reg, Item>(a235, a236); origami_utils::swap<Reg, Item>(a237, a238); origami_utils::swap<Reg, Item>(a239, a240); origami_utils::swap<Reg, Item>(a241, a242); origami_utils::swap<Reg, Item>(a243, a244); origami_utils::swap<Reg, Item>(a245, a246); origami_utils::swap<Reg, Item>(a247, a248); origami_utils::swap<Reg, Item>(a249, a250); origami_utils::swap<Reg, Item>(a251, a252); origami_utils::swap<Reg, Item>(a253, a254);
		
		mrmerge_sort_reg<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		mrmerge_sort_reg<Reg, Item>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
		mrmerge_sort_reg<Reg, Item>(a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95);
		mrmerge_sort_reg<Reg, Item>(a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);
		mrmerge_sort_reg<Reg, Item>(a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159);
		mrmerge_sort_reg<Reg, Item>(a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191);
		mrmerge_sort_reg<Reg, Item>(a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223);
		mrmerge_sort_reg<Reg, Item>(a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255);
	}
	// -------------------- Phase 1 sort: in-register

	template <typename Item, typename KeyType, typename ValueType>
	FORCEINLINE void sort8_kv_scalar(Item* d) {
		constexpr ui KeySize = sizeof(KeyType);
		constexpr ui ValueSize = sizeof(ValueType);
		constexpr ui ItemSize = sizeof(Item);

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

		SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
		SWAPKV2(0, 2);	SWAPKV2(1, 3);	SWAPKV2(4, 6);	SWAPKV2(5, 7);
		SWAPKV2(2, 4);	SWAPKV2(3, 5);	SWAPKV2(0, 1);	SWAPKV2(6, 7);
		SWAPKV2(2, 3);	SWAPKV2(4, 5);
		SWAPKV2(1, 4);	SWAPKV2(3, 6);
		SWAPKV2(1, 2);	SWAPKV2(3, 4);	SWAPKV2(5, 6);

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

	template <typename Item, typename KeyType, typename ValueType>
	FORCEINLINE void sort16_kv_scalar(Item* d) {
		constexpr ui KeySize = sizeof(KeyType);
		constexpr ui ValueSize = sizeof(ValueType);
		constexpr ui ItemSize = sizeof(Item);

		char* d2 = (char*)d;

		KeyType a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
		ValueType b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15;

		a0 = *(KeyType*)d2; b0 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a1 = *(KeyType*)d2; b1 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a2 = *(KeyType*)d2; b2 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a3 = *(KeyType*)d2; b3 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a4 = *(KeyType*)d2; b4 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a5 = *(KeyType*)d2; b5 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a6 = *(KeyType*)d2; b6 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a7 = *(KeyType*)d2; b7 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a8 = *(KeyType*)d2; b8 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a9 = *(KeyType*)d2; b9 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a10 = *(KeyType*)d2; b10 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a11 = *(KeyType*)d2; b11 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a12 = *(KeyType*)d2; b12 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a13 = *(KeyType*)d2; b13 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a14 = *(KeyType*)d2; b14 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a15 = *(KeyType*)d2; b15 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;

		SWAPKV2(0, 1);	SWAPKV2(2, 3);	SWAPKV2(4, 5);	SWAPKV2(6, 7);	SWAPKV2(8, 9);	SWAPKV2(10, 11);	SWAPKV2(12, 13);	SWAPKV2(14, 15);	SWAPKV2(0, 2);	SWAPKV2(4, 6);	SWAPKV2(8, 10);	SWAPKV2(12, 14);	SWAPKV2(1, 3);	SWAPKV2(5, 7);	SWAPKV2(9, 11);	SWAPKV2(13, 15);	SWAPKV2(0, 4);	SWAPKV2(8, 12);	SWAPKV2(1, 5);	SWAPKV2(9, 13);	SWAPKV2(2, 6);	SWAPKV2(10, 14);	SWAPKV2(3, 7);	SWAPKV2(11, 15);	SWAPKV2(0, 8);	SWAPKV2(1, 9);	SWAPKV2(2, 10);	SWAPKV2(3, 11);	SWAPKV2(4, 12);	SWAPKV2(5, 13);	SWAPKV2(6, 14);	SWAPKV2(7, 15);	SWAPKV2(5, 10);	SWAPKV2(6, 9);	SWAPKV2(3, 12);	SWAPKV2(13, 14);	SWAPKV2(7, 11);	SWAPKV2(1, 2);	SWAPKV2(4, 8);	SWAPKV2(1, 4);	SWAPKV2(7, 13);	SWAPKV2(2, 8);	SWAPKV2(11, 14);	SWAPKV2(2, 4);	SWAPKV2(5, 6);	SWAPKV2(9, 10);	SWAPKV2(11, 13);	SWAPKV2(3, 8);	SWAPKV2(7, 12);
		SWAPKV2(6, 8);	SWAPKV2(10, 12);	SWAPKV2(3, 5);	SWAPKV2(7, 9);	SWAPKV2(3, 4);	SWAPKV2(5, 6);	SWAPKV2(7, 8);	SWAPKV2(9, 10);	SWAPKV2(11, 12);	SWAPKV2(6, 7);	SWAPKV2(8, 9);

		d2 = (char*)d;
		*(KeyType*)d2 = a0; *((ValueType*)(d2 + KeySize)) = b0; d2 += ItemSize;
		*(KeyType*)d2 = a1; *((ValueType*)(d2 + KeySize)) = b1; d2 += ItemSize;
		*(KeyType*)d2 = a2; *((ValueType*)(d2 + KeySize)) = b2; d2 += ItemSize;
		*(KeyType*)d2 = a3; *((ValueType*)(d2 + KeySize)) = b3; d2 += ItemSize;
		*(KeyType*)d2 = a4; *((ValueType*)(d2 + KeySize)) = b4; d2 += ItemSize;
		*(KeyType*)d2 = a5; *((ValueType*)(d2 + KeySize)) = b5; d2 += ItemSize;
		*(KeyType*)d2 = a6; *((ValueType*)(d2 + KeySize)) = b6; d2 += ItemSize;
		*(KeyType*)d2 = a7; *((ValueType*)(d2 + KeySize)) = b7; d2 += ItemSize;
		*(KeyType*)d2 = a8; *((ValueType*)(d2 + KeySize)) = b8; d2 += ItemSize;
		*(KeyType*)d2 = a9; *((ValueType*)(d2 + KeySize)) = b9; d2 += ItemSize;
		*(KeyType*)d2 = a10; *((ValueType*)(d2 + KeySize)) = b10; d2 += ItemSize;
		*(KeyType*)d2 = a11; *((ValueType*)(d2 + KeySize)) = b11; d2 += ItemSize;
		*(KeyType*)d2 = a12; *((ValueType*)(d2 + KeySize)) = b12; d2 += ItemSize;
		*(KeyType*)d2 = a13; *((ValueType*)(d2 + KeySize)) = b13; d2 += ItemSize;
		*(KeyType*)d2 = a14; *((ValueType*)(d2 + KeySize)) = b14; d2 += ItemSize;
		*(KeyType*)d2 = a15; *((ValueType*)(d2 + KeySize)) = b15; d2 += ItemSize;
	}

	template <typename Item, typename KeyType, typename ValueType>
	FORCEINLINE void sort32_kv_scalar(Item* d) {
		constexpr ui KeySize = sizeof(KeyType);
		constexpr ui ValueSize = sizeof(ValueType);
		constexpr ui ItemSize = sizeof(Item);

		char* d2 = (char*)d;

		KeyType a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
		ValueType b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31;

		a0 = *(KeyType*)d2; b0 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a1 = *(KeyType*)d2; b1 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a2 = *(KeyType*)d2; b2 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a3 = *(KeyType*)d2; b3 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a4 = *(KeyType*)d2; b4 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a5 = *(KeyType*)d2; b5 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a6 = *(KeyType*)d2; b6 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a7 = *(KeyType*)d2; b7 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a8 = *(KeyType*)d2; b8 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a9 = *(KeyType*)d2; b9 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a10 = *(KeyType*)d2; b10 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a11 = *(KeyType*)d2; b11 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a12 = *(KeyType*)d2; b12 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a13 = *(KeyType*)d2; b13 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a14 = *(KeyType*)d2; b14 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a15 = *(KeyType*)d2; b15 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a16 = *(KeyType*)d2; b16 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a17 = *(KeyType*)d2; b17 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a18 = *(KeyType*)d2; b18 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a19 = *(KeyType*)d2; b19 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a20 = *(KeyType*)d2; b20 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a21 = *(KeyType*)d2; b21 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a22 = *(KeyType*)d2; b22 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a23 = *(KeyType*)d2; b23 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a24 = *(KeyType*)d2; b24 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a25 = *(KeyType*)d2; b25 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a26 = *(KeyType*)d2; b26 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a27 = *(KeyType*)d2; b27 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a28 = *(KeyType*)d2; b28 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a29 = *(KeyType*)d2; b29 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a30 = *(KeyType*)d2; b30 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a31 = *(KeyType*)d2; b31 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;

		SWAPKV2(0, 16);
		SWAPKV2(1, 17);
		SWAPKV2(2, 18);
		SWAPKV2(3, 19);
		SWAPKV2(4, 20);
		SWAPKV2(5, 21);
		SWAPKV2(6, 22);
		SWAPKV2(7, 23);
		SWAPKV2(8, 24);
		SWAPKV2(9, 25);
		SWAPKV2(10, 26);
		SWAPKV2(11, 27);
		SWAPKV2(12, 28);
		SWAPKV2(13, 29);
		SWAPKV2(14, 30);
		SWAPKV2(15, 31);
		SWAPKV2(0, 8);
		SWAPKV2(1, 9);
		SWAPKV2(2, 10);
		SWAPKV2(3, 11);
		SWAPKV2(4, 12);
		SWAPKV2(5, 13);
		SWAPKV2(6, 14);
		SWAPKV2(7, 15);
		SWAPKV2(16, 24);
		SWAPKV2(17, 25);
		SWAPKV2(18, 26);
		SWAPKV2(19, 27);
		SWAPKV2(20, 28);
		SWAPKV2(21, 29);
		SWAPKV2(22, 30);
		SWAPKV2(23, 31);
		SWAPKV2(8, 16);
		SWAPKV2(9, 17);
		SWAPKV2(10, 18);
		SWAPKV2(11, 19);
		SWAPKV2(12, 20);
		SWAPKV2(13, 21);
		SWAPKV2(14, 22);
		SWAPKV2(15, 23);
		SWAPKV2(0, 4);
		SWAPKV2(1, 5);
		SWAPKV2(2, 6);
		SWAPKV2(3, 7);
		SWAPKV2(8, 12);
		SWAPKV2(9, 13);
		SWAPKV2(10, 14);
		SWAPKV2(11, 15);
		SWAPKV2(16, 20);
		SWAPKV2(17, 21);
		SWAPKV2(18, 22);
		SWAPKV2(19, 23);
		SWAPKV2(24, 28);
		SWAPKV2(25, 29);
		SWAPKV2(26, 30);
		SWAPKV2(27, 31);
		SWAPKV2(4, 16);
		SWAPKV2(5, 17);
		SWAPKV2(6, 18);
		SWAPKV2(7, 19);
		SWAPKV2(12, 24);
		SWAPKV2(13, 25);
		SWAPKV2(14, 26);
		SWAPKV2(15, 27);
		SWAPKV2(4, 8);
		SWAPKV2(5, 9);
		SWAPKV2(6, 10);
		SWAPKV2(7, 11);
		SWAPKV2(12, 16);
		SWAPKV2(13, 17);
		SWAPKV2(14, 18);
		SWAPKV2(15, 19);
		SWAPKV2(20, 24);
		SWAPKV2(21, 25);
		SWAPKV2(22, 26);
		SWAPKV2(23, 27);
		SWAPKV2(0, 2);
		SWAPKV2(1, 3);
		SWAPKV2(4, 6);
		SWAPKV2(5, 7);
		SWAPKV2(8, 10);
		SWAPKV2(9, 11);
		SWAPKV2(12, 14);
		SWAPKV2(13, 15);
		SWAPKV2(16, 18);
		SWAPKV2(17, 19);
		SWAPKV2(20, 22);
		SWAPKV2(21, 23);
		SWAPKV2(24, 26);
		SWAPKV2(25, 27);
		SWAPKV2(28, 30);
		SWAPKV2(29, 31);
		SWAPKV2(2, 16);
		SWAPKV2(3, 17);
		SWAPKV2(6, 20);
		SWAPKV2(7, 21);
		SWAPKV2(10, 24);
		SWAPKV2(11, 25);
		SWAPKV2(14, 28);
		SWAPKV2(15, 29);
		SWAPKV2(2, 8);
		SWAPKV2(3, 9);
		SWAPKV2(6, 12);
		SWAPKV2(7, 13);
		SWAPKV2(10, 16);
		SWAPKV2(11, 17);
		SWAPKV2(14, 20);
		SWAPKV2(15, 21);
		SWAPKV2(18, 24);
		SWAPKV2(19, 25);
		SWAPKV2(22, 28);
		SWAPKV2(23, 29);
		SWAPKV2(2, 4);
		SWAPKV2(3, 5);
		SWAPKV2(6, 8);
		SWAPKV2(7, 9);
		SWAPKV2(10, 12);
		SWAPKV2(11, 13);
		SWAPKV2(14, 16);
		SWAPKV2(15, 17);
		SWAPKV2(18, 20);
		SWAPKV2(19, 21);
		SWAPKV2(22, 24);
		SWAPKV2(23, 25);
		SWAPKV2(26, 28);
		SWAPKV2(27, 29);
		SWAPKV2(0, 1);
		SWAPKV2(2, 3);
		SWAPKV2(4, 5);
		SWAPKV2(6, 7);
		SWAPKV2(8, 9);
		SWAPKV2(10, 11);
		SWAPKV2(12, 13);
		SWAPKV2(14, 15);
		SWAPKV2(16, 17);
		SWAPKV2(18, 19);
		SWAPKV2(20, 21);
		SWAPKV2(22, 23);
		SWAPKV2(24, 25);
		SWAPKV2(26, 27);
		SWAPKV2(28, 29);
		SWAPKV2(30, 31);
		SWAPKV2(1, 16);
		SWAPKV2(3, 18);
		SWAPKV2(5, 20);
		SWAPKV2(7, 22);
		SWAPKV2(9, 24);
		SWAPKV2(11, 26);
		SWAPKV2(13, 28);
		SWAPKV2(15, 30);
		SWAPKV2(1, 8);
		SWAPKV2(3, 10);
		SWAPKV2(5, 12);
		SWAPKV2(7, 14);
		SWAPKV2(9, 16);
		SWAPKV2(11, 18);
		SWAPKV2(13, 20);
		SWAPKV2(15, 22);
		SWAPKV2(17, 24);
		SWAPKV2(19, 26);
		SWAPKV2(21, 28);
		SWAPKV2(23, 30);
		SWAPKV2(1, 4);
		SWAPKV2(3, 6);
		SWAPKV2(5, 8);
		SWAPKV2(7, 10);
		SWAPKV2(9, 12);
		SWAPKV2(11, 14);
		SWAPKV2(13, 16);
		SWAPKV2(15, 18);
		SWAPKV2(17, 20);
		SWAPKV2(19, 22);
		SWAPKV2(21, 24);
		SWAPKV2(23, 26);
		SWAPKV2(25, 28);
		SWAPKV2(27, 30);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(15, 16);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		
		d2 = (char*)d;
		*(KeyType*)d2 = a0; *((ValueType*)(d2 + KeySize)) = b0; d2 += ItemSize;
		*(KeyType*)d2 = a1; *((ValueType*)(d2 + KeySize)) = b1; d2 += ItemSize;
		*(KeyType*)d2 = a2; *((ValueType*)(d2 + KeySize)) = b2; d2 += ItemSize;
		*(KeyType*)d2 = a3; *((ValueType*)(d2 + KeySize)) = b3; d2 += ItemSize;
		*(KeyType*)d2 = a4; *((ValueType*)(d2 + KeySize)) = b4; d2 += ItemSize;
		*(KeyType*)d2 = a5; *((ValueType*)(d2 + KeySize)) = b5; d2 += ItemSize;
		*(KeyType*)d2 = a6; *((ValueType*)(d2 + KeySize)) = b6; d2 += ItemSize;
		*(KeyType*)d2 = a7; *((ValueType*)(d2 + KeySize)) = b7; d2 += ItemSize;
		*(KeyType*)d2 = a8; *((ValueType*)(d2 + KeySize)) = b8; d2 += ItemSize;
		*(KeyType*)d2 = a9; *((ValueType*)(d2 + KeySize)) = b9; d2 += ItemSize;
		*(KeyType*)d2 = a10; *((ValueType*)(d2 + KeySize)) = b10; d2 += ItemSize;
		*(KeyType*)d2 = a11; *((ValueType*)(d2 + KeySize)) = b11; d2 += ItemSize;
		*(KeyType*)d2 = a12; *((ValueType*)(d2 + KeySize)) = b12; d2 += ItemSize;
		*(KeyType*)d2 = a13; *((ValueType*)(d2 + KeySize)) = b13; d2 += ItemSize;
		*(KeyType*)d2 = a14; *((ValueType*)(d2 + KeySize)) = b14; d2 += ItemSize;
		*(KeyType*)d2 = a15; *((ValueType*)(d2 + KeySize)) = b15; d2 += ItemSize;
		*(KeyType*)d2 = a16; *((ValueType*)(d2 + KeySize)) = b16; d2 += ItemSize;
		*(KeyType*)d2 = a17; *((ValueType*)(d2 + KeySize)) = b17; d2 += ItemSize;
		*(KeyType*)d2 = a18; *((ValueType*)(d2 + KeySize)) = b18; d2 += ItemSize;
		*(KeyType*)d2 = a19; *((ValueType*)(d2 + KeySize)) = b19; d2 += ItemSize;
		*(KeyType*)d2 = a20; *((ValueType*)(d2 + KeySize)) = b20; d2 += ItemSize;
		*(KeyType*)d2 = a21; *((ValueType*)(d2 + KeySize)) = b21; d2 += ItemSize;
		*(KeyType*)d2 = a22; *((ValueType*)(d2 + KeySize)) = b22; d2 += ItemSize;
		*(KeyType*)d2 = a23; *((ValueType*)(d2 + KeySize)) = b23; d2 += ItemSize;
		*(KeyType*)d2 = a24; *((ValueType*)(d2 + KeySize)) = b24; d2 += ItemSize;
		*(KeyType*)d2 = a25; *((ValueType*)(d2 + KeySize)) = b25; d2 += ItemSize;
		*(KeyType*)d2 = a26; *((ValueType*)(d2 + KeySize)) = b26; d2 += ItemSize;
		*(KeyType*)d2 = a27; *((ValueType*)(d2 + KeySize)) = b27; d2 += ItemSize;
		*(KeyType*)d2 = a28; *((ValueType*)(d2 + KeySize)) = b28; d2 += ItemSize;
		*(KeyType*)d2 = a29; *((ValueType*)(d2 + KeySize)) = b29; d2 += ItemSize;
		*(KeyType*)d2 = a30; *((ValueType*)(d2 + KeySize)) = b30; d2 += ItemSize;
		*(KeyType*)d2 = a31; *((ValueType*)(d2 + KeySize)) = b31; d2 += ItemSize;
	}

	template <typename Item, typename KeyType, typename ValueType>
	FORCEINLINE void sort64_kv_scalar(Item* d) {
		constexpr ui KeySize = sizeof(KeyType);
		constexpr ui ValueSize = sizeof(ValueType);
		constexpr ui ItemSize = sizeof(Item);

		char* d2 = (char*)d;

		KeyType a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63;
		ValueType b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31, b32, b33, b34, b35, b36, b37, b38, b39, b40, b41, b42, b43, b44, b45, b46, b47, b48, b49, b50, b51, b52, b53, b54, b55, b56, b57, b58, b59, b60, b61, b62, b63;

		a0 = *(KeyType*)d2; b0 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a1 = *(KeyType*)d2; b1 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a2 = *(KeyType*)d2; b2 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a3 = *(KeyType*)d2; b3 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a4 = *(KeyType*)d2; b4 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a5 = *(KeyType*)d2; b5 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a6 = *(KeyType*)d2; b6 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a7 = *(KeyType*)d2; b7 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a8 = *(KeyType*)d2; b8 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a9 = *(KeyType*)d2; b9 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a10 = *(KeyType*)d2; b10 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a11 = *(KeyType*)d2; b11 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a12 = *(KeyType*)d2; b12 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a13 = *(KeyType*)d2; b13 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a14 = *(KeyType*)d2; b14 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a15 = *(KeyType*)d2; b15 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a16 = *(KeyType*)d2; b16 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a17 = *(KeyType*)d2; b17 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a18 = *(KeyType*)d2; b18 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a19 = *(KeyType*)d2; b19 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a20 = *(KeyType*)d2; b20 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a21 = *(KeyType*)d2; b21 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a22 = *(KeyType*)d2; b22 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a23 = *(KeyType*)d2; b23 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a24 = *(KeyType*)d2; b24 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a25 = *(KeyType*)d2; b25 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a26 = *(KeyType*)d2; b26 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a27 = *(KeyType*)d2; b27 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a28 = *(KeyType*)d2; b28 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a29 = *(KeyType*)d2; b29 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a30 = *(KeyType*)d2; b30 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a31 = *(KeyType*)d2; b31 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a32 = *(KeyType*)d2; b32 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a33 = *(KeyType*)d2; b33 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a34 = *(KeyType*)d2; b34 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a35 = *(KeyType*)d2; b35 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a36 = *(KeyType*)d2; b36 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a37 = *(KeyType*)d2; b37 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a38 = *(KeyType*)d2; b38 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a39 = *(KeyType*)d2; b39 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a40 = *(KeyType*)d2; b40 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a41 = *(KeyType*)d2; b41 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a42 = *(KeyType*)d2; b42 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a43 = *(KeyType*)d2; b43 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a44 = *(KeyType*)d2; b44 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a45 = *(KeyType*)d2; b45 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a46 = *(KeyType*)d2; b46 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a47 = *(KeyType*)d2; b47 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a48 = *(KeyType*)d2; b48 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a49 = *(KeyType*)d2; b49 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a50 = *(KeyType*)d2; b50 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a51 = *(KeyType*)d2; b51 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a52 = *(KeyType*)d2; b52 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a53 = *(KeyType*)d2; b53 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a54 = *(KeyType*)d2; b54 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a55 = *(KeyType*)d2; b55 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a56 = *(KeyType*)d2; b56 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a57 = *(KeyType*)d2; b57 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a58 = *(KeyType*)d2; b58 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a59 = *(KeyType*)d2; b59 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a60 = *(KeyType*)d2; b60 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a61 = *(KeyType*)d2; b61 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a62 = *(KeyType*)d2; b62 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;
		a63 = *(KeyType*)d2; b63 = *((ValueType*)(d2 + KeySize)); d2 += ItemSize;

		SWAPKV2(0, 1);
		SWAPKV2(2, 3);
		SWAPKV2(0, 2);
		SWAPKV2(1, 3);
		SWAPKV2(1, 2);
		SWAPKV2(4, 5);
		SWAPKV2(6, 7);
		SWAPKV2(4, 6);
		SWAPKV2(5, 7);
		SWAPKV2(5, 6);
		SWAPKV2(0, 4);
		SWAPKV2(2, 6);
		SWAPKV2(2, 4);
		SWAPKV2(1, 5);
		SWAPKV2(3, 7);
		SWAPKV2(3, 5);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(8, 9);
		SWAPKV2(10, 11);
		SWAPKV2(8, 10);
		SWAPKV2(9, 11);
		SWAPKV2(9, 10);
		SWAPKV2(12, 13);
		SWAPKV2(14, 15);
		SWAPKV2(12, 14);
		SWAPKV2(13, 15);
		SWAPKV2(13, 14);
		SWAPKV2(8, 12);
		SWAPKV2(10, 14);
		SWAPKV2(10, 12);
		SWAPKV2(9, 13);
		SWAPKV2(11, 15);
		SWAPKV2(11, 13);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(0, 8);
		SWAPKV2(4, 12);
		SWAPKV2(4, 8);
		SWAPKV2(2, 10);
		SWAPKV2(6, 14);
		SWAPKV2(6, 10);
		SWAPKV2(2, 4);
		SWAPKV2(6, 8);
		SWAPKV2(10, 12);
		SWAPKV2(1, 9);
		SWAPKV2(5, 13);
		SWAPKV2(5, 9);
		SWAPKV2(3, 11);
		SWAPKV2(7, 15);
		SWAPKV2(7, 11);
		SWAPKV2(3, 5);
		SWAPKV2(7, 9);
		SWAPKV2(11, 13);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(16, 17);
		SWAPKV2(18, 19);
		SWAPKV2(16, 18);
		SWAPKV2(17, 19);
		SWAPKV2(17, 18);
		SWAPKV2(20, 21);
		SWAPKV2(22, 23);
		SWAPKV2(20, 22);
		SWAPKV2(21, 23);
		SWAPKV2(21, 22);
		SWAPKV2(16, 20);
		SWAPKV2(18, 22);
		SWAPKV2(18, 20);
		SWAPKV2(17, 21);
		SWAPKV2(19, 23);
		SWAPKV2(19, 21);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(24, 25);
		SWAPKV2(26, 27);
		SWAPKV2(24, 26);
		SWAPKV2(25, 27);
		SWAPKV2(25, 26);
		SWAPKV2(28, 29);
		SWAPKV2(30, 31);
		SWAPKV2(28, 30);
		SWAPKV2(29, 31);
		SWAPKV2(29, 30);
		SWAPKV2(24, 28);
		SWAPKV2(26, 30);
		SWAPKV2(26, 28);
		SWAPKV2(25, 29);
		SWAPKV2(27, 31);
		SWAPKV2(27, 29);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(16, 24);
		SWAPKV2(20, 28);
		SWAPKV2(20, 24);
		SWAPKV2(18, 26);
		SWAPKV2(22, 30);
		SWAPKV2(22, 26);
		SWAPKV2(18, 20);
		SWAPKV2(22, 24);
		SWAPKV2(26, 28);
		SWAPKV2(17, 25);
		SWAPKV2(21, 29);
		SWAPKV2(21, 25);
		SWAPKV2(19, 27);
		SWAPKV2(23, 31);
		SWAPKV2(23, 27);
		SWAPKV2(19, 21);
		SWAPKV2(23, 25);
		SWAPKV2(27, 29);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(0, 16);
		SWAPKV2(8, 24);
		SWAPKV2(8, 16);
		SWAPKV2(4, 20);
		SWAPKV2(12, 28);
		SWAPKV2(12, 20);
		SWAPKV2(4, 8);
		SWAPKV2(12, 16);
		SWAPKV2(20, 24);
		SWAPKV2(2, 18);
		SWAPKV2(10, 26);
		SWAPKV2(10, 18);
		SWAPKV2(6, 22);
		SWAPKV2(14, 30);
		SWAPKV2(14, 22);
		SWAPKV2(6, 10);
		SWAPKV2(14, 18);
		SWAPKV2(22, 26);
		SWAPKV2(2, 4);
		SWAPKV2(6, 8);
		SWAPKV2(10, 12);
		SWAPKV2(14, 16);
		SWAPKV2(18, 20);
		SWAPKV2(22, 24);
		SWAPKV2(26, 28);
		SWAPKV2(1, 17);
		SWAPKV2(9, 25);
		SWAPKV2(9, 17);
		SWAPKV2(5, 21);
		SWAPKV2(13, 29);
		SWAPKV2(13, 21);
		SWAPKV2(5, 9);
		SWAPKV2(13, 17);
		SWAPKV2(21, 25);
		SWAPKV2(3, 19);
		SWAPKV2(11, 27);
		SWAPKV2(11, 19);
		SWAPKV2(7, 23);
		SWAPKV2(15, 31);
		SWAPKV2(15, 23);
		SWAPKV2(7, 11);
		SWAPKV2(15, 19);
		SWAPKV2(23, 27);
		SWAPKV2(3, 5);
		SWAPKV2(7, 9);
		SWAPKV2(11, 13);
		SWAPKV2(15, 17);
		SWAPKV2(19, 21);
		SWAPKV2(23, 25);
		SWAPKV2(27, 29);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(15, 16);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(32, 33);
		SWAPKV2(34, 35);
		SWAPKV2(32, 34);
		SWAPKV2(33, 35);
		SWAPKV2(33, 34);
		SWAPKV2(36, 37);
		SWAPKV2(38, 39);
		SWAPKV2(36, 38);
		SWAPKV2(37, 39);
		SWAPKV2(37, 38);
		SWAPKV2(32, 36);
		SWAPKV2(34, 38);
		SWAPKV2(34, 36);
		SWAPKV2(33, 37);
		SWAPKV2(35, 39);
		SWAPKV2(35, 37);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(40, 41);
		SWAPKV2(42, 43);
		SWAPKV2(40, 42);
		SWAPKV2(41, 43);
		SWAPKV2(41, 42);
		SWAPKV2(44, 45);
		SWAPKV2(46, 47);
		SWAPKV2(44, 46);
		SWAPKV2(45, 47);
		SWAPKV2(45, 46);
		SWAPKV2(40, 44);
		SWAPKV2(42, 46);
		SWAPKV2(42, 44);
		SWAPKV2(41, 45);
		SWAPKV2(43, 47);
		SWAPKV2(43, 45);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(32, 40);
		SWAPKV2(36, 44);
		SWAPKV2(36, 40);
		SWAPKV2(34, 42);
		SWAPKV2(38, 46);
		SWAPKV2(38, 42);
		SWAPKV2(34, 36);
		SWAPKV2(38, 40);
		SWAPKV2(42, 44);
		SWAPKV2(33, 41);
		SWAPKV2(37, 45);
		SWAPKV2(37, 41);
		SWAPKV2(35, 43);
		SWAPKV2(39, 47);
		SWAPKV2(39, 43);
		SWAPKV2(35, 37);
		SWAPKV2(39, 41);
		SWAPKV2(43, 45);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(39, 40);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(48, 49);
		SWAPKV2(50, 51);
		SWAPKV2(48, 50);
		SWAPKV2(49, 51);
		SWAPKV2(49, 50);
		SWAPKV2(52, 53);
		SWAPKV2(54, 55);
		SWAPKV2(52, 54);
		SWAPKV2(53, 55);
		SWAPKV2(53, 54);
		SWAPKV2(48, 52);
		SWAPKV2(50, 54);
		SWAPKV2(50, 52);
		SWAPKV2(49, 53);
		SWAPKV2(51, 55);
		SWAPKV2(51, 53);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(56, 57);
		SWAPKV2(58, 59);
		SWAPKV2(56, 58);
		SWAPKV2(57, 59);
		SWAPKV2(57, 58);
		SWAPKV2(60, 61);
		SWAPKV2(62, 63);
		SWAPKV2(60, 62);
		SWAPKV2(61, 63);
		SWAPKV2(61, 62);
		SWAPKV2(56, 60);
		SWAPKV2(58, 62);
		SWAPKV2(58, 60);
		SWAPKV2(57, 61);
		SWAPKV2(59, 63);
		SWAPKV2(59, 61);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
		SWAPKV2(48, 56);
		SWAPKV2(52, 60);
		SWAPKV2(52, 56);
		SWAPKV2(50, 58);
		SWAPKV2(54, 62);
		SWAPKV2(54, 58);
		SWAPKV2(50, 52);
		SWAPKV2(54, 56);
		SWAPKV2(58, 60);
		SWAPKV2(49, 57);
		SWAPKV2(53, 61);
		SWAPKV2(53, 57);
		SWAPKV2(51, 59);
		SWAPKV2(55, 63);
		SWAPKV2(55, 59);
		SWAPKV2(51, 53);
		SWAPKV2(55, 57);
		SWAPKV2(59, 61);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(55, 56);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
		SWAPKV2(32, 48);
		SWAPKV2(40, 56);
		SWAPKV2(40, 48);
		SWAPKV2(36, 52);
		SWAPKV2(44, 60);
		SWAPKV2(44, 52);
		SWAPKV2(36, 40);
		SWAPKV2(44, 48);
		SWAPKV2(52, 56);
		SWAPKV2(34, 50);
		SWAPKV2(42, 58);
		SWAPKV2(42, 50);
		SWAPKV2(38, 54);
		SWAPKV2(46, 62);
		SWAPKV2(46, 54);
		SWAPKV2(38, 42);
		SWAPKV2(46, 50);
		SWAPKV2(54, 58);
		SWAPKV2(34, 36);
		SWAPKV2(38, 40);
		SWAPKV2(42, 44);
		SWAPKV2(46, 48);
		SWAPKV2(50, 52);
		SWAPKV2(54, 56);
		SWAPKV2(58, 60);
		SWAPKV2(33, 49);
		SWAPKV2(41, 57);
		SWAPKV2(41, 49);
		SWAPKV2(37, 53);
		SWAPKV2(45, 61);
		SWAPKV2(45, 53);
		SWAPKV2(37, 41);
		SWAPKV2(45, 49);
		SWAPKV2(53, 57);
		SWAPKV2(35, 51);
		SWAPKV2(43, 59);
		SWAPKV2(43, 51);
		SWAPKV2(39, 55);
		SWAPKV2(47, 63);
		SWAPKV2(47, 55);
		SWAPKV2(39, 43);
		SWAPKV2(47, 51);
		SWAPKV2(55, 59);
		SWAPKV2(35, 37);
		SWAPKV2(39, 41);
		SWAPKV2(43, 45);
		SWAPKV2(47, 49);
		SWAPKV2(51, 53);
		SWAPKV2(55, 57);
		SWAPKV2(59, 61);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(39, 40);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(47, 48);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(55, 56);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);
		SWAPKV2(0, 32);
		SWAPKV2(16, 48);
		SWAPKV2(16, 32);
		SWAPKV2(8, 40);
		SWAPKV2(24, 56);
		SWAPKV2(24, 40);
		SWAPKV2(8, 16);
		SWAPKV2(24, 32);
		SWAPKV2(40, 48);
		SWAPKV2(4, 36);
		SWAPKV2(20, 52);
		SWAPKV2(20, 36);
		SWAPKV2(12, 44);
		SWAPKV2(28, 60);
		SWAPKV2(28, 44);
		SWAPKV2(12, 20);
		SWAPKV2(28, 36);
		SWAPKV2(44, 52);
		SWAPKV2(4, 8);
		SWAPKV2(12, 16);
		SWAPKV2(20, 24);
		SWAPKV2(28, 32);
		SWAPKV2(36, 40);
		SWAPKV2(44, 48);
		SWAPKV2(52, 56);
		SWAPKV2(2, 34);
		SWAPKV2(18, 50);
		SWAPKV2(18, 34);
		SWAPKV2(10, 42);
		SWAPKV2(26, 58);
		SWAPKV2(26, 42);
		SWAPKV2(10, 18);
		SWAPKV2(26, 34);
		SWAPKV2(42, 50);
		SWAPKV2(6, 38);
		SWAPKV2(22, 54);
		SWAPKV2(22, 38);
		SWAPKV2(14, 46);
		SWAPKV2(30, 62);
		SWAPKV2(30, 46);
		SWAPKV2(14, 22);
		SWAPKV2(30, 38);
		SWAPKV2(46, 54);
		SWAPKV2(6, 10);
		SWAPKV2(14, 18);
		SWAPKV2(22, 26);
		SWAPKV2(30, 34);
		SWAPKV2(38, 42);
		SWAPKV2(46, 50);
		SWAPKV2(54, 58);
		SWAPKV2(2, 4);
		SWAPKV2(6, 8);
		SWAPKV2(10, 12);
		SWAPKV2(14, 16);
		SWAPKV2(18, 20);
		SWAPKV2(22, 24);
		SWAPKV2(26, 28);
		SWAPKV2(30, 32);
		SWAPKV2(34, 36);
		SWAPKV2(38, 40);
		SWAPKV2(42, 44);
		SWAPKV2(46, 48);
		SWAPKV2(50, 52);
		SWAPKV2(54, 56);
		SWAPKV2(58, 60);
		SWAPKV2(1, 33);
		SWAPKV2(17, 49);
		SWAPKV2(17, 33);
		SWAPKV2(9, 41);
		SWAPKV2(25, 57);
		SWAPKV2(25, 41);
		SWAPKV2(9, 17);
		SWAPKV2(25, 33);
		SWAPKV2(41, 49);
		SWAPKV2(5, 37);
		SWAPKV2(21, 53);
		SWAPKV2(21, 37);
		SWAPKV2(13, 45);
		SWAPKV2(29, 61);
		SWAPKV2(29, 45);
		SWAPKV2(13, 21);
		SWAPKV2(29, 37);
		SWAPKV2(45, 53);
		SWAPKV2(5, 9);
		SWAPKV2(13, 17);
		SWAPKV2(21, 25);
		SWAPKV2(29, 33);
		SWAPKV2(37, 41);
		SWAPKV2(45, 49);
		SWAPKV2(53, 57);
		SWAPKV2(3, 35);
		SWAPKV2(19, 51);
		SWAPKV2(19, 35);
		SWAPKV2(11, 43);
		SWAPKV2(27, 59);
		SWAPKV2(27, 43);
		SWAPKV2(11, 19);
		SWAPKV2(27, 35);
		SWAPKV2(43, 51);
		SWAPKV2(7, 39);
		SWAPKV2(23, 55);
		SWAPKV2(23, 39);
		SWAPKV2(15, 47);
		SWAPKV2(31, 63);
		SWAPKV2(31, 47);
		SWAPKV2(15, 23);
		SWAPKV2(31, 39);
		SWAPKV2(47, 55);
		SWAPKV2(7, 11);
		SWAPKV2(15, 19);
		SWAPKV2(23, 27);
		SWAPKV2(31, 35);
		SWAPKV2(39, 43);
		SWAPKV2(47, 51);
		SWAPKV2(55, 59);
		SWAPKV2(3, 5);
		SWAPKV2(7, 9);
		SWAPKV2(11, 13);
		SWAPKV2(15, 17);
		SWAPKV2(19, 21);
		SWAPKV2(23, 25);
		SWAPKV2(27, 29);
		SWAPKV2(31, 33);
		SWAPKV2(35, 37);
		SWAPKV2(39, 41);
		SWAPKV2(43, 45);
		SWAPKV2(47, 49);
		SWAPKV2(51, 53);
		SWAPKV2(55, 57);
		SWAPKV2(59, 61);
		SWAPKV2(1, 2);
		SWAPKV2(3, 4);
		SWAPKV2(5, 6);
		SWAPKV2(7, 8);
		SWAPKV2(9, 10);
		SWAPKV2(11, 12);
		SWAPKV2(13, 14);
		SWAPKV2(15, 16);
		SWAPKV2(17, 18);
		SWAPKV2(19, 20);
		SWAPKV2(21, 22);
		SWAPKV2(23, 24);
		SWAPKV2(25, 26);
		SWAPKV2(27, 28);
		SWAPKV2(29, 30);
		SWAPKV2(31, 32);
		SWAPKV2(33, 34);
		SWAPKV2(35, 36);
		SWAPKV2(37, 38);
		SWAPKV2(39, 40);
		SWAPKV2(41, 42);
		SWAPKV2(43, 44);
		SWAPKV2(45, 46);
		SWAPKV2(47, 48);
		SWAPKV2(49, 50);
		SWAPKV2(51, 52);
		SWAPKV2(53, 54);
		SWAPKV2(55, 56);
		SWAPKV2(57, 58);
		SWAPKV2(59, 60);
		SWAPKV2(61, 62);

		d2 = (char*)d;
		*(KeyType*)d2 = a0; *((ValueType*)(d2 + KeySize)) = b0; d2 += ItemSize;
		*(KeyType*)d2 = a1; *((ValueType*)(d2 + KeySize)) = b1; d2 += ItemSize;
		*(KeyType*)d2 = a2; *((ValueType*)(d2 + KeySize)) = b2; d2 += ItemSize;
		*(KeyType*)d2 = a3; *((ValueType*)(d2 + KeySize)) = b3; d2 += ItemSize;
		*(KeyType*)d2 = a4; *((ValueType*)(d2 + KeySize)) = b4; d2 += ItemSize;
		*(KeyType*)d2 = a5; *((ValueType*)(d2 + KeySize)) = b5; d2 += ItemSize;
		*(KeyType*)d2 = a6; *((ValueType*)(d2 + KeySize)) = b6; d2 += ItemSize;
		*(KeyType*)d2 = a7; *((ValueType*)(d2 + KeySize)) = b7; d2 += ItemSize;
		*(KeyType*)d2 = a8; *((ValueType*)(d2 + KeySize)) = b8; d2 += ItemSize;
		*(KeyType*)d2 = a9; *((ValueType*)(d2 + KeySize)) = b9; d2 += ItemSize;
		*(KeyType*)d2 = a10; *((ValueType*)(d2 + KeySize)) = b10; d2 += ItemSize;
		*(KeyType*)d2 = a11; *((ValueType*)(d2 + KeySize)) = b11; d2 += ItemSize;
		*(KeyType*)d2 = a12; *((ValueType*)(d2 + KeySize)) = b12; d2 += ItemSize;
		*(KeyType*)d2 = a13; *((ValueType*)(d2 + KeySize)) = b13; d2 += ItemSize;
		*(KeyType*)d2 = a14; *((ValueType*)(d2 + KeySize)) = b14; d2 += ItemSize;
		*(KeyType*)d2 = a15; *((ValueType*)(d2 + KeySize)) = b15; d2 += ItemSize;
		*(KeyType*)d2 = a16; *((ValueType*)(d2 + KeySize)) = b16; d2 += ItemSize;
		*(KeyType*)d2 = a17; *((ValueType*)(d2 + KeySize)) = b17; d2 += ItemSize;
		*(KeyType*)d2 = a18; *((ValueType*)(d2 + KeySize)) = b18; d2 += ItemSize;
		*(KeyType*)d2 = a19; *((ValueType*)(d2 + KeySize)) = b19; d2 += ItemSize;
		*(KeyType*)d2 = a20; *((ValueType*)(d2 + KeySize)) = b20; d2 += ItemSize;
		*(KeyType*)d2 = a21; *((ValueType*)(d2 + KeySize)) = b21; d2 += ItemSize;
		*(KeyType*)d2 = a22; *((ValueType*)(d2 + KeySize)) = b22; d2 += ItemSize;
		*(KeyType*)d2 = a23; *((ValueType*)(d2 + KeySize)) = b23; d2 += ItemSize;
		*(KeyType*)d2 = a24; *((ValueType*)(d2 + KeySize)) = b24; d2 += ItemSize;
		*(KeyType*)d2 = a25; *((ValueType*)(d2 + KeySize)) = b25; d2 += ItemSize;
		*(KeyType*)d2 = a26; *((ValueType*)(d2 + KeySize)) = b26; d2 += ItemSize;
		*(KeyType*)d2 = a27; *((ValueType*)(d2 + KeySize)) = b27; d2 += ItemSize;
		*(KeyType*)d2 = a28; *((ValueType*)(d2 + KeySize)) = b28; d2 += ItemSize;
		*(KeyType*)d2 = a29; *((ValueType*)(d2 + KeySize)) = b29; d2 += ItemSize;
		*(KeyType*)d2 = a30; *((ValueType*)(d2 + KeySize)) = b30; d2 += ItemSize;
		*(KeyType*)d2 = a31; *((ValueType*)(d2 + KeySize)) = b31; d2 += ItemSize;
		*(KeyType*)d2 = a32; *((ValueType*)(d2 + KeySize)) = b32; d2 += ItemSize;
		*(KeyType*)d2 = a33; *((ValueType*)(d2 + KeySize)) = b33; d2 += ItemSize;
		*(KeyType*)d2 = a34; *((ValueType*)(d2 + KeySize)) = b34; d2 += ItemSize;
		*(KeyType*)d2 = a35; *((ValueType*)(d2 + KeySize)) = b35; d2 += ItemSize;
		*(KeyType*)d2 = a36; *((ValueType*)(d2 + KeySize)) = b36; d2 += ItemSize;
		*(KeyType*)d2 = a37; *((ValueType*)(d2 + KeySize)) = b37; d2 += ItemSize;
		*(KeyType*)d2 = a38; *((ValueType*)(d2 + KeySize)) = b38; d2 += ItemSize;
		*(KeyType*)d2 = a39; *((ValueType*)(d2 + KeySize)) = b39; d2 += ItemSize;
		*(KeyType*)d2 = a40; *((ValueType*)(d2 + KeySize)) = b40; d2 += ItemSize;
		*(KeyType*)d2 = a41; *((ValueType*)(d2 + KeySize)) = b41; d2 += ItemSize;
		*(KeyType*)d2 = a42; *((ValueType*)(d2 + KeySize)) = b42; d2 += ItemSize;
		*(KeyType*)d2 = a43; *((ValueType*)(d2 + KeySize)) = b43; d2 += ItemSize;
		*(KeyType*)d2 = a44; *((ValueType*)(d2 + KeySize)) = b44; d2 += ItemSize;
		*(KeyType*)d2 = a45; *((ValueType*)(d2 + KeySize)) = b45; d2 += ItemSize;
		*(KeyType*)d2 = a46; *((ValueType*)(d2 + KeySize)) = b46; d2 += ItemSize;
		*(KeyType*)d2 = a47; *((ValueType*)(d2 + KeySize)) = b47; d2 += ItemSize;
		*(KeyType*)d2 = a48; *((ValueType*)(d2 + KeySize)) = b48; d2 += ItemSize;
		*(KeyType*)d2 = a49; *((ValueType*)(d2 + KeySize)) = b49; d2 += ItemSize;
		*(KeyType*)d2 = a50; *((ValueType*)(d2 + KeySize)) = b50; d2 += ItemSize;
		*(KeyType*)d2 = a51; *((ValueType*)(d2 + KeySize)) = b51; d2 += ItemSize;
		*(KeyType*)d2 = a52; *((ValueType*)(d2 + KeySize)) = b52; d2 += ItemSize;
		*(KeyType*)d2 = a53; *((ValueType*)(d2 + KeySize)) = b53; d2 += ItemSize;
		*(KeyType*)d2 = a54; *((ValueType*)(d2 + KeySize)) = b54; d2 += ItemSize;
		*(KeyType*)d2 = a55; *((ValueType*)(d2 + KeySize)) = b55; d2 += ItemSize;
		*(KeyType*)d2 = a56; *((ValueType*)(d2 + KeySize)) = b56; d2 += ItemSize;
		*(KeyType*)d2 = a57; *((ValueType*)(d2 + KeySize)) = b57; d2 += ItemSize;
		*(KeyType*)d2 = a58; *((ValueType*)(d2 + KeySize)) = b58; d2 += ItemSize;
		*(KeyType*)d2 = a59; *((ValueType*)(d2 + KeySize)) = b59; d2 += ItemSize;
		*(KeyType*)d2 = a60; *((ValueType*)(d2 + KeySize)) = b60; d2 += ItemSize;
		*(KeyType*)d2 = a61; *((ValueType*)(d2 + KeySize)) = b61; d2 += ItemSize;
		*(KeyType*)d2 = a62; *((ValueType*)(d2 + KeySize)) = b62; d2 += ItemSize;
		*(KeyType*)d2 = a63; *((ValueType*)(d2 + KeySize)) = b63; d2 += ItemSize;
	}

	template <typename Item, typename Reg>
	FORCEINLINE void Sort8(Item* d, Item* o) {
		if constexpr (sizeof(Item) == sizeof(Reg)) {
			// do scalar sorting network
			Reg a0, a1, a2, a3, a4, a5, a6, a7;
			Reg* d2 = (Reg*)d, * o2 = (Reg*)o;
			origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); 
			origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
			origami_utils::store<Reg>(a0, o2 + 0); origami_utils::store<Reg>(a1, o2 + 1); origami_utils::store<Reg>(a2, o2 + 2); origami_utils::store<Reg>(a3, o2 + 3); origami_utils::store<Reg>(a4, o2 + 4); origami_utils::store<Reg>(a5, o2 + 5); origami_utils::store<Reg>(a6, o2 + 6); origami_utils::store<Reg>(a7, o2 + 7); 
		}
	}

	template <typename Item, typename Reg>
	FORCEINLINE void Sort16(Item* d, Item* o) {
		if constexpr (sizeof(Item) == sizeof(Reg)) {
			// do scalar sorting network
			Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
			Reg* d2 = (Reg*)d, *o2 = (Reg*)o;
			origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
			origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
			origami_utils::store<Reg>(a0, o2 + 0); origami_utils::store<Reg>(a1, o2 + 1); origami_utils::store<Reg>(a2, o2 + 2); origami_utils::store<Reg>(a3, o2 + 3); origami_utils::store<Reg>(a4, o2 + 4); origami_utils::store<Reg>(a5, o2 + 5); origami_utils::store<Reg>(a6, o2 + 6); origami_utils::store<Reg>(a7, o2 + 7); origami_utils::store<Reg>(a8, o2 + 8); origami_utils::store<Reg>(a9, o2 + 9); origami_utils::store<Reg>(a10, o2 + 10); origami_utils::store<Reg>(a11, o2 + 11); origami_utils::store<Reg>(a12, o2 + 12); origami_utils::store<Reg>(a13, o2 + 13); origami_utils::store<Reg>(a14, o2 + 14); origami_utils::store<Reg>(a15, o2 + 15);
		}
	}

	
	template <typename Item, typename Reg>
	FORCEINLINE void Sort32(Item* d, Item* o) {
		if constexpr (sizeof(Item) == sizeof(Reg)) {
			// do scalar sorting network
			Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
			Reg* d2 = (Reg*)d, * o2 = (Reg*)o;
			origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
			origami_sorter::in_register_sort32<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
			origami_utils::store<Reg>(a0, o2 + 0); origami_utils::store<Reg>(a1, o2 + 1); origami_utils::store<Reg>(a2, o2 + 2); origami_utils::store<Reg>(a3, o2 + 3); origami_utils::store<Reg>(a4, o2 + 4); origami_utils::store<Reg>(a5, o2 + 5); origami_utils::store<Reg>(a6, o2 + 6); origami_utils::store<Reg>(a7, o2 + 7); origami_utils::store<Reg>(a8, o2 + 8); origami_utils::store<Reg>(a9, o2 + 9); origami_utils::store<Reg>(a10, o2 + 10); origami_utils::store<Reg>(a11, o2 + 11); origami_utils::store<Reg>(a12, o2 + 12); origami_utils::store<Reg>(a13, o2 + 13); origami_utils::store<Reg>(a14, o2 + 14); origami_utils::store<Reg>(a15, o2 + 15); origami_utils::store<Reg>(a16, o2 + 16); origami_utils::store<Reg>(a17, o2 + 17); origami_utils::store<Reg>(a18, o2 + 18); origami_utils::store<Reg>(a19, o2 + 19); origami_utils::store<Reg>(a20, o2 + 20); origami_utils::store<Reg>(a21, o2 + 21); origami_utils::store<Reg>(a22, o2 + 22); origami_utils::store<Reg>(a23, o2 + 23); origami_utils::store<Reg>(a24, o2 + 24); origami_utils::store<Reg>(a25, o2 + 25); origami_utils::store<Reg>(a26, o2 + 26); origami_utils::store<Reg>(a27, o2 + 27); origami_utils::store<Reg>(a28, o2 + 28); origami_utils::store<Reg>(a29, o2 + 29); origami_utils::store<Reg>(a30, o2 + 30); origami_utils::store<Reg>(a31, o2 + 31);
		}
	}

	template <typename Item, typename Reg, int N = 64, int _SWITCH = 32>
	FORCEINLINE void phase1_sort(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {

		//origami_utils::print_registers<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		// csort 
		origami_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);			origami_sorter::in_register_sort8<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
		origami_sorter::in_register_sort8<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);	origami_sorter::in_register_sort8<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);

		// perform mcmerges until _switch point
		if constexpr (_SWITCH < 8 || _SWITCH > 128)
			ReportError("phase1_sortN: invalid _SWITCH, should be 8, 16, 32, 64, or 128");
		if constexpr (_SWITCH >= 16 && N >= 16)
			mcmerge8x8<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		if constexpr (_SWITCH >= 32 && N >= 32)
			mcmerge16x16<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		if constexpr (_SWITCH >= 64 && N >= 64)
			mcmerge32x32<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		if constexpr (_SWITCH == 128 && N >= 128)
			mcmerge64x64<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		// transpose 
		origami_utils::transpose_large<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);

		// re-arrange after transpose
		Reg _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31;
		_a0 = a0; _a1 = a1; _a2 = a2; _a3 = a3; _a4 = a4; _a5 = a5; _a6 = a6; _a7 = a7; _a8 = a8; _a9 = a9; _a10 = a10; _a11 = a11; _a12 = a12; _a13 = a13; _a14 = a14; _a15 = a15; _a16 = a16; _a17 = a17; _a18 = a18; _a19 = a19; _a20 = a20; _a21 = a21; _a22 = a22; _a23 = a23; _a24 = a24; _a25 = a25; _a26 = a26; _a27 = a27; _a28 = a28; _a29 = a29; _a30 = a30; _a31 = a31;

		constexpr ui W = sizeof(Reg) / sizeof(Item);		// items_per_reg or simd_width
		if constexpr (W > _SWITCH)
			ReportError("phase1_sortN: invalid W and _SWITCH; should be W < _SWITCH");
		if constexpr (_SWITCH == 8) {
			if constexpr (W == 2) {
				_a0 = a0; _a1 = a2; _a2 = a4; _a3 = a6; 
				_a4 = a8; _a5 = a10; _a6 = a12; _a7 = a14;
				_a8 = a1; _a9 = a3; _a10 = a5; _a11 = a7; 
				_a12 = a9; _a13 = a11; _a14 = a13; _a15 = a15;
				_a16 = a16; _a17 = a18; _a18 = a20; _a19 = a22; 
				_a20 = a24; _a21 = a26; _a22 = a28; _a23 = a30;
				_a24 = a17; _a25 = a19; _a26 = a21; _a27 = a23; 
				_a28 = a25; _a29 = a27; _a30 = a29; _a31 = a31;
			}
			else if constexpr (W == 4) {
				_a0 = a0; _a1 = a4; 
				_a2 = a8; _a3 = a12;
				_a4 = a1; _a5 = a5; 
				_a6 = a9; _a7 = a13;
				_a8 = a2; _a9 = a6; 
				_a10 = a10; _a11 = a14;
				_a12 = a3; _a13 = a7; 
				_a14 = a11; _a15 = a15;
				_a16 = a16; _a17 = a20; 
				_a18 = a24; _a19 = a28;
				_a20 = a17; _a21 = a21; 
				_a22 = a25; _a23 = a29;
				_a24 = a18; _a25 = a22;	
				_a26 = a26; _a27 = a30;
				_a28 = a19; _a29 = a23;	
				_a30 = a27; _a31 = a31;
			}
			else if constexpr (W == 8) {
				_a0 = a0; _a1 = a1; _a2 = a2; _a3 = a3; _a4 = a4; _a5 = a5; _a6 = a6; _a7 = a7; _a8 = a8; _a9 = a9; _a10 = a10; _a11 = a11; _a12 = a12; _a13 = a13; _a14 = a14; _a15 = a15; _a16 = a16; _a17 = a17; _a18 = a18; _a19 = a19; _a20 = a20; _a21 = a21; _a22 = a22; _a23 = a23; _a24 = a24; _a25 = a25; _a26 = a26; _a27 = a27; _a28 = a28; _a29 = a29; _a30 = a30; _a31 = a31;
			}
		}
		else if constexpr (_SWITCH == 16) {
			if constexpr (W == 2) {
				_a0 = a0; _a1 = a2; _a2 = a4; _a3 = a6; _a4 = a8; _a5 = a10; _a6 = a12; _a7 = a14;
				_a8 = a1; _a9 = a3; _a10 = a5; _a11 = a7; _a12 = a9; _a13 = a11; _a14 = a13; _a15 = a15;
				_a16 = a16; _a17 = a18; _a18 = a20; _a19 = a22; _a20 = a24; _a21 = a26; _a22 = a28; _a23 = a30;
				_a24 = a17; _a25 = a19; _a26 = a21; _a27 = a23; _a28 = a25; _a29 = a27; _a30 = a29; _a31 = a31;
			}
			else if constexpr (W == 4) {
				_a0 = a0; _a1 = a4; _a2 = a8; _a3 = a12;
				_a4 = a1; _a5 = a5; _a6 = a9; _a7 = a13;
				_a8 = a2; _a9 = a6; _a10 = a10; _a11 = a14;
				_a12 = a3; _a13 = a7; _a14 = a11; _a15 = a15;
				_a16 = a16; _a17 = a20; _a18 = a24; _a19 = a28;
				_a20 = a17; _a21 = a21; _a22 = a25; _a23 = a29;
				_a24 = a18; _a25 = a22;	_a26 = a26; _a27 = a30;
				_a28 = a19; _a29 = a23;	_a30 = a27; _a31 = a31;
			}
			else if constexpr (W == 8) {
				_a0 = a0; _a1 = a8;
				_a2 = a1; _a3 = a9;
				_a4 = a2; _a5 = a10;
				_a6 = a3; _a7 = a11;
				_a8 = a4; _a9 = a12;
				_a10 = a5; _a11 = a13;
				_a12 = a6; _a13 = a14;
				_a14 = a7; _a15 = a15;
				_a16 = a16; _a17 = a24;
				_a18 = a17; _a19 = a25;
				_a20 = a18; _a21 = a26;
				_a22 = a19; _a23 = a27;
				_a24 = a20; _a25 = a28;
				_a26 = a21; _a27 = a29;
				_a28 = a22; _a29 = a30;
				_a30 = a23; _a31 = a31;
			}
			else if constexpr (W == 16) {
				_a0 = a0; _a1 = a1; _a2 = a2; _a3 = a3; _a4 = a4; _a5 = a5; _a6 = a6; _a7 = a7; _a8 = a8; _a9 = a9; _a10 = a10; _a11 = a11; _a12 = a12; _a13 = a13; _a14 = a14; _a15 = a15; _a16 = a16; _a17 = a17; _a18 = a18; _a19 = a19; _a20 = a20; _a21 = a21; _a22 = a22; _a23 = a23; _a24 = a24; _a25 = a25; _a26 = a26; _a27 = a27; _a28 = a28; _a29 = a29; _a30 = a30; _a31 = a31;
			}
		}
		else if constexpr (_SWITCH == 32) {
			if constexpr (W == 2) {
				_a0 = a0; _a1 = a2; _a2 = a4; _a3 = a6; _a4 = a8; _a5 = a10; _a6 = a12; _a7 = a14; _a8 = a16; _a9 = a18; _a10 = a20; _a11 = a22; _a12 = a24; _a13 = a26; _a14 = a28; _a15 = a30;
				_a16 = a1; _a17 = a3; _a18 = a5; _a19 = a7; _a20 = a9; _a21 = a11; _a22 = a13; _a23 = a15; _a24 = a17; _a25 = a19; _a26 = a21; _a27 = a23; _a28 = a25; _a29 = a27; _a30 = a29; _a31 = a31;
			}
			else if constexpr (W == 4) {
				_a0 = a0; _a1 = a4; _a2 = a8; _a3 = a12; _a4 = a16; _a5 = a20; _a6 = a24; _a7 = a28;
				_a8 = a1; _a9 = a5; _a10 = a9; _a11 = a13; _a12 = a17; _a13 = a21; _a14 = a25; _a15 = a29;
				_a16 = a2; _a17 = a6; _a18 = a10; _a19 = a14; _a20 = a18; _a21 = a22; _a22 = a26; _a23 = a30;
				_a24 = a3; _a25 = a7; _a26 = a11; _a27 = a15; _a28 = a19; _a29 = a23; _a30 = a27; _a31 = a31;
			}
			else if constexpr (W == 8) {
				_a0 = a0; _a1 = a8; _a2 = a16; _a3 = a24;
				_a4 = a1; _a5 = a9; _a6 = a17; _a7 = a25;
				_a8 = a2; _a9 = a10; _a10 = a18; _a11 = a26;
				_a12 = a3; _a13 = a11; _a14 = a19; _a15 = a27;
				_a16 = a4; _a17 = a12; _a18 = a20; _a19 = a28;
				_a20 = a5; _a21 = a13; _a22 = a21; _a23 = a29;
				_a24 = a6; _a25 = a14; _a26 = a22; _a27 = a30;
				_a28 = a7; _a29 = a15; _a30 = a23; _a31 = a31;
			}
			else if constexpr (W == 16) {
				_a0 = a0; _a1 = a16;
				_a2 = a1; _a3 = a17;
				_a4 = a2; _a5 = a18;
				_a6 = a3; _a7 = a19;
				_a8 = a4; _a9 = a20;
				_a10 = a5; _a11 = a21;
				_a12 = a6; _a13 = a22;
				_a14 = a7; _a15 = a23;
				_a16 = a8; _a17 = a24;
				_a18 = a9; _a19 = a25;
				_a20 = a10; _a21 = a26;
				_a22 = a11; _a23 = a27;
				_a24 = a12; _a25 = a28;
				_a26 = a13; _a27 = a29;
				_a28 = a14; _a29 = a30;
				_a30 = a15; _a31 = a31;
			}
		}
		else if constexpr (_SWITCH == 64) {
			if constexpr (W == 2) {
				_a0 = a0; _a1 = a2; _a2 = a4; _a3 = a6; _a4 = a8; _a5 = a10; _a6 = a12; _a7 = a14; _a8 = a1; _a9 = a3; _a10 = a5; _a11 = a7; _a12 = a9; _a13 = a11; _a14 = a13; _a15 = a15;
				_a16 = a16; _a17 = a18; _a18 = a20; _a19 = a22; _a20 = a24; _a21 = a26; _a22 = a28; _a23 = a30; _a24 = a17; _a25 = a19; _a26 = a21; _a27 = a23; _a28 = a25; _a29 = a27; _a30 = a29; _a31 = a31;
			}
			else if constexpr (W == 4) {
				_a0 = a0; _a1 = a4; _a2 = a8; _a3 = a12; _a4 = a1; _a5 = a5; _a6 = a9; _a7 = a13; _a8 = a16; _a9 = a20; _a10 = a24; _a11 = a28; _a12 = a17; _a13 = a21; _a14 = a25; _a15 = a29;
				_a16 = a2; _a17 = a6; _a18 = a10; _a19 = a14; _a20 = a3; _a21 = a7; _a22 = a11; _a23 = a15; _a24 = a18; _a25 = a22; _a26 = a26; _a27 = a30;	_a28 = a19; _a29 = a23; _a30 = a27; _a31 = a31;
			}
			else if constexpr (W == 8) {
				_a0 = a0; _a1 = a8; _a2 = a1; _a3 = a9; _a4 = a16; _a5 = a24; _a6 = a17; _a7 = a25;
				_a8 = a2; _a9 = a10; _a10 = a3; _a11 = a11; _a12 = a18; _a13 = a26;  _a14 = a19; _a15 = a27;
				_a16 = a4; _a17 = a12; _a18 = a5; _a19 = a13; _a20 = a20; _a21 = a28;  _a22 = a21; _a23 = a29;
				_a24 = a6; _a25 = a14; _a26 = a7; _a27 = a15; _a28 = a22; _a29 = a30; _a30 = a23; _a31 = a31;
			}
			else if constexpr (W == 16) {
				_a0 = a0; _a1 = a1; _a2 = a16; _a3 = a17;
				_a4 = a2; _a5 = a3; _a6 = a18; _a7 = a19;
				_a8 = a4; _a9 = a5; _a10 = a20; _a11 = a21;
				_a12 = a6; _a13 = a7; _a14 = a22; _a15 = a23;
				_a16 = a8; _a17 = a9; _a18 = a24; _a19 = a25;
				_a20 = a10; _a21 = a11; _a22 = a26; _a23 = a27;
				_a24 = a12; _a25 = a13; _a26 = a28; _a27 = a29;
				_a28 = a14; _a29 = a15; _a30 = a30; _a31 = a31;
			}
		}
		else if constexpr (_SWITCH == 128) {
			if constexpr (W == 2) {
				_a0 = a0; _a1 = a2; _a2 = a4; _a3 = a6; _a4 = a8; _a5 = a10; _a6 = a12; _a7 = a14; _a8 = a1; _a9 = a3; _a10 = a5; _a11 = a7; _a12 = a9; _a13 = a11; _a14 = a13; _a15 = a15;
				_a16 = a16; _a17 = a18; _a18 = a20; _a19 = a22; _a20 = a24; _a21 = a26; _a22 = a28; _a23 = a30; _a24 = a17; _a25 = a19; _a26 = a21; _a27 = a23; _a28 = a25; _a29 = a27; _a30 = a29; _a31 = a31;
			}
			else if constexpr (W == 4) {
				_a0 = a0; _a1 = a4; _a2 = a8; _a3 = a12; _a4 = a1; _a5 = a5; _a6 = a9; _a7 = a13; _a8 = a16; _a9 = a20; _a10 = a24; _a11 = a28; _a12 = a17; _a13 = a21; _a14 = a25; _a15 = a29;
				_a16 = a2; _a17 = a6; _a18 = a10; _a19 = a14; _a20 = a3; _a21 = a7; _a22 = a11; _a23 = a15; _a24 = a18; _a25 = a22; _a26 = a26; _a27 = a30;	_a28 = a19; _a29 = a23; _a30 = a27; _a31 = a31;
			}
			else if constexpr (W == 8) {
				_a0 = a0; _a1 = a8; _a2 = a1; _a3 = a9; _a4 = a16; _a5 = a24; _a6 = a17; _a7 = a25;
				_a8 = a2; _a9 = a10; _a10 = a3; _a11 = a11; _a12 = a18; _a13 = a26;  _a14 = a19; _a15 = a27;
				_a16 = a4; _a17 = a12; _a18 = a5; _a19 = a13; _a20 = a20; _a21 = a28;  _a22 = a21; _a23 = a29;
				_a24 = a6; _a25 = a14; _a26 = a7; _a27 = a15; _a28 = a22; _a29 = a30; _a30 = a23; _a31 = a31;
			}
			else if constexpr (W == 16) {
				_a0 = a0; _a1 = a1; _a2 = a16; _a3 = a17;
				_a4 = a2; _a5 = a3; _a6 = a18; _a7 = a19;
				_a8 = a4; _a9 = a5; _a10 = a20; _a11 = a21;
				_a12 = a6; _a13 = a7; _a14 = a22; _a15 = a23;
				_a16 = a8; _a17 = a9; _a18 = a24; _a19 = a25;
				_a20 = a10; _a21 = a11; _a22 = a26; _a23 = a27;
				_a24 = a12; _a25 = a13; _a26 = a28; _a27 = a29;
				_a28 = a14; _a29 = a15; _a30 = a30; _a31 = a31;
			}
		}

		// perform mrmerges for the rest
		if constexpr (N > _SWITCH) {
			constexpr ui REG_PER_MERGE = _SWITCH / W;
			constexpr ui TARGET_REG = N / W;
			if constexpr (REG_PER_MERGE == 1) {
				if constexpr (TARGET_REG >= 2) mrmerge2R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 4) mrmerge4R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 8) mrmerge8R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 16) mrmerge16R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 32) mrmerge32R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
			}
			else if constexpr (REG_PER_MERGE == 2) {
				if constexpr (TARGET_REG >= 4) mrmerge4R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 8) mrmerge8R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 16) mrmerge16R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 32) mrmerge32R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
			}
			else if constexpr (REG_PER_MERGE == 4) {
				if constexpr (TARGET_REG >= 8) mrmerge8R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 16) mrmerge16R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 32) mrmerge32R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
			}
			else if constexpr (REG_PER_MERGE == 8) {
				if constexpr (TARGET_REG >= 16) mrmerge16R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
				if constexpr (TARGET_REG >= 32) mrmerge32R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
			}
			else if constexpr (REG_PER_MERGE == 16) {
				if constexpr (TARGET_REG >= 32) mrmerge32R<Reg, Item>(_a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21, _a22, _a23, _a24, _a25, _a26, _a27, _a28, _a29, _a30, _a31);
			}
		}
		a0 = _a0; a1 = _a1; a2 = _a2; a3 = _a3; a4 = _a4; a5 = _a5; a6 = _a6; a7 = _a7; a8 = _a8; a9 = _a9; a10 = _a10; a11 = _a11; a12 = _a12; a13 = _a13; a14 = _a14; a15 = _a15; a16 = _a16; a17 = _a17; a18 = _a18; a19 = _a19; a20 = _a20; a21 = _a21; a22 = _a22; a23 = _a23; a24 = _a24; a25 = _a25; a26 = _a26; a27 = _a27; a28 = _a28; a29 = _a29; a30 = _a30; a31 = _a31;

	}

	template <typename Item, typename Reg, int NREG = 32, int N = 64, int _SWITCH = 32>
	FORCEINLINE void phase1_sort(Item* d) {
		// handle scalar separately -- because we don't have upto 32 general purpose registers so want to sort8 with 8 registers etc.
		if constexpr (std::is_same<Reg, Item>::value) {		// cannot check sizeof(Reg) == sizeof(Item); false positive for SSE and 64+64
			// handle 64+64 separately
			if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
				/*constexpr ui KeySize = sizeof(i64);
				constexpr ui ValueSize = sizeof(i64);
				constexpr ui ItemSize = sizeof(Item);

				char* d2 = (char*)d;

				i64 a0, a1, a2, a3, a4, a5, a6, a7;
				i64 b0, b1, b2, b3, b4, b5, b6, b7;

				a0 = *(i64*)d2; b0 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a1 = *(i64*)d2; b1 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a2 = *(i64*)d2; b2 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a3 = *(i64*)d2; b3 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a4 = *(i64*)d2; b4 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a5 = *(i64*)d2; b5 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a6 = *(i64*)d2; b6 = *((i64*)(d2 + KeySize)); d2 += ItemSize;
				a7 = *(i64*)d2; b7 = *((i64*)(d2 + KeySize)); d2 += ItemSize;

				SWAPKV2(0, 4);	SWAPKV2(1, 5);	SWAPKV2(2, 6);	SWAPKV2(3, 7);
				SWAPKV2(0, 2);	SWAPKV2(1, 3);	SWAPKV2(4, 6);	SWAPKV2(5, 7);
				SWAPKV2(2, 4);	SWAPKV2(3, 5);	SWAPKV2(0, 1);	SWAPKV2(6, 7);
				SWAPKV2(2, 3);	SWAPKV2(4, 5);
				SWAPKV2(1, 4);	SWAPKV2(3, 6);
				SWAPKV2(1, 2);	SWAPKV2(3, 4);	SWAPKV2(5, 6);

				d2 = (char*)d;
				*(i64*)d2 = a0; *((i64*)(d2 + KeySize)) = b0; d2 += ItemSize;
				*(i64*)d2 = a1; *((i64*)(d2 + KeySize)) = b1; d2 += ItemSize;
				*(i64*)d2 = a2; *((i64*)(d2 + KeySize)) = b2; d2 += ItemSize;
				*(i64*)d2 = a3; *((i64*)(d2 + KeySize)) = b3; d2 += ItemSize;
				*(i64*)d2 = a4; *((i64*)(d2 + KeySize)) = b4; d2 += ItemSize;
				*(i64*)d2 = a5; *((i64*)(d2 + KeySize)) = b5; d2 += ItemSize;
				*(i64*)d2 = a6; *((i64*)(d2 + KeySize)) = b6; d2 += ItemSize;
				*(i64*)d2 = a7; *((i64*)(d2 + KeySize)) = b7; d2 += ItemSize;*/
				
				if constexpr (N == 8) sort8_kv_scalar<Item, i64, i64>(d);
				else if constexpr (N == 16) sort16_kv_scalar<Item, i64, i64>(d);
				else if constexpr (N == 32) sort32_kv_scalar<Item, i64, i64>(d);
				else if constexpr (N == 64) sort64_kv_scalar<Item, i64, i64>(d);
			}
			else {
				if constexpr (N == 8) {
					/*Reg a0, a1, a2, a3, a4, a5, a6, a7;
					Reg* d2 = (Reg*)d;
					origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7);
					irl_sorter::in_register_sort8<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
					origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7);*/
					register Item a0, a1, a2, a3, a4, a5, a6, a7;
					a0 = d[0]; a1 = d[1]; a2 = d[2]; a3 = d[3]; a4 = d[4]; a5 = d[5]; a6 = d[6]; a7 = d[7];
					SWAP2(0, 4); SWAP2(1, 5); SWAP2(2, 6); SWAP2(3, 7);
					SWAP2(0, 2); SWAP2(1, 3); SWAP2(4, 6); SWAP2(5, 7);
					SWAP2(2, 4); SWAP2(3, 5); 
					SWAP2(0, 1); SWAP2(2, 3); SWAP2(4, 5); SWAP2(6, 7);
					SWAP2(1, 4); SWAP2(3, 6); 
					SWAP2(1, 2); SWAP2(3, 4); SWAP2(5, 6);
					d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3; d[4] = a4; d[5] = a5; d[6] = a6; d[7] = a7;
					
				}
				else if constexpr (N == 16) {
					Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15;
					Reg* d2 = (Reg*)d;
					origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15);
					origami_sorter::in_register_sort16<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
					origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15);
				}
				else if constexpr (N == 32) {
					Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
					Reg* d2 = (Reg*)d;
					origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
					origami_sorter::in_register_sort32<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
					origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
				}
				else if constexpr (N == 64) {
					Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63;
					Reg* d2 = (Reg*)d;
					origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
					origami_utils::load<Reg>(a32, d2 + 32); origami_utils::load<Reg>(a33, d2 + 33); origami_utils::load<Reg>(a34, d2 + 34); origami_utils::load<Reg>(a35, d2 + 35); origami_utils::load<Reg>(a36, d2 + 36); origami_utils::load<Reg>(a37, d2 + 37); origami_utils::load<Reg>(a38, d2 + 38); origami_utils::load<Reg>(a39, d2 + 39); origami_utils::load<Reg>(a40, d2 + 40); origami_utils::load<Reg>(a41, d2 + 41); origami_utils::load<Reg>(a42, d2 + 42); origami_utils::load<Reg>(a43, d2 + 43); origami_utils::load<Reg>(a44, d2 + 44); origami_utils::load<Reg>(a45, d2 + 45); origami_utils::load<Reg>(a46, d2 + 46); origami_utils::load<Reg>(a47, d2 + 47); origami_utils::load<Reg>(a48, d2 + 48); origami_utils::load<Reg>(a49, d2 + 49); origami_utils::load<Reg>(a50, d2 + 50); origami_utils::load<Reg>(a51, d2 + 51); origami_utils::load<Reg>(a52, d2 + 52); origami_utils::load<Reg>(a53, d2 + 53); origami_utils::load<Reg>(a54, d2 + 54); origami_utils::load<Reg>(a55, d2 + 55); origami_utils::load<Reg>(a56, d2 + 56); origami_utils::load<Reg>(a57, d2 + 57); origami_utils::load<Reg>(a58, d2 + 58); origami_utils::load<Reg>(a59, d2 + 59); origami_utils::load<Reg>(a60, d2 + 60); origami_utils::load<Reg>(a61, d2 + 61); origami_utils::load<Reg>(a62, d2 + 62); origami_utils::load<Reg>(a63, d2 + 63);
					origami_sorter::in_register_sort64<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
					origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
					origami_utils::store<Reg>(a32, d2 + 32); origami_utils::store<Reg>(a33, d2 + 33); origami_utils::store<Reg>(a34, d2 + 34); origami_utils::store<Reg>(a35, d2 + 35); origami_utils::store<Reg>(a36, d2 + 36); origami_utils::store<Reg>(a37, d2 + 37); origami_utils::store<Reg>(a38, d2 + 38); origami_utils::store<Reg>(a39, d2 + 39); origami_utils::store<Reg>(a40, d2 + 40); origami_utils::store<Reg>(a41, d2 + 41); origami_utils::store<Reg>(a42, d2 + 42); origami_utils::store<Reg>(a43, d2 + 43); origami_utils::store<Reg>(a44, d2 + 44); origami_utils::store<Reg>(a45, d2 + 45); origami_utils::store<Reg>(a46, d2 + 46); origami_utils::store<Reg>(a47, d2 + 47); origami_utils::store<Reg>(a48, d2 + 48); origami_utils::store<Reg>(a49, d2 + 49); origami_utils::store<Reg>(a50, d2 + 50); origami_utils::store<Reg>(a51, d2 + 51); origami_utils::store<Reg>(a52, d2 + 52); origami_utils::store<Reg>(a53, d2 + 53); origami_utils::store<Reg>(a54, d2 + 54); origami_utils::store<Reg>(a55, d2 + 55); origami_utils::store<Reg>(a56, d2 + 56); origami_utils::store<Reg>(a57, d2 + 57); origami_utils::store<Reg>(a58, d2 + 58); origami_utils::store<Reg>(a59, d2 + 59); origami_utils::store<Reg>(a60, d2 + 60); origami_utils::store<Reg>(a61, d2 + 61); origami_utils::store<Reg>(a62, d2 + 62); origami_utils::store<Reg>(a63, d2 + 63);
				}
				else ReportError("phase1_sortN: invalid n for scalar");
			}
			
		}
		// vectorized
		else { 
			if constexpr (NREG == 32) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31;
				Reg* d2 = (Reg*)d;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				phase1_sort<Item, Reg, N, _SWITCH>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
			}
			else if constexpr (NREG == 64) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63;
				Reg* d2 = (Reg*)d;
				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				phase1_sort<Item, Reg, N, _SWITCH>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				
				origami_utils::load<Reg>(a32, d2 + 32); origami_utils::load<Reg>(a33, d2 + 33); origami_utils::load<Reg>(a34, d2 + 34); origami_utils::load<Reg>(a35, d2 + 35); origami_utils::load<Reg>(a36, d2 + 36); origami_utils::load<Reg>(a37, d2 + 37); origami_utils::load<Reg>(a38, d2 + 38); origami_utils::load<Reg>(a39, d2 + 39); origami_utils::load<Reg>(a40, d2 + 40); origami_utils::load<Reg>(a41, d2 + 41); origami_utils::load<Reg>(a42, d2 + 42); origami_utils::load<Reg>(a43, d2 + 43); origami_utils::load<Reg>(a44, d2 + 44); origami_utils::load<Reg>(a45, d2 + 45); origami_utils::load<Reg>(a46, d2 + 46); origami_utils::load<Reg>(a47, d2 + 47); origami_utils::load<Reg>(a48, d2 + 48); origami_utils::load<Reg>(a49, d2 + 49); origami_utils::load<Reg>(a50, d2 + 50); origami_utils::load<Reg>(a51, d2 + 51); origami_utils::load<Reg>(a52, d2 + 52); origami_utils::load<Reg>(a53, d2 + 53); origami_utils::load<Reg>(a54, d2 + 54); origami_utils::load<Reg>(a55, d2 + 55); origami_utils::load<Reg>(a56, d2 + 56); origami_utils::load<Reg>(a57, d2 + 57); origami_utils::load<Reg>(a58, d2 + 58); origami_utils::load<Reg>(a59, d2 + 59); origami_utils::load<Reg>(a60, d2 + 60); origami_utils::load<Reg>(a61, d2 + 61); origami_utils::load<Reg>(a62, d2 + 62); origami_utils::load<Reg>(a63, d2 + 63);
				phase1_sort<Item, Reg, N, _SWITCH>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);

				constexpr ui W = sizeof(Reg) / sizeof(Item);
				constexpr ui TARGET_REG = N / W;
				if constexpr (TARGET_REG == 64)
					origami_sorter::mrmerge64R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);

				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
				origami_utils::store<Reg>(a32, d2 + 32); origami_utils::store<Reg>(a33, d2 + 33); origami_utils::store<Reg>(a34, d2 + 34); origami_utils::store<Reg>(a35, d2 + 35); origami_utils::store<Reg>(a36, d2 + 36); origami_utils::store<Reg>(a37, d2 + 37); origami_utils::store<Reg>(a38, d2 + 38); origami_utils::store<Reg>(a39, d2 + 39); origami_utils::store<Reg>(a40, d2 + 40); origami_utils::store<Reg>(a41, d2 + 41); origami_utils::store<Reg>(a42, d2 + 42); origami_utils::store<Reg>(a43, d2 + 43); origami_utils::store<Reg>(a44, d2 + 44); origami_utils::store<Reg>(a45, d2 + 45); origami_utils::store<Reg>(a46, d2 + 46); origami_utils::store<Reg>(a47, d2 + 47); origami_utils::store<Reg>(a48, d2 + 48); origami_utils::store<Reg>(a49, d2 + 49); origami_utils::store<Reg>(a50, d2 + 50); origami_utils::store<Reg>(a51, d2 + 51); origami_utils::store<Reg>(a52, d2 + 52); origami_utils::store<Reg>(a53, d2 + 53); origami_utils::store<Reg>(a54, d2 + 54); origami_utils::store<Reg>(a55, d2 + 55); origami_utils::store<Reg>(a56, d2 + 56); origami_utils::store<Reg>(a57, d2 + 57); origami_utils::store<Reg>(a58, d2 + 58); origami_utils::store<Reg>(a59, d2 + 59); origami_utils::store<Reg>(a60, d2 + 60); origami_utils::store<Reg>(a61, d2 + 61); origami_utils::store<Reg>(a62, d2 + 62); origami_utils::store<Reg>(a63, d2 + 63);
			}
			else if constexpr (NREG == 128) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127;
				Reg* d2 = (Reg*)d;

				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				origami_utils::load<Reg>(a32, d2 + 32); origami_utils::load<Reg>(a33, d2 + 33); origami_utils::load<Reg>(a34, d2 + 34); origami_utils::load<Reg>(a35, d2 + 35); origami_utils::load<Reg>(a36, d2 + 36); origami_utils::load<Reg>(a37, d2 + 37); origami_utils::load<Reg>(a38, d2 + 38); origami_utils::load<Reg>(a39, d2 + 39); origami_utils::load<Reg>(a40, d2 + 40); origami_utils::load<Reg>(a41, d2 + 41); origami_utils::load<Reg>(a42, d2 + 42); origami_utils::load<Reg>(a43, d2 + 43); origami_utils::load<Reg>(a44, d2 + 44); origami_utils::load<Reg>(a45, d2 + 45); origami_utils::load<Reg>(a46, d2 + 46); origami_utils::load<Reg>(a47, d2 + 47); origami_utils::load<Reg>(a48, d2 + 48); origami_utils::load<Reg>(a49, d2 + 49); origami_utils::load<Reg>(a50, d2 + 50); origami_utils::load<Reg>(a51, d2 + 51); origami_utils::load<Reg>(a52, d2 + 52); origami_utils::load<Reg>(a53, d2 + 53); origami_utils::load<Reg>(a54, d2 + 54); origami_utils::load<Reg>(a55, d2 + 55); origami_utils::load<Reg>(a56, d2 + 56); origami_utils::load<Reg>(a57, d2 + 57); origami_utils::load<Reg>(a58, d2 + 58); origami_utils::load<Reg>(a59, d2 + 59); origami_utils::load<Reg>(a60, d2 + 60); origami_utils::load<Reg>(a61, d2 + 61); origami_utils::load<Reg>(a62, d2 + 62); origami_utils::load<Reg>(a63, d2 + 63);
				origami_utils::load<Reg>(a64, d2 + 64); origami_utils::load<Reg>(a65, d2 + 65); origami_utils::load<Reg>(a66, d2 + 66); origami_utils::load<Reg>(a67, d2 + 67); origami_utils::load<Reg>(a68, d2 + 68); origami_utils::load<Reg>(a69, d2 + 69); origami_utils::load<Reg>(a70, d2 + 70); origami_utils::load<Reg>(a71, d2 + 71); origami_utils::load<Reg>(a72, d2 + 72); origami_utils::load<Reg>(a73, d2 + 73); origami_utils::load<Reg>(a74, d2 + 74); origami_utils::load<Reg>(a75, d2 + 75); origami_utils::load<Reg>(a76, d2 + 76); origami_utils::load<Reg>(a77, d2 + 77); origami_utils::load<Reg>(a78, d2 + 78); origami_utils::load<Reg>(a79, d2 + 79); origami_utils::load<Reg>(a80, d2 + 80); origami_utils::load<Reg>(a81, d2 + 81); origami_utils::load<Reg>(a82, d2 + 82); origami_utils::load<Reg>(a83, d2 + 83); origami_utils::load<Reg>(a84, d2 + 84); origami_utils::load<Reg>(a85, d2 + 85); origami_utils::load<Reg>(a86, d2 + 86); origami_utils::load<Reg>(a87, d2 + 87); origami_utils::load<Reg>(a88, d2 + 88); origami_utils::load<Reg>(a89, d2 + 89); origami_utils::load<Reg>(a90, d2 + 90); origami_utils::load<Reg>(a91, d2 + 91); origami_utils::load<Reg>(a92, d2 + 92); origami_utils::load<Reg>(a93, d2 + 93); origami_utils::load<Reg>(a94, d2 + 94); origami_utils::load<Reg>(a95, d2 + 95);
				origami_utils::load<Reg>(a96, d2 + 96); origami_utils::load<Reg>(a97, d2 + 97); origami_utils::load<Reg>(a98, d2 + 98); origami_utils::load<Reg>(a99, d2 + 99); origami_utils::load<Reg>(a100, d2 + 100); origami_utils::load<Reg>(a101, d2 + 101); origami_utils::load<Reg>(a102, d2 + 102); origami_utils::load<Reg>(a103, d2 + 103); origami_utils::load<Reg>(a104, d2 + 104); origami_utils::load<Reg>(a105, d2 + 105); origami_utils::load<Reg>(a106, d2 + 106); origami_utils::load<Reg>(a107, d2 + 107); origami_utils::load<Reg>(a108, d2 + 108); origami_utils::load<Reg>(a109, d2 + 109); origami_utils::load<Reg>(a110, d2 + 110); origami_utils::load<Reg>(a111, d2 + 111); origami_utils::load<Reg>(a112, d2 + 112); origami_utils::load<Reg>(a113, d2 + 113); origami_utils::load<Reg>(a114, d2 + 114); origami_utils::load<Reg>(a115, d2 + 115); origami_utils::load<Reg>(a116, d2 + 116); origami_utils::load<Reg>(a117, d2 + 117); origami_utils::load<Reg>(a118, d2 + 118); origami_utils::load<Reg>(a119, d2 + 119); origami_utils::load<Reg>(a120, d2 + 120); origami_utils::load<Reg>(a121, d2 + 121); origami_utils::load<Reg>(a122, d2 + 122); origami_utils::load<Reg>(a123, d2 + 123); origami_utils::load<Reg>(a124, d2 + 124); origami_utils::load<Reg>(a125, d2 + 125); origami_utils::load<Reg>(a126, d2 + 126); origami_utils::load<Reg>(a127, d2 + 127);

				phase1_sort<Item, Reg, N, _SWITCH>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				phase1_sort<Item, Reg, N, _SWITCH>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
				phase1_sort<Item, Reg, N, _SWITCH>(a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95);
				phase1_sort<Item, Reg, N, _SWITCH>(a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);

				constexpr ui W = sizeof(Reg) / sizeof(Item);
				constexpr ui TARGET_REG = N / W;
				if constexpr (TARGET_REG >= 64) {
					origami_sorter::mrmerge64R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
					origami_sorter::mrmerge64R<Reg, Item>(a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);
				}
				if constexpr (TARGET_REG == 128)
					origami_sorter::mrmerge128R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);

				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
				origami_utils::store<Reg>(a32, d2 + 32); origami_utils::store<Reg>(a33, d2 + 33); origami_utils::store<Reg>(a34, d2 + 34); origami_utils::store<Reg>(a35, d2 + 35); origami_utils::store<Reg>(a36, d2 + 36); origami_utils::store<Reg>(a37, d2 + 37); origami_utils::store<Reg>(a38, d2 + 38); origami_utils::store<Reg>(a39, d2 + 39); origami_utils::store<Reg>(a40, d2 + 40); origami_utils::store<Reg>(a41, d2 + 41); origami_utils::store<Reg>(a42, d2 + 42); origami_utils::store<Reg>(a43, d2 + 43); origami_utils::store<Reg>(a44, d2 + 44); origami_utils::store<Reg>(a45, d2 + 45); origami_utils::store<Reg>(a46, d2 + 46); origami_utils::store<Reg>(a47, d2 + 47); origami_utils::store<Reg>(a48, d2 + 48); origami_utils::store<Reg>(a49, d2 + 49); origami_utils::store<Reg>(a50, d2 + 50); origami_utils::store<Reg>(a51, d2 + 51); origami_utils::store<Reg>(a52, d2 + 52); origami_utils::store<Reg>(a53, d2 + 53); origami_utils::store<Reg>(a54, d2 + 54); origami_utils::store<Reg>(a55, d2 + 55); origami_utils::store<Reg>(a56, d2 + 56); origami_utils::store<Reg>(a57, d2 + 57); origami_utils::store<Reg>(a58, d2 + 58); origami_utils::store<Reg>(a59, d2 + 59); origami_utils::store<Reg>(a60, d2 + 60); origami_utils::store<Reg>(a61, d2 + 61); origami_utils::store<Reg>(a62, d2 + 62); origami_utils::store<Reg>(a63, d2 + 63);
				origami_utils::store<Reg>(a64, d2 + 64); origami_utils::store<Reg>(a65, d2 + 65); origami_utils::store<Reg>(a66, d2 + 66); origami_utils::store<Reg>(a67, d2 + 67); origami_utils::store<Reg>(a68, d2 + 68); origami_utils::store<Reg>(a69, d2 + 69); origami_utils::store<Reg>(a70, d2 + 70); origami_utils::store<Reg>(a71, d2 + 71); origami_utils::store<Reg>(a72, d2 + 72); origami_utils::store<Reg>(a73, d2 + 73); origami_utils::store<Reg>(a74, d2 + 74); origami_utils::store<Reg>(a75, d2 + 75); origami_utils::store<Reg>(a76, d2 + 76); origami_utils::store<Reg>(a77, d2 + 77); origami_utils::store<Reg>(a78, d2 + 78); origami_utils::store<Reg>(a79, d2 + 79); origami_utils::store<Reg>(a80, d2 + 80); origami_utils::store<Reg>(a81, d2 + 81); origami_utils::store<Reg>(a82, d2 + 82); origami_utils::store<Reg>(a83, d2 + 83); origami_utils::store<Reg>(a84, d2 + 84); origami_utils::store<Reg>(a85, d2 + 85); origami_utils::store<Reg>(a86, d2 + 86); origami_utils::store<Reg>(a87, d2 + 87); origami_utils::store<Reg>(a88, d2 + 88); origami_utils::store<Reg>(a89, d2 + 89); origami_utils::store<Reg>(a90, d2 + 90); origami_utils::store<Reg>(a91, d2 + 91); origami_utils::store<Reg>(a92, d2 + 92); origami_utils::store<Reg>(a93, d2 + 93); origami_utils::store<Reg>(a94, d2 + 94); origami_utils::store<Reg>(a95, d2 + 95);
				origami_utils::store<Reg>(a96, d2 + 96); origami_utils::store<Reg>(a97, d2 + 97); origami_utils::store<Reg>(a98, d2 + 98); origami_utils::store<Reg>(a99, d2 + 99); origami_utils::store<Reg>(a100, d2 + 100); origami_utils::store<Reg>(a101, d2 + 101); origami_utils::store<Reg>(a102, d2 + 102); origami_utils::store<Reg>(a103, d2 + 103); origami_utils::store<Reg>(a104, d2 + 104); origami_utils::store<Reg>(a105, d2 + 105); origami_utils::store<Reg>(a106, d2 + 106); origami_utils::store<Reg>(a107, d2 + 107); origami_utils::store<Reg>(a108, d2 + 108); origami_utils::store<Reg>(a109, d2 + 109); origami_utils::store<Reg>(a110, d2 + 110); origami_utils::store<Reg>(a111, d2 + 111); origami_utils::store<Reg>(a112, d2 + 112); origami_utils::store<Reg>(a113, d2 + 113); origami_utils::store<Reg>(a114, d2 + 114); origami_utils::store<Reg>(a115, d2 + 115); origami_utils::store<Reg>(a116, d2 + 116); origami_utils::store<Reg>(a117, d2 + 117); origami_utils::store<Reg>(a118, d2 + 118); origami_utils::store<Reg>(a119, d2 + 119); origami_utils::store<Reg>(a120, d2 + 120); origami_utils::store<Reg>(a121, d2 + 121); origami_utils::store<Reg>(a122, d2 + 122); origami_utils::store<Reg>(a123, d2 + 123); origami_utils::store<Reg>(a124, d2 + 124); origami_utils::store<Reg>(a125, d2 + 125); origami_utils::store<Reg>(a126, d2 + 126); origami_utils::store<Reg>(a127, d2 + 127);
			}
			else if constexpr (NREG == 256) {
				Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127, a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159, a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191, a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223, a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255;
				Reg* d2 = (Reg*)d;

				origami_utils::load<Reg>(a0, d2 + 0); origami_utils::load<Reg>(a1, d2 + 1); origami_utils::load<Reg>(a2, d2 + 2); origami_utils::load<Reg>(a3, d2 + 3); origami_utils::load<Reg>(a4, d2 + 4); origami_utils::load<Reg>(a5, d2 + 5); origami_utils::load<Reg>(a6, d2 + 6); origami_utils::load<Reg>(a7, d2 + 7); origami_utils::load<Reg>(a8, d2 + 8); origami_utils::load<Reg>(a9, d2 + 9); origami_utils::load<Reg>(a10, d2 + 10); origami_utils::load<Reg>(a11, d2 + 11); origami_utils::load<Reg>(a12, d2 + 12); origami_utils::load<Reg>(a13, d2 + 13); origami_utils::load<Reg>(a14, d2 + 14); origami_utils::load<Reg>(a15, d2 + 15); origami_utils::load<Reg>(a16, d2 + 16); origami_utils::load<Reg>(a17, d2 + 17); origami_utils::load<Reg>(a18, d2 + 18); origami_utils::load<Reg>(a19, d2 + 19); origami_utils::load<Reg>(a20, d2 + 20); origami_utils::load<Reg>(a21, d2 + 21); origami_utils::load<Reg>(a22, d2 + 22); origami_utils::load<Reg>(a23, d2 + 23); origami_utils::load<Reg>(a24, d2 + 24); origami_utils::load<Reg>(a25, d2 + 25); origami_utils::load<Reg>(a26, d2 + 26); origami_utils::load<Reg>(a27, d2 + 27); origami_utils::load<Reg>(a28, d2 + 28); origami_utils::load<Reg>(a29, d2 + 29); origami_utils::load<Reg>(a30, d2 + 30); origami_utils::load<Reg>(a31, d2 + 31);
				origami_utils::load<Reg>(a32, d2 + 32); origami_utils::load<Reg>(a33, d2 + 33); origami_utils::load<Reg>(a34, d2 + 34); origami_utils::load<Reg>(a35, d2 + 35); origami_utils::load<Reg>(a36, d2 + 36); origami_utils::load<Reg>(a37, d2 + 37); origami_utils::load<Reg>(a38, d2 + 38); origami_utils::load<Reg>(a39, d2 + 39); origami_utils::load<Reg>(a40, d2 + 40); origami_utils::load<Reg>(a41, d2 + 41); origami_utils::load<Reg>(a42, d2 + 42); origami_utils::load<Reg>(a43, d2 + 43); origami_utils::load<Reg>(a44, d2 + 44); origami_utils::load<Reg>(a45, d2 + 45); origami_utils::load<Reg>(a46, d2 + 46); origami_utils::load<Reg>(a47, d2 + 47); origami_utils::load<Reg>(a48, d2 + 48); origami_utils::load<Reg>(a49, d2 + 49); origami_utils::load<Reg>(a50, d2 + 50); origami_utils::load<Reg>(a51, d2 + 51); origami_utils::load<Reg>(a52, d2 + 52); origami_utils::load<Reg>(a53, d2 + 53); origami_utils::load<Reg>(a54, d2 + 54); origami_utils::load<Reg>(a55, d2 + 55); origami_utils::load<Reg>(a56, d2 + 56); origami_utils::load<Reg>(a57, d2 + 57); origami_utils::load<Reg>(a58, d2 + 58); origami_utils::load<Reg>(a59, d2 + 59); origami_utils::load<Reg>(a60, d2 + 60); origami_utils::load<Reg>(a61, d2 + 61); origami_utils::load<Reg>(a62, d2 + 62); origami_utils::load<Reg>(a63, d2 + 63);
				origami_utils::load<Reg>(a64, d2 + 64); origami_utils::load<Reg>(a65, d2 + 65); origami_utils::load<Reg>(a66, d2 + 66); origami_utils::load<Reg>(a67, d2 + 67); origami_utils::load<Reg>(a68, d2 + 68); origami_utils::load<Reg>(a69, d2 + 69); origami_utils::load<Reg>(a70, d2 + 70); origami_utils::load<Reg>(a71, d2 + 71); origami_utils::load<Reg>(a72, d2 + 72); origami_utils::load<Reg>(a73, d2 + 73); origami_utils::load<Reg>(a74, d2 + 74); origami_utils::load<Reg>(a75, d2 + 75); origami_utils::load<Reg>(a76, d2 + 76); origami_utils::load<Reg>(a77, d2 + 77); origami_utils::load<Reg>(a78, d2 + 78); origami_utils::load<Reg>(a79, d2 + 79); origami_utils::load<Reg>(a80, d2 + 80); origami_utils::load<Reg>(a81, d2 + 81); origami_utils::load<Reg>(a82, d2 + 82); origami_utils::load<Reg>(a83, d2 + 83); origami_utils::load<Reg>(a84, d2 + 84); origami_utils::load<Reg>(a85, d2 + 85); origami_utils::load<Reg>(a86, d2 + 86); origami_utils::load<Reg>(a87, d2 + 87); origami_utils::load<Reg>(a88, d2 + 88); origami_utils::load<Reg>(a89, d2 + 89); origami_utils::load<Reg>(a90, d2 + 90); origami_utils::load<Reg>(a91, d2 + 91); origami_utils::load<Reg>(a92, d2 + 92); origami_utils::load<Reg>(a93, d2 + 93); origami_utils::load<Reg>(a94, d2 + 94); origami_utils::load<Reg>(a95, d2 + 95);
				origami_utils::load<Reg>(a96, d2 + 96); origami_utils::load<Reg>(a97, d2 + 97); origami_utils::load<Reg>(a98, d2 + 98); origami_utils::load<Reg>(a99, d2 + 99); origami_utils::load<Reg>(a100, d2 + 100); origami_utils::load<Reg>(a101, d2 + 101); origami_utils::load<Reg>(a102, d2 + 102); origami_utils::load<Reg>(a103, d2 + 103); origami_utils::load<Reg>(a104, d2 + 104); origami_utils::load<Reg>(a105, d2 + 105); origami_utils::load<Reg>(a106, d2 + 106); origami_utils::load<Reg>(a107, d2 + 107); origami_utils::load<Reg>(a108, d2 + 108); origami_utils::load<Reg>(a109, d2 + 109); origami_utils::load<Reg>(a110, d2 + 110); origami_utils::load<Reg>(a111, d2 + 111); origami_utils::load<Reg>(a112, d2 + 112); origami_utils::load<Reg>(a113, d2 + 113); origami_utils::load<Reg>(a114, d2 + 114); origami_utils::load<Reg>(a115, d2 + 115); origami_utils::load<Reg>(a116, d2 + 116); origami_utils::load<Reg>(a117, d2 + 117); origami_utils::load<Reg>(a118, d2 + 118); origami_utils::load<Reg>(a119, d2 + 119); origami_utils::load<Reg>(a120, d2 + 120); origami_utils::load<Reg>(a121, d2 + 121); origami_utils::load<Reg>(a122, d2 + 122); origami_utils::load<Reg>(a123, d2 + 123); origami_utils::load<Reg>(a124, d2 + 124); origami_utils::load<Reg>(a125, d2 + 125); origami_utils::load<Reg>(a126, d2 + 126); origami_utils::load<Reg>(a127, d2 + 127);
				origami_utils::load<Reg>(a128, d2 + 128); origami_utils::load<Reg>(a129, d2 + 129); origami_utils::load<Reg>(a130, d2 + 130); origami_utils::load<Reg>(a131, d2 + 131); origami_utils::load<Reg>(a132, d2 + 132); origami_utils::load<Reg>(a133, d2 + 133); origami_utils::load<Reg>(a134, d2 + 134); origami_utils::load<Reg>(a135, d2 + 135); origami_utils::load<Reg>(a136, d2 + 136); origami_utils::load<Reg>(a137, d2 + 137); origami_utils::load<Reg>(a138, d2 + 138); origami_utils::load<Reg>(a139, d2 + 139); origami_utils::load<Reg>(a140, d2 + 140); origami_utils::load<Reg>(a141, d2 + 141); origami_utils::load<Reg>(a142, d2 + 142); origami_utils::load<Reg>(a143, d2 + 143); origami_utils::load<Reg>(a144, d2 + 144); origami_utils::load<Reg>(a145, d2 + 145); origami_utils::load<Reg>(a146, d2 + 146); origami_utils::load<Reg>(a147, d2 + 147); origami_utils::load<Reg>(a148, d2 + 148); origami_utils::load<Reg>(a149, d2 + 149); origami_utils::load<Reg>(a150, d2 + 150); origami_utils::load<Reg>(a151, d2 + 151); origami_utils::load<Reg>(a152, d2 + 152); origami_utils::load<Reg>(a153, d2 + 153); origami_utils::load<Reg>(a154, d2 + 154); origami_utils::load<Reg>(a155, d2 + 155); origami_utils::load<Reg>(a156, d2 + 156); origami_utils::load<Reg>(a157, d2 + 157); origami_utils::load<Reg>(a158, d2 + 158); origami_utils::load<Reg>(a159, d2 + 159);
				origami_utils::load<Reg>(a160, d2 + 160); origami_utils::load<Reg>(a161, d2 + 161); origami_utils::load<Reg>(a162, d2 + 162); origami_utils::load<Reg>(a163, d2 + 163); origami_utils::load<Reg>(a164, d2 + 164); origami_utils::load<Reg>(a165, d2 + 165); origami_utils::load<Reg>(a166, d2 + 166); origami_utils::load<Reg>(a167, d2 + 167); origami_utils::load<Reg>(a168, d2 + 168); origami_utils::load<Reg>(a169, d2 + 169); origami_utils::load<Reg>(a170, d2 + 170); origami_utils::load<Reg>(a171, d2 + 171); origami_utils::load<Reg>(a172, d2 + 172); origami_utils::load<Reg>(a173, d2 + 173); origami_utils::load<Reg>(a174, d2 + 174); origami_utils::load<Reg>(a175, d2 + 175); origami_utils::load<Reg>(a176, d2 + 176); origami_utils::load<Reg>(a177, d2 + 177); origami_utils::load<Reg>(a178, d2 + 178); origami_utils::load<Reg>(a179, d2 + 179); origami_utils::load<Reg>(a180, d2 + 180); origami_utils::load<Reg>(a181, d2 + 181); origami_utils::load<Reg>(a182, d2 + 182); origami_utils::load<Reg>(a183, d2 + 183); origami_utils::load<Reg>(a184, d2 + 184); origami_utils::load<Reg>(a185, d2 + 185); origami_utils::load<Reg>(a186, d2 + 186); origami_utils::load<Reg>(a187, d2 + 187); origami_utils::load<Reg>(a188, d2 + 188); origami_utils::load<Reg>(a189, d2 + 189); origami_utils::load<Reg>(a190, d2 + 190); origami_utils::load<Reg>(a191, d2 + 191);
				origami_utils::load<Reg>(a192, d2 + 192); origami_utils::load<Reg>(a193, d2 + 193); origami_utils::load<Reg>(a194, d2 + 194); origami_utils::load<Reg>(a195, d2 + 195); origami_utils::load<Reg>(a196, d2 + 196); origami_utils::load<Reg>(a197, d2 + 197); origami_utils::load<Reg>(a198, d2 + 198); origami_utils::load<Reg>(a199, d2 + 199); origami_utils::load<Reg>(a200, d2 + 200); origami_utils::load<Reg>(a201, d2 + 201); origami_utils::load<Reg>(a202, d2 + 202); origami_utils::load<Reg>(a203, d2 + 203); origami_utils::load<Reg>(a204, d2 + 204); origami_utils::load<Reg>(a205, d2 + 205); origami_utils::load<Reg>(a206, d2 + 206); origami_utils::load<Reg>(a207, d2 + 207); origami_utils::load<Reg>(a208, d2 + 208); origami_utils::load<Reg>(a209, d2 + 209); origami_utils::load<Reg>(a210, d2 + 210); origami_utils::load<Reg>(a211, d2 + 211); origami_utils::load<Reg>(a212, d2 + 212); origami_utils::load<Reg>(a213, d2 + 213); origami_utils::load<Reg>(a214, d2 + 214); origami_utils::load<Reg>(a215, d2 + 215); origami_utils::load<Reg>(a216, d2 + 216); origami_utils::load<Reg>(a217, d2 + 217); origami_utils::load<Reg>(a218, d2 + 218); origami_utils::load<Reg>(a219, d2 + 219); origami_utils::load<Reg>(a220, d2 + 220); origami_utils::load<Reg>(a221, d2 + 221); origami_utils::load<Reg>(a222, d2 + 222); origami_utils::load<Reg>(a223, d2 + 223);
				origami_utils::load<Reg>(a224, d2 + 224); origami_utils::load<Reg>(a225, d2 + 225); origami_utils::load<Reg>(a226, d2 + 226); origami_utils::load<Reg>(a227, d2 + 227); origami_utils::load<Reg>(a228, d2 + 228); origami_utils::load<Reg>(a229, d2 + 229); origami_utils::load<Reg>(a230, d2 + 230); origami_utils::load<Reg>(a231, d2 + 231); origami_utils::load<Reg>(a232, d2 + 232); origami_utils::load<Reg>(a233, d2 + 233); origami_utils::load<Reg>(a234, d2 + 234); origami_utils::load<Reg>(a235, d2 + 235); origami_utils::load<Reg>(a236, d2 + 236); origami_utils::load<Reg>(a237, d2 + 237); origami_utils::load<Reg>(a238, d2 + 238); origami_utils::load<Reg>(a239, d2 + 239); origami_utils::load<Reg>(a240, d2 + 240); origami_utils::load<Reg>(a241, d2 + 241); origami_utils::load<Reg>(a242, d2 + 242); origami_utils::load<Reg>(a243, d2 + 243); origami_utils::load<Reg>(a244, d2 + 244); origami_utils::load<Reg>(a245, d2 + 245); origami_utils::load<Reg>(a246, d2 + 246); origami_utils::load<Reg>(a247, d2 + 247); origami_utils::load<Reg>(a248, d2 + 248); origami_utils::load<Reg>(a249, d2 + 249); origami_utils::load<Reg>(a250, d2 + 250); origami_utils::load<Reg>(a251, d2 + 251); origami_utils::load<Reg>(a252, d2 + 252); origami_utils::load<Reg>(a253, d2 + 253); origami_utils::load<Reg>(a254, d2 + 254); origami_utils::load<Reg>(a255, d2 + 255);

				phase1_sort<Item, Reg, N, _SWITCH>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
				phase1_sort<Item, Reg, N, _SWITCH>(a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
				phase1_sort<Item, Reg, N, _SWITCH>(a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95);
				phase1_sort<Item, Reg, N, _SWITCH>(a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);
				phase1_sort<Item, Reg, N, _SWITCH>(a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159);
				phase1_sort<Item, Reg, N, _SWITCH>(a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191);
				phase1_sort<Item, Reg, N, _SWITCH>(a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223);
				phase1_sort<Item, Reg, N, _SWITCH>(a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255);

				constexpr ui W = sizeof(Reg) / sizeof(Item);
				constexpr ui TARGET_REG = N / W;
				if constexpr (TARGET_REG >= 64) {
					origami_sorter::mrmerge64R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
					origami_sorter::mrmerge64R<Reg, Item>(a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);
					origami_sorter::mrmerge64R<Reg, Item>(a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159, a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191);
					origami_sorter::mrmerge64R<Reg, Item>(a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223, a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255);
				}
				if constexpr (TARGET_REG == 128)
					origami_sorter::mrmerge128R<Reg, Item>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, a101, a102, a103, a104, a105, a106, a107, a108, a109, a110, a111, a112, a113, a114, a115, a116, a117, a118, a119, a120, a121, a122, a123, a124, a125, a126, a127);
					origami_sorter::mrmerge128R<Reg, Item>(a128, a129, a130, a131, a132, a133, a134, a135, a136, a137, a138, a139, a140, a141, a142, a143, a144, a145, a146, a147, a148, a149, a150, a151, a152, a153, a154, a155, a156, a157, a158, a159, a160, a161, a162, a163, a164, a165, a166, a167, a168, a169, a170, a171, a172, a173, a174, a175, a176, a177, a178, a179, a180, a181, a182, a183, a184, a185, a186, a187, a188, a189, a190, a191, a192, a193, a194, a195, a196, a197, a198, a199, a200, a201, a202, a203, a204, a205, a206, a207, a208, a209, a210, a211, a212, a213, a214, a215, a216, a217, a218, a219, a220, a221, a222, a223, a224, a225, a226, a227, a228, a229, a230, a231, a232, a233, a234, a235, a236, a237, a238, a239, a240, a241, a242, a243, a244, a245, a246, a247, a248, a249, a250, a251, a252, a253, a254, a255);

				origami_utils::store<Reg>(a0, d2 + 0); origami_utils::store<Reg>(a1, d2 + 1); origami_utils::store<Reg>(a2, d2 + 2); origami_utils::store<Reg>(a3, d2 + 3); origami_utils::store<Reg>(a4, d2 + 4); origami_utils::store<Reg>(a5, d2 + 5); origami_utils::store<Reg>(a6, d2 + 6); origami_utils::store<Reg>(a7, d2 + 7); origami_utils::store<Reg>(a8, d2 + 8); origami_utils::store<Reg>(a9, d2 + 9); origami_utils::store<Reg>(a10, d2 + 10); origami_utils::store<Reg>(a11, d2 + 11); origami_utils::store<Reg>(a12, d2 + 12); origami_utils::store<Reg>(a13, d2 + 13); origami_utils::store<Reg>(a14, d2 + 14); origami_utils::store<Reg>(a15, d2 + 15); origami_utils::store<Reg>(a16, d2 + 16); origami_utils::store<Reg>(a17, d2 + 17); origami_utils::store<Reg>(a18, d2 + 18); origami_utils::store<Reg>(a19, d2 + 19); origami_utils::store<Reg>(a20, d2 + 20); origami_utils::store<Reg>(a21, d2 + 21); origami_utils::store<Reg>(a22, d2 + 22); origami_utils::store<Reg>(a23, d2 + 23); origami_utils::store<Reg>(a24, d2 + 24); origami_utils::store<Reg>(a25, d2 + 25); origami_utils::store<Reg>(a26, d2 + 26); origami_utils::store<Reg>(a27, d2 + 27); origami_utils::store<Reg>(a28, d2 + 28); origami_utils::store<Reg>(a29, d2 + 29); origami_utils::store<Reg>(a30, d2 + 30); origami_utils::store<Reg>(a31, d2 + 31);
				origami_utils::store<Reg>(a32, d2 + 32); origami_utils::store<Reg>(a33, d2 + 33); origami_utils::store<Reg>(a34, d2 + 34); origami_utils::store<Reg>(a35, d2 + 35); origami_utils::store<Reg>(a36, d2 + 36); origami_utils::store<Reg>(a37, d2 + 37); origami_utils::store<Reg>(a38, d2 + 38); origami_utils::store<Reg>(a39, d2 + 39); origami_utils::store<Reg>(a40, d2 + 40); origami_utils::store<Reg>(a41, d2 + 41); origami_utils::store<Reg>(a42, d2 + 42); origami_utils::store<Reg>(a43, d2 + 43); origami_utils::store<Reg>(a44, d2 + 44); origami_utils::store<Reg>(a45, d2 + 45); origami_utils::store<Reg>(a46, d2 + 46); origami_utils::store<Reg>(a47, d2 + 47); origami_utils::store<Reg>(a48, d2 + 48); origami_utils::store<Reg>(a49, d2 + 49); origami_utils::store<Reg>(a50, d2 + 50); origami_utils::store<Reg>(a51, d2 + 51); origami_utils::store<Reg>(a52, d2 + 52); origami_utils::store<Reg>(a53, d2 + 53); origami_utils::store<Reg>(a54, d2 + 54); origami_utils::store<Reg>(a55, d2 + 55); origami_utils::store<Reg>(a56, d2 + 56); origami_utils::store<Reg>(a57, d2 + 57); origami_utils::store<Reg>(a58, d2 + 58); origami_utils::store<Reg>(a59, d2 + 59); origami_utils::store<Reg>(a60, d2 + 60); origami_utils::store<Reg>(a61, d2 + 61); origami_utils::store<Reg>(a62, d2 + 62); origami_utils::store<Reg>(a63, d2 + 63);
				origami_utils::store<Reg>(a64, d2 + 64); origami_utils::store<Reg>(a65, d2 + 65); origami_utils::store<Reg>(a66, d2 + 66); origami_utils::store<Reg>(a67, d2 + 67); origami_utils::store<Reg>(a68, d2 + 68); origami_utils::store<Reg>(a69, d2 + 69); origami_utils::store<Reg>(a70, d2 + 70); origami_utils::store<Reg>(a71, d2 + 71); origami_utils::store<Reg>(a72, d2 + 72); origami_utils::store<Reg>(a73, d2 + 73); origami_utils::store<Reg>(a74, d2 + 74); origami_utils::store<Reg>(a75, d2 + 75); origami_utils::store<Reg>(a76, d2 + 76); origami_utils::store<Reg>(a77, d2 + 77); origami_utils::store<Reg>(a78, d2 + 78); origami_utils::store<Reg>(a79, d2 + 79); origami_utils::store<Reg>(a80, d2 + 80); origami_utils::store<Reg>(a81, d2 + 81); origami_utils::store<Reg>(a82, d2 + 82); origami_utils::store<Reg>(a83, d2 + 83); origami_utils::store<Reg>(a84, d2 + 84); origami_utils::store<Reg>(a85, d2 + 85); origami_utils::store<Reg>(a86, d2 + 86); origami_utils::store<Reg>(a87, d2 + 87); origami_utils::store<Reg>(a88, d2 + 88); origami_utils::store<Reg>(a89, d2 + 89); origami_utils::store<Reg>(a90, d2 + 90); origami_utils::store<Reg>(a91, d2 + 91); origami_utils::store<Reg>(a92, d2 + 92); origami_utils::store<Reg>(a93, d2 + 93); origami_utils::store<Reg>(a94, d2 + 94); origami_utils::store<Reg>(a95, d2 + 95);
				origami_utils::store<Reg>(a96, d2 + 96); origami_utils::store<Reg>(a97, d2 + 97); origami_utils::store<Reg>(a98, d2 + 98); origami_utils::store<Reg>(a99, d2 + 99); origami_utils::store<Reg>(a100, d2 + 100); origami_utils::store<Reg>(a101, d2 + 101); origami_utils::store<Reg>(a102, d2 + 102); origami_utils::store<Reg>(a103, d2 + 103); origami_utils::store<Reg>(a104, d2 + 104); origami_utils::store<Reg>(a105, d2 + 105); origami_utils::store<Reg>(a106, d2 + 106); origami_utils::store<Reg>(a107, d2 + 107); origami_utils::store<Reg>(a108, d2 + 108); origami_utils::store<Reg>(a109, d2 + 109); origami_utils::store<Reg>(a110, d2 + 110); origami_utils::store<Reg>(a111, d2 + 111); origami_utils::store<Reg>(a112, d2 + 112); origami_utils::store<Reg>(a113, d2 + 113); origami_utils::store<Reg>(a114, d2 + 114); origami_utils::store<Reg>(a115, d2 + 115); origami_utils::store<Reg>(a116, d2 + 116); origami_utils::store<Reg>(a117, d2 + 117); origami_utils::store<Reg>(a118, d2 + 118); origami_utils::store<Reg>(a119, d2 + 119); origami_utils::store<Reg>(a120, d2 + 120); origami_utils::store<Reg>(a121, d2 + 121); origami_utils::store<Reg>(a122, d2 + 122); origami_utils::store<Reg>(a123, d2 + 123); origami_utils::store<Reg>(a124, d2 + 124); origami_utils::store<Reg>(a125, d2 + 125); origami_utils::store<Reg>(a126, d2 + 126); origami_utils::store<Reg>(a127, d2 + 127);
				origami_utils::store<Reg>(a128, d2 + 128); origami_utils::store<Reg>(a129, d2 + 129); origami_utils::store<Reg>(a130, d2 + 130); origami_utils::store<Reg>(a131, d2 + 131); origami_utils::store<Reg>(a132, d2 + 132); origami_utils::store<Reg>(a133, d2 + 133); origami_utils::store<Reg>(a134, d2 + 134); origami_utils::store<Reg>(a135, d2 + 135); origami_utils::store<Reg>(a136, d2 + 136); origami_utils::store<Reg>(a137, d2 + 137); origami_utils::store<Reg>(a138, d2 + 138); origami_utils::store<Reg>(a139, d2 + 139); origami_utils::store<Reg>(a140, d2 + 140); origami_utils::store<Reg>(a141, d2 + 141); origami_utils::store<Reg>(a142, d2 + 142); origami_utils::store<Reg>(a143, d2 + 143); origami_utils::store<Reg>(a144, d2 + 144); origami_utils::store<Reg>(a145, d2 + 145); origami_utils::store<Reg>(a146, d2 + 146); origami_utils::store<Reg>(a147, d2 + 147); origami_utils::store<Reg>(a148, d2 + 148); origami_utils::store<Reg>(a149, d2 + 149); origami_utils::store<Reg>(a150, d2 + 150); origami_utils::store<Reg>(a151, d2 + 151); origami_utils::store<Reg>(a152, d2 + 152); origami_utils::store<Reg>(a153, d2 + 153); origami_utils::store<Reg>(a154, d2 + 154); origami_utils::store<Reg>(a155, d2 + 155); origami_utils::store<Reg>(a156, d2 + 156); origami_utils::store<Reg>(a157, d2 + 157); origami_utils::store<Reg>(a158, d2 + 158); origami_utils::store<Reg>(a159, d2 + 159);
				origami_utils::store<Reg>(a160, d2 + 160); origami_utils::store<Reg>(a161, d2 + 161); origami_utils::store<Reg>(a162, d2 + 162); origami_utils::store<Reg>(a163, d2 + 163); origami_utils::store<Reg>(a164, d2 + 164); origami_utils::store<Reg>(a165, d2 + 165); origami_utils::store<Reg>(a166, d2 + 166); origami_utils::store<Reg>(a167, d2 + 167); origami_utils::store<Reg>(a168, d2 + 168); origami_utils::store<Reg>(a169, d2 + 169); origami_utils::store<Reg>(a170, d2 + 170); origami_utils::store<Reg>(a171, d2 + 171); origami_utils::store<Reg>(a172, d2 + 172); origami_utils::store<Reg>(a173, d2 + 173); origami_utils::store<Reg>(a174, d2 + 174); origami_utils::store<Reg>(a175, d2 + 175); origami_utils::store<Reg>(a176, d2 + 176); origami_utils::store<Reg>(a177, d2 + 177); origami_utils::store<Reg>(a178, d2 + 178); origami_utils::store<Reg>(a179, d2 + 179); origami_utils::store<Reg>(a180, d2 + 180); origami_utils::store<Reg>(a181, d2 + 181); origami_utils::store<Reg>(a182, d2 + 182); origami_utils::store<Reg>(a183, d2 + 183); origami_utils::store<Reg>(a184, d2 + 184); origami_utils::store<Reg>(a185, d2 + 185); origami_utils::store<Reg>(a186, d2 + 186); origami_utils::store<Reg>(a187, d2 + 187); origami_utils::store<Reg>(a188, d2 + 188); origami_utils::store<Reg>(a189, d2 + 189); origami_utils::store<Reg>(a190, d2 + 190); origami_utils::store<Reg>(a191, d2 + 191);
				origami_utils::store<Reg>(a192, d2 + 192); origami_utils::store<Reg>(a193, d2 + 193); origami_utils::store<Reg>(a194, d2 + 194); origami_utils::store<Reg>(a195, d2 + 195); origami_utils::store<Reg>(a196, d2 + 196); origami_utils::store<Reg>(a197, d2 + 197); origami_utils::store<Reg>(a198, d2 + 198); origami_utils::store<Reg>(a199, d2 + 199); origami_utils::store<Reg>(a200, d2 + 200); origami_utils::store<Reg>(a201, d2 + 201); origami_utils::store<Reg>(a202, d2 + 202); origami_utils::store<Reg>(a203, d2 + 203); origami_utils::store<Reg>(a204, d2 + 204); origami_utils::store<Reg>(a205, d2 + 205); origami_utils::store<Reg>(a206, d2 + 206); origami_utils::store<Reg>(a207, d2 + 207); origami_utils::store<Reg>(a208, d2 + 208); origami_utils::store<Reg>(a209, d2 + 209); origami_utils::store<Reg>(a210, d2 + 210); origami_utils::store<Reg>(a211, d2 + 211); origami_utils::store<Reg>(a212, d2 + 212); origami_utils::store<Reg>(a213, d2 + 213); origami_utils::store<Reg>(a214, d2 + 214); origami_utils::store<Reg>(a215, d2 + 215); origami_utils::store<Reg>(a216, d2 + 216); origami_utils::store<Reg>(a217, d2 + 217); origami_utils::store<Reg>(a218, d2 + 218); origami_utils::store<Reg>(a219, d2 + 219); origami_utils::store<Reg>(a220, d2 + 220); origami_utils::store<Reg>(a221, d2 + 221); origami_utils::store<Reg>(a222, d2 + 222); origami_utils::store<Reg>(a223, d2 + 223);
				origami_utils::store<Reg>(a224, d2 + 224); origami_utils::store<Reg>(a225, d2 + 225); origami_utils::store<Reg>(a226, d2 + 226); origami_utils::store<Reg>(a227, d2 + 227); origami_utils::store<Reg>(a228, d2 + 228); origami_utils::store<Reg>(a229, d2 + 229); origami_utils::store<Reg>(a230, d2 + 230); origami_utils::store<Reg>(a231, d2 + 231); origami_utils::store<Reg>(a232, d2 + 232); origami_utils::store<Reg>(a233, d2 + 233); origami_utils::store<Reg>(a234, d2 + 234); origami_utils::store<Reg>(a235, d2 + 235); origami_utils::store<Reg>(a236, d2 + 236); origami_utils::store<Reg>(a237, d2 + 237); origami_utils::store<Reg>(a238, d2 + 238); origami_utils::store<Reg>(a239, d2 + 239); origami_utils::store<Reg>(a240, d2 + 240); origami_utils::store<Reg>(a241, d2 + 241); origami_utils::store<Reg>(a242, d2 + 242); origami_utils::store<Reg>(a243, d2 + 243); origami_utils::store<Reg>(a244, d2 + 244); origami_utils::store<Reg>(a245, d2 + 245); origami_utils::store<Reg>(a246, d2 + 246); origami_utils::store<Reg>(a247, d2 + 247); origami_utils::store<Reg>(a248, d2 + 248); origami_utils::store<Reg>(a249, d2 + 249); origami_utils::store<Reg>(a250, d2 + 250); origami_utils::store<Reg>(a251, d2 + 251); origami_utils::store<Reg>(a252, d2 + 252); origami_utils::store<Reg>(a253, d2 + 253); origami_utils::store<Reg>(a254, d2 + 254); origami_utils::store<Reg>(a255, d2 + 255);
			}

		}

		
	}

	// ----- Phase 2-3: large sort
	template <typename Item, typename Reg, ui P1_NREG = 32, ui P1_N = 16, ui P1_SWITCH = 32, ui P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x>
	FORCEINLINE Item* phase2_sort(Item* d, Item* tmp_buf, Item* end, ui64 sort_size, Item* d_back = nullptr) {
		d_back = d_back != nullptr ? d_back : d;
		Item* d2 = d;
		Item* d3 = d_back;

		// phase-1
		constexpr ui P1_INC = P1_NREG * sizeof(Reg) / sizeof(Item);
		while (d2 < end) {
			phase1_sort<Item, Reg, P1_NREG, P1_N, P1_SWITCH>(d2);
			d2 += P1_INC; 
			//_mm_prefetch((char*)(d2 + 512), _MM_HINT_T2);		// -> hurts performance
		}
		ui64 nItems = end - d;
		Item* src = d_back, *src_end = src + nItems;
		Item* dst = tmp_buf;
		
		return origami_merger::merge_series<Reg, Item, false, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x>(src, dst, nItems, P1_N, sort_size);
	}

	template <typename Item, typename Reg, ui P1_NREG = _P1_NREG, ui P1_N = _P1_N, ui P1_SWITCH = _P1_SWITCH, ui P2_N = _P2_N, ui P2_MERGE_UNROLL = _P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x = _P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x = _P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x = _P2_MERGE_NREG_3x, ui P3_MERGE_UNROLL = _P3_MERGE_UNROLL, ui P3_MERGE_NREG_1x = _P3_MERGE_NREG_1x, ui P3_MERGE_NREG_2x = _P3_MERGE_NREG_2x, ui P3_MERGE_NREG_3x = _P3_MERGE_NREG_3x>
	FORCEINLINE Item* sort_single_thread(Item* d, Item* tmp_buf, Item* end, ui64 sort_n) {
		ui64 n_items = end - d;
		Item* src = d;
		Item* dst = tmp_buf;

		constexpr ui64 Y = P2_N;
		ui64 Y_sort = min(Y, sort_n);

		// Phase 1-2
		Item* p = src, * p_dst = dst;
		Item* o = phase2_sort<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x>(p, p_dst, p + Y, Y_sort);
		p += Y; p_dst += Y;
		while (p < end) {
			// SortN<Item, Reg, p1_n>(p, p_dst, p + Y, Y_sort);
			phase2_sort<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x>(p, p_dst, p + Y, Y_sort);
			p += Y; p_dst += Y;
		}

		// Phase 3
		if (sort_n > Y) {
			src = (o == d) ? d : tmp_buf;
			dst = (o == d) ? tmp_buf : d;
			return origami_merger::merge_series<Reg, Item, true, P3_MERGE_UNROLL, P3_MERGE_NREG_1x, P3_MERGE_NREG_2x, P3_MERGE_NREG_3x>(src, dst, n_items, Y, sort_n);
		}
		return o;
	}

	// ------------
	// MULTI-THREAD
#define PHASE_3
#define PHASE_4
//#define SCALAR
#define PHASE3_ITERS_MAX 12

	template <typename Item>
	struct kway_merge_job {
		Item** X, ** endX, * out; ui64 tot;
		kway_merge_job(Item** _X, Item** _endX, Item* _out, ui64 _tot) : X(_X), endX(_endX), out(_out), tot(_tot) {}
	};
	template <typename Item>
	void print_job(kway_merge_job<Item>* job, ui K) {
		ui64 tot = 0;
		FOR(i, K, 1) {
			ui64 items = job->endX[i] - job->X[i];
			printf("[%llX %llX]; N: %llu\n", job->X[i], job->endX[i], items);
			Item* _p = job->X[i]; Item* _endp = job->endX[i]; ui _tot = _endp - _p;
			tot += items;
		}
		printf("Tot: %llu\n", tot);
	}

	ui _M = 8;
	__int64 phase1_2_curr_job_id = -1;
	__int64 phase1_2_jobs_tot = 0;
	HANDLE phase1_2_event, phase1_2_sync;
	const ui MAX_PARTITIONS = 8192;
	const ui MAX_STREAMS = 256;
	void*** p, ** pout;
	void*** p_tmp;
	
	template <typename Item>
	std::queue<kway_merge_job<Item>*> jobQueue;

	CRITICAL_SECTION cs, cs2;
	SYNCHRONIZATION_BARRIER barrier;

	ui phase3_iters = 2;
	ui phase3_ways_pow[PHASE3_ITERS_MAX];
	ui phase3_n_jobs[PHASE3_ITERS_MAX];			// how many jobs in each iteration of phase 3
	std::queue<ui> phase3_Q[PHASE3_ITERS_MAX];
	__int64 phase3_curr_job_id[PHASE3_ITERS_MAX];

	

	template <typename Item, typename Reg, ui P1_NREG = 32, ui P1_N = 32, ui P1_SWITCH = 16, ui P2_N, ui P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x, ui MT_L1_BUFF_N, ui MT_L2_BUFF_N>
	void sortN_multi_thread_worker(ui t_idx, ui64 Y, Item* d, Item* out, ui64 n_per_thread, ui n_threads, ui n_cores, ui n_partitions, Item* interim_buf) {
		int coreid = t_idx;
		if (coreid < n_threads) {
			if (coreid < n_cores) coreid <<= 1;
			else coreid = ((coreid % n_cores) << 1) + 1;
			SetThreadAffinityMask(GetCurrentThread(), 1LLU << coreid);
		}

		constexpr ui interim_buff_n = MT_L1_BUFF_N;	 
		constexpr ui l2_buff_n = MT_L2_BUFF_N;		 

		ui M_loc = _M;
		ui M_pow_loc = (ui)(log2(M_loc));
		ui64 Y_loc = Y;
		__int64 phase1_2_jobs_tot_loc = phase1_2_jobs_tot;

		std::queue<ui>* phase3_Q0_loc = &phase3_Q[0];
		Item* src, * dst;

		// do phase 1+2
		if (t_idx < n_cores) {
			while (1) {
				__int64 phase1_2_job = InterlockedIncrement64(&phase1_2_curr_job_id);
				if (phase1_2_job >= phase1_2_jobs_tot_loc) {
					if (phase1_2_job == phase1_2_jobs_tot_loc) SetEvent(phase1_2_event);
					break;
				}
				//printf("Thread %3lu doing job %d\n", t_idx, phase1_2_job);
				ui64 offset = phase1_2_job * Y_loc;
				src = d + offset;
				dst = out + offset;

				//irl_avx::SortN(src, dst, src + Y_loc, Y_loc);
				phase2_sort<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x>(src, dst, src + Y_loc, Y_loc);
#ifdef PHASE_3
				EnterCriticalSection(&cs);
				phase3_Q0_loc->push(phase1_2_job);
				LeaveCriticalSection(&cs);
#endif
			}
		}


		// Phase 3
#ifdef PHASE_3
#ifndef SCALAR
		//printf("Thread %3lu Phase 3 begin ... \n", t_idx);
		const ui phase3_iters_loc = phase3_iters;
		ui phase3_job_ids[1024];
		Item* X[1024], * endX[1024];
		ui64 prev_sort_n = Y_loc;

		ui p2_iters = (ui)(log2(P2_N) - log2(P1_N));
		// ** TODO: BUG -- SSE needs these arrays to be in the opposite order
		// ** NOTE: only for 32-bits
		if constexpr ((sizeof(Reg) == 64 && sizeof(Item) == 16) || (sizeof(Reg) == 32 && sizeof(Item) == 16) || (sizeof(Reg) == 16 && sizeof(Item) == 4)) {
			src = p2_iters ? d: out; // 	
			dst = p2_iters ? out: d; //
		}
		else {
			src = p2_iters ? out : d;	//d: out; // 	// NOTE: *** should be opposite for Scalar
			dst = p2_iters ? d : out;	//out: d; // 
		}

		FOR(i, phase3_iters_loc, 1) {
			std::queue<ui>* phase3_this_Q = &phase3_Q[i];
			std::queue<ui>* phase3_next_Q = &phase3_Q[i + 1];
			ui phase3_way_pow_loc = phase3_ways_pow[i];
			ui phase3_way = 1 << phase3_way_pow_loc;
			const ui64 merged_n = phase3_way * prev_sort_n;
			const ui n_jobs = phase3_n_jobs[i];
			__int64* phase3_curr_job_id_loc = &phase3_curr_job_id[i];

			origami_merge_tree::MergeReuseTree<Reg, Item>* tree = nullptr;
			if (phase3_way_pow_loc & 1) tree = new origami_merge_tree::MergeReuseTreeOdd<Reg, Item>();
			else tree = new origami_merge_tree::MergeReuseTreeEven<Reg, Item>();

			tree->merge_reuse_init(phase3_way, interim_buf, interim_buff_n, l2_buff_n);
			
			while (1) {
				EnterCriticalSection(&cs);
				if (phase3_this_Q->size() >= phase3_way) {
					FOR(j, phase3_way, 1) {
						phase3_job_ids[j] = phase3_this_Q->front();
						phase3_this_Q->pop();
					}
					LeaveCriticalSection(&cs);

					// merge
					FOR(j, phase3_way, 1) {
						ui job_id = phase3_job_ids[j];
						Item* p = src + job_id * prev_sort_n;
						X[j] = p;
						endX[j] = p + prev_sort_n;
					}
					__int64 curr_job_id = InterlockedIncrement64(phase3_curr_job_id_loc);

					Item* pout = dst + curr_job_id * merged_n;

					// *** DEBUG
					//EnterCriticalSection(&cs);
					//printf("Th %u %d-way merging %lu keys ... ", t_idx, phase3_way, merged_n);
					tree->merge_reuse(X, endX, pout, merged_n, interim_buff_n, l2_buff_n, interim_buf, phase3_way);
					/*printf("done\n");
					printf("Checking correctness ... ");
					SortCorrectnessChecker(pout, merged_n);
					printf("done\n");
					system("pause");
					LeaveCriticalSection(&cs);*/
					
					if (i != phase3_iters_loc - 1) {
						EnterCriticalSection(&cs);
						phase3_next_Q->push(curr_job_id);
						LeaveCriticalSection(&cs);
					}
				}
				else {
					// Q does not have enough jobs
					LeaveCriticalSection(&cs);
					// check to see all jobs are done in this level
					__int64 curr_job_id = *phase3_curr_job_id_loc;
					if (curr_job_id >= n_jobs - 1)
						break;
					// if not, sleep for a while
					SwitchToThread();
				}
			}
			delete tree;
			prev_sort_n = merged_n;

			// swap
			Item* tmp = src; src = dst; dst = tmp;
		}
#else 
		//printf("Thread %3lu Phase 3 begin ... \n", t_idx);
		const ui phase3_iters_loc = phase3_iters;
		ui phase3_job_ids[1024];
		Item* X[1024], * endX[1024];
		ui64 prev_sort_n = Y_loc;

		ui p2_iters = (ui)(log2(P2_N) - log2(P1_N));
		src = p2_iters ? d : out;
		dst = p2_iters ? out : d;

		FOR(i, phase3_iters_loc, 1) {
			std::queue<ui>* phase3_this_Q = &phase3_Q[i];
			std::queue<ui>* phase3_next_Q = &phase3_Q[i + 1];
			const ui64 merged_n = prev_sort_n << 1;
			const ui n_jobs = phase3_n_jobs[i];
			__int64* phase3_curr_job_id_loc = &phase3_curr_job_id[i];


			while (1) {
				EnterCriticalSection(&cs);
				if (phase3_this_Q->size() >= 2) {
					ui j1 = phase3_this_Q->front(); phase3_this_Q->pop();
					ui j2 = phase3_this_Q->front(); phase3_this_Q->pop();
					LeaveCriticalSection(&cs);

					Item* src1 = src + j1 * prev_sort_n;
					Item* src2 = src + j2 * prev_sort_n;
					
					__int64 curr_job_id = InterlockedIncrement64(phase3_curr_job_id_loc);

					Item* pout = dst + curr_job_id * merged_n;

					if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
						origami_merger::mergebl2_scalar_kv<Item, 4>(src1, prev_sort_n, src2, prev_sort_n, pout);
					}
					else {
						/*EnterCriticalSection(&cs2);
						printf("Th %d checking correctness on %llu items ... \n", t_idx, merged_n);
						printf("Src1 ... ");  SortCorrectnessChecker<Item>(src1, prev_sort_n); printf("done\n");
						printf("Src2 ... ");  SortCorrectnessChecker<Item>(src2, prev_sort_n); printf("done\n");*/
						origami_merger::mergebl2_scalar<Item, 4>(src1, prev_sort_n, src2, prev_sort_n, pout);
						/*printf("Merged ... ");  SortCorrectnessChecker<Item>(pout, merged_n); printf("done\n");
						PRINT_DASH(20);
						system("pause");
						LeaveCriticalSection(&cs2);*/
					}

					if (i != phase3_iters_loc - 1) {
						EnterCriticalSection(&cs);
						phase3_next_Q->push(curr_job_id);
						LeaveCriticalSection(&cs);
					}
				}
				else {
					// Q does not have enough jobs
					LeaveCriticalSection(&cs);
					// check to see all jobs are done in this level
					__int64 curr_job_id = *phase3_curr_job_id_loc;
					if (curr_job_id >= n_jobs - 1)
						break;
					// if not, sleep for a while
					SwitchToThread();
				}
			}
			prev_sort_n = merged_n;

			// swap
			Item* tmp = src; src = dst; dst = tmp;
		}
#endif
#endif

		// Phase 4
#ifdef PHASE_4
#ifndef SCALAR
		// need synchronization at this point
		EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);
		//printf("Thread %3lu beginning phase 4\n", t_idx);
		ui n_partitions_per_thread = n_partitions / n_threads;			// we can probably enfore n_partitions to be a multiple of n_threads
		const ui partition_idx_base = t_idx * n_partitions_per_thread;

		const ui64 n_per_partition = n_per_thread * n_threads / n_partitions;
		const ui n_per_stream_per_partition = n_per_partition / M_loc;
		const ui n_per_thread_per_stream = n_per_thread / M_loc;

		Item*** p_local = (Item***)p;
		Item*** p_tmp_local = (Item***)p_tmp;
		Item** pout_local = (Item**)pout;
		Item* L[MAX_STREAMS];
		Item* R[MAX_STREAMS];
		Item* S[MAX_STREAMS];

		// 4.1 Phase 4.1 -> first level of partitioning to n_threads jobs
		if (t_idx) {
			//Item** S = p_tmp_local[t_idx];
			FOR(s, M_loc, 1) {
				L[s] = p_tmp_local[0][s];
				R[s] = p_tmp_local[n_threads][s];
				S[s] = L[s] + n_per_thread_per_stream * t_idx;
			}
			//partition::Partition_Ptrs(L, R, S, M_loc);
			partition_templated::Partition_Ptrs(L, R, S, M_loc);
			memcpy(p_tmp_local[t_idx], S, sizeof(Item*) * M_loc);
		}
		// wait for all threads to finish
		EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);

		//printf("Thread %3lu Phase 4.1 done\n", t_idx);

		// 4.2 Phase 4.2 -> parallel partitioning
		memcpy(p_local[partition_idx_base], p_tmp_local[t_idx], M_loc * sizeof(Item*));
		memcpy(p_local[partition_idx_base + n_partitions_per_thread], p_tmp_local[t_idx + 1], M_loc * sizeof(Item*));
		FOR_INIT(k, 1, n_partitions_per_thread, 1) {
			ui partition_idx = partition_idx_base + k;
			//Item** S = p_local[partition_idx];
			memcpy(L, p_local[partition_idx_base], M_loc * sizeof(Item*));
			memcpy(R, p_local[partition_idx_base + n_partitions_per_thread], M_loc * sizeof(Item*));
			FOR(s, M_loc, 1)
				S[s] = L[s] + n_per_stream_per_partition * k;
			//partition::Partition_Ptrs(L, R, S, M_loc);
			partition_templated::Partition_Ptrs(L, R, S, M_loc);
			memcpy(p_local[partition_idx], S, sizeof(Item*) * M_loc);
		}
		//printf("Thread %3lu Phase 4.2 done\n", t_idx);

		// 4.3 Phase 4.3 -> push jobs in Q
		FOR_INIT(i, (partition_idx_base + 1), (partition_idx_base + n_partitions_per_thread + 1), 1) {
			kway_merge_job<Item>* job = new kway_merge_job<Item>(p_local[i - 1], p_local[i], pout_local[i - 1], n_per_partition);
			EnterCriticalSection(&cs2);
			jobQueue<Item>.push(job);
			LeaveCriticalSection(&cs2);
		}
		//printf("Thread %3lu Phase 4.3 done, Job Q size: %lu\n", t_idx, jobQueue<Item>.size());

		// 4.4 Phase 4.4 -> pop and work on jobs
		origami_merge_tree::MergeReuseTree<Reg, Item>* tree = nullptr;
		if (M_pow_loc & 1) tree = new origami_merge_tree::MergeReuseTreeOdd<Reg, Item>();
		else tree = new origami_merge_tree::MergeReuseTreeEven<Reg, Item>();

		tree->merge_reuse_init(M_loc, interim_buf, interim_buff_n, l2_buff_n);
		kway_merge_job<Item>* job = nullptr;
		while (1) {
			EnterCriticalSection(&cs2);
			if (jobQueue<Item>.size() > 0) {
				job = jobQueue<Item>.front(); jobQueue<Item>.pop();
				//print_job(job, M_loc);
				Item* _p = job->out; ui64 _n = job->tot;
				LeaveCriticalSection(&cs2);
				tree->merge_reuse(job->X, job->endX, job->out, job->tot, interim_buff_n, l2_buff_n, interim_buf, M_loc);
				// DEBUG **********
				//printf("Sort checker by th: %3lu @ [%llX %llX]\n", t_idx, _p, _p + _n); 
				//if (!SortCorrectnessChecker(_p, _n)) {
				//	printf("Dumping data ... \n");
				//	std::ofstream myf;
				//	myf.open("avx2_data_dump.txt");
				//	FOR(i, M_loc, 1) {
				//		myf << i << " ";		// array id
				//		Item* s = job->X[i];
				//		Item* e = job->endX[i];
				//		myf << (e - s) << " ";	// # of items in this array
				//		while (s < e) 
				//			myf << *s++ << " ";
				//		myf << std::endl;
				//	}
				//	myf.close();
				//	printf("Data dump done\n");
				//}
				//system("pause");
				//LeaveCriticalSection(&cs2);
			}
			else {
				LeaveCriticalSection(&cs2);
				break;
			}
		}
		//printf("Thread %3lu, Phase 4.4 done\n", t_idx);

		tree->merge_reuse_cleanup();
		delete tree;
#else 
	// nothing to do	
#endif	// SCALAR
#endif	// PHASE 4

		// printf("Thread %3lu done, Job Q: %lu\n", t_idx, phase1_2_complete_Q.size());
		/*__int64 finished = InterlockedIncrement64(&done);
		if (finished == 1)
			del_en_first = hrc::now();
		else if (finished == n_threads) {
			del_en_last = hrc::now();
			double el = ELAPSED_MS(del_st, del_en_last);
			avg_delta += el - ELAPSED_MS(del_st, del_en_first);
			done = 0;
		}*/
	}

	template <typename Item, typename Reg, ui P1_NREG = 32, ui P1_N = 32, ui P1_SWITCH = 16, ui P2_N, ui P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x, ui MT_L1_BUFF_N, ui MT_L2_BUFF_N>
	Item* sortN_multi_thread(Item* d, Item* out, ui64 n_items, ui n_threads, ui n_cores, ui n_partitions, int argc, char** argv, Item* kway_buf, Item* d_back = nullptr) {
		std::thread** threads = new std::thread * [n_threads];
		_M = n_threads;		// merge in phase 4

		constexpr ui64 Y = P2_N;
		ui Y_pow = (ui)(log2(Y));
		ui phase1_pow = (ui)(log2(P1_N));
		phase1_2_jobs_tot = n_items / Y;
		phase1_2_event = CreateEvent(NULL, true, false, NULL);		// manual reset, initially false


#ifdef PHASE_3
		phase3_iters = atoi(argv[4]); // 2;		// printf("Phase 3 iters: "); scanf_s("%lu", &phase3_iters);
		if (argc < 5 + phase3_iters) {
			printf("Not enough args\n");
			exit(1);			
		}
		ui prev_jobs_tot = phase1_2_jobs_tot;
		FOR(i, phase3_iters, 1) {
			phase3_ways_pow[i] = atoi(argv[5 + i]); //4; printf("Phase 3 ways pow %2lu: ", i); scanf_s("%lu", &phase3_ways_pow[i]);
			phase3_n_jobs[i] = prev_jobs_tot / (1LU << phase3_ways_pow[i]);
			prev_jobs_tot = phase3_n_jobs[i];
		}

		PRINT_DASH(20);
		printf("Phase 3 setup: \n");
		printf("Iters: %lu\n", phase3_iters);
		FOR(i, phase3_iters, 1)
			printf("Iter: %3lu, Pow: %lu, Target jobs: %4lu\n", i, phase3_ways_pow[i], phase3_n_jobs[i]);
		PRINT_DASH(20);
		memset(phase3_curr_job_id, -1, sizeof(__int64) * PHASE3_ITERS_MAX);
#endif

		// Phase 4 stuff
		const ui n_per_thread = n_items / n_threads;
		const ui tot_iters_before_phase4 = (Y_pow - phase1_pow) + phase3_iters;
		Item* sorted = (tot_iters_before_phase4 & 1) ? out : d;
#ifdef PHASE_4
		sorted = (tot_iters_before_phase4 & 1) ? d : out;
		const ui n_per_partition = n_items / n_partitions;

		// ---------- partitioining stuff for Phase 4
		// p[i][j] gives the start ptr of jth stream of partition i;			// xxx of stream i, end ptr for that would be in p[i][j + 1]
		p = new void **[n_partitions + 1];
		p_tmp = new void **[n_threads + 1];
		pout = new void *[n_partitions + 1];
		FOR(i, n_partitions + 1, 1) p[i] = new void * [_M];
		FOR(i, n_threads + 1, 1)	p_tmp[i] = new void * [_M];
		// init pointers
		// for every pair of sorted lists
		const ui n_per_stream_last = n_items / _M;
#endif

		InitializeCriticalSection(&cs);
		InitializeCriticalSection(&cs2);
		InitializeSynchronizationBarrier(&barrier, n_threads, -1);

		double tot_elapsed = 0;
		const ui repeat = 1;
		// test ground
		FOR(r, repeat, 1) {
			printf("Iter: %4lu\r", r);
			//done = 0;
#ifdef PHASE_4
			Item* p1 = (tot_iters_before_phase4 & 1) ? out : d;
			FOR(k, _M, 1) {
				p[0][k] = p1;
				p_tmp[0][k] = p1;
				p1 += n_per_stream_last;
				p[n_partitions][k] = p1;
				p_tmp[n_threads][k] = p1;
			}
			pout[0] = (tot_iters_before_phase4 & 1) ? d : out;
			FOR_INIT(i, 1, n_partitions + 1, 1) pout[i] = (Item*)(pout[0]) + i * n_per_partition;
#endif
			hrc::time_point st = hrc::now();
			//del_st = st;
			FOR(t, n_cores, 1) threads[t] = new std::thread(sortN_multi_thread_worker<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_N, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x, MT_L1_BUFF_N, MT_L2_BUFF_N>, t, Y, d, out, n_per_thread, n_threads, n_cores, n_partitions, (Item*)((char*)kway_buf + t * MB(8)));
			if (n_threads > n_cores) {
				WaitForSingleObject(phase1_2_event, INFINITE);
				FOR_INIT(t, n_cores, n_threads, 1) threads[t] = new std::thread(sortN_multi_thread_worker<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_N, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x, MT_L1_BUFF_N, MT_L2_BUFF_N>, t, Y, d, out, n_per_thread, n_threads, n_cores, n_partitions, (Item*)((char*)kway_buf + t * MB(8)));
			}
			FOR(t, n_threads, 1) threads[t]->join();
			hrc::time_point en = hrc::now();

			double el = ELAPSED_MS(st, en);
			//printf("Iter: %3lu, Speed: %.2f _M/s\n", r, n_items / el / 1e3);
			tot_elapsed += el;
			FOR(t, n_threads, 1) delete threads[t];
			if (r + 1 != repeat) memcpy(d, d_back, n_items * sizeof(Item));		// copy back if not last iteration
			phase1_2_curr_job_id = -1;
			memset(phase3_curr_job_id, -1, sizeof(__int64) * PHASE3_ITERS_MAX);
			ResetEvent(phase1_2_event);
		}
		PRINT_DASH(50);
		printf("Elapsed: %.2f ms, Speed: %.2f _M/s\n", tot_elapsed, repeat * n_items / tot_elapsed / 1e3);
		//printf("Delta: %.2f ms\n", avg_delta / repeat);


		// cleanup
		DeleteCriticalSection(&cs);
		DeleteCriticalSection(&cs2);
		DeleteSynchronizationBarrier(&barrier);
		CloseHandle(phase1_2_event);
		delete[] threads;
#ifdef PHASE_4
		FOR(i, n_partitions + 1, 1) delete[] p[i];
		FOR(i, n_threads + 1, 1)	delete[] p_tmp[i];
		delete[] pout;
		delete[] p;
		delete[] p_tmp;
#endif
		return sorted;

	}


	template <typename Reg, typename Item, ui P1_NREG, ui P1_N, ui P1_SWITCH, ui P2_N, ui P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x, ui P3_MERGE_UNROLL, ui P3_MERGE_NREG_1x, ui P3_MERGE_NREG_2x, ui P3_MERGE_NREG_3x>
	void sortN_multi_thread_scalar_worker(ui t_idx, Item* d, Item* out, ui64) {

	}

	void* src = nullptr, * dst = nullptr;
	constexpr ui NREG_1x = 4;
	template <typename Item, typename Reg, ui P1_NREG, ui P1_N, ui P1_SWITCH, ui P2_N, ui P2_MERGE_UNROLL, ui P2_MERGE_NREG_1x, ui P2_MERGE_NREG_2x, ui P2_MERGE_NREG_3x, ui P3_MERGE_UNROLL, ui P3_MERGE_NREG_1x, ui P3_MERGE_NREG_2x, ui P3_MERGE_NREG_3x>
	Item* sortN_multi_thread_scalar(Item* d, Item* tmp, ui64 n_items, ui n_threads, ui n_cores, Item* d_back = nullptr) {
#pragma omp parallel num_threads(n_threads)
		// run phase 1 through 3
		InitializeCriticalSection(&cs);
#pragma omp parallel for 
		for (int i = 0; i < n_threads; ++i) {
			//SetThreadAffinityMask(GetCurrentThread(), 1 << i);
			ui64 n_per_thread = n_items / n_threads;
			//EnterCriticalSection(&cs);
			Item* ret = sort_single_thread<Item, Reg, P1_NREG, P1_N, P1_SWITCH, P2_N, P2_MERGE_UNROLL, P2_MERGE_NREG_1x, P2_MERGE_NREG_2x, P2_MERGE_NREG_3x, P3_MERGE_UNROLL, P3_MERGE_NREG_1x, P3_MERGE_NREG_2x, P3_MERGE_NREG_3x>(d + i * n_per_thread, tmp + i * n_per_thread, d + (i + 1) * n_per_thread, n_per_thread);
			//printf("Checking correctness in th: %d ... ", i);
			//SortCorrectnessChecker<Item>(ret, n_per_thread);
			//printf("done\n");
			src = (i == 0) ? (void*)ret : src;
			//LeaveCriticalSection(&cs);
		}
		
//		// run the rest of phase 3
		dst = (void*)((src == d) ? tmp : d);
		ui64 n_per_thread = n_items / n_threads;
		for (__int64 merge_start = n_per_thread; merge_start < n_items; (merge_start <<= 1)) {
			int tid = 0;
#pragma omp parallel for 
			for (__int64 i = 0; i < n_items; i += 2 * merge_start) {
				//SetThreadAffinityMask(GetCurrentThread(), 1 << tid++);
				if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value)	// key-value
					origami_merger::mergebl2_scalar_kv<Item, NREG_1x>((Item*)src + i, merge_start, (Item*)src + i + merge_start, merge_start, (Item*)dst + i);
				else
					origami_merger::mergebl2_scalar<Item, NREG_1x>((Item*)src + i, merge_start, (Item*)src + i + merge_start, merge_start, (Item*)dst + i);
			}
			void* t = src; src = dst; dst = t;
		}
		DeleteCriticalSection(&cs);
		return (Item*)src;

	}


#undef SCALAR
#undef PHASE_3
#undef PHASE_4

	

}