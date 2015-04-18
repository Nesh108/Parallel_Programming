#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 500

int main(int argc, char **argv)
{
	const int root = 0;
	char console_input[MAX_BUFFER_SIZE];

	// Initialize the MPI environment
	MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the processes
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if(world_rank == root) {
		if(argc == 2)
			strcpy(console_input, argv[1]);
		else {
			fprintf(stderr, "Missing argument!\n");
			return 1;
		}
	}

	if(world_size < 2) {
		fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
		MPI_Abort(MPI_COMM_WORLD, 1);	
	}	

	MPI_Bcast(&console_input, strlen(console_input), MPI_BYTE, root, 
		MPI_COMM_WORLD);

		

	printf("Process %d received message '%s' from process %d\n", world_rank,
		console_input, root);
		

	// Finalize the MPI environment
	MPI_Finalize();

}
