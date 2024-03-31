//Compile: g++ -O2 Source.cpp Board.cpp -o abtetris
//Run: ./abtetris 10 10 

////////////////////////////////////////////////////////////////////////////
#include"Board.h"

int wissel;
const int n = 200;
bool wissel_array[n];

// overloaded constructor initializes field to rows x cols size
Board::Board(int rows, int cols){
	vector<bool> row;

	// set variables
	this->rows = rows + 3;
	this->cols = cols;
	this->score_p1 = 0;
	this->score_p2 = 0;
	this->turn = 0;
	this->pieceCount = 0;
	//Used to save orientation and position for alpha beta result:
	this->lastPos = 0;
	this->lastOrient = 0;

	//Build Board field:
	for (int i = 0; i < rows + 3; i++){
		row.clear();
		for (int j = 0; j < cols; j++){
			row.push_back(false);
		}
		field.push_back(row);
	}
}


// default constructor
Board::Board(){
	//Set variables:
	this->rows = 0 + 3;
	this->cols = 0;
	this->score_p1 = 0; 
	this->score_p2 = 0;
	this->turn = 0;
	this->pieceCount = 0;
	//Used to save Orientation and Position for Alpha-Beta Result:
	this->lastPos = 0;
	this->lastOrient = 0;
}


//How many empties has Row <numberrow>?
int Board::vacantblocks(int row){
	int i;
	int amount = 0;
	for(i = 0; i < cols; i++)
		if(field[row][i] == false)
			amount++;
	return amount;
}


//Gives number of empties in highest Non-Empty Row and copies
// this Row into therow; corresponding index being the row nr.
// if this is -1, the whole field is empty.
void Board::check_top_row(bool row[MAX_width], int & which_row, int & emptyblocks){
	int i;
	int j;
	int amount;
	which_row = -1;
	emptyblocks = cols;
	for(i = 0; i < rows; i++){
		amount = vacantblocks(i);
		if(amount < cols){
			for(j = 0; j < cols; j++)
				row[j] = field[i][j];
			emptyblocks = amount;
			which_row = i;
			//cout << "CHECK TOP ROW: " << which_row << endl;
    		}
  	}
}


//Function to clear complete field Tetris lines:
int Board::clearLines(){
	bool full_line;
	int cleared = 0;
	int i;
	int j;
	int k;

	for(i = (rows - 2); i >= 0; i--){
		full_line = true;
		j = 0;
		while(full_line && j < cols)
			if(field[i][j] == false)
				full_line = false;
			else
				j++;
		if(full_line){
			//cout << "Row cleared ..." << endl;
			cleared++;
			for(k = i; k < rows - 1; k++)
				for(j = 0; j < cols; j++)
					field[k][j] = field[k+1][j];
					for(j = 0; j < cols; j++)
						field[rows - 1][j] = false; //weird
		}//if
	}//for
	
	/* Previous version:
	//Traverse all rows:
	for(int i = 0; i < rows; i++){
		count = 0;
		//Traverse all columns of each row:
		for (int j = 0; j < cols; j++){
			//If location is occupied increment count:
			if (field[i][j])
				count++;
		}

		// If whole row was occupied we need to clear it and shift top rows down:
		if (count == cols){
			//Traverse rows down from current complete row:
			for (int k = i; k < rows - 1; k++){
				//Copy all cols:
				for (int j = 0; j < cols; j++){
					field[k][j] = field[k + 1][j];
				}
			}
			//Clear top most row:
			for (int j = 0; j < cols; j++){
				field[rows - 1][j] = false;
			}
			cleared++;
		}
	}*/

	//Return number of rows cleared:
	return cleared;
}


//Function to print current board on the screen:
void Board::print(){
	int i;
	int j;
	cout << "Board: " << endl << endl;
	//Start from top row since that is how C++ cout will work:
	for (i = rows - 1; i >= 0; i--){
    	
	/*	OLD:
		//Draw left boundary line:
		cout << '|';
		for (int j = 0; j < cols; j++){
			//If occupied:
			if (field[i][j])
				cout << "X";
			else
				cout << " ";
		}
		//Draw right boundary line:
		cout << "|";
		cout << endl;
	}
	//Draw bottom boundary line:
	for (int j = 0; j < cols+2; j++){
		cout << "-";
	}
	cout << endl;
	cout << endl;		*/
		
		if(i < rows-3)
			cout << "|";
		else
      			cout << " ";
		for(j=0; j < cols; j++){
	      		if(field[i][j])
				cout << "X";
	      		else
	      			cout << " ";
    		}
		if(i < rows-3)
			cout << "|" << endl;
		else
			cout << endl;
	}//for
  	for(j=0; j<cols+2; j++)
    		cout << "-";
	cout << endl;
	cout << " ";
	for(j=0; j<cols; j++)
		cout << j % 10;
	cout << endl;
}


