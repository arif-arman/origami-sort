#pragma once
#include "commons.h"

namespace avx512_utils {

	const ui mask1 = 0;
	const ui mask2 = 0xFFFF;

	const int ROL_CONST = _MM_SHUFFLE(0, 3, 2, 1);
	const int ROR_CONST = _MM_SHUFFLE(2, 1, 0, 3);
	const int SHUFF_32_CONST = _MM_SHUFFLE(2, 3, 0, 1);						// 32 bit shuffle within 64 bit lane
	const int SHUFF_64_CONST = _MM_SHUFFLE(1, 0, 3, 2);						// shuffle 64 bits within 128 bit lane
	const int SHUFF_128_CONST = _MM_SHUFFLE(2, 3, 0, 1);					// shuffle 128 bits within 256 bit lane

	// v1 diag_exchange consts --> all PERM consts
	const avx512 DIAG_128_CONST_v1_0 = _mm512_set_epi64(13, 12, 5, 4, 9, 8, 1, 0);		
	const avx512 DIAG_128_CONST_v1_1 = _mm512_set_epi64(15, 14, 7, 6, 11, 10, 3, 2);
	const avx512 DIAG_256_CONST_v1_0 = _mm512_set_epi64(11, 10, 9, 8, 3, 2, 1, 0);
	const avx512 DIAG_256_CONST_v1_1 = _mm512_set_epi64(15, 14, 13, 12, 7, 6, 5, 4);

	// v2 diag_exchange consts
	const __mmask16	DIAG_32_BLEND_CONST = 0xAAAA;
	const __mmask16	DIAG_64_BLEND_CONST = 0xCCCC;
	const avx512	DIAG_128_PERM_CONST = _mm512_set_epi64(13, 12, 7, 6, 9, 8, 3, 2);
	const __mmask8	DIAG_128_BLEND_CONST = 0xCC;
	const __mmask8	DIAG_256_BLEND_CONST = 0x0F;

	// MCMERGE CONSTS
	const avx512 MN16_LEVEL1_PERM_CONST_32 = _mm512_set_epi32(13, 8, 15, 10, 9, 14, 12, 11, 5, 0, 7, 2, 1, 6, 4, 3);
	const avx512 MN16_LEVEL1_PERM_CONST_64 = _mm512_set_epi64(5, 0, 7, 2, 1, 6, 4, 3);
	const avx512 MN16_LEVEL2_PERM_CONST_32_0 = _mm512_set_epi32(11, 14, 10, 13, 12, 15, 9, 8, 3, 6, 2, 5, 4, 7, 1, 0);
	const avx512 MN16_LEVEL2_PERM_CONST_32_1 = _mm512_set_epi32(8, 10, 12, 15, 11, 14, 9, 13, 0, 2, 4, 7, 3, 6, 1, 5);
	const avx512 MN16_LEVEL2_PERM_CONST_64_0 = _mm512_set_epi64(3, 6, 2, 5, 4, 7, 1, 0);
	const avx512 MN16_LEVEL2_PERM_CONST_64_1 = _mm512_set_epi64(0, 2, 4, 7, 3, 6, 1, 5);
	const avx512 MN16_LEVEL3_PERM_CONST_32_0 = _mm512_set_epi32(15, 13, 14, 9, 10, 11, 12, 8, 7, 5, 6, 1, 2, 3, 4, 0);
	const avx512 MN16_LEVEL3_PERM_CONST_32_1 = _mm512_set_epi32(15, 14, 11, 12, 13, 10, 9, 8, 7, 6, 3, 4, 5, 2, 1, 0);
	const avx512 MN16_LEVEL3_PERM_CONST_32_2 = _mm512_set_epi32(14, 15, 12, 11, 10, 13, 9, 8, 6, 7, 4, 3, 2, 5, 1, 0);
	const avx512 MN16_LEVEL3_PERM_CONST_32_3 = _mm512_set_epi32(8, 14, 15, 10, 13, 12, 11, 9, 0, 6, 7, 2, 5, 4, 3, 1);
	const avx512 MN16_LEVEL3_PERM_CONST_64_0 = _mm512_set_epi64(7, 5, 6, 1, 2, 3, 4, 0);
	const avx512 MN16_LEVEL3_PERM_CONST_64_1 = _mm512_set_epi64(7, 6, 3, 4, 5, 2, 1, 0);
	const avx512 MN16_LEVEL3_PERM_CONST_64_2 = _mm512_set_epi64(6, 7, 4, 3, 2, 5, 1, 0);
	const avx512 MN16_LEVEL3_PERM_CONST_64_3 = _mm512_set_epi64(0, 6, 7, 2, 5, 4, 3, 1);

