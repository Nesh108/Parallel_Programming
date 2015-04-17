EXECS=mpi_telephone
MPICC?=mpicc

all:	${EXECS}

mpi_telephone: ${EXECS}.c
	${MPICC} -o ${EXECS} ${EXECS}.c

clean:
	rm ${EXECS}
