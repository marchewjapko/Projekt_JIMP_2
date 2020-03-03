#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_grid(int size_row, int size_col, int grid[size_row][size_col]) //Wyswietla stan planszy
{
	for(int i=0; i<size_row; i++)
	{
		for(int k=0; k<size_col; k++)
		{
			printf("%d\t", grid[i][k]);
		}
		printf("\n");
	}
	printf("------------------------\n");
}
int how_many_alive(int current_row, int current_col, int size_row, int size_col, int grid[size_row][size_col])
{
	int number=0;
	if(current_row!=0 && current_row!=size_row && current_col!=0 && current_col!=size_col) //Jezeli nie znajdujemy sie na krancach planszy
	{
		for(int i=-1; i<=1; i++)
		{
			for(int k=-1; k<=1; k++)
			{
				if(i==0 && k==0)
					continue;
				//printf("i: %d, k: %d\n", i, k);
				if(grid[current_row+i][current_col+k]==1)
					number++;
			}
		}

	}
	if(number==3 || number==2) //Interesuja nas przypadki w ktorych 2 lub 3 sasiednie komorki sa zywe
		return number;
	else
		return -1;
}
int main(int argc, char *argv[])
{
	if(argc<=3)
	{
		printf("Podano nieprawidlowwa ilosc argumentow\n");
		return EXIT_FAILURE;
	}
	srand(time(NULL));
	int size_row = atoi(argv[1]); //Ilosc wierszy
	int size_col = atoi(argv[2]); //Ilosc kolumn
	int number_png = atoi(argv[3]); //Ile plikow png program ma tworzyc
	int was_change = 0; //Czy po przejsciu przez cala tablice stan jakies komorki sie zmienil
	int grid[size_row][size_col]; //Tablica reprezentujaca plansze
	for(int i=0; i<size_row; i++) //Losujemy stany komorek
	{
		for(int k=0; k<size_col; k++)
		{
			//int temp=rand() % 4;
			int temp=rand() % 3;

			if(/*temp==3 || */temp==2) 	//Chce aby wiecej bylo komorek martwych niz zywych
				temp=0;		//Wiec losuje liczby od 0 do 3 i zamieniam 3 i 2 na 0 (na komorki martwe)

			grid[i][k]=temp; //Zapisuje stan do komorki	0-komorka martwa	1- komorka zywa
		}
	}
	printf("Poczatkowy stan komorek:\n");
	print_grid(size_row, size_col, grid);
	for(int i=0; i<number_png; i++)
	{
		for(int current_row=0; current_row<size_row; current_row++)
		{
			for(int current_col=0; current_col<size_col; current_col++)
			{
				//printf("ROW: %d, COL: %d\n", current_row, current_col); //Pomocnicze
				int number_of_alive = how_many_alive(current_row, current_col, size_row, size_col, grid);
				if(number_of_alive==-1)
				{
					if(grid[current_row][current_col]==1)
						grid[current_row][current_col]=0;
				}
				else if(number_of_alive==3)
				{
					if(grid[current_row][current_col]==0)
						grid[current_row][current_col]=1;
				}
			}
		}
		print_grid(size_row, size_col, grid);
	}
}