//Function to let piece fall in position and orientation given
// assuming it still fits in top rows of the board.:
void Board::letItFall(PieceName piece, int orient, int pos){
	int x[4] = { 0 };
	int y[4] = { 0 };
	int i;

	int globalheight = rows;
	int globalwidth = cols;

	pieceCount++;
	switch (piece){
		case O: x[0] = pos; y[0] = globalheight - 2;
			x[1] = pos; y[1] = globalheight - 1;
			x[2] = pos + 1; y[2] = globalheight - 2;
			x[3] = pos + 1; y[3] = globalheight - 1;
			break;
		case J: switch (orient){
				case 0: x[0] = pos + 2; y[0] = globalheight - 2;
					x[1] = pos + 2; y[1] = globalheight - 1;
					x[2] = pos + 1; y[2] = globalheight - 1;
					x[3] = pos; y[3] = globalheight - 1;
					break;
				case 1: x[0] = pos; y[0] = globalheight - 3;
					x[1] = pos; y[1] = globalheight - 2;
					x[2] = pos; y[2] = globalheight - 1;
					x[3] = pos + 1; y[3] = globalheight - 1;
					break;
				case 2: x[0] = pos; y[0] = globalheight - 2;
					x[1] = pos + 1; y[1] = globalheight - 2;
					x[2] = pos + 2; y[2] = globalheight - 2;
					x[3] = pos; y[3] = globalheight - 1;
					break;
				case 3: x[0] = pos; y[0] = globalheight - 3;
					x[1] = pos + 1; y[1] = globalheight - 1;
					x[2] = pos + 1; y[2] = globalheight - 2;
					x[3] = pos + 1; y[3] = globalheight - 3;
					break;
		}//switch
		break;
		case L: switch (orient){
				case 0: x[0] = pos; y[0] = globalheight - 2;
					x[1] = pos + 2; y[1] = globalheight - 1;
					x[2] = pos + 1; y[2] = globalheight - 1;
					x[3] = pos; y[3] = globalheight - 1;
					break;
				case 1: x[0] = pos; y[0] = globalheight - 3;
					x[1] = pos; y[1] = globalheight - 2;
					x[2] = pos; y[2] = globalheight - 1;
					x[3] = pos + 1; y[3] = globalheight - 3;
					break;
				case 2: x[0] = pos; y[0] = globalheight - 2;
					x[1] = pos + 1; y[1] = globalheight - 2;
					x[2] = pos + 2; y[2] = globalheight - 2;
					x[3] = pos + 2; y[3] = globalheight - 1;
					break;
				case 3: x[0] = pos + 1; y[0] = globalheight - 3;
					x[1] = pos + 1; y[1] = globalheight - 1;
					x[2] = pos + 1; y[2] = globalheight - 2;
					x[3] = pos; y[3] = globalheight - 1;
					break;
		}//switch
		break;
		case Z: switch (orient){
				case 0: x[0] = pos + 1; y[0] = globalheight - 2;
					x[1] = pos + 1; y[1] = globalheight - 1;
					x[2] = pos + 2; y[2] = globalheight - 2;
					x[3] = pos; y[3] = globalheight - 1;
					break;
				case 1: x[0] = pos; y[0] = globalheight - 3;
					x[1] = pos; y[1] = globalheight - 2;
					x[2] = pos + 1; y[2] = globalheight - 1;
					x[3] = pos + 1; y[3] = globalheight - 2;
					break;
		}//switch
		break;
		case S: switch (orient){
				case 0: x[0] = pos + 1; y[0] = globalheight - 2;
					x[1] = pos + 1; y[1] = globalheight - 1;
					x[2] = pos + 2; y[2] = globalheight - 1;
					x[3] = pos; y[3] = globalheight - 2;
					break;
				case 1: x[0] = pos + 1; y[0] = globalheight - 3;
					x[1] = pos; y[1] = globalheight - 2;
					x[2] = pos + 1; y[2] = globalheight - 2;
					x[3] = pos; y[3] = globalheight - 1;
					break;
		}//switch
		break;
		case I: switch (orient){
				case 0: x[0] = pos; y[0] = globalheight - 1;
					x[1] = pos + 1; y[1] = globalheight - 1;
					x[2] = pos + 2; y[2] = globalheight - 1;
					x[3] = pos + 3; y[3] = globalheight - 1;
					break;
				case 1: x[0] = pos; y[0] = globalheight - 4;
					x[1] = pos; y[1] = globalheight - 3;
					x[2] = pos; y[2] = globalheight - 2;
					x[3] = pos; y[3] = globalheight - 1;
					break;
		}//switch
		break;
		case T: switch (orient){
				case 0: x[0] = pos + 1; y[0] = globalheight - 2;
					x[1] = pos; y[1] = globalheight - 1;
					x[2] = pos + 1; y[2] = globalheight - 1;
					x[3] = pos + 2; y[3] = globalheight - 1;
					break;
				case 1: x[0] = pos; y[0] = globalheight - 3;
					x[1] = pos; y[1] = globalheight - 2;
					x[2] = pos; y[2] = globalheight - 1;
					x[3] = pos + 1; y[3] = globalheight - 2;
					break;
				case 2: x[0] = pos; y[0] = globalheight - 2;
					x[1] = pos + 1; y[1] = globalheight - 2;
					x[2] = pos + 2; y[2] = globalheight - 2;
					x[3] = pos + 1; y[3] = globalheight - 1;
					break;
				case 3: x[0] = pos + 1; y[0] = globalheight - 3;
					x[1] = pos + 1; y[1] = globalheight - 2;
					x[2] = pos + 1; y[2] = globalheight - 1;
					x[3] = pos; y[3] = globalheight - 2;
					break;
		}//switch
		break;
	}//switch
	while (y[0] > 0 && !field[y[0] - 1][x[0]]
		&& !field[y[1] - 1][x[1]] && !field[y[2] - 1][x[2]]
		&& !field[y[3] - 1][x[3]]){
		for (i = 0; i < 4; i++)
			y[i]--;
	}
	//Zorgt dat tetromino true is:
	for (i = 0; i<4; i++)
		field[y[i]][x[i]] = true;
}


//Function to generate number of possible moves:
int Board::possible_moves(PieceName piece){
	int globalwidth = cols;
	if (piece == O){
		return (globalwidth - 1);
	}
	else if (piece == Z || piece == S || piece == I){
		return (2 * globalwidth - 3);
	}
	else{
		return (4 * globalwidth - 6);
	}
}


