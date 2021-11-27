#pragma once

#include "commons.h"


namespace sse_utils {

	const int ROL_CONST = _MM_SHUFFLE(0, 3, 2, 1);
	const int ROR_CONST = _MM_SHUFFLE(2, 1, 0, 3);
	const int SHUFF_32_CONST = _MM_SHUFFLE(2, 3, 0, 1);			// 32 bit shuffle within lane
	const int SHUFF_64_CONST = _MM_SHUFFLE(1, 0, 3, 2);			// shuffle 64 bits across lane

	// v2 diag_exchange consts
	const __mmask8 DIAG_32_BLEND_CONST = 0xCC;
	const __mmask8 DIAG_64_PERM_CONST = 0x4E;
	const __mmask8 DIAG_64_BLEND_CONST = 0xCC;

	// PRINT REG
	template <typename Item, typename Reg>
	FORCEINLINE void print(Reg& a0) {
		if constexpr (std::is_same<Reg, sse>::value) {
			if constexpr (std::is_same<Item, ui>::value) {
				printf("%10lX ", _mm_extract_epi32(a0, 0));
				printf("%10lX ", _mm_extract_epi32(a0, 1));
				printf("%10lX ", _mm_extract_epi32(a0, 2));
				printf("%10lX\n", _mm_extract_epi32(a0, 3));
			}
			else if constexpr (std::is_same<Item, i64>::value || std::is_same<Item, KeyValue<i64, i64>>::value) {
				printf("%20llX ", _mm_extract_epi64(a0, 0));
				printf("%20llX\n", _mm_extract_epi64(a0, 1));
			}
		}
		else if constexpr (std::is_same<Reg, ssef>::value) {
			printf("%f ", _mm_extract_ps(a0, 0));
			printf("%f ", _mm_extract_ps(a0, 1));
			printf("%f ", _mm_extract_ps(a0, 2));
			printf("%f\n", _mm_extract_ps(a0, 3));
		}
		else {
			ReportError("Not implemented\n");
		}
	}

	// MIN
	template <typename Item, typename Reg>
	FORCEINLINE Reg _min(Reg a0, Reg a1) {
		if constexpr (std::is_same<Item, ui>::value) return _mm_min_epu32(a0, a1);
		if constexpr (std::is_same<Item, int>::value) return _mm_min_epi32(a0, a1);
		if constexpr (std::is_same<Item, float>::value) return _mm_min_ps(a0, a1);
		if constexpr (std::is_same<Item, double>::value) return _mm_min_pd(a0, a1);
	}
	

	// MAX
	template <typename Item, typename Reg>
	FORCEINLINE Reg _max(Reg a0, Reg a1) {
		if constexpr (std::is_same<Item, ui>::value) return _mm_max_epu32(a0, a1);
		if constexpr (std::is_same<Item, int>::value) return _mm_max_epi32(a0, a1);
		if constexpr (std::is_same<Item, float>::value) return _mm_max_ps(a0, a1);
		if constexpr (std::is_same<Item, double>::value) return _mm_max_pd(a0, a1);
	}

	// REVERSE
	template <typename Item, typename Reg>
	FORCEINLINE void reverse(Reg& a0) {
		if constexpr (std::is_same<Reg, sse>::value) {
			if constexpr (sizeof(Item) == 4) a0 = _mm_shuffle_epi32(a0, _MM_SHUFFLE(0, 1, 2, 3));
			if constexpr (sizeof(Item) == 8) a0 = _mm_shuffle_epi32(a0, _MM_SHUFFLE(1, 0, 3, 2));
		}
		if constexpr (std::is_same<Reg, ssef>::value) a0 = _mm_shuffle_ps(a0, a0, _MM_SHUFFLE(0, 1, 2, 3));
		if constexpr (std::is_same<Reg, ssed>::value) a0 = _mm_shuffle_pd(a0, a0, 0b01);
	}
	
