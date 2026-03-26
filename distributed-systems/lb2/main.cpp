#include <iostream>
#include <windows.h>
#include <process.h>
#include <chrono>

int getBitLength(int x) {
	int count = 0;

	while (x > 0) {
		x = x >> 1;
		count++;
	}

	return count;
}

// y = a^n mod m

int squareAndMultiply(int a, int n, int m) {
	int y = 1;
	int bits = getBitLength(n);

	for (int i = bits - 1; i >= 0; i--) {
		y = (y * y) % m;

		if (n >> i & 1) {
			y = (y * a) % m;
		}
	}

	return y;
}


int montgomeryLadderSync(int a, int n, int m) {
	int y1 = 1;
	int y2 = (a) % m;

	int N = getBitLength(n);

	for (int i = N - 1; i >= 0; i--) {
		if (n >> i & 1) {
			y1 = (1LL * y1 * y2) % m;
			y2 = (1LL * y2 * y2) % m;
		} else {
			y2 = (1LL * y1 * y2) % m;
			y1 = (1LL * y1 * y1) % m;
		}
	}

	return y1;
}

int montgomeryLadderAsync(int a, int n, int m) {
	int y1 = 1;
	int y2 = (a) % m;

	int N = getBitLength(n);

	for (int i = N-1; i >= 0; i--) {
		int bit = n >> i & 1;

		int data1[5] = { y1, y2, bit, m, 0 };
		int data2[5] = { y1, y2, bit, m, 0 };

		auto proc1 = [](void* p) {
			int* d = (int*)p;

			if (d[2] == 1) d[4] = (1LL * d[0] * d[1]) % d[3];
			else           d[4] = (1LL * d[0] * d[0]) % d[3];
		};

		auto proc2 = [](void* p) {
			int* d = (int*)p;
			if (d[2] == 1) d[4] = (1LL * d[1] * d[1]) % d[3];
			else           d[4] = (1LL * d[0] * d[1]) % d[3];
		};

		HANDLE h[2];
		h[0] = (HANDLE)_beginthread(proc1, 0, data1);
		h[1] = (HANDLE)_beginthread(proc2, 0, data2);

		WaitForMultipleObjects(2, h, TRUE, INFINITE);

		y1 = data1[4];
		y2 = data2[4];
	}

	return y1;
}

int power(int base, int exp, int mod) {
	int res = 1;
	base %= mod;
	while (exp > 0) {
		if (exp % 2 == 1) res = (1LL * res * base) % mod;
		base = (1LL * base * base) % mod;
		exp /= 2;
	}
	return res;
}

struct TableData {
	int a;
	int exp;
	int m;
	int result;
};

void __cdecl calcBaseValue(void* p) {
	TableData* d = (TableData*)p;
	d->result = power(d->a, d->exp, d->m);
}

int combMethodAsync(int a, int n, int m, int numThreads) {
	int w = numThreads;
	if (w > 16) w = 16;

	int bits = getBitLength(n);
	int h = (bits + w - 1) / w;
	if (h == 0) h = 1;

	int tableSize = 1 << w;
	int* table = new int[tableSize];
	table[0] = 1;

	TableData* tData = new TableData[w];
	HANDLE* threads = new HANDLE[w];

	for (int i = 0; i < w; i++) {
		tData[i] = { a, (1 << (i * h)), m, 0 };
		threads[i] = (HANDLE)_beginthread(calcBaseValue, 0, &tData[i]);
	}

	WaitForMultipleObjects(w, threads, TRUE, INFINITE);
	
	for (int i = 0; i < w; i++) {
		int val = tData[i].result;
		int step = 1 << i;
		for (int j = 0; j < step; j++) {
			int targetIndex = step + j;

			if (targetIndex < tableSize) {
				table[targetIndex] = (1LL * table[j] * val) % m;
			}
		}
	}

	int Y = 1;
	for (int i = h - 1; i >= 0; i--) {
		Y = (1LL * Y * Y) % m;

		int index = 0;
		for (int j = 0; j < w; j++) {
			if ((n >> (j * h + i)) & 1) {
				index |= (1 << j);
			}
		}

		if (index > 0) {
			Y = (1LL * Y * table[index]) % m;
		}
	}

	delete[] table;
	delete[] tData;
	delete[] threads;

	return Y;
}

void measure(const char* title, int (*func)(int, int, int), int a, int n, int m) {
	auto start = std::chrono::high_resolution_clock::now();

	int result = 0;
	for (int i = 0; i < 1000; i++) {
		result = func(a, n, m);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	std::cout << title << ": " << duration.count() << " microseconds | Result: " << result << std::endl;
}

void measureComp(const char* title, int (*func)(int, int, int, int), int a, int n, int m, int numThreads) {
	auto start = std::chrono::high_resolution_clock::now();

	int result = 0;
	for (int i = 0; i < 1000; i++) {
		result = func(a, n, m, numThreads);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	std::cout << title << ": " << duration.count() << " microseconds | Result: " << result << std::endl;
}

int main() {
	const int A = 69234;
	const int N = 65537;
	const int M = 6784;

	std::cout << "y = " << A << "^" << N << " mod " << M << "\n";
	std::cout << "Expected answer = 4480\n";

	std::cout << std::endl;

	measure("Square and multiply", squareAndMultiply, A, N, M);
	measure("Montgomery Ladder sync", montgomeryLadderSync, A, N, M);
	measure("Montgomery Ladder async", montgomeryLadderAsync, A, N, M);
	measureComp("Comb method async with 1 threads", combMethodAsync, A, N, M, 1);
	measureComp("Comb method async with 3 threads", combMethodAsync, A, N, M, 3);

	return 0;
}