	// PRINT REG
	template <typename Item, typename Reg>
	FORCEINLINE void print(Reg& r0) {
		if constexpr (std::is_same<Reg, avx512>::value) {
			if constexpr (std::is_same<Item, ui>::value) {
				sse a0 = _mm512_extracti32x4_epi32(r0, 0);
				printf("%12lX ", _mm_extract_epi32(a0, 0)); printf("%12lX ", _mm_extract_epi32(a0, 1)); printf("%12lX ", _mm_extract_epi32(a0, 2)); printf("%12lX ", _mm_extract_epi32(a0, 3));
				a0 = _mm512_extracti32x4_epi32(r0, 1);
				printf("%12lX ", _mm_extract_epi32(a0, 0)); printf("%12lX ", _mm_extract_epi32(a0, 1)); printf("%12lX ", _mm_extract_epi32(a0, 2)); printf("%12lX ", _mm_extract_epi32(a0, 3));
				a0 = _mm512_extracti32x4_epi32(r0, 2);
				printf("%12lX ", _mm_extract_epi32(a0, 0)); printf("%12lX ", _mm_extract_epi32(a0, 1)); printf("%12lX ", _mm_extract_epi32(a0, 2)); printf("%12lX ", _mm_extract_epi32(a0, 3));
				a0 = _mm512_extracti32x4_epi32(r0, 3);
				printf("%12lX ", _mm_extract_epi32(a0, 0)); printf("%12lX ", _mm_extract_epi32(a0, 1)); printf("%12lX ", _mm_extract_epi32(a0, 2)); printf("%12lX\n", _mm_extract_epi32(a0, 3)); 
			}
			else if constexpr (std::is_same<Item, i64>::value || std::is_same<Item, KeyValue<i64, i64>>::value) {
				avx2 a0 = _mm512_extracti64x4_epi64(r0, 0);
				printf("%20llX ", _mm256_extract_epi64(a0, 0)); printf("%20llX ", _mm256_extract_epi64(a0, 1)); printf("%20llX ", _mm256_extract_epi64(a0, 2)); printf("%20llX ", _mm256_extract_epi64(a0, 3));
				a0 = _mm512_extracti64x4_epi64(r0, 1);
				printf("%20llX ", _mm256_extract_epi64(a0, 0)); printf("%20llX ", _mm256_extract_epi64(a0, 1)); printf("%20llX ", _mm256_extract_epi64(a0, 2)); printf("%20llX\n", _mm256_extract_epi64(a0, 3));
			}
		}
		else if constexpr (std::is_same<Reg, avx512f>::value) {
			ssef a0 = _mm512_extractf32x4_ps(r0, 0);
			printf("%f ", _mm_extract_ps(a0, 0)); printf("%f ", _mm_extract_ps(a0, 1)); printf("%f ", _mm_extract_ps(a0, 2)); printf("%f ", _mm_extract_ps(a0, 3));
			a0 = _mm512_extractf32x4_ps(r0, 1);
			printf("%f ", _mm_extract_ps(a0, 0)); printf("%f ", _mm_extract_ps(a0, 1)); printf("%f ", _mm_extract_ps(a0, 2)); printf("%f ", _mm_extract_ps(a0, 3));
			a0 = _mm512_extractf32x4_ps(r0, 2);
			printf("%f ", _mm_extract_ps(a0, 0)); printf("%f ", _mm_extract_ps(a0, 1)); printf("%f ", _mm_extract_ps(a0, 2)); printf("%f ", _mm_extract_ps(a0, 3));
			a0 = _mm512_extractf32x4_ps(r0, 3);
			printf("%f ", _mm_extract_ps(a0, 0)); printf("%f ", _mm_extract_ps(a0, 1)); printf("%f ", _mm_extract_ps(a0, 2)); printf("%f\n", _mm_extract_ps(a0, 3));
		}
		else if constexpr (std::is_same<Reg, avx2d>::value) {
			avx2d a0 = _mm512_extractf64x4_pd(a0, 0);
			printf("%lf ", _mm256_extract_pd(a0, 0)); printf("%lf ", _mm256_extract_pd(a0, 1)); printf("%lf ", _mm256_extract_pd(a0, 2)); printf("%lf ", _mm256_extract_pd(a0, 3));
			a0 = _mm512_extractf64x4_pd(a0, 1);
			printf("%lf ", _mm256_extract_pd(a0, 0)); printf("%lf ", _mm256_extract_pd(a0, 1)); printf("%lf ", _mm256_extract_pd(a0, 2)); printf("%lf\n", _mm256_extract_pd(a0, 3));
		}
	}

