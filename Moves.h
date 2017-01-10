#ifndef MOVES_H_
#define MOVES_H_

#include <bitset>
#include <string>
#include "ChessBoard.h"
#include <vector>
#include <cstdint>
#include <fstream>

using namespace std;


class Moves{
	private:
		bitset<64> rank1;
		bitset<64> rank2;
		bitset<64> rank3;
		bitset<64> rank4;
		bitset<64> rank5;
		bitset<64> rank6;
		bitset<64> rank7;
		bitset<64> rank8;
		bitset<64> fileH;
		bitset<64> fileG;
		bitset<64> fileF;
		bitset<64> fileE;
		bitset<64> fileD;
		bitset<64> fileC;
		bitset<64> fileB;
		bitset<64> fileA;
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

		vector<bitset<64> > ranks;
		vector<bitset<64> > files;
		static uint64_t diagnols[15];
		static uint64_t adiagnols[15];
		string possibleWP(char* gameboard, bitset<64> WP, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleWR(bitset<64> WR, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleWB(bitset<64> WB, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleWQ(bitset<64> WQ, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleWN(bitset<64> WN, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleWK(bitset<64> WK, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleBP(char* gameboard, bitset<64> BP, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleBR(bitset<64> BR, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleBB(bitset<64> BB, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleBQ(bitset<64> BQ, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleBN(bitset<64> BN, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleBK(bitset<64> BK, char* gameboard, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares);
		string possibleCW(bool CWK, bool CWQ);
		string possibleCB(bool CBK, bool CBQ);


		uint64_t unsafeForWhite();
		uint64_t unsafeForBlack();
		uint64_t HandVMoves(int arrayPos);
		uint64_t DandAntiMoves(int arrayPos);
		void updateGenPositions();
		
		void generateNotationMap();



	public:
		Moves();
		~Moves();
		vector<string> possibleWMoves(char* board);
		vector<string> possibleBMoves(char* board);
		bool WKCheck();
		bool BKCheck();
		void makeMove(string move);
		map<string, int> notationMap;
		map<int, string> positionMap;




		
	private:







	protected:
};



#endif