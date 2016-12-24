#ifndef MOVES_H_
#define MOVES_H_

#include <bitset>

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
	public:
		Moves();

	protected:
};



#endif