	// CAST REG
	template <typename RegTo, typename RegFrom>
	FORCEINLINE void cast_reg(RegTo& r0, RegFrom& r1) {
		if constexpr (std::is_same<RegTo, RegFrom>::value) {
			r0 = r1;
		}
		else if constexpr (std::is_same<RegTo, avx512>::value) {
			if constexpr (std::is_same<RegFrom, avx512f>::value)
				r0 = _mm512_castps_si512(r1);
			else if constexpr (std::is_same<RegFrom, avx512d>::value)
				r0 = _mm512_castpd_si512(r1);
		}
		else if constexpr (std::is_same<RegTo, avx512f>::value) {
			if constexpr (std::is_same<RegFrom, avx512>::value)
				r0 = _mm512_castsi512_ps(r1);
			else if constexpr (std::is_same<RegFrom, avx512d>::value)
				r0 = _mm512_castpd_ps(r1);
		}
		else if constexpr (std::is_same<RegTo, avx512d>::value) {
			if constexpr (std::is_same<RegFrom, avx512>::value)
				r0 = _mm512_castsi512_pd(r1);
			else if constexpr (std::is_same<RegFrom, avx512f>::value)
				r0 = _mm512_castps_pd(r1);
		}
	}

	// MIN
	template <typename Item, typename Reg>
	FORCEINLINE Reg _min(Reg a0, Reg a1) {
		if constexpr (std::is_same<Item, ui>::value) return _mm512_min_epu32(a0, a1);
		else if constexpr (std::is_same<Item, int>::value) return _mm512_min_epi32(a0, a1);
		else if constexpr (std::is_same<Item, float>::value) return _mm512_min_ps(a0, a1);
		else if constexpr (std::is_same<Item, int64_t>::value) return _mm512_min_epi64(a0, a1);
		else if constexpr (std::is_same<Item, double>::value) return _mm512_min_pd(a0, a1);
		else {
			printf("Min not implemented\n");
			return a0;
		}
	}
	
	// MAX
	template <typename Item, typename Reg>
	FORCEINLINE Reg _max(Reg a0, Reg a1) {
		if constexpr (std::is_same<Item, ui>::value) return _mm512_max_epu32(a0, a1);
		else if constexpr (std::is_same<Item, int>::value) return _mm512_max_epi32(a0, a1);
		else if constexpr (std::is_same<Item, float>::value) return _mm512_max_ps(a0, a1);
		else if constexpr (std::is_same<Item, int64_t>::value) return _mm512_max_epi64(a0, a1);
		else if constexpr (std::is_same<Item, double>::value) return _mm512_max_pd(a0, a1);
		else {
			printf("Max not implemented\n");
			return a0;
		}
	}

