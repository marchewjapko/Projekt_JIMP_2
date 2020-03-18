
#include <stdio.h>

void upscale(int height, int width, int cord[height][width], int nheight, int nwidth,int upcord [nheight][nwidth], int scale);

void write_png_file(int height, int width, char* file_name, int number, int scale);

void process_file(int height, int width, int cord[height][width], int scale);

