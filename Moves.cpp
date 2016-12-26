#include "Moves.h"
#include <iostream>

Moves::Moves(){
	rank1=255U;
	rank2=65280U;
	rank3=16711680U;
	rank4=4278190080U;
	rank5=1095216660480U;
	rank6=280375465082880U;
	rank7=71776119061217280U;
	rank8=18374686479671623680U;
	fileH=72340172838076673U;
	fileG=144680345676153346U;
	fileF=289360691352306692U;
	fileE=578721382704613384U;
	fileD=1157442765409226768U;
	fileC=2314885530818453536U;
	fileB=4629771061636907072U;
	fileA=9259542123273814144U;

	

	boardRep = new ChessBoard();
}


template <typename T, T m, int k> static inline T swapbits(T p) {
	T q = ((p>>k)^p)&m;
	return p^q^(q<<k);
}

uint64_t reverseBits (uint64_t n)
{
  static const uint64_t m0 = 0x5555555555555555LLU;
  static const uint64_t m1 = 0x0300c0303030c303LLU;
  static const uint64_t m2 = 0x00c0300c03f0003fLLU;
  static const uint64_t m3 = 0x00000ffc00003fffLLU;
  n = ((n>>1)&m0) | (n&m0)<<1;
  n = swapbits<uint64_t, m1, 4>(n);
  n = swapbits<uint64_t, m2, 8>(n);
  n = swapbits<uint64_t, m3, 20>(n);
  n = (n >> 34) | (n << 30);
  return n;
}

Moves::~Moves(){
	delete boardRep;
}


string Moves::possibleWP(string lastMove){
	/********************************
	Variable Definition
	********************************/
	string startSquare=lastMove.substr(0,2);
	string destinationSquare=lastMove.substr(3,2);

	string list = "";
	bitset<64> PAWN_MOVES;

	/*******************************
	EN PASSANT CODE
	********************************/
	if ((boardRep->getPiece(destinationSquare)=='p')&&(startSquare[0]=destinationSquare[0])&&(destinationSquare[1]==startSquare[1]-2)){
		int destinationIndex=boardRep->notationMap[destinationSquare];
		bitset<64> EPBlack;
		EPBlack.set(destinationIndex);
		PAWN_MOVES=(boardRep->WP<<7)&~fileA&(EPBlack<<8)&rank6&boardRep->emptySquares;

		for (int i=0;i<64;i++){
			if (PAWN_MOVES[i]){
				list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+"ep ";
			}
		}

		PAWN_MOVES=(boardRep->WP<<9)&~fileH&(EPBlack<<8)&rank6&boardRep->emptySquares;
		for (int i=0;i<64;i++){
			if (PAWN_MOVES[i]){
				list+=boardRep->positionMap[i-9]+"x"+boardRep->positionMap[i]+"ep ";
			}
		}

	}

	/****************************************
	ALL OTHER PAWN FUNCTION CODE
	*****************************************/
	PAWN_MOVES=(boardRep->WP<<9)&boardRep->blackPieces&~rank8&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-9]+"x"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->WP<<7)&boardRep->blackPieces&~rank8&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->WP<<8)&boardRep->emptySquares&~rank8;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-8]+"-"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->WP<<16)&boardRep->emptySquares&(boardRep->emptySquares<<8)&~rank8&rank4;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-16]+"-"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->WP<<7)&boardRep->blackPieces&rank8&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+"=Q ";
			list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+"=R ";
			list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+"=N ";
			list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+"=B ";
		}
	}

	PAWN_MOVES=(boardRep->WP<<9)&boardRep->blackPieces&rank8&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-9]+"x"+boardRep->positionMap[i]+"=Q ";
			list+=boardRep->positionMap[i-9]+"x"+boardRep->positionMap[i]+"=R ";
			list+=boardRep->positionMap[i-9]+"x"+boardRep->positionMap[i]+"=N ";
			list+=boardRep->positionMap[i-9]+"x"+boardRep->positionMap[i]+"=B ";
		}
	}

	PAWN_MOVES=(boardRep->WP<<8)&boardRep->emptySquares&rank8;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i-8]+"-"+boardRep->positionMap[i]+"=Q ";
			list+=boardRep->positionMap[i-8]+"-"+boardRep->positionMap[i]+"=R ";
			list+=boardRep->positionMap[i-8]+"-"+boardRep->positionMap[i]+"=N ";
			list+=boardRep->positionMap[i-8]+"-"+boardRep->positionMap[i]+"=B ";
		}
	}

	return list;
}


int main(){
	Moves moveSet;
	bitset<64> n(6327421892U);
	cout<<n<<endl;
	bitset<64> reverser(reverseBits((uint64_t)n.to_ulong()));			//THIS IS THE SYNTAX FOR REVERSING 64 BIT BITSET
	cout<<reverser<<endl;
}