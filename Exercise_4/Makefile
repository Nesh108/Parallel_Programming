EXECS=mpi_bcast_timer
MPICC?=mpicc

all:	${EXECS}

mpi_bcast_timer: ${EXECS}.c
	${MPICC} -o ${EXECS} ${EXECS}.c

clean:
	rm ${EXECS}