	// SWAP
	template <typename Item, typename Reg>
	FORCEINLINE void swap(Reg& a0, Reg& a1) {
		// no min-max for 64 bit ints in sse
		if constexpr (std::is_same<Item, i64>::value) {
			Reg mask = _mm_cmpgt_epi64(a0, a1);
			Reg vTmp = _mm_blendv_epi8(a0, a1, mask);
			a1 = _mm_blendv_epi8(a1, a0, mask);
			a0 = vTmp;
		}
		else if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			sse msk = _mm_cmpgt_epi64(a0, a1);
			msk = _mm_shuffle_epi32(msk, _MM_SHUFFLE(0, 0, 0, 0));
			sse max_key = _mm_blendv_epi8(a1, a0, msk);
			a0 = _mm_blendv_epi8(a0, a1, msk);
			a1 = max_key;
		}
		else {
			Reg vTmp = _min<Item, Reg>(a0, a1);
			a1 = _max<Item, Reg>(a0, a1);
			a0 = vTmp;
		}
	}
	
	// ALIGNR for rswap
	template <typename Reg, ui bytes>
	FORCEINLINE Reg alignr(Reg a0) {
		if constexpr (bytes == 4) {
			if constexpr (std::is_same<Reg, sse>::value) return _mm_alignr_epi8(a0, a0, 4);
			if constexpr (std::is_same<Reg, ssef>::value) return _mm_castsi128_ps(_mm_alignr_epi8(_mm_castps_si128(a0), _mm_castps_si128(a0), 4));
		}
		if constexpr (bytes == 8) {
			if constexpr (std::is_same<Reg, sse>::value) return _mm_alignr_epi8(a0, a0, 8);
			if constexpr (std::is_same<Reg, ssed>::value) return _mm_castsi128_pd(_mm_alignr_epi8(_mm_castpd_si128(a0), _mm_castpd_si128(a0), 8));
		}
	}
	
	// CAST REG

	template <typename RegTo, typename RegFrom>
	FORCEINLINE void cast_reg(RegTo& r0, RegFrom& r1) {
		if constexpr (std::is_same<RegTo, RegFrom>::value) {
			r0 = r1;
		}
		else if constexpr (std::is_same<RegTo, sse>::value) {
			if constexpr (std::is_same<RegFrom, ssef>::value) 
				r0 = _mm_castps_si128(r1);
			else if constexpr (std::is_same<RegFrom, ssed>::value) 
				r0 = _mm_castpd_si128(r1);
		}
		else if constexpr (std::is_same<RegTo, ssef>::value) {
			if constexpr (std::is_same<RegFrom, sse>::value)
				r0 = _mm_castsi128_ps(r1);
			else if constexpr (std::is_same<RegFrom, ssed>::value)
				r0 = _mm_castpd_ps(r1);
		}
		else if constexpr (std::is_same<RegTo, ssed>::value) {
			if constexpr (std::is_same<RegFrom, sse>::value)
				r0 = _mm_castsi128_pd(r1);
			else if constexpr (std::is_same<RegFrom, ssef>::value)
				r0 = _mm_castps_pd(r1);
		}
	}

	// DIAG EXCHANGE
	template <typename Item, typename Reg, int bits> 
	FORCEINLINE void diag_exchange(Reg& a0, Reg& a1) {	
//#define v1
		if constexpr (bits == 32) {
			sse _a0, _a1;
			cast_reg<sse, Reg>(_a0, a0);
			cast_reg<sse, Reg>(_a1, a1);
			
			sse __a0 = _mm_shuffle_epi32(_a0, ROL_CONST); 
			sse __a1 = _mm_shuffle_epi32(_a1, ROR_CONST); 
			_a0 = _mm_blend_epi16(_a0, __a1, DIAG_32_BLEND_CONST);
			_a1 = _mm_blend_epi16(__a0, _a1, DIAG_32_BLEND_CONST);

			cast_reg<Reg, sse>(a0, _a0);
			cast_reg<Reg, sse>(a1, _a1);
		}
		else if constexpr (bits == 64) {
			ssef _a0, _a1;
			cast_reg<ssef, Reg>(_a0, a0);
			cast_reg<ssef, Reg>(_a1, a1);
			
			// v1
#ifdef v1
			ssef __a0 = _mm_shuffle_ps(_a0, _a1, _MM_SHUFFLE(1, 0, 1, 0));
			_a1 = _mm_shuffle_ps(_a0, _a1, _MM_SHUFFLE(3, 2, 3, 2));
			_a0 = __a0;
#else
			// v2
			ssef vTmp = _mm_shuffle_ps(_a0, _a1, DIAG_64_PERM_CONST);
			_a0 = _mm_blend_ps(_a0, vTmp, DIAG_64_BLEND_CONST);
			_a1 = _mm_blend_ps(vTmp, _a1, DIAG_64_BLEND_CONST);
#endif
			cast_reg<Reg, ssef>(a0, _a0);
			cast_reg<Reg, ssef>(a1, _a1);
		}
		else ReportError("Only supports 32 and 64 bits");
#undef v1
	}


	// SHUFFLE
	template <typename Reg, ui bits>
	FORCEINLINE void shuffle(Reg& a0) {
		if constexpr (bits == 32) {
			a0 = _mm_shuffle_epi32(a0, SHUFF_32_CONST);
		}
		else if constexpr (bits == 64) {
			a0 = _mm_shuffle_epi32(a0, SHUFF_64_CONST);
		}
		else {
			ReportError("Works only with 32 and 64 bits\n");
		}
	}


	// SORT REGISTER
	template <typename Item, typename Reg>
	FORCEINLINE void sort_reg(Reg& a0) {
		// *** NOTE: WORKS ONLY FOR SIGNED INTS!
		if constexpr (std::is_same<Reg, sse>::value) {
			if constexpr (sizeof(Item) == 4) {
				// *** NOTE: WORKS ONLY FOR SIGNED INTS!
				Reg aux = a0;
				shuffle<Reg, 64>(aux);
				sse msk = _mm_cmpgt_epi32(a0, aux);
				sse msk1 = _mm_set_epi32(0, 0, 0xFFFFFFFF, 0xFFFFFFFF);

				sse msk2 = _mm_or_si128(msk, msk1);
				msk1 = _mm_and_si128(msk, msk1);

				a0 = _mm_blendv_epi8(a0, aux, msk1);
				a0 = _mm_blendv_epi8(aux, a0, msk2);

				aux = a0;
				shuffle<Reg, 32>(aux);
				msk = _mm_cmpgt_epi32(a0, aux);
				msk1 = _mm_set_epi32(0, 0xFFFFFFFF, 0, 0xFFFFFFFF);

				msk2 = _mm_or_si128(msk, msk1);
				msk1 = _mm_and_si128(msk, msk1);

				a0 = _mm_blendv_epi8(a0, aux, msk1);
				a0 = _mm_blendv_epi8(aux, a0, msk2);
			}
			else if constexpr (sizeof(Item) == 8) {
				// *** NOTE: WORKS ONLY FOR SIGNED INTS!

				Reg aux = a0;
				shuffle<Reg, 64>(aux);

				sse msk = _mm_cmpgt_epi64(a0, aux);
				sse msk1 = _mm_set_epi64x(0, 0xFFFFFFFF);

				sse msk2 = _mm_or_si128(msk, msk1);
				msk1 = _mm_and_si128(msk, msk1);

				a0 = _mm_blendv_epi8(a0, aux, msk1);
				a0 = _mm_blendv_epi8(aux, a0, msk2);
			}
			else if constexpr (sizeof(Item) == 16) {
				// nothing to do
			}
		}
	}


	// rswap
	template <typename Item, typename Reg>
	FORCEINLINE void rswap(Reg& a0, Reg& a1) {
#define SWAP_VER 0	// 0: rotate and swap, 1: bitonic netwok, 2: mrmerge

		// key-value 64 + 64
		if constexpr (sizeof(Item) == 16) {
			swap<Item, Reg>(a0, a1);
		}
		else {
#if SWAP_VER == 2
			// bitonic with mrmerge
			reverse<Item, Reg>(a1);
			swap<Item, Reg>(a0, a1);
			sort_reg<Item, Reg>(a0);
			sort_reg<Item, Reg>(a1);
#else 

			if constexpr (sizeof(Item) == 4) {
#if SWAP_VER == 0
				// 12 instructions
				swap<Item, Reg>(a0, a1); a0 = alignr<Reg, 4>(a0);
				swap<Item, Reg>(a0, a1); a0 = alignr<Reg, 4>(a0);
				swap<Item, Reg>(a0, a1); a0 = alignr<Reg, 4>(a0);
				swap<Item, Reg>(a0, a1); a0 = alignr<Reg, 4>(a0);

#else 
				// base design in literature: bitonic merge
				reverse<Item, Reg>(a1);
				swap<Item, Reg>(a0, a1);
				diag_exchange<Item, Reg, 64>(a0, a1);
				swap<Item, Reg>(a0, a1);
				diag_exchange<Item, Reg, 32>(a0, a1);
				swap<Item, Reg>(a0, a1);
				diag_exchange<Item, Reg, 32>(a0, a1);
				diag_exchange<Item, Reg, 64>(a0, a1);
#endif 
			}
#if SWAP_VER == 0
			if constexpr (sizeof(Item) == 8) {
				swap<Item, Reg>(a0, a1); a0 = alignr<Reg, 8>(a0);
				swap<Item, Reg>(a0, a1); a0 = alignr<Reg, 8>(a0);
#else 
			// bitonic merge
			reverse<Item, Reg>(a1);
			swap<Item, Reg>(a0, a1);
			diag_exchange<Item, Reg, 64>(a0, a1);
			swap<Item, Reg>(a0, a1);
			diag_exchange<Item, Reg, 64>(a0, a1);
#endif
			}
#endif 
		}


#undef SWAP_VER
	}
	
	// MCMERGE stuff
	// mcmerge shuffle stuff
	template <typename Item, typename Reg>
	FORCEINLINE void mn8_level2_shuffle(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Item) == 4) {
			sse v0 = _mm_shuffle_epi32(a0, _MM_SHUFFLE(0, 3, 2, 1));
			sse v1 = _mm_shuffle_epi32(a1, _MM_SHUFFLE(0, 2, 3, 1));	// v1 = h f e g

			a0 = _mm_blend_epi16(a0, v0, 0xF0);						// a =  a b d a
			a0 = _mm_blend_epi16(a0, a1, 0xC0);						// a =  a b d f
			a1 = _mm_blend_epi16(v1, v0, 0xC);
		}
		else 
			ReportError("Works only for int 32 bits\n");
	}


	template <typename Item, typename Reg>
	FORCEINLINE Item get_last_item(Reg& a) {
		if constexpr (sizeof(Item) == 4)
			return _mm_extract_epi32(a, 3);
		else if constexpr (sizeof(Item) == 8)
			return _mm_extract_epi64(a, 1);
		else {
			KeyValue<i64, i64> x;
			x.key = _mm_extract_epi64(a, 0);
			x.value = _mm_extract_epi64(a, 1);
			return x;
		}
	}
}
