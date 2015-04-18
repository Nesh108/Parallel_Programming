EXECS=mpi_bcast
MPICC?=mpicc

all:	${EXECS}

mpi_bcast: ${EXECS}.c
	${MPICC} -o ${EXECS} ${EXECS}.c

clean:
	rm ${EXECS}
