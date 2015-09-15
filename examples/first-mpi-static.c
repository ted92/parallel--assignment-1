#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes (how many processes are working?) and put it in world_size
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process (which process is working)
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);



    // Print off a hello world message
    // printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);
/* bool flag = false;
    int i;
    for(i = 0; i < world_size; i++){
        if(!flag){
            //if the processor has the flag false then you have to print the result
            printf("Hello from %d\n", world_rank);
            flag = true;
        }
    }*/

    int data;
    //MPI Communication
    /*if(world_rank == 0){
        //master
        data = 5;
        printf("The master is calling the slave!\n");

        //send 1 data int at process 1 with tag 5
        MPI_Send(&data, 1, MPI_INT, 1, 5, MPI_COMM_WORLD);

    }
    else{
        //slave

        //receive 1 data of int type from process 0 with tag 5.
        MPI_Recv(&data, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        data = data*data;
        printf("%d\n",data);
    }
    */
    //Embarrassingly parallel problem

    int w;
    int h;
    w = 800;
    h = 800;


    //map display 800*800
    int temp_map[w][h];

    int row;
    int d_row; //delta row (row + 10)

    if(world_rank == 0){
        //master
        //send 10 rows per time
        int i, j, process;

        //start from the process slave = 1
        process = 1;

        for(i = 0, row = 0; i < 80; i++, row = row + 10){

            //check the process, if >= of number of processes involved then = 1
            if(process >= world_size){
                process = 1;
                MPI_Send(&row, 1, MPI_INT, process, 4, MPI_COMM_WORLD);
            }
            else{
                MPI_Send(&row, 1, MPI_INT, process, 4, MPI_COMM_WORLD);
                process++;
            }
            printf("row sent: %d, to a process: %x |", row, process);

        }

        //when finish send -1
        row = -1;
        MPI_Send(&row, 1, MPI_INT, 1, 4, MPI_COMM_WORLD);

        //initialize temp_map
        for(i = 0; i < w; i++){
            for(j = 0; j < h; j++){
                temp_map[i][j] = 0;
            }
        }

        //receive the new coordinates


    }
    else if (world_rank == 1){
        //slave
        //run the receiver as much time as the sender

        while(row != -1) {
            MPI_Recv(&row, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("row received: %d from process %d|", row, world_rank);
            d_row = row + 10;
        }


        int m,n;
        for(n = row; n < d_row; n++){
            for(m = 0; m < w; m++){
                /*printf("row: %d d_row %d ", row, d_row);
                temp_map[m][n] = row;
                printf("%d ", temp_map[m][n]);*/
            }
        }

    }

    // Finalize the MPI environment.
    MPI_Finalize();
}