//Compute orientation and position for the move from piece:
void Board::get_info(PieceName piece, int & orient, int & pos, int move){
	int globalwidth = cols;
	orient = 0;
	pos = move;
	switch (piece){
		case Z: if (move > globalwidth - 3){
				orient = 1;
				pos = move - (globalwidth - 2);
			}//if
			break;
		case S: if (move > globalwidth - 3){
				orient = 1;
				pos = move - (globalwidth - 2);
			}//if
			break;
		case I: if (move > globalwidth - 4){
				orient = 1;
				pos = move - (globalwidth - 3);
			}//if
			break;
		case O: break;
		case T: if (move > 3 * globalwidth - 6){
				orient = 3;
				pos = move - (3 * globalwidth - 5);
			}//if
			else if (move > 2 * globalwidth - 4){
				orient = 2;
				pos = move - (2 * globalwidth - 3);
			}//if
			else if (move > globalwidth - 3){
				orient = 1;
				pos = move - (globalwidth - 2);
			}//if
			break;
		case J: if (move > 3 * globalwidth - 6){
				orient = 3;
				pos = move - (3 * globalwidth - 5);
			}//if
			else if (move > 2 * globalwidth - 4){
				orient = 2;
				pos = move - (2 * globalwidth - 3);
			}//elseif
			else if (move > globalwidth - 3){
				orient = 1;
				pos = move - (globalwidth - 2);
			}//elseif
			break;
		case L: if (move > 3 * globalwidth - 6){
				orient = 3;
				pos = move - (3 * globalwidth - 5);
			}//if
			else if (move > 2 * globalwidth - 4){
				orient = 2;
				pos = move - (2 * globalwidth - 3);
			}//if
			else if (move > globalwidth - 3){
				orient = 1;
				pos = move - (globalwidth - 2);
			}//if
			break;
	}//switch
}


//Function generates a vector of all possible moves for every piece that exists:
vector<Board> Board::list_all_moves(){

	vector<Board> ret;
	PieceName piece;
	int moves = 0;
	int pos = 0;
	int orient = 0;

	for (int i = 0; i < NUM_PIECES; i++){
		piece = PieceName(i);
		//Get all moves for current piece:
		moves = possible_moves(piece);

		//Get all Orientation and Position for current piece:
		for (int j = 0; j < moves; j++){
			//Copy current Board and have it play the current move:
			Board b(*this);
			//Play the jth move:
			get_info(piece, orient, pos, j);
			b.lastOrient = orient;
			b.lastPos = pos;
			b.letItFall(piece, orient, pos);
			//Push it in the vector of all possible moves:
			ret.push_back(b);
		}
	}
	return ret;
}


//Function generates a vector of all possible moves for the given piece:
vector<Board> Board::list_all_moves_with(PieceName piece){

	vector<Board> ret;
	int moves = 0;
	int pos = 0;
	int orient = 0;

	//Get the number of possible moves for the given piece:
	moves = possible_moves(piece);
	//Play all moves 1 by 1 and add to vector:
	for (int j = 0; j < moves; j++){
		//Copy the current board:
		Board b(*this);
		//Play the jth move:
		get_info(piece, orient, pos, j);
		b.lastOrient = orient;
		b.lastPos = pos;
		b.letItFall(piece, orient, pos);
		//Push it in the vector of all possible moves:
		ret.push_back(b);
	}
	return ret;
}


//Function generates a vector of all possible moves for the given pieces:
//Includes the piece in Hold-function if present.
vector<Board> Board::list_all_moves_with_pieces(PieceName piece, PieceName hold){

	vector<Board> ret;
	int moves = 0;
	int hold_moves = 0;
	int pos = 0;
	int orient = 0;
	int number;
	int j;

	//Get the number of possible moves for the given piece:
	moves = possible_moves(piece);
	hold_moves = possible_moves(hold);
	//For Boolean Array: (Unused. Maybe future work.)
	//number = moves + hold_moves;
	
	//Play all moves 1 by 1 and add to vector:
	for (j = 0; j < moves; j++){
		//Copy the current board:
		Board b(*this);
		//Play the jth move:
		get_info(piece, orient, pos, j);
		b.lastOrient = orient;
		b.lastPos = pos;
		b.letItFall(piece, orient, pos);
		wissel_array[j] = false;
		//Push it in the vector of all possible moves:
		ret.push_back(b);
	}
	
	for (int k = 0; k < hold_moves; k++){
    		j++;
		//Copy the current board:
		Board b(*this);
		//Play the jth move:
		get_info(hold, orient, pos, k);
		b.lastOrient = orient;
		b.lastPos = pos;
		b.letItFall(hold, orient, pos);
		wissel_array[j] = true;
		//Push it in the vector of all possible moves:
		ret.push_back(b);
	}
	return ret;
}


//Function to play a game of Tetris using random moves:
void Board::randomMoves(){
	PieceName piece;
	int pos = 0;
	int orient = 0;
	int i = 0;
	int cleared = 0;

	//Keep playing moves while game is not over:
	while (GameOver(1, cleared) == false){
		//cout << "---------------------------------------------" << endl;
		//cout << "Turn #" << i << endl;
		//cout << "Random Piece: " << endl;
		
		//Initialize random piece:
		piece = PieceName(rand() % NUM_PIECES);
		int move = rand() % possible_moves(piece);
		get_info(piece, orient, pos, move);
		//Print the piece that was randomized with its orientation and position:
		//printPiece(piece, orient, pos);
		letItFall(piece, orient, pos);
		//Check if any rows need to be cleared.:
		cleared = clearLines();
		//cout << "Random Result: " << endl;
		//print();
		score_p1 += cleared;
		//cout << cleared << " rows were cleared" << endl;
		//cout << "score: " << score_p1 << endl;
		i++;
	}
	//cout << endl;
	//cout << "**********" << endl;
	//cout << "Game Over!" << endl;
	//cout << "**********" << endl;
}

