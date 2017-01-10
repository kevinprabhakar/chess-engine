#include "Moves.h"
#include "Search.h"
#include <float.h>
#include <random>
#include <iostream>

using namespace std;

Search::Search(){
	moveSet = new Moves();
}

Search::~Search(){
	delete moveSet;
}

int alpha=-32767;
int beta=32767;

double Search::negaMax(char* board, bool white, int depth, int a, int b){
	double moveScore;
	double bestMoveScore = -DBL_MAX;

	vector<string> moves;

	if (white){
		moves = moveSet->possibleWMoves(board);
	}else{
		moves = moveSet->possibleBMoves(board);
	}

	for (int i=0;i<(int)moves.size();i++){
		makeMove(board, moves[i]);
		char pieceTaken = board[moveSet->notationMap[moves[i].substr(3,2)]];

		if (depth==0){
			moveScore = staticEvaluate(board);
		}else{
			moveScore = negaMax(board, !white, depth-1,-b,-a)*-1;
		}
		undoMove(board, moves[i], pieceTaken, white);

		if (moveScore > bestMoveScore)bestMoveScore = moveScore;
		if (bestMoveScore > alpha) alpha=bestMoveScore;
		if (alpha>=beta) return alpha;


	}
	return bestMoveScore;
}

void Search::makeMove(char*& board, string move){
	int startIndex;
	int destinationIndex;
	string startSquare=(move).substr(0,2);
	string destinationSquare=(move).substr(3,2);

	if (move.size()==5){
		startIndex = moveSet->notationMap[move.substr(0,2)];
		destinationIndex = moveSet->notationMap[move.substr(3,2)];
		char startPiece = board[startIndex];

		board[startIndex]='*';
		board[destinationIndex]=startPiece;
	}
	if (move.size()==7){
		startIndex = moveSet->notationMap[move.substr(0,2)];
		destinationIndex = moveSet->notationMap[move.substr(3,2)];
		char endPiece = move[6];

		board[startIndex]='*';
		board[destinationIndex]=endPiece;
	}
}

void Search::undoMove(char*& board, string lastMove, char pieceTaken, bool white){
	if (lastMove.size()==5){
		int startIndex = moveSet->notationMap[lastMove.substr(0,2)];
		int destinationIndex = moveSet->notationMap[lastMove.substr(3,2)];
		char currentPiece = board[destinationIndex];

		board[destinationIndex]=pieceTaken;
		board[startIndex]=currentPiece;
	}
	if (lastMove.size()==7){
		int startIndex = moveSet->notationMap[lastMove.substr(0,2)];
		int destinationIndex = moveSet->notationMap[lastMove.substr(3,2)];

		board[destinationIndex]=pieceTaken;

		if (white){
			board[startIndex]='P';
		}else{
			board[startIndex]='p';
		}
	}
}

double Search::staticEvaluate(char* board){
	std::uniform_real_distribution<double> dist(0, 100);
	std::mt19937 rng;
	rng.seed(std::random_device{}()); 
	return dist(rng);
}

void Search::makeBestMove(char* board, bool white, int plyDepth){
	vector<string> moves;
	string bestMove;
	int moveScore;
	double bestMoveScore=-DBL_MAX;

	if (white){
		moves = moveSet->possibleWMoves(board);
	}else{
		moves = moveSet->possibleBMoves(board);
	}

	for (int i=0;i<(int)moves.size();i++){
		makeMove(board, moves[i]);
		char pieceTaken = board[moveSet->notationMap[moves[i].substr(3,2)]];

		moveScore=negaMax(board,white,plyDepth, alpha, beta);

		undoMove(board, moves[i], pieceTaken, white);

		if (moveScore > bestMoveScore){
			bestMoveScore = moveScore;
			bestMove = moves[i];
		}
	}

	cout<<"BEST MOVE: "<<bestMove<<endl;


}

int main(){
	Search temp;


	char gameboard[64];
	for (int i=0;i<64;i++){
		gameboard[i]='*';
	}
	
	
	gameboard[0]='R';
	gameboard[1]='N';
	gameboard[2]='B';
	gameboard[3]='Q';
	gameboard[4]='K';
	gameboard[5]='B';
	gameboard[6]='N';
	gameboard[7]='R';
	for (int i=8;i<16;i++){
		gameboard[i]='P';
	}


	for (int i=48;i<56;i++){
		gameboard[i]='p';
	}
	gameboard[56]='r';
	gameboard[57]='n';
	gameboard[58]='b';
	gameboard[59]='q';
	gameboard[60]='k';
	gameboard[61]='b';
	gameboard[62]='n';
	gameboard[63]='r';

	cout<<endl;
	for (int i=63;i>=0;i--){
		cout<<gameboard[i];
		if (i%8==0)cout<<endl;
	}
	cout<<endl;

	temp.makeBestMove(gameboard,true,1);
}