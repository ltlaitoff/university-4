#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>

int first_original(int argc, char** argv) {
	int rank, numprocs;
	double PI25DT = 3.141592653589793238462643;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double start;
	if (rank == 0) {
		start = MPI_Wtime();
	}

	int n = 1;
	if (argc > 1) {
		n = atoll(argv[1]);
	}

	double mypi, pi, h, sum, x;

	h = 1.0 / (double)n;
	sum = 0.0;

	for (int i = rank + 1; i <= n; i += numprocs) {
		x = h * ((double)i - 0.5);
		sum += 4.0 / (1.0 + x * x);
	}

	mypi = h * sum;

	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		double end = MPI_Wtime();

		printf("%d: pi is approximately %.16f; \t p = %d \t Error is % .16f\t Time: %fsec \n", rank, pi, numprocs, fabs(pi - PI25DT), end - start);
	}

	MPI_Finalize();
	return 0;
}

int first_sequence(int argc, char** argv) {
	int rank, processors;

	double PI25DT = 3.141592653589793238462643;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processors);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = 1;
	if (argc > 1) {
		n = atoll(argv[1]);
	}

	double start = MPI_Wtime();

	double h = 1.0 / (double)n;
	double sum = 0.0;
	double x;

	for (int i = 1; i <= n; i ++) {
		x = h * ((double)i - 0.5);
		sum += 4.0 / (1.0 + x * x);
	}

	double pi = h * sum;
	double end = MPI_Wtime();

	printf("%d: pi is approximately %.16f; \t Error is % .16f\t Time: %fsec \t N = %d\n", rank, pi, fabs(pi - PI25DT), end - start, n);

	MPI_Finalize();
	return 0;
}

int first_cos(int argc, char** argv) {
	int rank, processors;

	// From wolfram alpha
	double CORRECT = 1.5707963267948966192313216916397514420985846996875529104874722961;
	double PI = 3.141592653589793238462643;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processors);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int n = 1;
	if (argc > 1) {
		n = atoll(argv[1]);
	}

	double start = MPI_Wtime();

	double a = 0;
	double b = PI;

	double h = (b - a) / (double)n;
	double sum = 0.0;
	
	for (int i = 1; i <= n; i++) {
		double x = a + h * ((double)i - 0.5);
		
		double val = cos(x);
		sum += val * val;
	}

	double answer = h * sum;
	double end = MPI_Wtime();

	printf("%d: Answer is approximately %.16f; \t Error is % .16f\t Time: %fsec \t N = %d\n", rank, answer, fabs(answer - CORRECT), end - start, n);

	MPI_Finalize();
	return 0;
}


int second_original(int argc, char* argv[]) {
	const int n_in_proc = 10000000;
	const double R = 0.5;
	const double pi = 3.141592653589793238462643;

	int rank, numprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int N_in_proc = atoi(argv[1]);
	
	srand(time(NULL) + rank);

	long long in_circle = 0L;
	double sum_xi = 0.0;
	double sum_xi_sq = 0.0;

	double start;
	if (rank == 0) {
		start = MPI_Wtime();
	}

	for (int i = 0; i < N_in_proc; i++) {	
		for (int j = 0L; j < n_in_proc; j++) {
			double x = rand() / (double)RAND_MAX - 0.5;
			double y = rand() / (double)RAND_MAX - 0.5;

			if (x * x + y * y < R * R) {
				in_circle++;
			}
		}

		long long total_in_circle;
		MPI_Reduce(&in_circle, &total_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

		if (rank == 0) {
			long long total = (long long)n_in_proc * (i + 1) * numprocs;
			double approx = 4.0 * ((double)total_in_circle / total);

			double xi = approx;
			sum_xi += xi;
			sum_xi_sq += xi * xi;

			int current_N = i + 1;
			if (current_N > 1) {
				double variance = (1.0 / (current_N - 1)) * (sum_xi_sq - (1.0 / current_N) * (sum_xi * sum_xi));
				printf("Step %d: \tpi=%.10f \t sigma^2=%.10e \t", current_N, approx, variance);
			}

			printf("pi=%.16f; \terror=%.16f, \tpoints=%lld\n", approx, fabs(pi - approx), total);
		}
	}

	if (rank == 0) {
		double end = MPI_Wtime();
		printf("Time = %f", end - start);
	}

	MPI_Finalize();
	return 0;
}

int third(int argc, char* argv[]) {
	const double EXPECTED_PI = 3.141592653589793238462643;

	int rank, processors_count;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processors_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// printf("%d: Hello (p = %d)\n", rank, processors_count);
	srand(time(NULL) + rank);

	long long N = 1000000000;
	if (argc > 1) {
		N = atoll(argv[1]);
	}

	long long perProcess = N / processors_count;
	if (rank == processors_count - 1) {
		perProcess += N % processors_count;
	}
	printf("%d: Processor %d will do %lld points\n", rank, rank, perProcess);

	double start;
	if (rank == 0) {
		start = MPI_Wtime();
	}

	double local_sum = 0.0;
	double local_sum_square = 0.0;
	long long points = 0;

	for (points = 0L; points < perProcess; points++) {
		double x = rand() / (double)RAND_MAX;
		double fx = (4.0 / (1.0 + x * x));
		
		local_sum += fx ;
		local_sum_square += fx * fx;
	}

	double sum = 0;
	double sum_square = 0;
	long long points_sum = 0;
	MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&local_sum_square, &sum_square, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&points, &points_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		double pi = sum / N;
		double dispersion = (sum_square / N) - (pi * pi);

		printf("PI=%.16lf \tError=%.15lf \tPoints = %lld\n", pi, fabs(EXPECTED_PI - pi), points_sum);
		printf("Dispersion = %.16lf\n", dispersion);

		double end = MPI_Wtime();
		printf("Time = %f\n", end - start);
	}

	// printf("%d: Bye\n", rank);
	MPI_Finalize();
	return 0;
}

int four(int argc, char* argv[]) {
	double R = 1.0;
	double EXPECTED = 0.125 * pow(R, 4);

	int rank, processors_count;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processors_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(time(NULL) + rank);

	long long N = 100000000;
	if (argc > 1) {
		N = atoll(argv[1]);
	}

	long long perProcess = N / processors_count;
	if (rank == processors_count - 1) {
		perProcess += N % processors_count;
	}

	double start;
	if (rank == 0) {
		start = MPI_Wtime();
	}

	double local_sum = 0.0;
	double local_sum_sq = 0.0;

	for (long long i = 0; i < perProcess; i++) {
		double x = (double)rand() / RAND_MAX * R;
		double y = (double)rand() / RAND_MAX * R;

		if (x * x + y * y <= R * R) {
			double val = x * y;

			local_sum += val;
			local_sum_sq += val * val;
		}
	}

	double total_sum, total_sum_sq;
	MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&local_sum_sq, &total_sum_sq, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		double area_rect = R * R;

		double result = (total_sum / N) * area_rect;
		double variance = (total_sum_sq / N) - pow(total_sum / N, 2);

		printf("Result = %.10f \tError  = %.10f \t Expected = %.10f\n", result, fabs(EXPECTED - result), EXPECTED);
		printf("Dispersion = %.10f\nTime = %f\n", variance, MPI_Wtime() - start);
	}

	MPI_Finalize();
	return 0;
}

int main(int argc, char** argv) {
	// first_original(argc, argv);
	// first_sequence(argc, argv);
	// first_cos(argc, argv);
	// second_original(argc, argv);
	// third(argc, argv);
	four(argc, argv);
}