#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>

#include "pngFile.h"

int x, y;

png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void upscale(int width, int height, int cord[width][height],int nheight, int nwidth, int upCord[nwidth][nheight], int scale)
{
	int num = scale - 1;
//	int nwidth = scale*width;
//	int nheight = scale*height;

//	int upCord[nwidth][nheight];

	for(int i = 0; i < nwidth; i++) // wpisanie w upCord samych zer
	{
		for(int  j = 0; j < nheight; j++)
			upCord[i][j]=0;
	}

	for(int i = 0; i < width; i++)

		for(int j = 0; j < height; j++)
		{
			if(cord[i][j] == 1)
			{
				printf("i:%d j:%d\n", i,j);
				for(int k = 0; k <= num; k++)
				{
					int sec=0;
					for(int l = 0; l <= num; l++)
					{
					//	int sec = 0;
						//i=i+1;
						//j=j+1;
						upCord[(scale*(i+1))-k-1][(scale*(j+1))-l-1] = 1;
						printf("Done l %d\n", sec++);
					}
				}
			}
		}

//	return 1;
} 


	

void write_png_file(int width, int height, char* file_name, int number, int scale) {
//  char str[7];
//  sprintf(str, "_%d.png", number);
//  strcat(file_name, str);


//if(scale > 1)

 // height = height*scale;
 // width = width*scale;


  FILE *fp = fopen(file_name, "wb");
  if (!fp)
    printf("[write_png_file] File %s could not be opened for writing", file_name);

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    printf("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    printf("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height,
   bit_depth, color_type, PNG_INTERLACE_NONE,
   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  for (y=0; y<height; y++)
    free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}

void process_file(int width, int height, int cord[width][height], int scale) {

//if(scale > 1)

//  height = height*scale;	
//  width = width*scale;

  bit_depth = 8;
  int black = 0;
  int white = 255;
  color_type = PNG_COLOR_TYPE_GRAY;

  number_of_passes = 7;
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (y=0; y<height; y++)
    row_pointers[y] = (png_byte*) malloc(sizeof(png_byte) * width);

  for (y=0; y<height; y++) {
    png_byte* row = row_pointers[y];
    for (x=0; x<width; x++) {
      if(cord[y][x]==1)
	      row[x]=black;
      else
	      row[x]=white;

//      printf("Pixel at position [ %d - %d ] has RGBA values: %d\n",
//       x, y, row[x]);
    }
  }
}

/*
int main(int argc, char **argv) {
  process_file();
  write_png_file("out.png");

  return 0;
}
*/
