
#include <stdio.h>

void upscale(int width, int height, int cord[width][height], int nwidth, int nheight,int upcord [nwidth][nheight], int scale);

void write_png_file(int width, int height, char* file_name, int number, int scale);

void process_file(int width, int height, int cord[width][height], int scale);