	// REVERSE
	template <typename Item, typename Reg>
	FORCEINLINE void reverse(Reg& a0) {
		if constexpr (std::is_same<Reg, avx512>::value) {
			if constexpr (sizeof(Item) == 4) a0 = _mm512_permutevar_epi32(_mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), a0);
			if constexpr (sizeof(Item) == 8) a0 = _mm512_permutexvar_epi64(_mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7), a0);
			if constexpr (sizeof(Item) == 16) a0 = _mm512_permutexvar_epi64(_mm512_set_epi64(1, 0, 3, 2, 5, 4, 7, 6), a0);
		}
		else if constexpr (std::is_same<Reg, avx512f>::value) a0 = _mm512_permutevar_ps(a0, _mm512_set_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
		else if constexpr (std::is_same<Reg, avx512d>::value) a0 = _mm512_permutevar_pd(a0, _mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7));
		else printf("Reverse not implemented\n");
	}

	// MASK_MIN
	template <typename Item, typename Reg>
	FORCEINLINE Reg mask_min(Reg a0, Reg a1, ui64 mask) {
		if constexpr (std::is_same<Item, ui>::value) return _mm512_mask_min_epu32(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, int>::value) return _mm512_mask_min_epi32(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, float>::value) return _mm512_mask_min_ps(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, int64_t>::value) return _mm512_mask_min_epi64(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, double>::value) return _mm512_mask_min_pd(a0, mask, a0, a1);
		else printf("Mask-min not implemented\n");
	}

	// MASK_MAX
	template <typename Item, typename Reg>
	FORCEINLINE Reg mask_max(Reg a0, Reg a1, ui64 mask) {
		if constexpr (std::is_same<Item, ui>::value) return _mm512_mask_max_epu32(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, int>::value) return _mm512_mask_max_epi32(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, float>::value) return _mm512_mask_max_ps(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, int64_t>::value) return _mm512_mask_max_epi64(a0, mask, a0, a1);
		else if constexpr (std::is_same<Item, double>::value) return _mm512_mask_max_pd(a0, mask, a0, a1);
		else printf("Mask-max not implemented\n");
	}

	// SHUFFLE within REGISTER
	template <typename Reg, ui bits>
	FORCEINLINE void shuffle(Reg& a0) {
		if constexpr (bits == 256) {
			if constexpr (std::is_same<Reg, avx512>::value) a0 = _mm512_shuffle_i64x2(a0, a0, _MM_PERM_BADC);
			if constexpr (std::is_same<Reg, avx512d>::value) a0 = _mm512_shuffle_f64x2(a0, a0, _MM_PERM_BADC);
			if constexpr (std::is_same<Reg, avx512f>::value) a0 = _mm512_shuffle_f32x4(a0, a0, _MM_PERM_BADC);
		}
		else if constexpr (bits == 128) {
			if constexpr (std::is_same<Reg, avx512>::value) a0 = _mm512_shuffle_i64x2(a0, a0, _MM_PERM_CDAB);
			if constexpr (std::is_same<Reg, avx512d>::value) a0 = _mm512_shuffle_f64x2(a0, a0, _MM_PERM_CDAB);
			if constexpr (std::is_same<Reg, avx512f>::value) a0 = _mm512_shuffle_f32x4(a0, a0, _MM_PERM_CDAB);
		}
		else if constexpr (bits == 64) {
			if constexpr (std::is_same<Reg, avx512>::value) a0 = _mm512_shuffle_epi32(a0, _MM_PERM_BADC);
			if constexpr (std::is_same<Reg, avx512d>::value) a0 = _mm512_castsi512_pd(_mm512_shuffle_epi32(_mm512_castpd_si512(a0), _MM_PERM_BADC));
			if constexpr (std::is_same<Reg, avx512f>::value) a0 = _mm512_castsi512_ps(_mm512_shuffle_epi32(_mm512_castps_si512(a0), _MM_PERM_BADC));
		}
		else if constexpr (bits == 32) {
			if constexpr (std::is_same<Reg, avx512>::value) a0 = _mm512_shuffle_epi32(a0, SHUFF_32_CONST);
			if constexpr (std::is_same<Reg, avx512f>::value) a0 = _mm512_castsi512_ps(_mm512_shuffle_epi32(_mm512_castps_si512(a0), SHUFF_32_CONST));
		}
		else printf("> Works only for 32, 64, 128 and 256 bits\n");
	}

	// SORT_REGISTER
	template <typename Item, typename Reg>
	FORCEINLINE void sort_reg(Reg& a0) {
		// algorithm for 32-bit keys
		
		if constexpr (sizeof(Item) == 4) {
			const ui mask1 = 0;
			const ui mask2 = 0xFFFF;
			Reg aux = a0;
			shuffle<Reg, 256>(aux);

			a0 = mask_min<Item, Reg>(a0, aux, 0b0000000011111111 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b0000000011111111 ^ mask2);

			aux = a0;
			shuffle<Reg, 128>(aux);

			a0 = mask_min<Item, Reg>(a0, aux, 0b0000111100001111 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b0000111100001111 ^ mask2);

			aux = a0;
			shuffle<Reg, 64>(aux);

			a0 = mask_min<Item, Reg>(a0, aux, 0b0011001100110011 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b0011001100110011 ^ mask2);

			aux = a0;
			shuffle<Reg, 32>(aux);

			a0 = mask_min<Item, Reg>(a0, aux, 0b0101010101010101 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b0101010101010101 ^ mask2);
		}
		else if constexpr (sizeof(Item) == 8) {
			const ui mask2_64 = 0xFF;
			Reg aux = a0;
			shuffle<Reg, 256>(aux);
			a0 = mask_min<Item, Reg>(a0, aux, 0b00001111 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b00001111 ^ mask2);

			aux = a0;
			shuffle<Reg, 128>(aux);
			a0 = mask_min<Item, Reg>(a0, aux, 0b00110011 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b00110011 ^ mask2);

			aux = a0;
			shuffle<Reg, 64>(aux);
			a0 = mask_min<Item, Reg>(a0, aux, 0b01010101 ^ mask1);
			a0 = mask_max<Item, Reg>(a0, aux, 0b01010101 ^ mask2);
		}
		else if constexpr (sizeof(Item) == 16) {
			Reg aux = a0;
			shuffle<Reg, 256>(aux);
			__mmask8 msk = _mm512_mask_cmpgt_epi64_mask(0x55, a0, aux);
			__mmask8 msk_val = msk | (msk << 1);

			a0 = _mm512_mask_blend_epi64(msk_val & 0b00001111, a0, aux);
			a0 = _mm512_mask_blend_epi64((~msk_val & 0b11110000), a0, aux);

			aux = a0;
			shuffle<Reg, 128>(aux);
			msk = _mm512_mask_cmpgt_epi64_mask(0x55, a0, aux);
			msk_val = msk | (msk << 1);

			a0 = _mm512_mask_blend_epi64(msk_val & 0b00110011, a0, aux);
			a0 = _mm512_mask_blend_epi64((~msk_val & 0b11001100), a0, aux);
		}
		else {
			printf("Sort-register not implemented\n");
		}
	}
	
	// SWAP
	template <typename Item, typename Reg>
	FORCEINLINE void swap(Reg& a0, Reg& a1) {
		if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			// need cmpgt for this
			__mmask8 msk = _mm512_mask_cmpgt_epi64_mask(0x55, a0, a1);
			__mmask8 msk_val = msk | (msk << 1);
			
			avx512 max_key = _mm512_mask_blend_epi64(msk_val, a1, a0);
			a0 = _mm512_mask_blend_epi64(msk_val, a0, a1);
			a1 = max_key;
		}
		else {
			Reg vTmp = _min<Item, Reg>(a0, a1);
			a1 = _max<Item, Reg>(a0, a1);
			a0 = vTmp;
		}
	}

	template <typename Item, typename Reg, int bits>
	FORCEINLINE void diag_exchange(Reg& a0, Reg& a1) {
//#define v0
		if constexpr (bits == 32) {
			avx512 _a0, _a1;
			cast_reg<avx512, Reg>(_a0, a0);
			cast_reg<avx512, Reg>(_a1, a1);

			avx512 __a0 = _mm512_shuffle_epi32(_a0, ROL_CONST);
			avx512 __a1 = _mm512_shuffle_epi32(_a1, ROR_CONST);
			_a0 = _mm512_mask_blend_epi32(DIAG_32_BLEND_CONST, _a0, __a1);
			_a1 = _mm512_mask_blend_epi32(DIAG_32_BLEND_CONST, __a0, _a1);

			cast_reg<Reg, avx512>(a0, _a0);
			cast_reg<Reg, avx512>(a1, _a1);
		}
		else if constexpr (bits == 64) {
			avx512f _a0, _a1;
			cast_reg<avx512f, Reg>(_a0, a0);
			cast_reg<avx512f, Reg>(_a1, a1);

			// v0
#ifdef v0
			avx512f __a0 = _mm512_shuffle_ps(_a0, _a1, _MM_SHUFFLE(1, 0, 1, 0));
			_a1 = _mm512_shuffle_ps(_a0, _a1, _MM_SHUFFLE(3, 2, 3, 2));
			_a0 = __a0;
#else 
			// v1
			avx512f vTmp = _mm512_shuffle_ps(_a0, _a1, 0x4E4E);
			_a0 = _mm512_mask_blend_ps(DIAG_64_BLEND_CONST, _a0, vTmp);
			_a1 = _mm512_mask_blend_ps(DIAG_64_BLEND_CONST, vTmp, _a1);
#endif

			cast_reg<Reg, avx512f>(a0, _a0);
			cast_reg<Reg, avx512f>(a1, _a1);
		}
		else if constexpr (bits == 128) {
			avx512d _a0, _a1;
			cast_reg<avx512d, Reg>(_a0, a0);
			cast_reg<avx512d, Reg>(_a1, a1);

#ifdef v0
			
			avx512d __a0 = _mm512_permutex2var_pd(_a0, DIAG_128_CONST_v1_0, _a1);
			_a1 = _mm512_permutex2var_pd(_a0, DIAG_128_CONST_v1_1, _a1);
			_a0 = __a0;

#else 
			// v1
			avx512d v = _mm512_permutex2var_pd(_a0, DIAG_128_PERM_CONST, _a1);
			_a0 = _mm512_mask_blend_pd(DIAG_128_BLEND_CONST, _a0, v);
			_a1 = _mm512_mask_blend_pd(DIAG_128_BLEND_CONST, v, _a1);
#endif
			cast_reg<Reg, avx512d>(a0, _a0);
			cast_reg<Reg, avx512d>(a1, _a1);
		}
		else if constexpr (bits == 256) {
			avx512d _a0, _a1;
			cast_reg<avx512d, Reg>(_a0, a0);
			cast_reg<avx512d, Reg>(_a1, a1);

#ifdef v0
			avx512d __a0 = _mm512_permutex2var_pd(_a0, DIAG_256_CONST_v1_0, _a1);
			_a1 = _mm512_permutex2var_pd(_a0, DIAG_256_CONST_v1_1, _a1);
			_a0 = __a0;
#else 	
			// v1
			avx512d t = _mm512_shuffle_f64x2(_a0, _a1, _MM_SHUFFLE(1, 0, 3, 2));
			_a0 = _mm512_mask_blend_pd(DIAG_256_BLEND_CONST, t, _a0);
			_a1 = _mm512_mask_blend_pd(DIAG_256_BLEND_CONST, _a1, t);
#endif
			cast_reg<Reg, avx512d>(a0, _a0);
			cast_reg<Reg, avx512d>(a1, _a1);
		}
		else ReportError("Only supports 32, 64, 128 and 256 bits");
#undef v0
	}

	// RSWAP
	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1) {
#define SWAP_VER 2		// 0: swap and rotate, 1: regular bitonic, 2: mrmerge with sort_reg
		
		// mrmerge
#if SWAP_VER == 2
		reverse<Item, Reg>(a1);
		swap<Item, Reg>(a0, a1);
		sort_reg<Item, Reg>(a0);
		sort_reg<Item, Reg>(a1);

#else
		// others: rotate and swap; bitonic network
		if constexpr (sizeof(Item) == 4) {
			// rotate and swap
#if SWAP_VER == 0
			// 2.2 B
			/*swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi32(a0, a0, 1);*/

			// 3.3 B
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutevar_epi32(_mm512_set_epi32(8, 11, 10, 9, 12, 15, 14, 13, 0, 3, 2, 1, 4, 7, 6, 5), a0);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutevar_epi32(_mm512_set_epi32(0, 3, 2, 1, 4, 7, 6, 5, 8, 11, 10, 9, 12, 15, 14, 13), a0);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutevar_epi32(_mm512_set_epi32(8, 11, 10, 9, 12, 15, 14, 13, 0, 3, 2, 1, 4, 7, 6, 5), a0);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 4);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutevar_epi32(_mm512_set_epi32(0, 3, 2, 1, 4, 7, 6, 5, 8, 11, 10, 9, 12, 15, 14, 13), a0);

#elif SWAP_VER == 1
			// bitonic network: 3.3 B
			reverse<Item, Reg>(a1);
			swap<Item, Reg>(a0, a1);
			diag_exchange<Item, Reg, 256>(a0, a1);
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
			diag_exchange<Item, Reg, 256>(a0, a1);
#endif
		}
		else if constexpr (sizeof(Item) == 8) {
#if SWAP_VER == 0
			// 1.48
			/*swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi64(a0, a0, 1);*/

			// 1.56
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 8);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutex_epi64(a0, _MM_SHUFFLE(0, 1, 2, 3));
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 8);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutexvar_epi64(_mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7), a0);
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 8);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutex_epi64(a0, _MM_SHUFFLE(0, 1, 2, 3));
			swap<Item, Reg>(a0, a1); a0 = _mm512_alignr_epi8(a0, a0, 8);
			swap<Item, Reg>(a0, a1); a0 = _mm512_permutexvar_epi64(_mm512_set_epi64(0, 1, 2, 3, 4, 5, 6, 7), a0);
