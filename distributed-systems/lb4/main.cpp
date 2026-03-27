#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>

int first_original(int argc, char** argv) {
	int N = 400000;
	int rank;
	int number_processes;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("%d: Hello (p = %d)\n", rank, number_processes);

	double start_time, use_time;

	int M;
	double* x = nullptr;
	double* a = nullptr;

	if (rank == 0) {
		x = new double[N];
		a = new double[N];

		srand((unsigned)time(NULL));

		for (long i = 0; i < N; i++) {
			x[i] = -1.073741824 + rand() * 1E-9;
			a[i] = (-1.073741824 + rand() * 1E-9) * 0.1;
		}

		M = N / number_processes;
	}

	if (rank == 0) {
		start_time = MPI_Wtime();
	}

	MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

	double *xn = new double[M];
	double * an = new double[M];
	
	MPI_Scatter(x, M, MPI_DOUBLE, xn, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(a, M, MPI_DOUBLE, an, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double sum = 0.0, total = 0.0;

	for (long i = 0;i < M;i++) {
		sum += an[i] * xn[i];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		use_time = MPI_Wtime() - start_time;
		printf("%d: t=%lf sec.\n", rank, use_time);
		printf("%d: sum in %d procs is %.5f\n", rank, number_processes, total);

		delete[] x;
		delete[] a;
	}

	delete[] xn;
	delete[] an;

	MPI_Finalize();
	return 0;
}

double cos_by_series(double x) {
	double y = x;
	double s = y;
	int k = 1;
	int K = 500;

	do {
		y = -((x * x) / ((k + 1) * (k + 2))) * y;

		s = s + y;
		k = k + 2;
	} while (k <= K);

	return s;
}

int first_variant(int argc, char** argv) {
	int rank;
	int number_processes;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("\n%d: Hello (p = %d)\n", rank, number_processes);

	int N = 240000;
	if (argc > 1) {
		N = atoi(argv[1]);
	}

	double start_time;
	double use_time;

	double* aFull = nullptr;

	if (rank == 0) {
		aFull = new double[N];

		srand((unsigned)time(NULL));

		printf("%d: Generating array\n", rank);
		for (long i = 0; i < N; i++) {
			aFull[i] = ((double)rand() / RAND_MAX) * 2 - 1;
		}
	}

	if (rank == 0) {
		start_time = MPI_Wtime();
	}

	const int M = N / number_processes;
	
	double* a = new double[M];

	printf("%d: Received %d elements\n", rank, M);
	MPI_Scatter(aFull, M, MPI_DOUBLE, a, M, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	const double xi = 0.0001;
	double perProcessorSum = 0.0;

	for (long i = 0; i < M; i++) {
		perProcessorSum += a[i] * cos_by_series(xi * i);
	}

	printf("%d: Processor sum = %f\n", rank, perProcessorSum);

	MPI_Barrier(MPI_COMM_WORLD);

	double sum = 0.0;
	MPI_Reduce(&perProcessorSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		use_time = MPI_Wtime() - start_time;
		printf("%d: \tN = %d \tp = %d \tt=%lf sec.\n", rank, N, number_processes, use_time);
		printf("%d: sum in %d procs is %.5f\n", rank, number_processes, sum);
	}

	delete[] aFull;
	delete[] a;

	printf("%d: Goodbye\n", rank);

	MPI_Finalize();
	return 0;
}

int second_original(int argc, char* argv[]) {
	int N = 10000;
	int rank;
	int number_processes;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (argc > 1) {
		N = atoi(argv[1]);
	}

	// printf("\n%d: Hello (p = %d)\n", rank, number_processes);

	int* process_n = NULL;
	int* process_n_offset = NULL;

	if (rank == 0) {
		process_n = (int*)malloc(number_processes * sizeof(int));

		if (process_n == NULL) {
			printf("%d: Not memory for n\n", rank); 
			return -1;
		}

		process_n_offset = (int*)malloc(number_processes * sizeof(int));
		if (process_n_offset == NULL) {
			printf("%d: Not memory for offset\n", rank); 
			return -1;
		}
		
		double perProcess = N * (1.0 + N) / number_processes;
		double an = 1;

		process_n[number_processes - 1] = N;
		for (long i = 0; i < number_processes - 1; i++) {
			double q = (2 * an - 1) / 2.0;
			double result = -q + sqrt(q * q + perProcess);

			an += result;
			process_n[i] = (int)floor(result);
			process_n[number_processes - 1] -= process_n[i];
		}

		process_n_offset[0] = 0;
		for (long i = 1; i < number_processes; i++) {
			process_n_offset[i] = process_n_offset[i - 1] + process_n[i - 1];
		}

		for (long i = 0; i < number_processes; i++) {
			// printf("%d: Process %d will be do %d multiples\n", rank, i, process_n[i]);
		}
	}
	
	int n, offset;

	MPI_Scatter(process_n, 1, MPI_INT, &n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(process_n_offset, 1, MPI_INT, &offset, 1, MPI_INT, 0, MPI_COMM_WORLD);

	double* x = NULL;
	double* a = NULL;
	double* xn;
	double* an;

	if (rank == 0) {
		x = (double*)malloc(N * sizeof(*x));
		if (x == NULL) {
			printf("%d: Not memory\n", rank);
			return -1;
		}

		a = (double*)malloc(N * sizeof(*a));
		if (a == NULL) {
			printf("%d: Not memory\n", rank);
			free(x);
			return -1;
		}

		srand((unsigned)time(NULL));

		for (long i = 0; i < N; i++) {
			x[i] = (-1.073741824 + rand() * 1E-9) * 0.8;
			a[i] = (-1.073741824 + rand() * 1E-9) * 0.01;
		}
	}

	xn = (double*)malloc(n * sizeof(*x));
	if (xn == NULL) {
		printf("%d: Not memory\n", rank);
		return -1;
	}

	an = (double*)malloc(n * sizeof(*a));
	if (an == NULL) {
		printf("%d: Not memory\n", rank);
		free(xn);
		return -1;
	}

	double start_time;
	if (rank == 0) {
		start_time = MPI_Wtime();
	}

	MPI_Scatterv(x, process_n, process_n_offset, MPI_DOUBLE, xn, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatterv(a, process_n, process_n_offset, MPI_DOUBLE, an, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double processSum = 0.0;
	
	for (long i = 0; i < n; i++) {
		double mul = 1.0;

		for (long j = 0; j < i + 1 + offset; j++) {
			mul *= xn[i];
		}

		processSum += an[i] * mul;
		mul = 1.0;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	double sum = 0.0;
	MPI_Reduce(&processSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		double testSum = 0.0;

		for (long i = 0; i < N; i++) {
			testSum += a[i] * pow(x[i], i + 1);
		}

		// printf("%d: Total test=\t%.12e\n", rank, testSum);
	}

	if (rank == 0) {
		double use_time = MPI_Wtime() - start_time;
		// printf("%d: Total=\t%.12e\n", rank, sum);
		printf("%d: Time=%f sec.\n", rank, use_time);
		free(process_n);
		free(process_n_offset);
	}

	// printf("%d: Bye\n", rank);

	free(x);
	free(a);
	free(xn);
	free(an);
	MPI_Finalize();
	
	return 0;
}

int second_individual(int argc, char* argv[]) {
	int N = 10000;
	int rank;
	int number_processes;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (argc > 1) {
		N = atoi(argv[1]);
	}

	// printf("\n%d: Hello (p = %d)\n", rank, number_processes);

	int* process_n = NULL;
	int* process_n_offset = NULL;

	if (rank == 0) {
		process_n = (int*)malloc(number_processes * sizeof(int));

		if (process_n == NULL) {
			printf("%d: Not memory for n\n", rank);
			return -1;
		}

		process_n_offset = (int*)malloc(number_processes * sizeof(int));
		if (process_n_offset == NULL) {
			printf("%d: Not memory for offset\n", rank);
			return -1;
		}

		int perProcess = N / number_processes;
		for (long i = 0; i < number_processes; i++) {
			process_n[i] = perProcess;
		}
		process_n[number_processes - 1] = perProcess + (N - perProcess * number_processes);

		process_n_offset[0] = 0;
		for (long i = 1; i < number_processes; i++) {
			process_n_offset[i] = process_n_offset[i - 1] + process_n[i - 1];
		}

		for (long i = 0; i < number_processes; i++) {
			// printf("%d: Process %d will be do %d multiples\n", rank, i, process_n[i]);
		}
	}

	int n, offset;

	MPI_Scatter(process_n, 1, MPI_INT, &n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(process_n_offset, 1, MPI_INT, &offset, 1, MPI_INT, 0, MPI_COMM_WORLD);

	double* x = NULL;
	double* a = NULL;
	double* xn;
	double* an;

	if (rank == 0) {
		x = (double*)malloc(N * sizeof(*x));
		if (x == NULL) {
			printf("%d: Not memory for x\n", rank);
			return -1;
		}

		a = (double*)malloc(N * sizeof(*a));
		if (a == NULL) {
			printf("%d: Not memory for a\n", rank);
			free(x);
			return -1;
		}

		srand((unsigned)time(NULL));

		for (long i = 0; i < N; i++) {
			x[i] = (-1.073741824 + rand() * 1E-9) * 0.8;
			a[i] = (-1.073741824 + rand() * 1E-9) * 0.01;
		}
	}

	xn = (double*)malloc(n * sizeof(*x));
	if (xn == NULL) {
		printf("%d: Not memory for xn\n", rank);
		return -1;
	}

	an = (double*)malloc(n * sizeof(*a));
	if (an == NULL) {
		printf("%d: Not memory for an\n", rank);
		free(xn);
		return -1;
	}

	double start_time;
	if (rank == 0) {
		start_time = MPI_Wtime();
	}

	MPI_Scatterv(x, process_n, process_n_offset, MPI_DOUBLE, xn, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatterv(a, process_n, process_n_offset, MPI_DOUBLE, an, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double processSum = 0.0;

	for (long i = 0; i < n; i++) {
		double mul = 1.0;

		for (long j = 0; j < i + 1 + offset; j++) {
			mul *= xn[i];
		}

		processSum += an[i] * mul;
		mul = 1.0;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	double sum = 0.0;
	MPI_Reduce(&processSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		double testSum = 0.0;

		for (long i = 0; i < N; i++) {
			testSum += a[i] * pow(x[i], i + 1);
		}

		// printf("%d: Total test=\t%.12e\n", rank, testSum);
	}

	if (rank == 0) {
		double use_time = MPI_Wtime() - start_time;
		// printf("%d: Total=\t%.12e\n", rank, sum);
		printf("%d: \t N = %d \t p = %d \t Time=%f sec.\n", rank, N, number_processes, use_time);
		free(process_n);
		free(process_n_offset);
	}

	// printf("%d: Bye\n", rank);

	free(x);
	free(a);
	free(xn);
	free(an);
	MPI_Finalize();

	return 0;
}

int main(int argc, char** argv) {
	second_original(argc, argv);
	// second_individual(argc, argv);
}