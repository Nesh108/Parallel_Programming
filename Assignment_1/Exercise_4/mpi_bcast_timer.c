#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 500

int main(int argc, char **argv)
{
	const int root = 0;
	char console_input[MAX_BUFFER_SIZE];

	// Timer
	double total_bcast_time = 0.0;

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

		if(world_size < 2) {
			fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
			MPI_Abort(MPI_COMM_WORLD, 1);	
		}
	}

	// Synchronization and starting timer
	MPI_Barrier(MPI_COMM_WORLD);
	total_bcast_time -= MPI_Wtime();

	MPI_Bcast(&console_input, MAX_BUFFER_SIZE, MPI_BYTE, root, 
		MPI_COMM_WORLD);

	// Synchronization and stopping timer
	MPI_Barrier(MPI_COMM_WORLD);
	total_bcast_time += MPI_Wtime();

	printf("Process %d received message '%s' - Broadcast time is %f\n", 
		world_rank, console_input, total_bcast_time);

	// Finalize the MPI environment
	MPI_Finalize();

}