//Function to play a game of Tetris using AlphaBeta approach 1-player:
//Zorg dat Mode en Hold het doen voor Singleplayer
void Board::alphaBetaPlay(int depth, int modus, int holdings){
	PieceName piece;
	int pos = 0;
	int orient = 0;
	int i = 0;
	int cleared = 0;

	//Keep playing moves while game is not over:
	while (GameOver(1, cleared) == false){
		cout << "---------------------------------------------" << endl;
		cout << "Turn #" << i << endl;
		cout << "Random Piece: " << endl;
		piece = PieceName(rand() % NUM_PIECES);
		Board result = ABMinMax(*this, depth, piece, temp_hold, wissel, modus, holdings);
		*this = result;
		//Print the piece that with its Orientation and Position came out to be best:
		printPiece(piece, lastOrient, lastPos);
		//Check if any rows need to be cleared.:
		cleared = clearLines();
		cout << "Optimized Result: " << endl;
		print();
		score_p1 += cleared;
		cout << cleared << " rows were cleared" << endl;
		cout << "score: " << score_p1 << endl;
		i++;
	}
	cout << endl;
	cout << "**********" << endl;
	cout << "Game Over!" << endl;
	cout << "**********" << endl;
}

//Function to play a game of Tetris using AlphaBeta approach 2-player:
//Zorg dat Mode en Hold het doen 
void Board::alphaBetaPlay_two(int depth, int modus, int holdings){
	PieceName piece;
	//Laten we beginnen met een random gevulde Hold.
	temp_hold = PieceName(rand() % NUM_PIECES);
	wissel = 0;
	int pos = 0;
	int orient = 0;
	int i = 0;
	int cleared = 0;
	int last_turn;
  //Debugging purposes:
  //cout << "SWITCH: " << wissel << endl;
	// keep playing moves while game is not over
	while (GameOver(1, cleared) == false){
		Board result;

		cout << "---------------------------------------------" << endl;
		cout << "Turn #" << i << ", Player #" << turn + 1 <<  endl;
		cout << "Tetromino in HOLD: " << temp_hold << endl;
		cout << "Random Piece: " << endl;
		piece = PieceName(rand() % NUM_PIECES);
		// check which player's move it is and evaluate accordingly
		if (turn == P1)
		  //Je moet in deze functies de Hold uitproberen.
			result = ABMinMax(*this, depth, piece, temp_hold, wissel, modus, holdings);
		else
			result = ABMinMax_two(*this, depth, piece, temp_hold, wissel, modus, holdings);
			
		//Checks if a Tetromino in the Hold is to be switched with the current piece
		cout << "WISSEL!!!!: " << wissel << endl;
		*this = result;
		/*If 'wissel' is > 0 then a Hold move evaluated better and thus needs to be
    switched with the current piece:*/
		if(wissel > 0){ //Zo ja? Wissel.
		  //Debugging purposes:
			cout << "HOLD TETROMINO IS GEBRUIKT!" << endl;
			printPiece(temp_hold, lastOrient, lastPos);	
			printPiece(piece, lastOrient, lastPos);	
			temp_hold = piece;
		}
		else{
			// print the piece that with its orientation and position came out to be best
			printPiece(piece, lastOrient, lastPos);
		}

		// check if any rows need to be cleared.
		cleared = clearLines();
		cout << "Optimized Result: " << endl;
		print();
		if (turn == P1)
			score_p1 += cleared;
		else
			score_p2 += cleared;
		cout << cleared << " rows were cleared" << endl;
		cout << "score P1: " << score_p1 << endl;
		cout << "score P2: " << score_p2 << endl;
		last_turn = turn + 1;
		turn = (turn + 1) % 2;
		i++;
		for(int i = 0; i < n; i++){
      wissel_array[i] = false;
    }
		wissel = 0;
		//system("pause");
	}
	cout << endl;
	cout << "**********" << endl;
	cout << "Game Over!" << endl;
	cout << "**********" << endl;
	cout << endl;
	//Verander als je de parameters voor evaluate en gameover niet meer handmatig hoeft te doen.
	//Je kan dat (turn + 1) % 2 wel mooier doen.
	cout << "Last turn was Player #: " << last_turn << endl;
	
}

