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
		PAWN_MOVES=(boardRep->WP<<7)&~fileA&(boardRep->BP<<8)&rank6&boardRep->emptySquares;
		for (int i=0;i<64;i++){
			if (PAWN_MOVES[i]){
				list+=boardRep->positionMap[i-7]+"x"+boardRep->positionMap[i]+"ep ";
			}
		}

		PAWN_MOVES=(boardRep->WP<<9)&~fileH&(boardRep->BP<<8)&rank6&boardRep->emptySquares;
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
	cout<<moveSet.possibleWP("e7-e5")<<endl;

}