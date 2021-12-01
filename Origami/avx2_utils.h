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

namespace avx2_utils {
	const int ROL_CONST = _MM_SHUFFLE(0, 3, 2, 1);
	const int ROR_CONST = _MM_SHUFFLE(2, 1, 0, 3);
	const int SHUFF_32_CONST = _MM_SHUFFLE(2, 3, 0, 1);						// 32 bit shuffle within each 64 bit block
	const int SHUFF_64_CONST = _MM_SHUFFLE(1, 0, 3, 2);						// 64 bit shuffle within each 128 bit block
	const avx2 SHUFF_128_CONST = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);	// 128 bit shuffle 

	// DIAG_EXCHANGE (v2) CONSTS
	const __mmask8 DIAG_32_BLEND_CONST = 0xAA;
	const __mmask8 DIAG_64_PERM_CONST = 0x4E;
	const __mmask8 DIAG_64_BLEND_CONST = 0xCC;
	const __mmask8 DIAG_128_PERM_CONST = 0x21;
	const __mmask8 DIAG_128_BLEND_CONST = 0xF0;

	// MCMERGE CONSTS
	const avx2 MN16_LEVEL1_PERM_CONST = _mm256_set_epi32(5, 0, 7, 2, 1, 6, 4, 3);
	const avx2 MN16_LEVEL2_PERM_CONST_0 = _mm256_set_epi32(3, 6, 2, 5, 4, 7, 1, 0);
	const avx2 MN16_LEVEL2_PERM_CONST_1 = _mm256_set_epi32(0, 2, 4, 7, 3, 6, 1, 5);
	const avx2 MN16_LEVEL3_PERM_CONST_0 = _mm256_set_epi32(7, 5, 6, 1, 2, 3, 4, 0);
	const avx2 MN16_LEVEL3_PERM_CONST_1 = _mm256_set_epi32(7, 6, 3, 4, 5, 2, 1, 0);
	const avx2 MN16_LEVEL3_PERM_CONST_2 = _mm256_set_epi32(6, 7, 4, 3, 2, 5, 1, 0);
	const avx2 MN16_LEVEL3_PERM_CONST_3 = _mm256_set_epi32(0, 6, 7, 2, 5, 4, 3, 1);
	

	// PRINT REG
	template <typename Item, typename Reg>
	FORCEINLINE void print(Reg& a0) {
		if constexpr (std::is_same<Reg, avx2>::value) {
			if constexpr (std::is_same<Item, ui>::value) {
				printf("%12lX ", _mm256_extract_epi32(a0, 0));
				printf("%12lX ", _mm256_extract_epi32(a0, 1));
				printf("%12lX ", _mm256_extract_epi32(a0, 2));
				printf("%12lX ", _mm256_extract_epi32(a0, 3));
				printf("%12lX ", _mm256_extract_epi32(a0, 4));
				printf("%12lX ", _mm256_extract_epi32(a0, 5));
				printf("%12lX ", _mm256_extract_epi32(a0, 6));
				printf("%12lX\n", _mm256_extract_epi32(a0, 7));
			}
			else if constexpr (std::is_same<Item, i64>::value || std::is_same<Item, KeyValue<i64, i64>>::value) {
				printf("%20llX ", _mm256_extract_epi64(a0, 0));
				printf("%20llX ", _mm256_extract_epi64(a0, 1));
				printf("%20llX ", _mm256_extract_epi64(a0, 2));
				printf("%20llX\n", _mm256_extract_epi64(a0, 3));
			}
		}
		else if constexpr (std::is_same<Reg, avx2f>::value) {
			printf("%f ", _mm256_extract_ps(a0, 0));
			printf("%f ", _mm256_extract_ps(a0, 1));
			printf("%f ", _mm256_extract_ps(a0, 2));
			printf("%f ", _mm256_extract_ps(a0, 3));
			printf("%f ", _mm256_extract_ps(a0, 4));
			printf("%f ", _mm256_extract_ps(a0, 5));
			printf("%f ", _mm256_extract_ps(a0, 6));
			printf("%f\n", _mm256_extract_ps(a0, 7));
		}
		else if constexpr (std::is_same<Reg, avx2d>::value) {
			printf("%lf ", _mm256_extract_pd(a0, 0));
			printf("%lf ", _mm256_extract_pd(a0, 1));
			printf("%lf ", _mm256_extract_pd(a0, 2));
			printf("%lf ", _mm256_extract_pd(a0, 3));
		}
	}

	// CAST REG
	template <typename RegTo, typename RegFrom>
	FORCEINLINE void cast_reg(RegTo& r0, RegFrom& r1) {
		if constexpr (std::is_same<RegTo, RegFrom>::value) {
			r0 = r1;
		}
		else if constexpr (std::is_same<RegTo, avx2>::value) {
			if constexpr (std::is_same<RegFrom, avx2f>::value)
				r0 = _mm256_castps_si256(r1);
			else if constexpr (std::is_same<RegFrom, avx2d>::value)
				r0 = _mm256_castpd_si256(r1);
		}
		else if constexpr (std::is_same<RegTo, avx2f>::value) {
			if constexpr (std::is_same<RegFrom, avx2>::value)
				r0 = _mm256_castsi256_ps(r1);
			else if constexpr (std::is_same<RegFrom, avx2d>::value)
				r0 = _mm256_castpd_ps(r1);
		}
		else if constexpr (std::is_same<RegTo, avx2d>::value) {
			if constexpr (std::is_same<RegFrom, avx2>::value)
				r0 = _mm256_castsi256_pd(r1);
			else if constexpr (std::is_same<RegFrom, avx2f>::value)
				r0 = _mm256_castps_pd(r1);
		}
	}

	// REVERSE
	template <typename Item, typename Reg>
	FORCEINLINE void reverse(Reg& a0) {
		if constexpr (std::is_same<Reg, avx2>::value) {
			if constexpr (sizeof(Item) == 4) a0 = _mm256_permutevar8x32_epi32(a0, _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7));
			if constexpr (sizeof(Item) == 8) a0 = _mm256_permute4x64_epi64(a0, _MM_SHUFFLE(0, 1, 2, 3));
			if constexpr (sizeof(Item) == 16) a0 = _mm256_permute2x128_si256(a0, a0, 0x01);
		}
		if constexpr (std::is_same<Reg, avx2f>::value) a0 = _mm256_permutevar8x32_ps(a0, _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7));
		if constexpr (std::is_same<Reg, avx2d>::value) a0 = _mm256_permute4x64_pd(a0, _MM_SHUFFLE(0, 1, 2, 3));
	}
	
	// SWAP
	template <typename Item, typename Reg>
	FORCEINLINE void swap(Reg& a0, Reg& a1) {
		// using templated min-max is slower
		// 32-bit
		if constexpr (std::is_same<Item, ui>::value) {
			Reg vTmp = _mm256_min_epu32(a0, a1);
			a1 = _mm256_max_epu32(a0, a1);
			a0 = vTmp;
		}
		else if constexpr (std::is_same<Item, float>::value) {
			Reg vTmp = _mm256_min_ps(a0, a1);
			a1 = _mm256_max_ps(a0, a1);
			a0 = vTmp;
		}
		// no min-max for 64 bit ints in avx2
		else if constexpr (std::is_same<Item, int64_t>::value) {
			Reg mask = _mm256_cmpgt_epi64(a0, a1);
			Reg vTmp = _mm256_blendv_epi8(a0, a1, mask);
			a1 = _mm256_blendv_epi8(a1, a0, mask);
			a0 = vTmp;
		}
		else if constexpr (std::is_same<Item, double>::value) {
			Reg vTmp = _mm256_min_pd(a0, a1);
			a1 = _mm256_max_pd(a0, a1);
			a0 = vTmp;
		}
		else if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			avx2 msk = _mm256_cmpgt_epi64(a0, a1);
			msk = _mm256_shuffle_epi32(msk, _MM_SHUFFLE(0, 0, 0, 0));
			avx2 max_key = _mm256_blendv_epi8(a1, a0, msk);
			a0 = _mm256_blendv_epi8(a0, a1, msk);
			a1 = max_key;
		}
	}
	
	// DIAG EXCHANGE
	template <typename Item, typename Reg, int bits>
	FORCEINLINE void diag_exchange(Reg& a0, Reg& a1) {
//#define v0
		if constexpr (bits == 32) {
			avx2 _a0, _a1;
			cast_reg<avx2, Reg>(_a0, a0);
			cast_reg<avx2, Reg>(_a1, a1);

			avx2 __a0 = _mm256_shuffle_epi32(_a0, ROL_CONST);
			avx2 __a1 = _mm256_shuffle_epi32(_a1, ROR_CONST);
			_a0 = _mm256_blend_epi32(_a0, __a1, DIAG_32_BLEND_CONST);
			_a1 = _mm256_blend_epi32(__a0, _a1, DIAG_32_BLEND_CONST);

			cast_reg<Reg, avx2>(a0, _a0);
			cast_reg<Reg, avx2>(a1, _a1);
		}
		else if constexpr (bits == 64) {
			avx2f _a0, _a1;
			cast_reg<avx2f, Reg>(_a0, a0);
			cast_reg<avx2f, Reg>(_a1, a1);
#ifdef v0
			// v0
			avx2f __a0 = _mm256_shuffle_ps(_a0, _a1, _MM_SHUFFLE(1, 0, 1, 0));
			_a1 = _mm256_shuffle_ps(_a0, _a1, _MM_SHUFFLE(3, 2, 3, 2));
			_a0 = __a0;
#else 
			
			// v1
			avx2f vTmp = _mm256_shuffle_ps(_a0, _a1, DIAG_64_PERM_CONST);
			_a0 = _mm256_blend_ps(_a0, vTmp, DIAG_64_BLEND_CONST);
			_a1 = _mm256_blend_ps(vTmp, _a1, DIAG_64_BLEND_CONST);
#endif

			cast_reg<Reg, avx2f>(a0, _a0);
			cast_reg<Reg, avx2f>(a1, _a1);
		}
		else if constexpr (bits == 128) {
			avx2 _a0, _a1;
			cast_reg<avx2, Reg>(_a0, a0);
			cast_reg<avx2, Reg>(_a1, a1);

			
#ifdef v0
			// v0
			avx2 v = _mm256_permute2x128_si256(_a0, _a1, 0x20); 
			_a1 = _mm256_permute2x128_si256(_a0, _a1, 0x31);
			_a0 = v;
#else 
			// v1
			avx2 v = _mm256_permute2x128_si256(_a0, _a1, DIAG_128_PERM_CONST);
			_a0 = _mm256_blend_epi32(_a0, v, DIAG_128_BLEND_CONST);
			_a1 = _mm256_blend_epi32(v, _a1, DIAG_128_BLEND_CONST);
#endif
			cast_reg<Reg, avx2>(a0, _a0);
			cast_reg<Reg, avx2>(a1, _a1);
		}
		else 
			ReportError("Only supports 32, 64 and 128 bits");
#undef v0
	}

	// ALIGNR for rswap
	template <typename Reg, ui bytes>
	FORCEINLINE Reg alignr(Reg a0) {
		if constexpr (bytes == 4) {
			if constexpr (std::is_same<Reg, avx2>::value) return _mm256_alignr_epi8(a0, a0, 4);
			if constexpr (std::is_same<Reg, avx2f>::value) return _mm256_castsi256_ps(_mm256_alignr_epi8(_mm256_castps_si256(a0), _mm256_castps_si256(a0), 4));
		}
		if constexpr (bytes == 8) {
			if constexpr (std::is_same<Reg, avx2>::value) return _mm256_alignr_epi8(a0, a0, 8);
			if constexpr (std::is_same<Reg, avx2d>::value) return _mm256_castsi256_pd(_mm256_alignr_epi8(_mm256_castpd_si256(a0), _mm256_castpd_si256(a0), 8));
		}
	}

	// SHUFFLE within REGISTER
	template <typename Reg, ui bits>
	FORCEINLINE void shuffle(Reg& a0) {
		if constexpr (bits == 128) {
			if constexpr (std::is_same<Reg, avx2>::value) a0 = _mm256_permutevar8x32_epi32(a0, SHUFF_128_CONST);
			if constexpr (std::is_same<Reg, avx2d>::value) a0 = _mm256_permutevar4x64_pd(a0, SHUFF_64_CONST);
			if constexpr (std::is_same<Reg, avx2f>::value) a0 = _mm256_permutevar8x32_ps(a0, SHUFF_128_CONST);
		}
		else if constexpr (bits == 64) {
			if constexpr (std::is_same<Reg, avx2>::value) a0 = _mm256_shuffle_epi32(a0, SHUFF_64_CONST);
			if constexpr (std::is_same<Reg, avx2d>::value) a0 = _mm256_castsi256_pd(_mm512_shuffle_epi32(_mm256_castpd_si256(a0), SHUFF_64_CONST));
			if constexpr (std::is_same<Reg, avx2f>::value) a0 = _mm256_castsi256_ps(_mm512_shuffle_epi32(_mm256_castps_si256(a0), SHUFF_64_CONST));
		}
		else if constexpr (bits == 32) {
			if constexpr (std::is_same<Reg, avx2>::value) a0 = _mm256_shuffle_epi32(a0, SHUFF_32_CONST);;
			if constexpr (std::is_same<Reg, avx2f>::value) a0 = _mm256_castsi256_ps(_mm512_shuffle_epi32(_mm256_castps_si256(a0), SHUFF_32_CONST));
		}
		else {
			ReportError("Works only for 32, 64 and 128 bits");
		}
	}

	// SORT REGISTER
	template <typename Item, typename Reg>
	FORCEINLINE void sort_reg(Reg& a0) {
		// *** NOTE: WORKS ONLY FOR SIGNED INTS!
		if constexpr (std::is_same<Reg, avx2>::value) {
			if constexpr (sizeof(Item) == 4) {
				Reg aux = a0;
				shuffle<Reg, 128>(aux);

				avx2 msk = _mm256_cmpgt_epi32(a0, aux);
				avx2 msk1 = _mm256_set_epi32(0, 0, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);

				avx2 msk2 = _mm256_or_si256(msk, msk1);
				msk1 = _mm256_and_si256(msk, msk1);

				a0 = _mm256_blendv_epi8(a0, aux, msk1);
				a0 = _mm256_blendv_epi8(aux, a0, msk2);

				aux = a0;
				shuffle<Reg, 64>(aux);
				msk = _mm256_cmpgt_epi32(a0, aux);
				msk1 = _mm256_set_epi32(0, 0, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0, 0xFFFFFFFF, 0xFFFFFFFF);

				msk2 = _mm256_or_si256(msk, msk1);
				msk1 = _mm256_and_si256(msk, msk1);

				a0 = _mm256_blendv_epi8(a0, aux, msk1);
				a0 = _mm256_blendv_epi8(aux, a0, msk2);

				aux = a0;
				shuffle<Reg, 32>(aux);
				msk = _mm256_cmpgt_epi32(a0, aux);
				msk1 = _mm256_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF, 0, 0xFFFFFFFF);

				msk2 = _mm256_or_si256(msk, msk1);
				msk1 = _mm256_and_si256(msk, msk1);

				a0 = _mm256_blendv_epi8(a0, aux, msk1);
				a0 = _mm256_blendv_epi8(aux, a0, msk2);
			}
			else if constexpr (sizeof(Item) == 8) {
				Reg aux = a0;
				shuffle<Reg, 128>(aux);

				avx2 msk = _mm256_cmpgt_epi64(a0, aux);
				avx2 msk1 = _mm256_set_epi64x(0, 0, 0xFFFFFFFF, 0xFFFFFFFF);

				avx2 msk2 = _mm256_or_si256(msk, msk1);
				msk1 = _mm256_and_si256(msk, msk1);

				a0 = _mm256_blendv_epi8(a0, aux, msk1);
				a0 = _mm256_blendv_epi8(aux, a0, msk2);

				aux = a0;
				shuffle<Reg, 64>(aux);
				msk = _mm256_cmpgt_epi64(a0, aux);
				msk1 = _mm256_set_epi64x(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);

				msk2 = _mm256_or_si256(msk, msk1);
				msk1 = _mm256_and_si256(msk, msk1);

				a0 = _mm256_blendv_epi8(a0, aux, msk1);
				a0 = _mm256_blendv_epi8(aux, a0, msk2);
			}
			else if constexpr (sizeof(Item) == 16) {
				Reg aux = a0;
				shuffle<Reg, 128>(aux);

				avx2 msk = _mm256_cmpgt_epi64(a0, aux);
				msk = _mm256_shuffle_epi32(msk, _MM_SHUFFLE(0, 0, 0, 0));
				avx2 msk1 = _mm256_set_epi64x(0, 0, 0xFFFFFFFF, 0xFFFFFFFF);

				avx2 msk2 = _mm256_or_si256(msk, msk1);
				msk1 = _mm256_and_si256(msk, msk1);

				a0 = _mm256_blendv_epi8(a0, aux, msk1);
				a0 = _mm256_blendv_epi8(aux, a0, msk2);
			}
		}
	}

	// RSWAP
	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1) {
#define SWAP_VER 0		// 0: swap and rotate, 1: regular bitonic, 2: mrmerge with sort_reg

#if SWAP_VER == 2
		// bitonic with mrmerge
		reverse<Item, Reg>(a1);
		swap<Item, Reg>(a0, a1);
		sort_reg<Item, Reg>(a0);
		sort_reg<Item, Reg>(a1);

#else
		if constexpr (std::is_same<Reg, avx2>::value) {
			if constexpr (sizeof(Item) == 4) {
#if SWAP_VER == 0
				swap<Item, Reg>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 4);
				swap<Item, Reg>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 4);
				swap<Item, Reg>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 4);
				swap<Item, Reg>(a0, a1); a0 = _mm256_permutexvar_epi32(_mm256_set_epi32(0, 3, 2, 1, 4, 7, 6, 5), a0);
				swap<Item, Reg>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 4);
				swap<Item, Reg>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 4);
				swap<Item, Reg>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 4);
				swap<Item, Reg>(a0, a1); a0 = _mm256_permutexvar_epi32(_mm256_set_epi32(0, 3, 2, 1, 4, 7, 6, 5), a0);