// function to play a game  of tetris using alphabeta approach 1-player
void Board::play_two(int p1, int p2, int modus, int holdings)
{
	PieceName piece;
	//Laten we beginnen met een random gevulde Hold.
	temp_hold = PieceName(rand() % NUM_PIECES);
	wissel = 0;
	int win = 0;
	int pos = 0;
	int orient = 0;
	int i = 0;
	int cleared = 0;
	int last_turn;
	int depth = 2;
	int move = 0;
	int best_move = 0;
	int max_points = 0;
	int points = 0;
	int moves = 0;
	int hold_moves = 0;
	int max_piececount = -1;
	int sum = 0;
	int temp_orient;
	int temp_pos;
	int temp_morient;
	int temp_mpos;
	int low_top_row = rows - 3;
	int nr;
	int emp;
	int random_choice = 0;
	bool therow[MAX_width];
	//Debugging purposes:
	//cout << "SWITCH: " << wissel << endl;
	// keep playing moves while game is not over
	//cout << "LOW TOP ROW!!!:" << low_top_row << endl;
	while (GameOver(modus, cleared) == false)
	{
		Board result;
		Board temp;
		//cout << "---------------------------------------------" << endl;
		//cout << "Turn #" << i << ", Player #" << turn + 1 << endl;
		//cout << "Tetromino in HOLD: " << temp_hold << endl;
		//cout << "Random Piece: " << endl;
		piece = PieceName(rand() % NUM_PIECES);
		// check which player's move it is and evaluate accordingly
		if (turn == P1)
		{
			switch (p1)
			{
      /*Picks the hold or current tetromino randomly and then picks the lowest
      in top row height difference*/
			case RANDOM:
				low_top_row = rows;
				moves = possible_moves(piece);
				if(holdings > 0){
					hold_moves = possible_moves(temp_hold);
				}
				random_choice = rand() % 2;
  			if(random_choice == 0){
  				for(int k = 0; k < moves; k++){
  					move = rand() % moves;
  					result = *this;
  					result.get_info(piece, orient, pos, move);
  					// print the piece that was randomized with its orientation and position
  					//result.printPiece(piece, orient, pos);
  					result.letItFall(piece, orient, pos);
  					result.clearLines();
    				check_top_row(therow,nr,emp);
  					if(nr < low_top_row){
  						low_top_row = nr;
  						get_info(piece, temp_morient, temp_mpos, move);
  					}//if
  				}//for
        }//if
        else if(random_choice != 0 && holdings > 0){
  				for(int p = 0; p < hold_moves; p++){
  					move = rand() % hold_moves;
  					result = *this;
  					result.get_info(temp_hold, orient, pos, move);
  					// print the piece that was randomized with its orientation and position
  					//result.printPiece(temp_hold, orient, pos);
  					result.letItFall(temp_hold, orient, pos);
  					result.clearLines();
    				check_top_row(therow,nr,emp);
  					if(nr < low_top_row){
  						low_top_row = nr;
  						get_info(temp_hold, temp_orient, temp_pos, move);
  					}//if
  				}//for
  				wissel++;
        }//else
        if(wissel > 0 && holdings > 0){
				  letItFall(temp_hold, temp_orient, temp_pos);
        }
        else{
          letItFall(piece, temp_morient, temp_mpos);
        }
				result = *this;
				break;
			case MONTE_CARLO:
				max_piececount = -1;
				moves = possible_moves(piece);
				if(holdings > 0){
					hold_moves = possible_moves(temp_hold);
				}
				for (int k = 0; k < moves; k++)
				{
					sum = 0;
					result = *this;
					result.get_info(piece, orient, pos, k);
					result.letItFall(piece, orient, pos);
					result.clearLines();
					for (int i = 0; i < MONTECARLOGAMES; i++)
					{
						temp = result;
						temp.randomMoves();
						sum += temp.pieceCount;
					}
					if (sum > max_piececount)
					{
						get_info(piece, temp_morient, temp_mpos, k);
						max_piececount = sum;
					}
				}
				if(holdings > 0){
					for (int p = 0; p < hold_moves; p++)
					{
						sum = 0;
						result = *this;
						result.get_info(temp_hold, orient, pos, p);
						result.letItFall(temp_hold, orient, pos);
						result.clearLines();
						for (int q = 0; q < MONTECARLOGAMES; q++)
						{
							temp = result;
							temp.randomMoves();
							sum += temp.pieceCount;
						}
						if (sum > max_piececount)
						{
							get_info(temp_hold, temp_orient, temp_pos, p);
							max_piececount = sum;
							wissel++;
						}
					}
				}
				if(wissel > 0 && holdings > 0){
				  letItFall(temp_hold, temp_orient, temp_pos);
        }
        else{
          letItFall(piece, temp_morient, temp_mpos);
        }
				result = *this;
				break;
			case ALPHABETA:
				//Je moet in deze functies de Hold uitproberen.
				result = ABMinMax(*this, depth, piece, temp_hold, wissel, modus, holdings);
				break;
			}
		}
		else
		{	
			switch (p2)
			{
			case RANDOM:
				low_top_row = rows;
				moves = possible_moves(piece);
				if(holdings > 0){
					hold_moves = possible_moves(temp_hold);
				}
				random_choice = rand() % 2;
  			if(random_choice == 0){
  				for(int k = 0; k < moves; k++){
  					move = rand() % moves;
  					result = *this;
  					result.get_info(piece, orient, pos, move);
  					// print the piece that was randomized with its orientation and position
  					//result.printPiece(piece, orient, pos);
  					result.letItFall(piece, orient, pos);
  					result.clearLines();
    				check_top_row(therow,nr,emp);
  					if(nr < low_top_row){
  						low_top_row = nr;
  						get_info(piece, temp_morient, temp_mpos, move);
  					}//if
  				}//for
        }//if
        else if(random_choice != 0 && holdings > 0){
  				for(int p = 0; p < hold_moves; p++){
  					move = rand() % hold_moves;
  					result = *this;
  					result.get_info(temp_hold, orient, pos, move);
  					// print the piece that was randomized with its orientation and position
  					//result.printPiece(temp_hold, orient, pos);
  					result.letItFall(temp_hold, orient, pos);
  					result.clearLines();
    				check_top_row(therow,nr,emp);
  					if(nr < low_top_row){
  						low_top_row = nr;
  						get_info(temp_hold, temp_orient, temp_pos, move);
  					}//if
  				}//for
  				wissel++;
        }//else
        if(wissel > 0 && holdings > 0){
				  letItFall(temp_hold, temp_orient, temp_pos);
        }
        else{
          letItFall(piece, temp_morient, temp_mpos);
        }
				result = *this;
				break;
			case MONTE_CARLO:
				max_piececount = -1;
				moves = possible_moves(piece);
				if(holdings > 0){
					hold_moves = possible_moves(temp_hold);
				}
				for (int k = 0; k < moves; k++)
				{
					sum = 0;
					result = *this;
					result.get_info(piece, orient, pos, k);
					result.letItFall(piece, orient, pos);
					result.clearLines();
					for (int i = 0; i < 100; i++)
					{
						temp = result;
						temp.randomMoves();
						sum += temp.pieceCount;
					}
					if (sum > max_piececount)
					{
						get_info(piece, temp_morient, temp_mpos, k);
						max_piececount = sum;
					}
				}
				if(holdings > 0){
					for (int p = 0; p < hold_moves; p++)
					{
						sum = 0;
						result = *this;
						result.get_info(temp_hold, orient, pos, p);
						result.letItFall(temp_hold, orient, pos);
						result.clearLines();
						for (int q = 0; q < 100; q++)
						{
							temp = result;
							temp.randomMoves();
							sum += temp.pieceCount;
						}
						if (sum > max_piececount)
						{
							get_info(temp_hold, temp_orient, temp_pos, p);
							max_piececount = sum;
							wissel++;
						}
					}
				}
				if(wissel > 0 && holdings > 0){
				  letItFall(temp_hold, temp_orient, temp_pos);
        }
        else{
          letItFall(piece, temp_morient, temp_mpos);
        }
				result = *this;
				break;
			case ALPHABETA:
				//Je moet in deze functies de Hold uitproberen.
				result = ABMinMax_two(*this, depth, piece, temp_hold, wissel, modus, holdings); 
				break;
			}

		}

		//Checks if a Tetromino in the Hold is to be switched with the current piece
		//cout << "WISSEL!!!!: " << wissel << endl;
		*this = result;
		/*If 'wissel' is > 0 then a Hold move evaluated better and thus needs to be
		switched with the current piece:*/
		if (wissel > 0 && holdings > 0){ //Zo ja? Wissel.
			//Debugging purposes:
			//cout << "HOLD TETROMINO IS GEBRUIKT!" << endl;
			//printPiece(temp_hold, lastOrient, lastPos);
			//printPiece(piece, lastOrient, lastPos);
			temp_hold = piece;
		}
		else{
			// print the piece that with its orientation and position came out to be best
			//printPiece(piece, lastOrient, lastPos);
		}

		// check if any rows need to be cleared.
		cleared = clearLines();
		//cout << "Optimized Result: " << endl;
		//print();
		if (turn == P1)
			score_p1 += cleared;
		else
			score_p2 += cleared;
		//cout << cleared << " rows were cleared" << endl;
		//cout << "score P1: " << score_p1 << endl;
		//cout << "score P2: " << score_p2 << endl;
		last_turn = turn + 1;
		turn = (turn + 1) % 2;
		i++;
		if(holdings > 0){
			for (int i = 0; i < n; i++){
				wissel_array[i] = false;
			}
			wissel = 0;
		}
		//system("pause");
	}
	//cout << "PLAYING MODE = " << modus << endl;
	//cout << "HOLD = " << holdings << endl;
	cout << score_p1 << " " << score_p2 << " ";
	//cout << endl;
	/*cout << "**********" << endl;
	cout << "Game Over!" << endl;
	cout << "**********" << endl;
	cout << endl;*/
	//Verander als je de parameters voor evaluate en gameover niet meer handmatig hoeft te doen.
	//Je kan dat (turn + 1) % 2 wel mooier doen.
	
	if(modus == 3){
		//cout << "Last turn was Player #: " << last_turn << endl;
		win = last_turn;
		
		if(win == 1){
			cout << "2" << endl;
		}
		else if(win == 2){
			cout << "1" << endl;
		}
		else{
			cout << "0" << endl;
		}
	}
	else{
		if(score_p1 > score_p2){
			win = 1;
		}
		else if(score_p2 > score_p1){
			win = 2;
		}

		if(win == 1){
			cout << "1" << endl;
		}
		else if(win == 2){
			cout << "2" << endl;
		}
		else{
			cout << "0" << endl;
		}
	}
}

