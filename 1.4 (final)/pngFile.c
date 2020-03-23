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

void upscale(int height, int width, int cord[height][width],int nheight, int nwidth, int upCord[nheight][nwidth], int scale)
{

	//height - liczba wierszy
	//width - liczba kolumn

	int num = scale - 1;

	for(int i = 0; i < nheight; i++) // wpisanie w upCord samych zer
	{
		for(int  j = 0; j < nwidth; j++)
		{
			upCord[i][j]=0;
			//printf("j=%d\n",j);
		}
	//printf("i=%d\n",i);	
	}

	for(int i = 0; i < height; i++)

		for(int j = 0; j < width; j++)
		{
			if(cord[i][j] == 1)
			{
				//printf("i:%d j:%d\n", i,j);
				for(int k = 0; k <= num; k++)
				{
					int sec=0;
					for(int l = 0; l <= num; l++)
					{
						int sec = 0;

						upCord[(scale*(i+1))-k-1][(scale*(j+1))-l-1] = 1;
						//printf("Done l %d\n", sec++);
					}
				}
			}
		}

} 


	

void write_png_file(int height, int width, char* file_name, int number, int scale) {

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

void process_file(int height, int width, int cord[height][width], int scale) {

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

    }
  }
}

