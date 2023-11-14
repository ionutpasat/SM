build: rank
rank:
	mpicc rankSortMPI.c -o rankSort -lm
run:
	mpirun -n 4 rankSort
clean:
	rm rankSort