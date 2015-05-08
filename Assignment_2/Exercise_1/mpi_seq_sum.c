#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BUFFER_SIZE 500

void para_range(const int n1, const int n2, const int nprocs, const int irank, int * __restrict__ ista, int * __restrict__ iend);
inline int min(const int a, const int b);

int main(int argc, char **argv)
{
	const int ROOT = 0;
	char console_input[MAX_BUFFER_SIZE];
	
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the processes
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// Sequential Sum
	int N = 10;
	int a[10] = {-1, 5, -2, 0, 1, 4, 6, -4, 3, -6};
	int sum = 0;
	int max_sum = 0;
	int partial_sum = 0;
	int startval, endval;

	// Calculating sum range for each process
	para_range(0, 9, world_size, my_rank, &startval, &endval);

	printf("Process %d has sum from %d to %d\n", my_rank, startval, endval);
	
	int i;
	for(i = startval; i <= endval; i++)
	{
		// Filters negative sums on ROOT
		if(my_rank == ROOT && (sum+a[i]) < 0)
			sum = 0;			
		else
			sum += a[i];
		
	}

	printf("Process %d has local sum %d\n", my_rank, sum);
	
	// If not the main processor, send the local sum to ROOT
	if(my_rank != ROOT)
		MPI_Send(&sum, sizeof(MPI_INT), MPI_INT, 0, 0, MPI_COMM_WORLD);
	else{
		
		// Sets current sum to maximum sum
		max_sum = sum;

		int j;
		for(j = 1; j < world_size; j++)
		{
			MPI_Recv(&partial_sum, sizeof(MPI_INT), MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += partial_sum;

			// Filters negative sums
			if(sum < 0)
				sum = 0;

			// Checks if current sum is the maximum sum so far
			if(sum > max_sum)
				max_sum = sum;
		}

		printf("The sum is %d and the maximum sum is %d.\n", sum, max_sum);

	}
	

	// Finalize the MPI environment
	MPI_Finalize();

}
// Credit to ArthurChamz - 
// http://stackoverflow.com/questions/26684480/mpi-allreduce-not-working-as-expected-red-black-sor
void para_range(const int n1, const int n2, const int nprocs, const int irank, int * __restrict__ ista, int * __restrict__ iend)
{
    const int iwork = ((n2 - n1) / nprocs) + 1;

    *ista = min(irank * iwork + n1, n2 + 1);
    *iend = min(*ista + iwork - 1, n2);
}

inline int min(const int a, const int b)
{
    if(a > b)
    {
        return b;
    }

    return a;
}


