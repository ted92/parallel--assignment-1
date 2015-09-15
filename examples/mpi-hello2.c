#include <stdio.h> 
#include <sys/unistd.h>
#include <mpi.h>

#define MAX_HOSTNAME_LEN 256

int main(int argc, char*argv[])
{
    int size, rank; 
    int len = MAX_HOSTNAME_LEN; 
    char hostname[MAX_HOSTNAME_LEN]; 

    MPI_Init(&argc, &argv); 

    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if (rank == 0)
	gethostname(hostname, len);  

    MPI_Bcast(hostname, MAX_HOSTNAME_LEN, MPI_CHAR, 0, MPI_COMM_WORLD); 
    
    if (rank != 0) 
	printf("Root is running on %s says rank %d\n", hostname, rank); 

    
    MPI_Finalize();  
}

