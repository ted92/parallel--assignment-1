#include "graphicsScreen.h"
#include "StopWatch.h"
#include <stdio.h>
#include <mpi.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800
#define ZOOMS_L 10
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

//added the zoom level that contains all levels of the mandelbrot set
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
    int x, y;

    //processor which is being used
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //total processes involved
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //is the variable which contains how many rows each slave has to compute
    int rows;
    rows = (HEIGHT / (size-1));

    int j, slave, row_index;

    int buffer_r[WIDTH+1];
    /*
     * buffer_to_send[0-799] = color
     * buffer_to_send[800] = y coordinate
     *
     * */

    row_index = 0;
    //if rank == 0 then execute the master otherwise execute the slave
    if (rank == 0) {
        //MASTER
        //master is the only one who can modify the global variables and is the only one who communicates with the user!!
        //display area is 800x800 so divide each row to processes

        //from 1st slave till size
        for(slave = 1; slave < size; slave++){
            //for each row per each slave
            for(row_index = (rows*(slave-1)); row_index < (rows*slave); row_index++){
                MPI_Recv(&buffer_r, (WIDTH+1), MPI_INT, MPI_ANY_SOURCE, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                for(j = 0; j < WIDTH; j++){
                    y = buffer_r[WIDTH];
                    //register in buffer the values and then print the map!
                    colorMap[y][j] = buffer_r[j];
                    crc += colorMap[y][j];
                    //gs_plot(j, y, colorMap[y][j]);
                }
            }
        }

        //the master calculates the rows left
        for(row_index = (rows*(size-1)); row_index < HEIGHT; row_index++){
            for(x = 0; x < WIDTH; x++){
                colorMap[row_index][x] = palette[solve(translate_x(x),translate_y(row_index))];
                crc += colorMap[row_index][x];
                //gs_plot(x, row_index, colorMap[row_index][x]);
            }
        }
        //gs_update();
    }

    else{
        //SLAVE

        int buffer_to_send[WIDTH+1];
        /*
         * buffer_to_send[0-799] = color
         * buffer_to_send[800] = y coordinate
         *
         * */

        //calculate the portion to compute:
        int current_row;
        int start_row;
        // the rows starts alway at: 0 (rows* 1 - 1), rows (rows * 2 - 1), ... , HEIGHT - rows - remainder of division (rows * size - 1)
        start_row = rows * (rank-1);

        //ALL THE VARIABLE MUST BE LOCAL

        for(current_row = start_row; current_row < (start_row + rows); current_row ++){
            buffer_to_send[WIDTH] = current_row;
            for(x = 0; x < WIDTH; x++){
                //The color is determined by the number of iterations
                //More iterations means brighter color
                buffer_to_send[x] = palette[solve(translate_x(x),translate_y(current_row))];
            }
            MPI_Send(&buffer_to_send, (WIDTH+1), MPI_INT, 0, 6, MPI_COMM_WORLD);
        }
    }
    //synchronize each zoom level
    MPI_Barrier(MPI_COMM_WORLD);
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
