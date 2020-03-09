#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "pngFile.h"

void print_grid(int size_row, int size_col, int grid[size_row][size_col]) //Wyswietla stan planszy
{
	for(int i = 0; i < size_row; i++)
	{
		for(int k = 0; k < size_col; k++)
		{
			printf("%d\t", grid[i][k]);
		}
		printf("\n");
	}
	printf("------------------------------------------------\n");
}

int how_many_alive(int current_row, int current_col, int size_row, int size_col, int grid[size_row][size_col])
{
	int number = 0; //Ilosc sasiadow
	int i_start = -1;
	int k_start = -1;
	int i_end = 1;
	int k_end = 1;
	//Ponizsze if-y zmieniaja indeksy sprawdzanych komorek w zaleznosci gdzie sie znajduja, jezeli komorka znajduje sie w srodku planszy to ma osiem sasiadow i nie trzeba tego zmieniac
	if(current_row==0 && current_col==0) //Poczatek planszy
	{
        	i_start = k_start = 0;
	}
        else if(current_col==size_col-1 && current_row==size_row-1) //Koniec planszy
        {
                k_end = i_end = 0;
        }
	else if(current_row==0 && current_col==size_col-1) //Prawy gorny rog planszy
	{
		i_start = 0;
		k_end = 0;
	}
	else if(current_row==size_row && current_col==0) //Lewy dolny rog planszy
	{
		i_end = 0;
		k_start = 0;

	}
	else if(current_row==0) //Gorny wiersz planszy
		i_start = 0;
	else if(current_col==0) //Pierwsza kolumna planszy
		k_start = 0;
	else if(current_row==size_row-1) //Dolny wiersz planszy
		i_end = 0;
	else if(current_col==size_col-1) //Ostatnia kolumna planszy
		k_end = 0;

	for(int i=i_start; i<=i_end; i++) //Badanie stanow sasiadow komorki grid[current_row][current_col]
	{
		for(int k=k_start; k<=k_end; k++)
		{
			if(i==0 && k==0) //Nie chcemy zeby komorka byla swoim sasiedem
				continue;
			if(grid[current_row+i][current_col+k]==1 || grid[current_row+i][current_col+k]==3) //Jezeli obecnie badana komorka jest zywa lub przed zmiana byla zywa
				number++;
		}
	}
	if(number==3 || number==2) //Interesuja nas przypadki w ktorych 2 lub 3 sasiednie komorki sa zywe
		return number;
	else
		return -1; //Jezeli ilosc sasiednich komorek zywych nie jest ani 2 ani 3 to nas to nie obchodzi
}
int main(int argc, char *argv[])
{
	if(argc<=4)
	{
		printf("Podano nieprawidlowwa ilosc argumentow\n");
		return EXIT_FAILURE;
	}
	int size_row = atoi(argv[1]); //Ilosc wierszy
	int size_col = atoi(argv[2]); //Ilosc kolumn
	int number_png = atoi(argv[3]); //Ile plikow png program ma tworzyc
	int scale = atoi(argv[4]);
	int was_change = 0; //Czy po przejsciu przez cala tablice stan jakies komorki sie zmienil
	int grid[size_row][size_col]; //Tablica reprezentujaca plansze
	
	int sc_row = size_row * scale;
	int sc_col = size_col * scale;

//	int **upGrid = (int **)malloc(sc_row * sizeof(int*));
//	for (int i = 0; i<sc_row; i++)
//		upGrid[i] = (int*)malloc(sc_col * sizeof(int));

//	int upGrid[sc_row][sc_col];
	
	srand(time(NULL));
	for(int i=0; i<size_row; i++) //Losujemy stany komorek
	{
		for(int k=0; k<size_col; k++)
		{
			int temp=rand() % 3;

			if(temp==2) 	//Chce aby wiecej bylo komorek martwych niz zywych
				temp=0;		//Wiec losuje liczby od 0 do 3 i zamieniam 3 i 2 na 0 (na komorki martwe)
			grid[i][k]=temp; //Zapisuje stan do komorki 0-komorka martwa 1- komorka zywa 2- komorka zywa, ktora byla martwa 3- komorka martwa, ktora byla zywa
		}
	}
	
//	upscale(size_row, size_col, grid, sc_row, sc_col, upGrid, scale); //skalowanie tablicy, po przejsciu grid

	printf("Poczatkowy stan komorek:\n");
	print_grid(size_row, size_col, grid);
//	print_grid(sc_row, sc_col, upGrid);

	if (scale > 1) {
		int upGrid[sc_row][sc_col];
		 upscale(size_row, size_col, grid, sc_row, sc_col, upGrid, scale); //skalowanie tablicy, po przejsciu grid
		process_file(sc_row,sc_col, upGrid,scale); //pngFile.c
		write_png_file(sc_row, sc_col,"Life_Start.png", -1, scale);
	}
	else
	{
 	     process_file(size_row,size_col, grid,1); //pngFile.c
             write_png_file(size_row, size_col,"Life_Start.png", -1, 1);
//	     process_file(sc_row,sc_col, upGrid,scale); //pngFile.c
//     	     write_png_file(sc_row, sc_col,"Life_Start2.png", -1, scale);

	}
	
	for(int i=0; i<number_png; i++) //Petla na ktorej beda tworzone kolejne iteracje
	{
		for(int current_row=0; current_row<size_row; current_row++)
		{
			for(int current_col=0; current_col<size_col; current_col++)
			{
				int number_of_alive = how_many_alive(current_row, current_col, size_row, size_col, grid);
				if(number_of_alive==-1 && (grid[current_row][current_col]==1 || grid[current_row][current_col]==2)) //Jezeli badana komorka nie ma ani 3 ani 2 sasiadow && (jest zywya || jest zywa, ale byla martwa)
				{
					grid[current_row][current_col]=3; //Komorka staje sie martwa, pamietajac ze przed zmiana byla zywa
				}
				else if(number_of_alive==3 && (grid[current_row][current_col]==0 || grid[current_row][current_col]==3)) //Jezeli badana komorka ma 3 sasiadow && (jest martwa || jest martwa, ale byla zywa)
				{
					grid[current_row][current_col]=2; //Komorka staje sie zywa, pamietalajac ze byla martwa
				}
			}
		}
		for(int current_row=0; current_row<size_row; current_row++) //Petla ktora zmieni wartosci komorek: 2 -> 1 , 3 -> 0 
		{
			for(int current_col=0; current_col<size_col; current_col++)
			{
				if(grid[current_row][current_col]==2)
					grid[current_row][current_col] = 1;
				else if(grid[current_row][current_col]==3)
					grid[current_row][current_col] = 0;
			}
		}
//		print_grid(size_row, size_col, grid);
		
		char file_name[] = "Life";
                char num_str[20];
                sprintf(num_str, "_%d.png", i);
                strcat(file_name, num_str);

		if (scale != 1 && scale >0)
		{
			int upGrid[sc_row][sc_col];
			upscale(size_row, size_col, grid, sc_row, sc_col, upGrid,scale);	
			process_file(sc_row,sc_col, upGrid, scale); //pngFile.c
			write_png_file(sc_row, sc_col,file_name, i,scale);
		}
		else
		{
			process_file(size_row,size_col,grid,1);
			write_png_file(size_row, size_col,file_name, i,1);
		}
	}
}
