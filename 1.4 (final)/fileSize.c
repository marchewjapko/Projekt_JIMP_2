#include <stdio.h>
#include <stdlib.h>
#include "fileSize.h"

void fileSize(FILE *in, int *row, int *col)
{
	int chr;
	int temp=-1;
	while((chr=getc(in)) != EOF)
	{		
		if(chr != '\n')
		{
			(*col)++;
		}

		if(chr == '\n')
		{
			if(temp!=*col && *row>1)
        	        {
                	       	*row=-1;
                        	*col=-1;
                      		 break;
               		 }
			(*row)++;
			temp = *col;
			(*col)=0;
		} 
	}
	if(*col != -1)
		*col=temp;
}
