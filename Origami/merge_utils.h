#pragma once
#include "commons.h"
#include "scalar_utils.h"
#include "sse_utils.h"
#include "avx2_utils.h"
#include "avx512_utils.h"

namespace origami_utils {
//#define _DEBUG
	class IOHelper {
	public:
		HANDLE* fp_in, * fp_out;
		ui64* bytes_left;
		char** X, ** endX;
		char* out, * out_end;
		ui64 out_buff_size, in_buff_size;
		ui64 tot_bytes_written = 0;
		ui num_f_handles;

		void init_file_ptrs(char* file_path, ui64 sorted_size) {
			HANDLE h_in = CreateFile(
				file_path,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);

			LARGE_INTEGER li_file_end;
			BOOL bRet = GetFileSizeEx(h_in, &li_file_end);
			if (bRet == false) {
				printf("external_sorter/init_file_ptrs: GetFileSizeEx error %d\n", GetLastError());
				exit(-1);
			}
			ui64 file_end_byte = li_file_end.QuadPart;
			ui64 num_ptrs = file_end_byte / sorted_size;

			fp_in = new HANDLE[num_ptrs];
			bytes_left = new ui64[num_ptrs];
			num_f_handles = num_ptrs;

			LARGE_INTEGER li_dist;
			ui i = 0;
			FOR(curr_byte, file_end_byte, sorted_size) {
				HANDLE h_tmp = CreateFile(
					file_path,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL
				);
				li_dist.QuadPart = curr_byte;
				BOOL bRet = SetFilePointerEx(
					h_tmp,
					li_dist,
					NULL,
					FILE_BEGIN
				);
				if (bRet < 0) {
					printf("SetFilePointerEx error %d\n", GetLastError());
					exit(-1);
				}

				fp_in[i] = h_tmp;
				bytes_left[i++] = sorted_size;
			}
			CloseHandle(h_in);

			fp_out = new HANDLE[1];
			fp_out[0] = CreateFile(
				"merged.dat", // "C:\\Users\\arif\\Desktop\\External\\merged.dat",
				GENERIC_WRITE | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
		}

		void cleanup_fileptrs() {
			FOR(i, num_f_handles, 1) 
				CloseHandle(fp_in[i]);
			delete[] bytes_left;
		}

		void init_buffers(char* buf, ui WAY, ui64 mem_size) {
			X = new char* [WAY];
			endX = new char* [WAY];

			out_buff_size = MB(1);				// 256
			in_buff_size = mem_size / WAY;		// 256
			char* p = buf;
			FOR(i, WAY, 1) {
				X[i] = p;
				endX[i] = p + in_buff_size;
				p += in_buff_size + 64;
			}
			out = p;
			out_end = p + out_buff_size;
		}

		void cleanup_buffers() {
			delete[] X;
			delete[] endX;
		}

		void fill_buffer(ui idx) {
#ifdef _DEBUG
			printf("Filling buffer for node: %lu ...", idx);
#endif 
			HANDLE f = fp_in[idx];
			ui64 bytes = min(in_buff_size, bytes_left[idx]);
			DWORD bytes_read;
			char* p = X[idx];
			ui64 tot_bytes_read = 0;
			DWORD max_read = 4294967295;
			while (bytes > 0) {
				DWORD bytes_to_read = min(max_read, bytes);
				BOOL bRet = ReadFile(
					f,
					p,
					bytes_to_read,
					&bytes_read,
					NULL
				);
				if (bRet == false) {
					printf("ReadFile failed with %d\n", GetLastError());
					exit(-1);
				}
				tot_bytes_read += bytes_read;
				bytes -= bytes_read;
				p += bytes_read;
			}
			
			endX[idx] = X[idx] + tot_bytes_read;
			bytes_left[idx] -= tot_bytes_read;
#ifdef _DEBUG
			printf("Loaded: %llu, Left: %llu bytes\n", tot_bytes_read, bytes_left[idx]);
			/*ui* p = (ui*)X[idx];
			ui64 n = (endX[idx] - X[idx]) / 4;
			PRINT_ARR(p, n);*/
#endif 
		}
	
		void dump_buffer(ui idx, char* endpos) {
			ui64 bytes = endpos - out;
#ifdef _DEBUG
			printf("Dump output buffer -- %llu bytes ... ", bytes);
#endif 
			HANDLE h_write = fp_out[idx];
			DWORD bytesWritten;
			int bWrt = WriteFile(h_write, out, bytes, &bytesWritten, NULL);
			if (bWrt == 0) {
				printf("WriteFile failed with %d\n", GetLastError());
				exit(-1);
			}
#ifdef _DEBUG
			tot_bytes_written += bytesWritten;
			printf("done, written %llu bytes (tot: %llu)\n", bytesWritten, tot_bytes_written);
#endif 
			tot_bytes_written += bytesWritten;
			if (tot_bytes_written % GB(1LLU) == 0) {
				printf("                                                                          \r");
				printf("Written: %llu", tot_bytes_written);
			}
		}
	};