// function checks if the field's top has been reached indicating game over
bool Board::GameOver(int option, int cleared){
  int i;
  int j;
    
  for(i = 0; i < rows; i++){
  //if any column of last row is filled
    for(j = 0; j < cols; j++){
    	if (field[rows - 3][j]){
    		return true;
      }//if
      if(option == 2 && cleared > 0){
        return true;
      }//if
    }//for
    return false;
  }
}

// function to evaluate the current board, used in alpha beta analysis
int Board::evaluate_board(color c, int option)  //Hier kan nog piece bij
{//Temporarily for clearing most lines
	int score = 0;
	int full_row = 0;
	int blocks;
	int empties;
	int nr;
  int emp;
  int bouwwerk_blocks = 0; //kijk even of je dit wilt gebruiken
  bool therow[MAX_width];
  check_top_row(therow,nr,emp);
  
  //TODO: Speel ook met dat je de eerstvolgende zet al weet
  //TODO: Speel ook met HOLDS - Array ofzo waar je 1 entry in dumpt en kan veranderen.
  
  //OPTION: Whoever clears the Most Lines wins:
  if(option == 1){
    if(c == Black){// Black gets score for having filled fields
    	//score based on fill base rows first, give more credit to base rows
    	for (int i = 0; i < rows; i++)
    	{  
        blocks = 0;
				empties = 0;
    		for (int j = 0; j < cols; j++)
    		{
    			if (field[i][j])
    			{
    				score += (rows - i);
    				blocks++;
						if(i == nr){
							score += 50;
						}
						if(i == nr-1){
							score += 75;
						}
    			}
					else{
						empties++;
					}
        }
				//Misschien ook beter voor optie 3
				score += (blocks*25);
				//if(i == nr && (blocks == cols -1)){
					
				if(empties >= 2){
					if(score >= 800){
            score = score - (empties*15);
          }
          else{
            score = score - (empties*10);
          }
				}
        //DONE: de zojuist gecheckte rij clearen is gut getan!
        if(blocks == cols){
          //cout << "A LINE WOOOOO!!" << endl;
          score += (cols*200);
          full_row++;
        }
        //gaan voor een egaler verdeeld veld
        else if((blocks < cols) && (blocks >= 0)){
          score += (blocks * 25);
          if(blocks == (cols - 1)){//Je wilt geen lijnen weggeven
            if(score >= 800){
              score = (score/3);
            }
            else{
              score = (score/6);
            }
          }
					if(blocks == (cols - 2)){
						if(score >= 800){
              score = (score/3);
            }
            else{
              score = (score/2);
            }
					}
        }
      }
      //Als de bovenste rij waar blokken in staan dicht bij het einde van het bord is, dan is dat slecht.
      //Je wilt namelijk zo lang mogelijk doorspelen om de kans te hebben om zoveel mogelijk lijnen te clearen.
      if(nr < (rows - 3) && nr > 0){ //hier moet je even over nadenken, want dit gaat mis.
        //cout << "TOP ROW IN BOUNDS: " << nr << endl;
        //cout << "ROWS: " << rows << endl;
        score = (score/nr);
      }
      else{ //GAMEOVER!!!!
        //cout << "GAMEOVER!!!" << endl;
        if(full_row > 0){
          score = (score/200) + (full_row * 100);
        }
        else{
          score = (score/200); //zorgt ervoor dat als je nog een lijn kan maken voordat het gameover is dat je dat ook doet.
        }
      }
      //meer rijen clearen is meer beter
      if(full_row > 1){
        score += (full_row * 100);
      }
    }
  			//Meer punten voor meer blokken op een rij:
          
  			
  			//Voor elke clear een stuk of (100 * x) + (100 * x + 1), enz. punten erbij.
  			
  			//
  }//if - LINES == 1
  
  //OPTION: First to have a line wins!
  else if(option == 2){
    if(c == Black){// Black gets score for having filled fields
    	//score based on fill base rows first, give more credit to base rows
    	for (int i = 0; i < rows; i++){  
        blocks = 0;
				empties = 0;
    		for (int j = 0; j < cols; j++){
    			if (field[i][j])
    			{
    				score += (rows - i);
    				blocks++;
						if(i == nr){
							score += 50;
						}
						if(i == nr-1){
							score += 75;
						}
    			}
					else{
						empties++;
					}
        }
				//Misschien ook beter voor optie 3
				score += (blocks*25);

				if(empties >= 2){
					if(score >= 800){
            score = score - (empties*15);
          }
          else{
            score = score - (empties*10);
          }
				}
        //DONE: de zojuist gecheckte rij clearen is gut getan!
        if(blocks == cols){
          //cout << "A LINE WOOOOO!!" << endl;
          score += (500 * cols);
          full_row++;
        }
        //gaan voor een egaler verdeeld veld
        else if((blocks < cols) && (blocks >= 0)){
          score += (blocks * 25);
          if(blocks == (cols - 1)){//Je wilt geen lijnen weggeven
            score = (score/4);
            /*if(score >= 800){
              score = (score/4);
            }
            else{
              score = (score/8);
            }*/
          }//if
          else if(blocks == (cols - 2)){
            score = (score/2);
            /*if(score >= 800){
              score = (score/2);
            }
            else{
              score = (score/8);
            }*/
          }//elseif
        }//else if
      }//for
      //Als de bovenste rij waar blokken in staan dicht bij het einde van het bord is, dan is dat slecht.
      //Je wilt namelijk zo lang mogelijk doorspelen om de kans te hebben om zoveel mogelijk lijnen te clearen.
      if(nr < (rows - 3) && nr > 0){ //hier moet je even over nadenken, want dit gaat mis.
        //cout << "TOP ROW IN BOUNDS: " << nr << endl;
        //cout << "ROWS: " << rows << endl;
        score = (score/nr);
      }
      else{ //GAMEOVER!!!!
        //cout << "GAMEOVER!!!" << endl;
        if(full_row > 0){
          score = (score/200) + (full_row * 100);
        }
        else{
          score = (score/200); //zorgt ervoor dat als je nog een lijn kan maken voordat het gameover is dat je dat ook doet.
        }
      }
      //meer rijen clearen is meer beter
      if(full_row > 1){
        score += (full_row * 100);
      }
    }
  }
  
  //OPTION: First one to get out of bounds loses
  else if(option == 3){
    if(c == Black){// Black gets score for having filled fields
    	//score based on fill base rows first, give more credit to base rows
    	for (int i = 0; i < rows; i++){  
        blocks = 0;
    		for (int j = 0; j < cols; j++){
    			if (field[i][j] == false){
    				score += (rows - i);
    				empties++; //This time counting empty blocks on the field
    			}
					
        }
        //Clearing a line is good to reduce the height.
        if(empties == 0){
          //cout << "A LINE WOOOOO!!" << endl;
          score += 2000;
          full_row++;
        }
        //gaan voor een egaler verdeeld veld
        else if((empties <= cols) && (empties > 0)){
          score += (empties * 25);
        }//else if
				
				if(i == nr && nr >= (rows-6) && blocks >= (cols/2)){
					score += (blocks*10);
				}
				 
      }//for
      //Als de bovenste rij waar blokken in staan dicht bij het einde van het bord is, dan is dat slecht.
      //Je wilt namelijk zo lang mogelijk doorspelen om de kans te hebben om zoveel mogelijk lijnen te clearen.
      if(nr < (rows - 3) && nr >= 0){ //hier moet je even over nadenken, want dit gaat mis.
        //cout << "TOP ROW IN BOUNDS: " << nr << endl;
        //cout << "ROWS: " << rows << endl;
        //The higher the top row, the less points will be appointed.
        score = score + (((rows - 3) - nr) * 100); 
      }
      else{ //GAMEOVER!!!!
        //cout << "GAMEOVER!!!" << endl;
        if(full_row > 0){
          score = (score/300) + (full_row * 100);
        }
        else{
          score = (score/300); //zorgt ervoor dat als je nog een lijn kan maken voordat het gameover is dat je dat ook doet.
        }
      }
      //meer rijen clearen is meer beter
      if(full_row > 1){
        score += (full_row * 100);
      }
    }//if
  }//Option 3
  
  
	if(c == White){// White gets score for having unfilled fields
    //score based on fill base rows first, give more credit to base rows
  	for (int i = 0; i < rows; i++)
  	{
      //full_row = 0;
      //blocks = 0;
  		for (int j = 0; j < cols; j++)
  		{
				if (!field[i][j])
				{
					score += (rows - i);
				}//if
			}//for
		}//for
  }//elseif
  //cout << "SCORE BLACK:" << score << endl;
  //cout << "SCORE WHITE:" << score << endl;
	return score;
}