#elif SWAP_VER == 1
			// bitonic
			reverse<Item, Reg>(a1);
			swap<Item, Reg>(a0, a1);
			diag_exchange<Item, Reg, 256>(a0, a1);
			swap<Item, Reg>(a0, a1);

			diag_exchange<Item, Reg, 128>(a0, a1);
			swap<Item, Reg>(a0, a1);

			diag_exchange<Item, Reg, 64>(a0, a1);
			swap<Item, Reg>(a0, a1);

			diag_exchange<Item, Reg, 64>(a0, a1);
			diag_exchange<Item, Reg, 128>(a0, a1);
			diag_exchange<Item, Reg, 256>(a0, a1); 
#endif
		}
		else if constexpr (sizeof(Item) == 16) {
#if SWAP_VER == 0
			swap<Item, Reg>(a0, a1);  a0 = _mm512_alignr_epi64(a0, a0, 2);
			swap<Item, Reg>(a0, a1);  a0 = _mm512_alignr_epi64(a0, a0, 2);
			swap<Item, Reg>(a0, a1);  a0 = _mm512_alignr_epi64(a0, a0, 2);
			swap<Item, Reg>(a0, a1);  a0 = _mm512_alignr_epi64(a0, a0, 2);

#elif SWAP_VER == 1 
			// bitonic
			reverse<Item, Reg>(a1);
			swap<Item, Reg>(a0, a1);
			diag_exchange<Item, Reg, 256>(a0, a1);
			swap<Item, Reg>(a0, a1);

			diag_exchange<Item, Reg, 128>(a0, a1);
			swap<Item, Reg>(a0, a1);

			diag_exchange<Item, Reg, 64>(a0, a1);
			swap<Item, Reg>(a0, a1);

			diag_exchange<Item, Reg, 64>(a0, a1);
			diag_exchange<Item, Reg, 128>(a0, a1);
			diag_exchange<Item, Reg, 256>(a0, a1);
#endif
		}
