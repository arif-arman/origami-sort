#pragma once

// TODO: add headers that you want to pre-compile here
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <intrin.h>		// MMX
#include <xmmintrin.h>	// SSE
#include <emmintrin.h>	// SSE2
#include <smmintrin.h>	// SSE4
#include <immintrin.h>	// AVX
#include <zmmintrin.h>	// AVX
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <thread>
#include <omp.h>
#include <queue>

using namespace std::chrono;
using hrc = high_resolution_clock;

// typedef key
typedef int64_t  i64;
typedef uint64_t ui64;
typedef uint32_t ui;

#pragma pack(push, 1)
template <typename Keytype, typename Valuetype>
struct KeyValue {
	Keytype key;
	Valuetype value;
	bool operator <(const KeyValue& kv) const {
		return key < kv.key;
	}
	bool operator >(const KeyValue& kv) const {
		return key > kv.key;
	}
	bool operator !=(const KeyValue& kv) const {
		return key != kv.key;
	}
	bool operator <=(const KeyValue& kv) const {
		return key <= kv.key;
	}
};
#pragma pack(pop)
template struct KeyValue<ui, ui>;
template struct KeyValue<i64, i64>;

#define MAX_THREADS	64

// typedef simd stuff
typedef __m128i sse;
typedef __m128	ssef;
typedef __m128d	ssed;
typedef __m256i avx2;
typedef __m256	avx2f;
typedef __m256d avx2d;
typedef __m512i avx512;
typedef __m512	avx512f;
typedef __m512d avx512d;

#define KEYS_PER_REG_AVX		(sizeof(avx2) / Keysize)	
#define KEYS_PER_REG_AVX512		(sizeof(avx512) / Keysize)	
#define KEYS_PER_REG_SSE		(sizeof(sse) / Keysize)	

#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)<(y)?(y):(x)) 
#define FOR(i,n,k)				for (ui64 (i) = 0; (i) < (n); (i)+=(k)) 
#define FOR_INIT(i, init, n, k)	for (ui64 (i) = (init); (i) < (n); (i) += (k)) 
#define PRINT_ARR(arr, n)		{ FOR((i), (n), 1) printf("%lu ", (arr)[(i)]); printf("\n"); }
#define PRINT_ARR64(arr, n)		{ FOR((i), (n), 1) printf("%llX ", ((ui64*)arr)[(i)]); printf("\n"); }
#define PRINT_DASH(n)			{ FOR(i, (n), 1) printf("-"); printf("\n"); }
#define ELAPSED(st, en)			( duration_cast<duration<double>>(en - st).count() )
#define ELAPSED_MS(st, en)		( duration_cast<duration<double, std::milli>>(en - st).count() )
#define NOINLINE				__declspec(noinline)
#define KB(x)					(x << 10)
#define MB(x)					(x << 20)
#define GB(x)					(x << 30)
#define HERE(x)					printf("Here %3lu\n", (x));
#define MAX_PATH_LEN			512
#define MAX_PRINTOUT			1024
#define PRINT(fmt, ...)			{ char buf_PRINT[MAX_PRINTOUT] = "%s: "; strcat_s(buf_PRINT, MAX_PRINTOUT, fmt); printf (buf_PRINT, __FUNCTION__, ##__VA_ARGS__); }
#define ReportError(fmt, ...)	{ PRINT(fmt, ##__VA_ARGS__); getchar(); exit(-1); }

#define LOAD(rg, ptr)			{ rg = *(ptr); }
#define STORE(rg, ptr)			{ *(ptr) = rg; }
#define VALLOC(sz)				(VirtualAlloc(NULL, (sz), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))
#define VFREE(ptr)				(VirtualFree((ptr), 0, MEM_RELEASE))

#define SWAP SWAPv2
#define SWAP2 SWAPv3
#define SWAPKV(x, y) {\
	{\
		const bool first = a##x.key < a##y.key;\
		const Item vTmp = first ? a##x : a##y;\
		a##y = first ? a##y : a##x;\
		a##x = vTmp;\
	}\
}
#define SWAPKV2(x,y) {\
    const bool bLess = a##x < a##y; \
    const int64_t tmp = bLess ? a##x : a##y; \
    a##y = bLess ? a##y : a##x; \
	a##x = tmp; \
    int64_t tmp2 = bLess ? b##x : b##y; \
    b##y = bLess ? b##y : b##x; \
    b##x = tmp2; \
}


const int rol_const = _MM_SHUFFLE(0, 3, 2, 1);
const int ror_const = _MM_SHUFFLE(2, 1, 0, 3);
const int shuff_32_const = _MM_SHUFFLE(2, 3, 0, 1);			// 32 bit shuffle within lane
const int shuff_64_const = _MM_SHUFFLE(1, 0, 3, 2);


#define SWAPv2(x,y) {\
	{\
		const Key tmp = MIN(a##x, a##y); \
		a##y = MAX(a##x, a##y); \
		a##x = tmp; \
	}\
}
#define SWAPv3(x,y) {\
	{\
		const Item tmp = MIN(a##x, a##y); \
		a##y = MAX(a##x, a##y); \
		a##x = tmp; \
	}\
}

#include "config.h"