// function to print piece information with its orientation and position
void Board::printPiece(PieceName piece, int orientation, int position)
{
	switch (piece) {
	case O: cout << "O-Tetromino "; break;
	case J: cout << "J-Tetromino "; break;
	case L: cout << "L-Tetromino "; break;
	case Z: cout << "Z-Tetromino "; break;
	case S: cout << "S-Tetromino "; break;
	case I: cout << "I-Tetromino "; break;
	case T: cout << "T-Tetromino "; break;
	}
	cout << orientation << " " << position << endl;
}

///////////////////////////////// ALPHA BETA CODE //////////////////////////////

// base caller function, calls max move recursive function
Board ABMinMax(Board board, short int depth_limit, PieceName p, PieceName hold, int &wissel, int modus, int holdings) {
	return ABMaxMove(board, depth_limit, p , 1, 0, 0, hold, wissel, modus, holdings);
}

// base caller function, calls min move recursive function
Board ABMinMax_two(Board board, short int depth_limit, PieceName p, PieceName hold, int &wissel, int modus, int holdings) {
	return ABMinMove(board, depth_limit, p, 1, 0, 0, hold, wissel, modus, holdings);
}

Board ABMaxMove(Board board, short int depth_limit, PieceName p, short int depth, int a, int b, PieceName hold, int &wissel, int modus, int holdings) {
	vector<Board> moves;
	Board best_move;
	Board best_real_move;
	Board move;
	bool isNotSet = true;
	int temp_wissel = 0;

	// alpha being a, beta being b
	int alpha = a, beta = b;

	// simply return if we have evaluated till given depth
	if (depth >= depth_limit) {//if depth limit is reached
		return board;
	}
	else {
		// if its first depth then we will find positions for the given piece only
		if (depth == 1){
      //For debugging purposes:
			//cout << "DEPTH IS HERE!!!!!!" << endl;

      //Generate all possible moves for piece p and piece Hold:
			if(holdings > 0){
      	moves = board.list_all_moves_with_pieces(p, hold);
			}
			else{
				moves = board.list_all_moves_with(p);
			}
		}
		// if not 1 then we will count all possible moves for all pieces since we dont know which would come next
		else{
			moves = board.list_all_moves();
    }
		for (vector<Board>::iterator it = moves.begin(); it != moves.end(); it++) {
			move = ABMinMove(*it, depth_limit, p, depth + 1, alpha, beta, hold, wissel, modus, holdings);

			//cout << "HOLD_MOVE" << endl;
			//move.print();
			//cout << "HOLD_MOVE" << endl;

      // remove comment for debugging purposes
			//move.print();
      
			// rate the move and update best move
			if (isNotSet || move.evaluate_board(Black, modus)
				> best_move.evaluate_board(Black, modus)) {
        //For debugging purposes:
        //cout << "I AM HERE!!!" << endl;
				best_move = move;
				best_real_move = *it;
				alpha = move.evaluate_board(Black, modus);
				
			// remove comment for debugging purposes
        //cout << "Potato" << endl;
				//move.print();
				//cout << "ALPHA:" << alpha << endl;
				isNotSet = false;
				/*If the Hold function holds a Tetromino that evaluates to being a 
        better move:*/
				if(depth == 1 && wissel_array[temp_wissel] == true && holdings > 0){
          //cout << "HOLY SHIT!!!" << endl;
          wissel++;
        }
			}
			if(holdings > 0){
      	temp_wissel++;
			}
			//Function never gets here:
			if (beta > alpha){
				//cout << "ALPHA < BETA" << endl;
				return best_real_move;
			}
		}
		return best_real_move;
	}
}