#endif
#undef SWAP_VER
	}

	// MCMERGE shuffle stuff
	template <typename Item, typename Reg>
	FORCEINLINE void mn8_level2_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			avx512 v0 = _mm512_shuffle_epi32(a0, _MM_SHUFFLE(0, 3, 2, 1));
			avx512 v1 = _mm512_shuffle_epi32(a1, _MM_SHUFFLE(0, 2, 3 ,1));
			a0 = _mm512_mask_blend_epi32(0xCCCC, a0, v0);
			a0 = _mm512_mask_blend_epi32(0x8888, a0, a1);
			a1 = _mm512_mask_blend_epi32(0x2222, v1, v0);
		}
		else if constexpr (sizeof(Item) == 8) {
			avx512 v0 = _mm512_permutex_epi64(a0, _MM_SHUFFLE(0, 3, 2, 1)); 
			avx512 v1 = _mm512_permutex_epi64(a1, _MM_SHUFFLE(0, 2, 3 ,1));
			a0 = _mm512_mask_blend_epi64(0xCC, a0, v0);
			a0 = _mm512_mask_blend_epi64(0x88, a0, a1);
			a1 = _mm512_mask_blend_epi64(0x22, v1, v0);
		}
		else if constexpr (sizeof(Item) == 16) {
			avx512 v0 = _mm512_permutexvar_epi64(_mm512_set_epi64(1, 0, 7, 6, 5, 4, 3, 2), a0);
			avx512 v1 = _mm512_permutexvar_epi64(_mm512_set_epi64(1, 0, 5, 4, 7, 6, 3, 2), a1);
			a0 = _mm512_mask_blend_epi64(0xF0, a0, v0);
			a0 = _mm512_mask_blend_epi64(0xC0, a0, a1);
			a1 = _mm512_mask_blend_epi64(0xC, v1, v0);
		}
		else 
			ReportError("Works only for int 32, 64, and 64 + 64 bits");
		
	}

	template <typename Item, typename Reg>
	FORCEINLINE void mn16_level1_shuffle(Reg& a0) {
		if constexpr (sizeof(Item) == 4)
			a0 = _mm512_permutexvar_epi32(MN16_LEVEL1_PERM_CONST_32, a0);
		else if constexpr (sizeof(Item) == 8)
			a0 = _mm512_permutexvar_epi64(MN16_LEVEL1_PERM_CONST_64, a0);
		else
			ReportError("Works only for 32 and 64 bits");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void mn16_level2_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			a0 = _mm512_permutexvar_epi32(MN16_LEVEL2_PERM_CONST_32_0, a0);
			a1 = _mm512_permutexvar_epi32(MN16_LEVEL2_PERM_CONST_32_1, a1);
			avx512 v0 = _mm512_shuffle_epi32(a0, SHUFF_64_CONST);
			avx512 v1 = _mm512_shuffle_epi32(a1, SHUFF_64_CONST);
			a0 = _mm512_mask_blend_epi32(0xC0C0, a0, v1);
			a1 = _mm512_mask_blend_epi32(0x3030, a1, v0);
		}
		else if constexpr (sizeof(Item) == 8) {
			a0 = _mm512_permutexvar_epi64(MN16_LEVEL2_PERM_CONST_64_0, a0);
			a1 = _mm512_permutexvar_epi64(MN16_LEVEL2_PERM_CONST_64_1, a1);
			avx512 v0 = _mm512_shuffle_i64x2(a0, a0, SHUFF_128_CONST);
			avx512 v1 = _mm512_shuffle_i64x2(a1, a1, SHUFF_128_CONST);
			a0 = _mm512_mask_blend_epi64(0xC0, a0, v1);
			a1 = _mm512_mask_blend_epi64(0x30, a1, v0);
		}
		else
			ReportError("Works only for int 32 and 64 bits");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void mn16_level3_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			avx512 v0 = _mm512_permutexvar_epi32(MN16_LEVEL3_PERM_CONST_32_0, a0);
			avx512 v1 = _mm512_permutexvar_epi32(MN16_LEVEL3_PERM_CONST_32_1, a1);
			a0 = _mm512_permutexvar_epi32(MN16_LEVEL3_PERM_CONST_32_2, a0);
			a0 = _mm512_mask_blend_epi32(0xA8A8, a0, v1);
			a1 = _mm512_permutexvar_epi32(MN16_LEVEL3_PERM_CONST_32_3, a1);
			a1 = _mm512_mask_blend_epi32(0x2A2A, a1, v0);
		}
		else if constexpr (sizeof(Item) == 8) {
			avx512 v0 = _mm512_permutexvar_epi64(MN16_LEVEL3_PERM_CONST_64_0, a0);
			avx512 v1 = _mm512_permutexvar_epi64(MN16_LEVEL3_PERM_CONST_64_1, a1);
			a0 = _mm512_permutexvar_epi64(MN16_LEVEL3_PERM_CONST_64_2, a0);
			a0 = _mm512_mask_blend_epi64(0xA8, a0, v1);
			a1 = _mm512_permutexvar_epi64(MN16_LEVEL3_PERM_CONST_64_3, a1);
			a1 = _mm512_mask_blend_epi64(0x2A, a1, v0);
		}
		else
			ReportError("Works only for int 32 and 64 bits");
	}


	template <typename Item, typename Reg>
	FORCEINLINE Item get_last_item(Reg& a) {
		sse t = _mm512_extracti32x4_epi32(a, 3);
		if constexpr (sizeof(Item) == 4)
			return _mm_extract_epi32(t, 3);
		else if constexpr (sizeof(Item) == 8)
			return _mm_extract_epi64(t, 1);
		else {
			KeyValue<i64, i64> x;
			x.key = _mm_extract_epi64(t, 0);
			x.value = _mm_extract_epi64(t, 1);
			return x;
		}
	}
}