#elif SWAP_VER == 1
				// bitonic merge network
				reverse<Item, Reg>(a1);
				swap<Item, Reg>(a0, a1);
				diag_exchange<Item, Reg, 128>(a0, a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 64>(a0, a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 32>(a0, a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 32>(a0, a1);
				diag_exchange<Item, Reg, 64>(a0, a1);
				diag_exchange<Item, Reg, 128>(a0, a1);
#endif
			}

			if constexpr (sizeof(Item) == 8) {
#if SWAP_VER == 0
				swap<Item>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 8);
				swap<Item>(a0, a1); a0 = _mm256_permute4x64_epi64(a0, _MM_SHUFFLE(0, 1, 2, 3));
				swap<Item>(a0, a1); a0 = _mm256_alignr_epi8(a0, a0, 8);
				swap<Item>(a0, a1); a0 = _mm256_permute4x64_epi64(a0, _MM_SHUFFLE(0, 1, 2, 3));
#elif SWAP_VER == 1
				// bitonic network
				reverse<Item, Reg>(a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 128>(a0, a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 64>(a0, a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 64>(a0, a1);
				diag_exchange<Item, Reg, 128>(a0, a1);
#endif
			}
			// 128-bit KV
			if constexpr (sizeof(Item) == 16) {
#if SWAP_VER == 0
				swap<Item>(a0, a1); a0 = _mm256_permute2x128_si256(a0, a0, 0x01);
				swap<Item>(a0, a1); a0 = _mm256_permute2x128_si256(a0, a0, 0x01);
#elif SWAP_VER == 1
				// bitonic network
				reverse<Item, Reg>(a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 128>(a0, a1);
				swap<Item, Reg>(a0, a1);

				diag_exchange<Item, Reg, 128>(a0, a1);
#endif
			}
		}
		
#endif
#undef SWAP_VER
	}
	

	// MCMERGE SHUFFLE STUFF
	template <typename Item, typename Reg>
	FORCEINLINE void mn8_level2_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			// init: a0: a b c d; a1: g h e f
			// we want: so that we can perform (b, c); (d, e); (f, g)
			// a0 -> a b d f
			// a1 -> h c e g
			avx2 v0 = _mm256_shuffle_epi32(a0, _MM_SHUFFLE(0, 3, 2, 1));	// v0 = b c d a
			avx2 v1 = _mm256_shuffle_epi32(a1, _MM_SHUFFLE(0, 2, 3, 1));	// v1 = h f e g
			a0 = _mm256_blend_epi32(a0, v0, 0xCC);						// a =  a b d a
			a0 = _mm256_blend_epi32(a0, a1, 0x88);						// a =  a b d f
			a1 = _mm256_blend_epi32(v1, v0, 0x22);						// b =  h c e g
		}
		else if constexpr (sizeof(Item) == 8) {
			avx2 v0 = _mm256_permute4x64_epi64(a0, _MM_SHUFFLE(0, 3, 2, 1));
			avx2 v1 = _mm256_permute4x64_epi64(a1, _MM_SHUFFLE(0, 2, 3, 1));
			a0 = _mm256_blend_epi32(a0, v0, 0xF0);
			a0 = _mm256_blend_epi32(a0, a1, 0xC0);
			a1 = _mm256_blend_epi32(v1, v0, 0x0C);
		}
		else 
			ReportError("Works only for int 32 and 64 bits");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void mn16_level1_shuffle(Reg& a0) {
		if constexpr (sizeof(Item) == 4) 
			a0 = _mm256_permutevar8x32_epi32(a0, MN16_LEVEL1_PERM_CONST);
		else
			ReportError("Works only for int 32 bits");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void mn16_level2_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			a0 = _mm256_permutevar8x32_epi32(a0, MN16_LEVEL2_PERM_CONST_0); 
			a1 = _mm256_permutevar8x32_epi32(a1, MN16_LEVEL2_PERM_CONST_1);
			avx2 v0 = _mm256_shuffle_epi32(a0, SHUFF_64_CONST);
			avx2 v1 = _mm256_shuffle_epi32(a1, SHUFF_64_CONST);
			a0 = _mm256_blend_epi32(a0, v1, 0xC0);
			a1 = _mm256_blend_epi32(a1, v0, 0x30);
		}
		else
			ReportError("Works only for int 32 bits");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void mn16_level3_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			avx2 v0 = _mm256_permutevar8x32_epi32(a0, MN16_LEVEL3_PERM_CONST_0); 
			avx2 v1 = _mm256_permutevar8x32_epi32(a1, MN16_LEVEL3_PERM_CONST_1);
			a0 = _mm256_permutevar8x32_epi32(a0, MN16_LEVEL3_PERM_CONST_2);
			a0 = _mm256_blend_epi32(a0, v1, 0xA8);
			a1 = _mm256_permutevar8x32_epi32(a1, MN16_LEVEL3_PERM_CONST_3);
			a1 = _mm256_blend_epi32(a1, v0, 0x2A);
		}
		else
			ReportError("Works only for int 32 bits");
	}

	template <typename Item, typename Reg>
	FORCEINLINE Item get_last_item(Reg& a) {
		if constexpr (sizeof(Item) == 4)
			return _mm256_extract_epi32(a, 7);
		else if constexpr (sizeof(Item) == 8)
			return _mm256_extract_epi64(a, 3);
		else {
			KeyValue<i64, i64> x;
			x.key = _mm256_extract_epi64(a, 2);
			x.value = _mm256_extract_epi64(a, 3);
			return x;
		}
	}
}
