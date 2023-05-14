#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POINTS 50

int main()
{
    int x[MAX_POINTS];
    int y[MAX_POINTS];
    int num_points = 0;
    int i;

    // Add some sample data
    for (i = 0; i < MAX_POINTS; i++) {
        x[i] = i;
        y[i] = rand() % 100;
        num_points++;
    }

    // Create a gnuplot pipe
    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    // Check if gnuplot is installed
    if (!gnuplotPipe) {
        printf("Gnuplot not found!");
        return -1;
    }

    // Set up the plot properties
    fprintf(gnuplotPipe, "set title 'Line Chart Example'\n");
    fprintf(gnuplotPipe, "set xlabel 'X Axis'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y Axis'\n");
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "plot '-' with lines title 'Data'\n");

    // Plot the data points
    for (i = 0; i < num_points; i++) {
        fprintf(gnuplotPipe, "%d %d\n", x[i], y[i]);
    }

    // End the plot
    fprintf(gnuplotPipe, "e\n");

    // Close the gnuplot pipe
    pclose(gnuplotPipe);

    return 0;
}
