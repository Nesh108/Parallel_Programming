#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BUFFER_SIZE 500

typedef int bool;
#define true 1
#define false 0

void para_range(const int n1, const int n2, const int nprocs, const int irank, int * __restrict__ ista, int * __restrict__ 
iend);
int get_sum(int v1, int v2);
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
	const int N = 10;
	int a[10] = {-1, 5, -2, 0, 1, 4, 6, -4, 3, -6};
	int sum = 0;
	int max_sum = 0;

	const int SIZE_PARTIAL = 3;
	int local_sum[3] = {0,0,0};	// {head_neg, local_sum, local_max}
	int partial_sums[3] = {0,0,0};
	int startval, endval;

	// Calculating sum range for each process
	para_range(0, N-1, world_size, my_rank, &startval, &endval);

	printf("Process %d has to sum from %d to %d\n", my_rank, startval, endval);
	
	int i;
	int head_neg = true;	// variable for looking for negative heading values

	
	for(i = startval; i <= endval; i++)
	{
		if(my_rank != ROOT){
			if(a[i] < 0 && head_neg)
				local_sum[0] += a[i];		// Add to negative part of the sum
			else if(a[i] > 0 && head_neg)
			{
				head_neg = false;
				local_sum[1] += a[i];
			}
			else
				local_sum[1] += a[i];

			if(local_sum[1] > local_sum[2])
				local_sum[2] = local_sum[1];
		}
		else
			{
				sum = get_sum(sum, a[i]);

				if(sum > max_sum)
					max_sum = sum;
			}		
	}
	
	// If not the main processor, send the local sum to ROOT
	if(my_rank != ROOT)
		{
			printf("Process %d has local negative sum %d, local positive sum %d and local MAX %d.\n", my_rank, local_sum[0], 
local_sum[1], local_sum[2]);
			MPI_Send(&local_sum, SIZE_PARTIAL, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
		}	
	else{
		
		int j;
		for(j = 1; j < world_size; j++)
		{
			MPI_Recv(&partial_sums, SIZE_PARTIAL, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(max_sum < partial_sums[2])
				max_sum =  partial_sums[2];

			sum = get_sum(sum , partial_sums[0]);
			sum = get_sum(sum , partial_sums[1]);

			if (sum > max_sum)
				max_sum = sum;

		}


		printf("The sum is %d and the maximum sum is %d.\n", sum, max_sum);

	}
	

	// Finalize the MPI environment
	MPI_Finalize();

}

int get_sum(int v1, int v2){

	v1 += v2;

	return (v1 < 0)?(0):(v1);
}
// Credit to ArthurChamz - 
// http://stackoverflow.com/questions/26684480/mpi-allreduce-not-working-as-expected-red-black-sor
void para_range(const int n1, const int n2, const int nprocs, const int irank, int * __restrict__ ista, int * __restrict__ 
iend)
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



