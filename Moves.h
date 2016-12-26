#ifndef MOVES_H_
#define MOVES_H_

#include <bitset>
#include <string>
#include "ChessBoard.h"
#include <vector>
#include <cstdint>

using namespace std;

class Moves{
	public:
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
		vector<bitset<64> > ranks;
		vector<bitset<64> > files;
		vector<bitset<64> > diagnols;
		vector<bitset<64> > adiagnols;

	public:
		Moves();
		~Moves();
		string possibleMoves();
		string possibleWP(string lastMove);
		ChessBoard* boardRep;
		uint64_t HandVMoves(int arrayPos);
	private:







	protected:
};



#endif