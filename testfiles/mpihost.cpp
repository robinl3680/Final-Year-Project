#include<iostream>
#include<bits/stdc++.h>
#include<mpi.h>

using namespace std;
int main(int args, char **argv)
{
	int rank, size;

	MPI_Init(&args,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	cout << " am " << rank << endl;
	MPI_Finalize();
	return 0;
}