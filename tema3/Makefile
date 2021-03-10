build: tema3.c string_operations.c
	mpicc tema3.c -c -lpthread -Wall
	mpicc string_operations.c -c -Wall
	mpicc tema3.o string_operations.o -o main -Wall
run:
	mpirun -oversubscribe -np 5 ./main $(in)
clean:
	rm main *.out *.o
