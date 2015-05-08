EXECS=mpi_seq_sum
MPICC?=mpicc

all:	${EXECS}

mpi_seq_sum: ${EXECS}.c
	${MPICC} -o ${EXECS} ${EXECS}.c

clean:
	rm ${EXECS}
