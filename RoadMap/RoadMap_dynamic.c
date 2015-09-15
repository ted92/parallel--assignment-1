#include "graphicsScreen.h"
#include "StopWatch.h"
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define SIZE WIDTH*HEIGHT

#ifndef GRAPHICS
#define gs_init(x,y)
#define gs_plot(x,y,z)
#define gs_exit()
#define gs_update()
#endif

//The number of iterations decides how long we will compute before giving up
#define ITERATIONS 1024

int zooms=10;

int crc = 0;

int *palette;

int colorMap[HEIGHT][WIDTH];

double box_x_min, box_x_max, box_y_min, box_y_max;

/**
 * Translate from screen coordinates to space coordinates
 *
 * @param	x	Screen coordinate
 * @returns	Space coordinate
 */
inline double translate_x(int x) {
    return (((box_x_max-box_x_min)/WIDTH)*x)+box_x_min;
}

/**
 * Translate from screen coordinates to space coordinates
 *
 * @param	y	Screen coordinate
 * @returns	Space coordinate
 */
inline double translate_y(int y) {
    return (((box_y_max-box_y_min)/HEIGHT)*y)+box_y_min;
}

/**
 * Mandelbrot divergence test
 *
 * @param	x,y	Space coordinates
 * @returns	Number of iterations before convergance
 */
inline int solve(double x, double y)
{
    double r=0.0,s=0.0;

    double next_r,next_s;
    int itt=0;


    //code to edit
    while((r*r+s*s)<=4.0) {
        next_r=r*r-s*s+x;
        next_s=2*r*s+y;
        r=next_r; s=next_s;
        if(++itt==ITERATIONS)break;
    }


    return itt;
}

/**
 * Set up the palette to translate from number of iterations to a color
 *
 */
void setup_colors() {
    palette = (int *)malloc(sizeof(int) * (ITERATIONS + 1));
    int temp_colors[4] = {0x0000ff, 0x00ff00, 0x00ff00, 0xff0000};
    int multiplier = 3;
    int i;
    for(i = 0; i <= ITERATIONS; i++) {
        float position = (float)(i/(float)(ITERATIONS + 1));
        float color_selection = position*multiplier;
        int real_selector = (int)floorf(color_selection);
        int real_color = 0x000000;
        real_color = (temp_colors[real_selector] + temp_colors[real_selector + 1] * color_selection - real_selector);
        palette[i] = real_color;
    }
    return;
}

/**
 * Creates all the mandelbrot images and shows them on screen
 *
 */
void CreateMap() {


    //tags are:
    int row_tag = 5;
    int result_tag = 6;

    //processor which is being used
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //total processes involved
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //if rank == 0 then execute the master otherwise execute the slave
    if (rank == 0){
        //MASTER
        //in the dynamic version each process get the job when is free. The job is to compute each row per time
        //give the initial job to processes

        int k;
        int row;

        row = 0;

        for(k = 1; k < size; k++){
            MPI_Send(&row, 1, MPI_INT, k, row_tag, MPI_COMM_WORLD);
            row++;
        }

        int buff_rcv[WIDTH+2];
        /*
        *0-799 -> colors
        * 800 -> row
        * 801 -> rank
        * */

        //check that the master has received all the rows
        int received_count = 0;

        do{
            if(received_count < HEIGHT){
                MPI_Recv(&buff_rcv, (WIDTH+2), MPI_INT, MPI_ANY_SOURCE, result_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                received_count++;

                //color the map
                for(k = 0; k < WIDTH; k++){
                    colorMap[buff_rcv[WIDTH]][k] = buff_rcv[k];
                    crc += colorMap[buff_rcv[WIDTH]][k];
                }

                //check that the master has sent all the rows
                if (row < HEIGHT) {
                    //send the row to the process which just get free (in buff_rcv[801]
                    MPI_Send(&row, 1, MPI_INT, buff_rcv[WIDTH+1], row_tag, MPI_COMM_WORLD);
                    row++;
                }
            }
            else{
                //terminate the process
                int poison = -1;
                for(k = 1; k < size; k++){
                    MPI_Send(&poison, 1, MPI_INT, k, row_tag, MPI_COMM_WORLD);
                }
                break;
                //with cycle or just one MPI call
            }
        }while(1);

        //dysplay the Mandelbrot set:
        int x,y;

        for(y = 0; y < HEIGHT; y++){
            for(x = 0; x < WIDTH; x++){
                //gs_plot(x, y, colorMap[y][x]);
            }
        }
        //gs_update();
    }

    else{
        //SLAVE
        int x;
        //row received
        int row_rcv;
        int buffer_to_send[WIDTH+2];
        /*
        *0-799 -> colors
        * 800 -> row
        * 801 -> rank
        * */
        buffer_to_send[WIDTH+1] = rank;

        //receive first row to compute
        MPI_Recv(&row_rcv, 1, MPI_INT, 0, row_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //do until the slave receive the row = -1
        while(row_rcv != -1){
            buffer_to_send[WIDTH] = row_rcv;
            //compute the row
            for(x = 0; x < WIDTH; x++){
                buffer_to_send[x] = palette[solve(translate_x(x),translate_y(row_rcv))];
            }
            MPI_Send(&buffer_to_send, (WIDTH+2), MPI_INT, 0, result_tag, MPI_COMM_WORLD);

            //receive next row
            MPI_Recv(&row_rcv, 1, MPI_INT, 0, row_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}


/**
 * Sets up the coordinate space and generates the map at different zoom level
 *
 */
void RoadMap ()
{
    int i;
    double deltaxmin, deltaymin, deltaxmax,deltaymax;

    //Sets the bounding box,
    //Note that the x_min is -1.5 instead of -2.5, since we are using a square window
    box_x_min=-1.5; box_x_max=0.5;
    box_y_min=-1.0; box_y_max=1.0;

    deltaxmin=(-0.9-box_x_min)/zooms;
    deltaxmax=(-0.65-box_x_max)/zooms;
    deltaymin=(-0.4-box_y_min)/zooms;
    deltaymax=(-0.1-box_y_max)/zooms;

    //Updates the map for every zoom level
    CreateMap();
    for(i=1;i<zooms;i++){
        box_x_min+=deltaxmin;
        box_x_max+=deltaxmax;
        box_y_min+=deltaymin;
        box_y_max+=deltaymax;
        CreateMap();
    }
}

/**
 * Main function
 *
 * @param	argc, argv	Number of command-line arguments and the arguments
 * @returns	0 on success
 */
int main (int argc, char *argv[]){
    char buf[256];    /*buffer size*/


    //initialize the MPI
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        sw_init();
        sw_start();
        gs_init(WIDTH, HEIGHT);
    }

    setup_colors();

    RoadMap();

    //Finalize MPI
    MPI_Finalize();


    if(rank == 0) {
        sw_stop();
        sw_timeString(buf);

        printf("CRC: %x, ",crc);
        printf("time: %s\n",buf);

        gs_exit();

    }

    return 0;
}
