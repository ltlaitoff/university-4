#include <iostream>
#include "mpi.h"
# define M_PI           3.14159265358979323846  /* pi */

void a(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int number_proccess;
	int rank;

	MPI_Comm_size(MPI_COMM_WORLD, &number_proccess);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("%d: hello (p = %d)", rank, number_proccess);

	MPI_Finalize();
}

void first(int argc, char** argv) {
	int number_proccess;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_proccess);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// printf("%d: hello (p = %d)\n", rank, number_proccess);

	if (rank == 30) {
		printf("%2d (p=%d): Schedrovskyi Ivan\n", rank, number_proccess);
	} else {
		printf("%2d (p=%d): %d\n", rank, number_proccess, rank);
	}

	// printf("%d: bye\n", rank);
	MPI_Finalize();
}

void second(int argc, char* argv[]) {
	int number_proccess;
	int rank;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_proccess);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("%d: hello (p = %d)\n", rank, number_proccess);

	if (number_proccess != 2) {
		printf("%d: Only 2 processes required, but p = %d\n", rank, number_proccess);

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
		return;
	}

	if (rank == 0) {
		double doubleNumbers[20];

		printf("%d: Sending 5 elements to 1\n", rank);
		MPI_Send(doubleNumbers, 5, MPI_DOUBLE, 1, 100, MPI_COMM_WORLD);
		
		MPI_Status status;
		double variantNumbers[30 * 11];

		MPI_Recv(variantNumbers, 31, MPI_FLOAT, 1, 101, MPI_COMM_WORLD, &status);
		
		int count;
		MPI_Get_count(&status, MPI_FLOAT, &count);
		printf("%d: Received %d elements from %d\n", rank, count, status.MPI_SOURCE);
	}
	
	if (rank == 1) {
		MPI_Status status;
		double doubleNumbers[20];

		MPI_Recv(doubleNumbers, 5, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD, &status);

		int count;
		MPI_Get_count(&status, MPI_DOUBLE, &count);
		printf("%d: Received %d elements from %d\n", rank, count, status.MPI_SOURCE);

		float variantNumbers[30 * 11];
		printf("%d: Sending 31 float element to 0\n", rank);
		MPI_Send(variantNumbers, 31, MPI_FLOAT, 0, 101, MPI_COMM_WORLD);
	}

	printf("%d: bye\n", rank);
	MPI_Finalize();
}

void third(int argc, char* argv[]) {
	int number_proccess;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_proccess);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("%d: hello (p = %d)\n", rank, number_proccess);

	if (number_proccess != 2) {
		printf("%d: Only 2 processes required, but p = %d\n", rank, number_proccess);

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
		return;
	}

	if (rank == 0) {
		double *doubleNumbers = new double[20];

		printf("%d: Sending 5 elements to 1\n", rank);
		MPI_Send(doubleNumbers, 5, MPI_DOUBLE, 1, 100, MPI_COMM_WORLD);

		MPI_Status status;
		float *variantNumbers = new float[30 * 11];
		MPI_Recv(variantNumbers, 31, MPI_FLOAT, 1, 101, MPI_COMM_WORLD, &status);

		int count;
		MPI_Get_count(&status, MPI_FLOAT, &count);
		printf("%d: Received %d elements from %d\n", rank, count, status.MPI_SOURCE);

		delete[] doubleNumbers;
		delete[] variantNumbers;
	}

	if (rank == 1) {
		MPI_Status status;
		double* doubleNumbers = new double[20];
		MPI_Recv(doubleNumbers, 5, MPI_DOUBLE, 0, 100, MPI_COMM_WORLD, &status);

		int count;
		MPI_Get_count(&status, MPI_DOUBLE, &count);
		printf("%d: Received %d elements from %d\n", rank, count, status.MPI_SOURCE);

		float* variantNumbers = new float[30 * 11];
		printf("%d: Sending 31 float element to 0\n", rank);
		MPI_Send(variantNumbers, 31, MPI_FLOAT, 0, 101, MPI_COMM_WORLD);

		delete[] doubleNumbers;
		delete[] variantNumbers;
	}

	printf("%d: bye\n", rank);
	MPI_Finalize();
}

void four(int argc, char* argv[]) {
	int number_proccess;
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_proccess);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("%d: hello (p = %d)\n", rank, number_proccess);

	if (number_proccess != 4) {
		printf("%d: Only 4 processes required, but p = %d\n", rank, number_proccess);

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
		return;
	}

	if (rank == 0) {
		std::string SURNAME = "Shchedrovskyi";

		printf("%d: Send name to 1\n", rank);
		MPI_Send(SURNAME.c_str(), std::size(SURNAME) + 1, MPI_CHAR, 1, 5, MPI_COMM_WORLD);

		printf("%d: Send name to 2\n", rank);
		MPI_Send(SURNAME.c_str(), std::size(SURNAME) + 1, MPI_CHAR, 2, 5, MPI_COMM_WORLD);
		
		printf("%d: Send name to 3\n", rank);
		MPI_Send(SURNAME.c_str(), std::size(SURNAME), MPI_CHAR, 3, 5, MPI_COMM_WORLD);

		MPI_Status status;
		
		int length;
		MPI_Probe(1, 6, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_CHAR, &length);

		printf("%d: Received full name from 1\n", rank);
		char* fullName = new char[length];
		MPI_Recv(fullName, length, MPI_CHAR, 1, 6, MPI_COMM_WORLD, &status);
		printf("%d: Full name from 1: %s\n", rank, fullName);

		delete[] fullName;

		int chars;
		MPI_Recv(&chars, 1, MPI_INT, 2, 7, MPI_COMM_WORLD, &status);
		printf("%d: Characters from 2: %d\n", rank, chars);

		double sum;
		MPI_Recv(&sum, 1, MPI_DOUBLE, 3, 8, MPI_COMM_WORLD, &status);
		printf("%d: Sum from 3: %f\n", rank, sum);

		printf("%d: bye\n", rank);
		MPI_Finalize();
		return;
	}

	MPI_Status status;
	int length;

	MPI_Probe(0, 5, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, MPI_CHAR, &length);
	printf("%d: Received surname from 0\n", rank);

	char* buf = new char[length];
	MPI_Recv(buf, length, MPI_CHAR, 0, 5, MPI_COMM_WORLD, &status);
	std::string surname = buf;
	delete[] buf;

	if (rank == 1) {
		std::string NAME = "Ivan";
		std::string FULL_NAME = surname + " " + NAME;

		printf("%d: Send full name to 0\n", rank);
		MPI_Send(FULL_NAME.c_str(), std::size(FULL_NAME)+1, MPI_CHAR, 0, 6, MPI_COMM_WORLD);
	}

	if (rank == 2) {
		int chars = surname.length();

		printf("%d: Send %d to 0\n", rank, chars);
		MPI_Send(&chars, 1, MPI_INT, 0, 7, MPI_COMM_WORLD);
	}

	if (rank == 3) {
		double sum = 0;
		for (char c : surname) {
			sum += c;
		}
		double result = sum * M_PI;

		printf("%d: Send %f to 0\n", rank, result);
		MPI_Send(&result, 1, MPI_DOUBLE, 0, 8, MPI_COMM_WORLD);
	}

	printf("%d: bye\n", rank);
	MPI_Finalize();
}

int main(int argc, char** argv) {
	//first(argc, argv);
	//second(argc, argv);
	//third(argc, argv);
	four(argc, argv);

	return 0;
}