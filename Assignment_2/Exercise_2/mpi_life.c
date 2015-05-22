#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 500

typedef int bool;
#define true 1
#define false 0

const int SIZE = 5;

int life_matrix[5][5] = {{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,1,1,0,0},
			{0,0,0,0,0}};
void next_frame();
int *calculate_neighbors(int x, int y);
bool check_neighbor(int x, int y);
void print_neighbors(int *n);

int main(int argc, char **argv)
{

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the processes
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	next_frame(5, 5);
/*
	if (world_rank == 0) {
		MPI_Send(&console_input, strlen(console_input), MPI_BYTE, 1, 0, 
			MPI_COMM_WORLD);
		int i= 0;
		
	} else if (world_rank > 0) {

		int prev_rank = world_rank - 1;
		int next_rank = world_rank + 1;

		MPI_Recv(&console_input, strlen(console_input), MPI_BYTE, prev_rank, 0,
			 MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		if(next_rank < world_size)
			MPI_Send(&console_input, strlen(console_input), MPI_BYTE, 
			next_rank, 0, MPI_COMM_WORLD);

	}	
*/


	// Finalize the MPI environment
	MPI_Finalize();
	
	return 0;

}

void next_frame(int w, int h){

	printf("Starting...\n");
	
	int i, j;
	for(i = 0; i < w; i++)
	{
		for(j = 0; j < h; j++)
		{
			int *current_neigh = calculate_neighbors(i, j);

			print_neighbors(current_neigh);
			
		}
	}

}

void print_neighbors(int *n){
	

	printf("Current node has the following neighbors: ");
	int i;
	for(i = 0; i < 8; i++)
		printf("%d  ", n[i]);
	
	
	printf("\n");
}

int *calculate_neighbors(int x, int y){

	int *neigh = malloc(8 * sizeof(int*));

	if((x-1) >= 0)
	{
		neigh[3] = check_neighbor(x-1, y);	// Check Up-Center
	
		if((y-1) >= 0)
		{
			neigh[0] = check_neighbor(x-1, y-1);	// Check Up-Left
			neigh[1] = check_neighbor(x, y-1);	// Check Mid-Left
			
			if((x+1) < SIZE)
			{
				neigh[2] = check_neighbor(x+1, y-1);	// Check Up-Right
				neigh[4] = check_neighbor(x+1, y);	// Check Mid-Right
				
				if((y+1) < SIZE)
				{	
					neigh[5] = check_neighbor(x-1, y+1);	// Check Down-Left
					neigh[6] = check_neighbor(x, y+1);	// Check Down-Center
					neigh[7] = check_neighbor(x+1, y+1);	// Check Down-Right
				}
				else
				{
					neigh[5] = -1;
					neigh[6] = -1;
					neigh[7] = -1;
				}
				
			}
			else
			{
				neigh[2] = -1;
				neigh[4] = -1;
			}
			
		}
		else	
		{
			neigh[0] = -1;
			neigh[1] = -1;
			neigh[2] = -1;
		}
	
		
		
	}
	else 
	{
		neigh[0] = -1;			
		neigh[3] = -1;
		neigh[5] = -1;
	}



	return neigh;
}

bool check_neighbor(int x, int y){
	
	return (life_matrix[x][y] == 1)?true:false;
		
}