	class MergeStream {
	public:
		char* A;
		ui64 len;		// length in bytes

		MergeStream(char* A, ui64 len) {
			this->A = A; this->len = len;
		}
	};

	template <typename Reg, ui NREG = 1>
	FORCEINLINE ui64 align(char* p) {
		return ((ui64)p & (~(sizeof(Reg) * NREG - 1)));
	}

	template <typename Reg, ui NREG = 1>
	FORCEINLINE ui64 aligned(char* p) {
		return (((ui64)p & (sizeof(Reg) * NREG - 1)) == 0);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void print(Reg a0) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::print<Item, Reg>(a0);
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value) avx2_utils::print<Item, Reg>(a0);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::print<Item, Reg>(a0);
		else {
			ReportError("Scalar print, not implemented");
		}
	}

	template <typename Reg>
	FORCEINLINE void load(Reg& rg, Reg* ptr) {
		if constexpr (std::is_same<Reg, sse>::value) rg = _mm_load_si128(ptr); 
		else if constexpr (std::is_same<Reg, ssef>::value) rg = _mm_load_ps((float*)ptr);
		else if constexpr (std::is_same<Reg, avx2>::value) rg = _mm256_load_si256(ptr);
		else if constexpr (std::is_same<Reg, avx2f>::value) rg = _mm256_load_ps((float*)ptr);
		else if constexpr (std::is_same<Reg, avx512>::value) rg = _mm512_load_si512(ptr);
		else if constexpr (std::is_same<Reg, avx512f>::value) rg = _mm512_load_ps((float*)ptr);
		else rg = ptr[0];
	}
	

	template <typename Reg, bool stream = false>
	FORCEINLINE void store(Reg rg, Reg* ptr) {
		if constexpr (stream) {
			if constexpr (std::is_same<Reg, sse>::value) _mm_stream_si128(ptr, rg);
			else if constexpr (std::is_same<Reg, ssef>::value) _mm_stream_ps((float*)ptr, rg);
			else if constexpr (std::is_same<Reg, avx2>::value) _mm256_stream_si256(ptr, rg);
			else if constexpr (std::is_same<Reg, avx2f>::value) _mm256_stream_ps((float*)ptr, rg);
			else if constexpr (std::is_same<Reg, avx512>::value) _mm512_stream_si512(ptr, rg);
			else if constexpr (std::is_same<Reg, avx512f>::value) _mm512_stream_ps((float*)ptr, rg);
			else ptr[0] = rg;
		}
		else {
			if constexpr (std::is_same<Reg, sse>::value) _mm_store_si128(ptr, rg);
			else if constexpr (std::is_same<Reg, ssef>::value) _mm_store_ps((float*)ptr, rg);
			else if constexpr (std::is_same<Reg, avx2>::value) _mm256_store_si256(ptr, rg);
			else if constexpr (std::is_same<Reg, avx2f>::value) _mm256_store_ps((float*)ptr, rg);
			else if constexpr (std::is_same<Reg, avx512>::value) _mm512_store_si512(ptr, rg); 
			else if constexpr (std::is_same<Reg, avx512f>::value) _mm512_store_ps((float*)ptr, rg);
			else ptr[0] = rg;
		}
	}