Board ABMinMove(Board board, short int depth_limit, PieceName p, short int depth, int a, int b, PieceName hold, int &wissel, int modus, int holdings) {
	vector<Board> moves;
	Board best_move;
	Board best_real_move;
	Board move;
	bool isNotSet = true;
	int temp_wissel = 0;

	// alpha being a, beta being b
	int alpha = a, beta = b;

	if (depth >= depth_limit) {//if depth limit is reached
		return board;
	}
	else {
		// if its first depth then we will find positions for the given piece only
		if (depth == 1){
			//cout << "DEPTH = 1" << endl;
      //Generate all possible moves for piece p and piece Hold:
      if(holdings > 0){
      	moves = board.list_all_moves_with_pieces(p, hold);
			}
			else{
				moves = board.list_all_moves_with(p);
			}
		}
		// if not 1 then we will count all possible moves for all pieces since we dont know which would come next
		else
			moves = board.list_all_moves();

		for (vector<Board>::iterator it = moves.begin(); it != moves.end(); it++) {
			move = ABMaxMove(*it, depth_limit, p, depth + 1, alpha, beta, hold, wissel, modus, holdings);

			// rate the move and update best move
			if (isNotSet || move.evaluate_board(White, modus) < best_move.evaluate_board(White, modus)) {
				best_move = move;
				best_real_move = *it;
				beta = move.evaluate_board(White, modus);
				
			// remove comment for debugging purposes
        //cout << "Potato" << endl;
				//move.print();
				//cout << "BETA:" << beta << endl;
				
				isNotSet = false;
				if(depth == 1 && wissel_array[temp_wissel] == true && holdings > 0){
          wissel++;
        }
			}
			if(holdings > 0){
				temp_wissel++;
			}
			if (beta < alpha){
				return best_real_move;
      }
		}
		return best_real_move;
	}
}
