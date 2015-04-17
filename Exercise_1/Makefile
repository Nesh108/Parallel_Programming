EXECS=hello_world
MPICC?=mpicc

all:	${EXECS}

hello_world: ${EXECS}.c
	${MPICC} -o ${EXECS} ${EXECS}.c

clean:
	rm ${EXECS}