	template <typename Reg, typename Item>
	FORCEINLINE void swap(Reg& a0, Reg& a1) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::swap<Item, Reg>(a0, a1); 
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::swap<Item, Reg>(a0, a1); 
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::swap<Item, Reg>(a0, a1); 
		else scalar_utils::swap<Reg>(a0, a1);
		
	}

	template <typename Reg, typename Item>
	FORCEINLINE void rswap(Reg& a0, Reg& a1) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::rswap<Item, Reg>(a0, a1); // sse_rswap(0, 1);
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::rswap<Item, Reg>(a0, a1); // avx_rswap(0, 1); // //irl_avx::rswap<Item>(a0, a1); 
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::rswap<Item, Reg>(a0, a1); //avx512_rswap(0, 1);
		else if constexpr (std::is_same<Reg, Item>::value) scalar_utils::swap<Reg>(a0, a1);
		else {
			ReportError("RSWAP not implemented");
		}
	}

	template <typename Reg, typename Item>
	FORCEINLINE void cswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7) {
		swap<Reg, Item>(a0, a4); swap<Reg, Item>(a1, a5); swap<Reg, Item>(a2, a6); swap<Reg, Item>(a3, a7);
		swap<Reg, Item>(a2, a4); swap<Reg, Item>(a3, a5);
		swap<Reg, Item>(a1, a2); swap<Reg, Item>(a3, a4);
	}

	template <typename Reg, typename Item>
	FORCEINLINE void cswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		swap<Reg, Item>(a0, a8); swap<Reg, Item>(a1, a9); swap<Reg, Item>(a2, a10); swap<Reg, Item>(a3, a11); swap<Reg, Item>(a4, a12); swap<Reg, Item>(a5, a13); swap<Reg, Item>(a6, a14); swap<Reg, Item>(a7, a15); 
		swap<Reg, Item>(a4, a8); swap<Reg, Item>(a5, a9); swap<Reg, Item>(a6, a10); swap<Reg, Item>(a7, a11); 
		swap<Reg, Item>(a2, a4); swap<Reg, Item>(a3, a5); swap<Reg, Item>(a6, a8); swap<Reg, Item>(a7, a9); swap<Reg, Item>(a10, a12); swap<Reg, Item>(a11, a13); 
		swap<Reg, Item>(a1, a2); swap<Reg, Item>(a3, a4); swap<Reg, Item>(a5, a6); swap<Reg, Item>(a7, a8); swap<Reg, Item>(a9, a10); swap<Reg, Item>(a11, a12); swap<Reg, Item>(a13, a14); 
	}

	template <typename Reg, typename Item>
	FORCEINLINE void cswap(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		swap<Reg, Item>(a0, a16); swap<Reg, Item>(a1, a17); swap<Reg, Item>(a2, a18); swap<Reg, Item>(a3, a19); swap<Reg, Item>(a4, a20); swap<Reg, Item>(a5, a21); swap<Reg, Item>(a6, a22); swap<Reg, Item>(a7, a23); swap<Reg, Item>(a8, a24); swap<Reg, Item>(a9, a25); swap<Reg, Item>(a10, a26); swap<Reg, Item>(a11, a27); swap<Reg, Item>(a12, a28); swap<Reg, Item>(a13, a29); swap<Reg, Item>(a14, a30); swap<Reg, Item>(a15, a31); swap<Reg, Item>(a8, a16); swap<Reg, Item>(a9, a17); swap<Reg, Item>(a10, a18); swap<Reg, Item>(a11, a19); swap<Reg, Item>(a12, a20); swap<Reg, Item>(a13, a21); swap<Reg, Item>(a14, a22); swap<Reg, Item>(a15, a23); swap<Reg, Item>(a4, a8); swap<Reg, Item>(a5, a9); swap<Reg, Item>(a6, a10); swap<Reg, Item>(a7, a11); swap<Reg, Item>(a12, a16); swap<Reg, Item>(a13, a17); swap<Reg, Item>(a14, a18); swap<Reg, Item>(a15, a19); swap<Reg, Item>(a20, a24); swap<Reg, Item>(a21, a25); swap<Reg, Item>(a22, a26); swap<Reg, Item>(a23, a27); swap<Reg, Item>(a2, a4); swap<Reg, Item>(a3, a5); swap<Reg, Item>(a6, a8); swap<Reg, Item>(a7, a9); swap<Reg, Item>(a10, a12); swap<Reg, Item>(a11, a13); swap<Reg, Item>(a14, a16); swap<Reg, Item>(a15, a17); swap<Reg, Item>(a18, a20); swap<Reg, Item>(a19, a21); swap<Reg, Item>(a22, a24); swap<Reg, Item>(a23, a25); swap<Reg, Item>(a26, a28); swap<Reg, Item>(a27, a29); swap<Reg, Item>(a1, a2); swap<Reg, Item>(a3, a4); swap<Reg, Item>(a5, a6); swap<Reg, Item>(a7, a8); swap<Reg, Item>(a9, a10); swap<Reg, Item>(a11, a12); swap<Reg, Item>(a13, a14); swap<Reg, Item>(a15, a16); swap<Reg, Item>(a17, a18); swap<Reg, Item>(a19, a20); swap<Reg, Item>(a21, a22); swap<Reg, Item>(a23, a24); swap<Reg, Item>(a25, a26); swap<Reg, Item>(a27, a28); swap<Reg, Item>(a29, a30); 
	}


	template <typename Reg, typename Item>
	FORCEINLINE Item get_last_item(Reg& a) {
		if constexpr (std::is_same<Reg, sse>::value) return sse_utils::get_last_item<Item, Reg>(a); 
		else if constexpr (std::is_same<Reg, avx2>::value) return avx2_utils::get_last_item<Item, Reg>(a); 
		else if constexpr (std::is_same<Reg, avx512>::value) return avx512_utils::get_last_item<Item, Reg>(a); 
		else {
			ReportError("Not implemented\n");
		}
	}


	template <typename Reg, typename Item>
	FORCEINLINE void reverse(Reg& a0) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::reverse<Item, Reg>(a0); 
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::reverse<Item, Reg>(a0); 
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::reverse<Item, Reg>(a0); 
	}

	template <typename Reg>
	FORCEINLINE void init_reg(Reg& reg) {
		if constexpr (std::is_same<Reg, sse>::value) reg = _mm_setzero_si128();
		else if constexpr (std::is_same<Reg, ssef>::value) reg = _mm_setzero_ps();
		else if constexpr (std::is_same<Reg, avx2>::value) reg = _mm256_setzero_si256();
		else if constexpr (std::is_same<Reg, avx2f>::value) reg = _mm256_setzero_ps();
		else if constexpr (std::is_same<Reg, avx512f>::value) reg = _mm512_setzero_ps();
		else if constexpr (std::is_same<Reg, avx512>::value) reg = _mm512_setzero_si512();
		else if constexpr (std::is_fundamental<Reg>::value) reg = 0;
	}

	template <typename Reg, typename Item, int bits>
	FORCEINLINE void diag_exchange(Reg& a0, Reg& a1) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::diag_exchange<Item, Reg, bits>(a0, a1);
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::diag_exchange<Item, Reg, bits>(a0, a1);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::diag_exchange<Item, Reg, bits>(a0, a1);
		else ReportError("Not supported");
	}

	template <typename Reg, int bits>
	FORCEINLINE void shuffle(Reg& a0) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::shuffle<Reg, bits>(a0);
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::shuffle<Reg, bits>(a0);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::shuffle<Reg, bits>(a0);
		else 
			ReportError("Not implemented");
		
	}

	template <typename Item>
	void random_writer(Item* A, ui64 n) {
		if constexpr (std::is_same<Item, ui>::value || std::is_same<Item, int>::value) {
			std::mt19937 g;
			std::uniform_int_distribution<Item> d;
			FOR(i, n, 1) A[i] = d(g);
		}
		else if constexpr (std::is_same<Item, i64>::value || std::is_same<Item, ui64>::value) {
			std::mt19937_64 g;
			std::uniform_int_distribution<Item> d;
			FOR(i, n, 1) A[i] = d(g);
		}
		else if constexpr (std::is_same<Item, float>::value) {
			std::random_device rd;
			std::mt19937 g(rd());
			std::uniform_real_distribution<Item> d;
			FOR(i, n, 1) A[i] = d(g);
		}
		else if constexpr (std::is_same<Item, double>::value) {
			std::random_device rd;
			std::mt19937_64 g(rd());
			std::uniform_real_distribution<Item> d;
			FOR(i, n, 1) A[i] = d(g);
		}
		else if constexpr (std::is_same<Item, KeyValue<ui, ui>>::value) {
			KeyValue<ui, ui> kv;
			std::mt19937 g;
			std::uniform_int_distribution<ui> d;
			FOR(i, n, 1) {
				kv.key = d(g);
				kv.value = 0xFFFFFFFF;
				A[i] = kv;
			}
		}
		else if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
			KeyValue<i64, i64> kv;
			std::mt19937_64 g;
			std::uniform_int_distribution<i64> d(0, INT64_MAX);
			FOR(i, n, 1) {
				kv.key = d(g);
				kv.value = kv.key + 1;
				A[i] = kv;
			}
		}
		else if constexpr (std::is_same<Item, KeyValue<ui64, ui64>>::value) {
			KeyValue<ui64, ui64> kv;
			std::mt19937_64 g;
			std::uniform_int_distribution<ui64> d;
			FOR(i, n, 1) {
				kv.key = d(g);
				kv.value = kv.key + 1;
				A[i] = kv;
			}
		}
	}

	
	template <typename Item>
	void pareto_writer(Item* A, ui64 n, int type = 0) {
		SetThreadAffinityMask(GetCurrentThread(), 1 << 2);
		ui64 a = 6364136223846793005, c = 1442695040888963407, x = 1;
		double ED = 20;
		double alpha = 1, beta = 7;
		ui64 sum = 0, keys = 0, y = 889;
		ui64 maxF = 0;
		for (ui64 i = 0; i < n; )
		{
			x = x * a + c;
			y = y * a + c;

			// generate frequency from the Pareto distribution with alpha=1; otherwise, the generator gets slow
			double u = (double)y / ((double)(1LLU << 63) * 2);			// uniform [0,1]
			ui64 f = min(ceil(beta * (1 / (1 - u) - 1)), 100000);		// rounded-up Pareto
			A[i++] = f;
		}
	}
	
	template <typename Item>
	void print_arr(Item* A, ui64 n) {
		FOR(i, n, 1) {
			if constexpr (std::is_same<Item, ui>::value)
				printf("%12lu ", A[i]);
			else if constexpr (std::is_same<Item, i64>::value)
				printf("%12llX ", A[i]);
			else if constexpr (std::is_same<Item, float>::value)
				printf("%12lf ", A[i]);
			else if constexpr (std::is_same<Item, double>::value)
				printf("%12llf ", A[i]);
			else if constexpr (std::is_same<Item, KeyValue<ui, ui>>::value) {
				KeyValue<ui, ui> kv = A[i];
				printf("<%12lu, %12lu>\n", kv.key, kv.value);
			}
			else if constexpr (std::is_same<Item, KeyValue<i64, i64>>::value) {
				KeyValue<i64, i64> kv = A[i];
				printf("<%12llu, %12llu>\n", kv.key, kv.value);
			}
		}
		printf("\n");
	}
	
	template <typename Item, typename KeyType>		// Item can be KV; then we can use the KeyType information
	void bitwise_hist(Item* A, ui64 n) {
		ui nbits = sizeof(KeyType) << 3;
		double* hist = new double[nbits];
		memset(hist, 0, nbits * sizeof(double));
		ui64 tot_1 = 0;
		// build histogram
		FOR(i, n, 1) {
			KeyType k = *(KeyType*)(A + i);
			FOR(j, nbits, 1) {
				hist[j] += (k & 1);
				tot_1 += (k & 1);
				k >>= 1;
			}
			
		}
		// get percentage
		FOR(j, nbits, 1) hist[j] = (hist[j] / tot_1) * 100;		
		// print hist
		PRINT_DASH(30);
		FOR(j, nbits, 1) printf("%3u: %.2f\n", j, hist[j]);
		PRINT_DASH(30);
	}



	// mcmerge shuffle stuff
	template <typename Reg, typename Item>
	FORCEINLINE void mn8_level2_shuffle(Reg& a0, Reg& a1) {
		if constexpr (std::is_same<Reg, sse>::value || std::is_same<Reg, ssef>::value || std::is_same<Reg, ssed>::value) sse_utils::mn8_level2_shuffle<Item, Reg>(a0, a1);
		else if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::mn8_level2_shuffle<Item, Reg>(a0, a1);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::mn8_level2_shuffle<Item, Reg>(a0, a1);
		else ReportError("Not implemented with these types");
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mn16_level1_shuffle(Reg& a0) {
		// NOTE: need only for AVX2 and AVX512
		if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::mn16_level1_shuffle<Item, Reg>(a0);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::mn16_level1_shuffle<Item, Reg>(a0);
		else ReportError("Not implemented with these types");
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mn16_level2_shuffle(Reg& a0, Reg& a1) {
		// NOTE: need only for AVX2 and AVX512
		if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::mn16_level2_shuffle<Item, Reg>(a0, a1);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::mn16_level2_shuffle<Item, Reg>(a0, a1);
		else ReportError("Not implemented with these types");
	}

	template <typename Reg, typename Item>
	FORCEINLINE void mn16_level3_shuffle(Reg& a0, Reg& a1) {
		// NOTE: need only for AVX2 and AVX512
		if constexpr (std::is_same<Reg, avx2>::value || std::is_same<Reg, avx2f>::value || std::is_same<Reg, avx2d>::value)  avx2_utils::mn16_level3_shuffle<Item, Reg>(a0, a1);
		else if constexpr (std::is_same<Reg, avx512>::value || std::is_same<Reg, avx512f>::value || std::is_same<Reg, avx512d>::value) avx512_utils::mn16_level3_shuffle<Item, Reg>(a0, a1);
		else ReportError("Not implemented with these types");
	}

	// TRANSPOSE
	// transpose by diagonal exchange
	template <typename Item, typename Reg>
	FORCEINLINE void transpose(Reg& a0, Reg& a1) {
		if constexpr (sizeof(Reg) / sizeof(Item) != 2) {
			ReportError("not supported");
		}
		else {
			// 2x2 cases
			// 1. SSE + 64 bits; 2. AVX2 + (64, 64) KV
			constexpr ui bits = sizeof(Item) << 3;
			diag_exchange<Reg, Item, bits>(a0, a1);
		}
	}

	template <typename Item, typename Reg>
	FORCEINLINE void transpose(Reg& a0, Reg& a1, Reg& a2, Reg& a3) {
		if constexpr (sizeof(Reg) / sizeof(Item) != 4) {
			ReportError("not supported");
		}
		else {
			// 4x4 cases:
			// 1. SSE + 32 bits 
			if constexpr (std::is_same<Reg, sse>::value) {

				// NOTE: 32 bit unpack is not the same as diagonal exchange
				constexpr ui bits = sizeof(Item) << 3;
				sse _a0, _a1, _a2, _a3;
				_a0 = _mm_unpacklo_epi32(a0, a1);
				_a1 = _mm_unpackhi_epi32(a0, a1);
				_a2 = _mm_unpacklo_epi32(a2, a3);
				_a3 = _mm_unpackhi_epi32(a2, a3);

				constexpr ui l1bits = bits << 1;
				diag_exchange<Reg, Item, l1bits>(_a0, _a2);
				diag_exchange<Reg, Item, l1bits>(_a1, _a3);

				a0 = _a0; a1 = _a2; a2 = _a1; a3 = _a3;
			}
			// 2. AVX2 + 64 bits; AVX512 + (64, 64) bits KV
			else {
				constexpr ui bits = sizeof(Item) << 3;
				diag_exchange<Reg, Item, bits>(a0, a1);
				diag_exchange<Reg, Item, bits>(a2, a3);

				constexpr ui l1_bits = bits << 1;
				diag_exchange<Reg, Item, l1_bits>(a0, a2);
				diag_exchange<Reg, Item, l1_bits>(a1, a3);
			}
		}
	}

	template <typename Item, typename Reg>
	FORCEINLINE void transpose(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7) {
		if constexpr (sizeof(Reg) / sizeof(Item) != 8) {
			ReportError("not supported");
		}
		else {
			constexpr ui bits = sizeof(Item) << 3;
			// case: AVX2 + 32 bits
			if constexpr (std::is_same<Reg, avx2>::value) {
				avx2 _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7;
				_a0 = _mm256_unpacklo_epi32(a0, a1);
				a1 = _mm256_unpackhi_epi32(a0, a1);
				a0 = _a0;
				_a2 = _mm256_unpacklo_epi32(a2, a3);
				a3 = _mm256_unpackhi_epi32(a2, a3);
				a2 = _a2;
				_a4 = _mm256_unpacklo_epi32(a4, a5);
				a5 = _mm256_unpackhi_epi32(a4, a5);
				a4 = _a4;
				_a6 = _mm256_unpacklo_epi32(a6, a7);
				a7 = _mm256_unpackhi_epi32(a6, a7);
				a6 = _a6;
			}
			// case: AVX512 + 64 bits
			else {
				diag_exchange<Reg, Item, bits>(a0, a1);
				diag_exchange<Reg, Item, bits>(a2, a3);
				diag_exchange<Reg, Item, bits>(a4, a5);
				diag_exchange<Reg, Item, bits>(a6, a7);
			}

			
			constexpr ui l1_bits = bits << 1;
			diag_exchange<Reg, Item, l1_bits>(a0, a2);
			diag_exchange<Reg, Item, l1_bits>(a1, a3);
			diag_exchange<Reg, Item, l1_bits>(a4, a6);
			diag_exchange<Reg, Item, l1_bits>(a5, a7);

			constexpr ui l2_bits = l1_bits << 1;
			diag_exchange<Reg, Item, l2_bits>(a0, a4);
			diag_exchange<Reg, Item, l2_bits>(a1, a5);
			diag_exchange<Reg, Item, l2_bits>(a2, a6);
			diag_exchange<Reg, Item, l2_bits>(a3, a7);

			
			//// case: AVX2 + 32 bits --> re-arrange registers
			if constexpr (std::is_same<Reg, avx2>::value) {
				Reg t = a1; a1 = a2; a2 = t;
				t = a5; a5 = a6; a6 = t;
			}
		}
	}

	template <typename Item, typename Reg>
	FORCEINLINE void transpose(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		if constexpr (sizeof(Reg) / sizeof(Item) != 16) {
			ReportError("not supported");
		}
		else {
			constexpr ui bits = sizeof(Item) << 3;
			// case: AVX512 + 32 bits
			if constexpr (std::is_same<Reg, avx512>::value) {
				avx512 _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9, _a10, _a11, _a12, _a13, _a14, _a15;

				_a0 = _mm512_unpacklo_epi32(a0, a1);
				a1 = _mm512_unpackhi_epi32(a0, a1); a0 = _a0;
				_a2 = _mm512_unpacklo_epi32(a2, a3);
				a3 = _mm512_unpackhi_epi32(a2, a3); a2 = _a2;
				_a4 = _mm512_unpacklo_epi32(a4, a5);
				a5 = _mm512_unpackhi_epi32(a4, a5); a4 = _a4;
				_a6 = _mm512_unpacklo_epi32(a6, a7);
				a7 = _mm512_unpackhi_epi32(a6, a7); a6 = _a6;
				_a8 = _mm512_unpacklo_epi32(a8, a9);
				a9 = _mm512_unpackhi_epi32(a8, a9); a8 = _a8;
				_a10 = _mm512_unpacklo_epi32(a10, a11);
				a11 = _mm512_unpackhi_epi32(a10, a11); a10 = _a10;
				_a12 = _mm512_unpacklo_epi32(a12, a13);
				a13 = _mm512_unpackhi_epi32(a12, a13); a12 = _a12;
				_a14 = _mm512_unpacklo_epi32(a14, a15);
				a15 = _mm512_unpackhi_epi32(a14, a15); a14 = _a14;

				constexpr ui l1_bits = bits << 1;
				diag_exchange<Reg, Item, l1_bits>(a0, a2);
				diag_exchange<Reg, Item, l1_bits>(a1, a3);
				diag_exchange<Reg, Item, l1_bits>(a4, a6);
				diag_exchange<Reg, Item, l1_bits>(a5, a7);
				diag_exchange<Reg, Item, l1_bits>(a8, a10);
				diag_exchange<Reg, Item, l1_bits>(a9, a11);
				diag_exchange<Reg, Item, l1_bits>(a12, a14);
				diag_exchange<Reg, Item, l1_bits>(a13, a15);

				constexpr ui l2_bits = l1_bits << 1;
				diag_exchange<Reg, Item, l2_bits>(a0, a4);
				diag_exchange<Reg, Item, l2_bits>(a1, a5);
				diag_exchange<Reg, Item, l2_bits>(a2, a6);
				diag_exchange<Reg, Item, l2_bits>(a3, a7);
				diag_exchange<Reg, Item, l2_bits>(a8, a12);
				diag_exchange<Reg, Item, l2_bits>(a9, a13);
				diag_exchange<Reg, Item, l2_bits>(a10, a14);
				diag_exchange<Reg, Item, l2_bits>(a11, a15);

				constexpr ui l3_bits = l2_bits << 1;
				diag_exchange<Reg, Item, l3_bits>(a0, a8);
				diag_exchange<Reg, Item, l3_bits>(a1, a9);
				diag_exchange<Reg, Item, l3_bits>(a2, a10);
				diag_exchange<Reg, Item, l3_bits>(a3, a11);
				diag_exchange<Reg, Item, l3_bits>(a4, a12);
				diag_exchange<Reg, Item, l3_bits>(a5, a13);
				diag_exchange<Reg, Item, l3_bits>(a6, a14);
				diag_exchange<Reg, Item, l3_bits>(a7, a15);

				// re-arrange registers
				Reg t = a1; a1 = a2; a2 = t;
				t = a5; a5 = a6; a6 = t;
				t = a9; a9 = a10; a10 = t;
				t = a13; a13 = a14; a14 = t;
			}
			else {
				ReportError("transpose 16x16 not supported for these types");
			}
		}
	}

	template <typename Item, typename Reg>
	FORCEINLINE void transpose_large(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		constexpr ui items_per_reg = sizeof(Reg) / sizeof(Item);
		if constexpr (items_per_reg == 1) return;
		else if constexpr (items_per_reg == 2) {
			origami_utils::transpose<Item, Reg>(a0, a1);
			origami_utils::transpose<Item, Reg>(a2, a3);
			origami_utils::transpose<Item, Reg>(a4, a5);
			origami_utils::transpose<Item, Reg>(a6, a7);
			origami_utils::transpose<Item, Reg>(a8, a9);
			origami_utils::transpose<Item, Reg>(a10, a11);
			origami_utils::transpose<Item, Reg>(a12, a13);
			origami_utils::transpose<Item, Reg>(a14, a15);
		}
		else if constexpr (items_per_reg == 4) {
			origami_utils::transpose<Item, Reg>(a0, a1, a2, a3);
			origami_utils::transpose<Item, Reg>(a4, a5, a6, a7);
			origami_utils::transpose<Item, Reg>(a8, a9, a10, a11);
			origami_utils::transpose<Item, Reg>(a12, a13, a14, a15);
		}
		else if constexpr (items_per_reg == 8) {
			origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
			origami_utils::transpose<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
		}
		else if constexpr (items_per_reg == 16) {
			origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		}
		else ReportError("Items per reg not supported");
	}

	template <typename Item, typename Reg>
	FORCEINLINE void transpose_large(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		constexpr ui items_per_reg = sizeof(Reg) / sizeof(Item);
		if constexpr (items_per_reg == 1) return;
		else if constexpr (items_per_reg == 2) {
			origami_utils::transpose<Item, Reg>(a0, a1); origami_utils::transpose<Item, Reg>(a2, a3);
			origami_utils::transpose<Item, Reg>(a4, a5); origami_utils::transpose<Item, Reg>(a6, a7);
			origami_utils::transpose<Item, Reg>(a8, a9); origami_utils::transpose<Item, Reg>(a10, a11);
			origami_utils::transpose<Item, Reg>(a12, a13); origami_utils::transpose<Item, Reg>(a14, a15);
			origami_utils::transpose<Item, Reg>(a16, a17); origami_utils::transpose<Item, Reg>(a18, a19);
			origami_utils::transpose<Item, Reg>(a20, a21); origami_utils::transpose<Item, Reg>(a22, a23);
			origami_utils::transpose<Item, Reg>(a24, a25); origami_utils::transpose<Item, Reg>(a26, a27);
			origami_utils::transpose<Item, Reg>(a28, a29); origami_utils::transpose<Item, Reg>(a30, a31);
		}
		else if constexpr (items_per_reg == 4) {
			origami_utils::transpose<Item, Reg>(a0, a1, a2, a3);		origami_utils::transpose<Item, Reg>(a4, a5, a6, a7);
			origami_utils::transpose<Item, Reg>(a8, a9, a10, a11);		origami_utils::transpose<Item, Reg>(a12, a13, a14, a15);
			origami_utils::transpose<Item, Reg>(a16, a17, a18, a19);	origami_utils::transpose<Item, Reg>(a20, a21, a22, a23);
			origami_utils::transpose<Item, Reg>(a24, a25, a26, a27);	origami_utils::transpose<Item, Reg>(a28, a29, a30, a31);
		}
		else if constexpr (items_per_reg == 8) {
			origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7);
			origami_utils::transpose<Item, Reg>(a8, a9, a10, a11, a12, a13, a14, a15);
			origami_utils::transpose<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23);
			origami_utils::transpose<Item, Reg>(a24, a25, a26, a27, a28, a29, a30, a31);
		}
		else if constexpr (items_per_reg == 16) {
			origami_utils::transpose<Item, Reg>(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
			origami_utils::transpose<Item, Reg>(a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		}
		else ReportError("Items per reg not supported");
	}



	template <typename Item, typename Reg>
	FORCEINLINE void print_registers(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15) {
		PRINT_DASH(30);
		PRINT_DASH(30);
		origami_utils::print<Reg, Item>(a0); origami_utils::print<Reg, Item>(a1);
		origami_utils::print<Reg, Item>(a2); origami_utils::print<Reg, Item>(a3);
		origami_utils::print<Reg, Item>(a4); origami_utils::print<Reg, Item>(a5);
		origami_utils::print<Reg, Item>(a6); origami_utils::print<Reg, Item>(a7);
		PRINT_DASH(30);
		origami_utils::print<Reg, Item>(a8); origami_utils::print<Reg, Item>(a9);
		origami_utils::print<Reg, Item>(a10); origami_utils::print<Reg, Item>(a11);
		origami_utils::print<Reg, Item>(a12); origami_utils::print<Reg, Item>(a13);
		origami_utils::print<Reg, Item>(a14); origami_utils::print<Reg, Item>(a15);
		PRINT_DASH(30);
	}

	template <typename Item, typename Reg>
	FORCEINLINE void print_registers(Reg& a0, Reg& a1, Reg& a2, Reg& a3, Reg& a4, Reg& a5, Reg& a6, Reg& a7, Reg& a8, Reg& a9, Reg& a10, Reg& a11, Reg& a12, Reg& a13, Reg& a14, Reg& a15,
		Reg& a16, Reg& a17, Reg& a18, Reg& a19, Reg& a20, Reg& a21, Reg& a22, Reg& a23, Reg& a24, Reg& a25, Reg& a26, Reg& a27, Reg& a28, Reg& a29, Reg& a30, Reg& a31) {
		PRINT_DASH(30);
		PRINT_DASH(30);
		origami_utils::print<Reg, Item>(a0); origami_utils::print<Reg, Item>(a1);
		origami_utils::print<Reg, Item>(a2); origami_utils::print<Reg, Item>(a3);
		origami_utils::print<Reg, Item>(a4); origami_utils::print<Reg, Item>(a5);
		origami_utils::print<Reg, Item>(a6); origami_utils::print<Reg, Item>(a7);
		origami_utils::print<Reg, Item>(a8); origami_utils::print<Reg, Item>(a9);
		origami_utils::print<Reg, Item>(a10); origami_utils::print<Reg, Item>(a11);
		origami_utils::print<Reg, Item>(a12); origami_utils::print<Reg, Item>(a13);
		origami_utils::print<Reg, Item>(a14); origami_utils::print<Reg, Item>(a15);
		PRINT_DASH(30);
		origami_utils::print<Reg, Item>(a16); origami_utils::print<Reg, Item>(a17);
		origami_utils::print<Reg, Item>(a18); origami_utils::print<Reg, Item>(a19);
		origami_utils::print<Reg, Item>(a20); origami_utils::print<Reg, Item>(a21);
		origami_utils::print<Reg, Item>(a22); origami_utils::print<Reg, Item>(a23);
		origami_utils::print<Reg, Item>(a24); origami_utils::print<Reg, Item>(a25);
		origami_utils::print<Reg, Item>(a26); origami_utils::print<Reg, Item>(a27);
		origami_utils::print<Reg, Item>(a28); origami_utils::print<Reg, Item>(a29);
		origami_utils::print<Reg, Item>(a30); origami_utils::print<Reg, Item>(a31);
	}
#undef _DEBUG
}