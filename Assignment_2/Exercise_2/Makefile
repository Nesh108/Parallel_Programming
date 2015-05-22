EXECS=mpi_life
MPICC?=mpicc

all:	${EXECS}

mpi_life: ${EXECS}.c
	${MPICC} -o ${EXECS} ${EXECS}.c

clean:
	rm ${EXECS}

