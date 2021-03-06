#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <bitset>
#include <map>
#include <string>

using namespace std;

class ChessBoard{
public:
	ChessBoard();
	void drawBoard();
	~ChessBoard();
	char getPiece(string square);
	void updateBoard();
	void updateGenPositions();



public:
	bitset<64> WK;
	bitset<64> WQ;
	bitset<64> WP;
	bitset<64> WR;
	bitset<64> WB;
	bitset<64> WN;
	bitset<64> BK;
	bitset<64> BQ;
	bitset<64> BP;
	bitset<64> BR;
	bitset<64> BB;
	bitset<64> BN;

	bitset<64> whitePieces;
	bitset<64> blackPieces;
	bitset<64> emptySquares;
	bitset<64> occupiedSquares;

	char gameBoard[64];
	map<string, int> notationMap;
	map<int, string> positionMap;

private:
	void setBoard();
	void generateNotationMap();

};

#endif