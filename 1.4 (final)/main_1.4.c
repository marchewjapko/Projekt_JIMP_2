#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "pngFile.h"
#include "fileSize.h"

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
	if(argc<=3)
	{
		printf("Podano nieprawidlowa ilosc argumentow\n");
		printf("\nWzór wywołania:\n");
		printf("./conways_game_of_life.o [plik tekstowy] [ilosc iteracji] [skalowanie]\n");
		printf("UWAGA -> Przy podaniu zbyt wielkiej skali, program zwróci błąd\n\n");
		return EXIT_FAILURE;
	}
	FILE *in = fopen(argv[1], "r");
	if(in==NULL)
	{
		printf("Blad podczas otwierania pliku\n");
		return EXIT_FAILURE;
	}
	FILE *out = fopen("skrypt", "w");
	int size_row = 0; //Ilosc wierszy
	int size_col = 0; //Ilosc kolumn
	
	fileSize(in, &size_row, &size_col); //Czytanie rozmiaru pliku wsadowego

	if(size_col == -1)
	{
		printf("Błędny plik. W pliku występuje gdzieś pusta linia.\n");
		return EXIT_FAILURE;
	}

	int number_png = atoi(argv[2]); //Ile plikow png program ma tworzyc
	int scale = atoi(argv[3]); //Skalowanie (reprezentowanie komorki za pomoca kednego piksela jest nie czytelne)
//	fileSize(in, &size_row, &size_col); //Czytanie rozmiaru pliku wsadowego
	int grid[size_row][size_col]; //Tablica reprezentujaca plansze
	
	int sc_row = size_row * scale;
	int sc_col = size_col * scale;

	int i = 0;
	int k = 0;
	char ch;
	rewind(in);
	while((ch=getc(in)) != EOF) //Czytanie wartosci z pliku
        {
                if(ch=='\n') //Jezeli napotkamy znak nowej linii
                {
                        k=0;
                        i++;
                        continue; //Jezeli napotkamy znak nowej linii to przechodzimy dalej i go nie zapisujemy
                }
                if(!isdigit(ch))
                {
                        printf("Napotkano nieprawidlowa wartosc w pliku \"%s\"\n", argv[2]);
                        return EXIT_FAILURE;
                }
                grid[i][k]=atoi(&ch);
                k++;
        }
	
	fprintf(out, "%s", "rm -r ./png\n"); //Usuwam folder z wczesniej wygenerowanymi plaszami
	fprintf(out, "%s", "mkdir ./png\n"); //Twoeze folder z wygenerowanymi plaszami

	if (scale > 1) {
		int upGrid[sc_row][sc_col];
		upscale(size_row, size_col, grid, sc_row, sc_col, upGrid, scale); //skalowanie tablicy, po przejsciu grid
		process_file(sc_row,sc_col, upGrid,scale); //pngFile.c
		write_png_file(sc_row, sc_col,"Life_0.png", -1, scale);
	}
	else
	{
 	     process_file(size_row,size_col, grid,1); //pngFile.c
             write_png_file(size_row, size_col,"Life_0.png", -1, 1);

	}

	fprintf(out, "%s", "mv ./Life_0.png ./png\n"); //Przeniesienie pliku Life_Start.png do folderu z plikami .png
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
		char file_name[] = "Life";
                char num_str[20];
                sprintf(num_str, "_%d.png", i+1);
                strcat(file_name, num_str);
		if (scale != 1 && scale >0)
		{
			int upGrid[sc_row][sc_col];
			upscale(size_row, size_col, grid, sc_row, sc_col, upGrid,scale);	
			process_file(sc_row,sc_col, upGrid, scale); //pngFile.c
			write_png_file(sc_row, sc_col, file_name, i, scale);
		}
		else
		{
			process_file(size_row,size_col,grid,1);
			write_png_file(size_row, size_col,file_name, i,1);
		}
		fprintf(out, "%s%s%s", "mv ./", file_name, " ./png\n");
	}
	fclose(out);
	system("chmod u+x skrypt");
	system("./skrypt");
	out = fopen("skrypt", "w");
	fprintf(out, "%s%s%s", "convert -delay 120 -loop 0 ./png/Life_{0..", argv[2], "}.png life.gif\n");
	fprintf(out, "%s", "mv ./life.gif ./png");
	fclose(out);
	system("rm skrypt");
}
