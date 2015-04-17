#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 500

int main(int argc, char **argv)
{
	char console_input[MAX_BUFFER_SIZE];
	if(argc == 2)
		strcpy(console_input, argv[1]);
	else {
		fprintf(stderr, "Missing argument!\n");
		return 1;
	}

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the processes
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if(world_size < 2) {
		fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);	
	}	

	if (world_rank == 0) {
		MPI_Send(&console_input, strlen(console_input), MPI_BYTE, 1, 0, 
			MPI_COMM_WORLD);
		
	} else if (world_rank > 0) {

		int prev_rank = world_rank - 1;
		int next_rank = world_rank + 1;

		MPI_Recv(&console_input, strlen(console_input), MPI_BYTE, prev_rank, 0,
			 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("Process %d received message '%s' from process %d\n", world_rank,
		 console_input, prev_rank);

		if(next_rank < world_size)
			MPI_Send(&console_input, strlen(console_input), MPI_BYTE, 
			next_rank, 0, MPI_COMM_WORLD);

	}	


	// Finalize the MPI environment
	MPI_Finalize();

}
