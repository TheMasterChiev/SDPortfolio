//Compile: g++ -O2 Source.cpp Board.cpp -o abtetris
//Run: ./abtetris 10 10 

////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<time.h>
#include<string.h>
#include <stdlib.h>
#include"Board.h"
using namespace std;


int menu(){
	int option;
	cout << endl;
	cout << " MENU : " << endl;
	cout << "1. Play random tetris" << endl;
	cout << "2. Play single player alpha-beta approach" << endl;
	cout << "3. Play two player alpha-beta approach" << endl;
	cout << "4. Exit" << endl;
	cout << "opt: ";
	cin >> option;
	if (option >= 1 && option <= 4){
		return option;
	}
	else{
		cout << "Error: invalid option selected";
		return 0;
	}
}

int main(int argc, char *argv[]){
	bool notEnd = true;
	int myseed = 0;
	int option = 0;
	int rows = 0;
	int cols = 0;
	int mode = 1;
	int holding = 0;
	char p1_mode[64];
	char p2_mode[64];
	int p1;
	int p2;
	if (argc != 8){
		cout << "Error: insufficient command line arguments given" << endl;
		cout << "Usage: "<< argv[0] << "<p1_mode> <p2_mode> <rows> <cols> <mode> <hold> <seed>" << endl;
		return 1;
	}

	rows = atoi(argv[3]);
	cols = atoi(argv[4]);
	mode = atoi(argv[5]);
	holding = atoi(argv[6]);
	myseed = atoi(argv[7]);
	strcpy(p1_mode, argv[1]);
	strcpy(p2_mode, argv[2]);

	// check p1 mode 
	if (strcmp(p1_mode, "r") == 0){
		p1 = RANDOM;
	}
	else if (strcmp(p1_mode, "ab") == 0){
		p1 = ALPHABETA;
	}
	else if (strcmp(p1_mode, "m") == 0){
		p1 = MONTE_CARLO;
	}

	// check p2 mode 
	if (strcmp(p2_mode, "r") == 0){
		p2 = RANDOM;
	}
	else if (strcmp(p2_mode, "ab") == 0){
		p2 = ALPHABETA;
	}
	else if (strcmp(p2_mode, "m") == 0){
		p2 = MONTE_CARLO;
	}


	// initialize random time seed
	//srand(time(NULL));
	srand(myseed);

	Board b(rows, cols);
	b.play_two(p1,p2,mode,holding);

	//cout << "Waiting.." << endl;
	//sleep(1);
	//cout << "Exiting system goodbye!" << endl;
	return 0;
}
