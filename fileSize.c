#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[])
{
	FILE *in = fopen(argv[1], "r");
	int row=0;
	int col=0;
	fileSize(in, &row, &col);
	printf("r=%d c=%d", row, col);
}

