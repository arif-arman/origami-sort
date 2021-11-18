#include "commons.h"
#include "utils.h"
#include "Writer.h"
#include "external_sorter.h"

// generate data for merge test
template <typename Item>
void generate_merge_data(ui64 total_size, ui WAY) {
	ui64 n = total_size / sizeof(Item);
	ui64 n_per_stream = n / WAY;
	Item* data = (Item*)VALLOC(total_size);
	hrc::time_point s, e;

	datagen::Writer<Item> writer;
	printf("Generating data (%llu bytes) ... \n", total_size);
	s = hrc::now();
	writer.generate(data, n, MT);
	e = hrc::now();
	printf("done in %.2f ms\n", ELAPSED_MS(s, e));


	printf("Sorting chunks (%llu bytes) ... ", n_per_stream * sizeof(Item));
	s = hrc::now();
	SortEvery<Item>(data, n, n_per_stream);
	e = hrc::now();
	printf("done in %.2f ms\n", ELAPSED_MS(s, e));
	

	char fname[MAX_PATH_LEN];
	char _WAY[5];
	strcpy(fname, "C:\\Users\\arif\\Desktop\\External\\");
	strcat(fname, "merge_data_32bit_");
	strcat(fname, _itoa(WAY, _WAY, 10));
	strcat(fname, "way.dat");
	printf("Writing to file %s ...", fname);
	HANDLE hWrite = CreateFile(
		fname, //"C:\\Users\\arif\\Desktop\\External\\merge_data_32bit_1MB_1024way.dat",  //"merge_data_32bit_8way.dat", //
		GENERIC_WRITE | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	DWORD bytesWritten;
	DWORD max_write = 4294967295;
	ui64 bytes_left = total_size;
	char* p = (char*)data;
	s = hrc::now();
	while (bytes_left > 0) {
		DWORD bytes_to_write = min(max_write, bytes_left);
		int bWrt = WriteFile(hWrite, p, bytes_to_write, &bytesWritten, NULL);
		if (bWrt == 0) {
			printf("WriteFile failed with %d\n", GetLastError());
			exit(-1);
		}
		bytes_left -= bytesWritten;
		p += bytesWritten;
	}
	
	e = hrc::now();
	double el = ELAPSED_MS(s, e);
	printf("done in %.2f ms; %llu bytes @ %.2f MB/s\n", el, total_size, total_size * 1.0 / el / 1e3);

	CloseHandle(hWrite);
	VFREE(data);
}

template <typename Item>
void sort_file(char* fname) {

}

template <typename Reg, typename Item>
void mtree_single_thread(char* fname, ui WAY) {
	ui64 mem_size = GB(16LLU);
	origami_external_sorter::ExternalMemorySort<Reg, Item> ex(fname, mem_size);
	ex.mtree_bench(fname, WAY);
}

int main(int argc, char** argv) {
	using Reg = avx2;
	using Item = ui;
	
	/*ui WAY = atoi(argv[1]);
	generate_merge_data<Item>(GB(128LLU), WAY);*/

	generate_merge_data<Item>(MB(1LLU), 16);

	// char fname[] = "C:\\Users\\arif\\Desktop\\External\\merge_data_32bit_1MB_512way.dat";
	/*if (argc < 3) {
		printf("Origami.exe <input file path> <WAY>");
		exit(-1);
	}
	mtree_single_thread<Reg, Item>(argv[1], atoi(argv[2]));*/

	return 0;
}