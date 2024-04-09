//Compile: g++ -O2 Source.cpp Board.cpp -o abtetris
//Run: ./abtetris 10 10 

////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;


#ifndef BOARD_H
#define BOARD_H

#define NUM_PIECES 7
#define MONTECARLOGAMES 100
#define RANDOM 1
#define MONTE_CARLO 2
#define ALPHABETA 3

enum PieceName { O, L, J, Z, S, I, T }; 
enum color { Black, White };
#define P1 0
#define P2 1

//Maak dit aub mooier en relevanter
const int MAX_width = 500;     // maximum width of the game board
const int MAX_height = 500;     // maximum total height of the game board

class Board{
	vector<vector<bool> > field;
	int pieceCount;
	int score_p1;
	int score_p2;
	int turn;
	int rows;
	int cols;
	int lastPos;
	int lastOrient;

public:
	Board(int rows, int cols);
	Board();
	int vacantblocks(int row);
	void check_top_row(bool row[MAX_width], int & which_row, int & emptyblocks);
	int clearLines();
	void print();
	void letItFall(PieceName piece, int orient, int pos);
	int possible_moves(PieceName piece);
	void get_info(PieceName piece, int & orient, int & pos, int move);
	vector<Board> list_all_moves();
	vector<Board> list_all_moves_with(PieceName piece);
	vector<Board> list_all_moves_with_pieces(PieceName piece, PieceName hold);
	void randomMoves();
	void alphaBetaPlay(int depth, int modus, int holdings);
	void alphaBetaPlay_two(int depth, int modus, int holdings);
	void play_two(int p1, int p2, int modus, int holdings);
	bool GameOver(int option, int cleared); //Added cleared for option 2
	void printPiece(PieceName piece, int orientation, int position);
	int evaluate_board(color c, int option);
	PieceName temp_hold; //Voor het onthouden van de Tetromino van de Hold.
  //bool hold_used; //Om bij te houden of de Hold Tetromino is gebruikt. Zo ja? Wissel.
};

Board ABMinMax(Board board, short int depth_limit, PieceName p, PieceName hold, int &wissel, int modus, int holdings);
Board ABMinMax_two(Board board, short int depth_limit, PieceName p, PieceName hold, int &wissel, int modus, int holdings);
Board ABMaxMove(Board board, short int depth_limit, PieceName p, short int depth, int a, int b, PieceName hold, int &wissel, int modus, int holdings);
Board ABMinMove(Board board, short int depth_limit, PieceName p, short int depth, int a, int b, PieceName hold, int &wissel, int modus, int holdings);


#endif
