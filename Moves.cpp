#include "Moves.h"
#include <iostream>
#include <math.h>
#include <cassert>
#include <sstream>

//index64 used in bitScan Reverse to find MSB
const int index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};

uint64_t Moves::diagnols[] = {1U,258U,66052U,16909320U,4328785936U,1108169199648U,283691315109952U,72624976668147840U,145249953336295424U,290499906672525312U,580999813328273408U,1161999622361579520U,2323998145211531264U,4647714815446351872U,9223372036854775808U};
uint64_t Moves::adiagnols[] = {128U,32832U,8405024U,2151686160U,550831656968U,141012904183812U,36099303471055874U,9241421688590303745U,4620710844295151872U,2310355422147575808U,1155177711073755136U,577588855528488960U,288794425616760832U,144396663052566528U,72057594037927936U};


int bitScanForward(uint64_t bb) {
	/********************************************
	USED TO FIND Least Significant Bit of bb
	*********************************************/
   unsigned int lsb;
   assert (bb != 0);
   bb &= -bb; // LS1B-Isolation
   lsb = (unsigned)bb
       | (unsigned)(bb>>32);
   return (((((((((((unsigned)(bb>>32) !=0)  * 2)
                 + ((lsb & 0xffff0000) !=0)) * 2)
                 + ((lsb & 0xff00ff00) !=0)) * 2)
                 + ((lsb & 0xf0f0f0f0) !=0)) * 2)
                 + ((lsb & 0xcccccccc) !=0)) * 2)
                 + ((lsb & 0xaaaaaaaa) !=0);
}

int bitScanReverse(uint64_t bb) {
   const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
   assert (bb != 0);
   bb |= bb >> 1; 
   bb |= bb >> 2;
   bb |= bb >> 4;
   bb |= bb >> 8;
   bb |= bb >> 16;
   bb |= bb >> 32;
   return index64[(bb * debruijn64) >> 58];
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

	ranks.push_back(rank1);				//USE floor(ARRAYPOS/8)
	ranks.push_back(rank2);
	ranks.push_back(rank3);
	ranks.push_back(rank4);
	ranks.push_back(rank5);
	ranks.push_back(rank6);
	ranks.push_back(rank7);
	ranks.push_back(rank8);

	files.push_back(fileH);				//USE (ARRAYPOS%8)
	files.push_back(fileG);
	files.push_back(fileF);
	files.push_back(fileE);
	files.push_back(fileD);
	files.push_back(fileC);
	files.push_back(fileB);
	files.push_back(fileA);

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

	PAWN_MOVES=(boardRep->WP<<16)&boardRep->emptySquares&(boardRep->emptySquares<<8)&rank4;
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

string Moves::possibleBP(string lastMove){
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
	if ((boardRep->getPiece(destinationSquare)=='P')&&(startSquare[0]=destinationSquare[0])&&(destinationSquare[1]==startSquare[1]+2)){
		int destinationIndex=boardRep->notationMap[destinationSquare];
		bitset<64> EPWHITE;
		EPWHITE.set(destinationIndex);
		PAWN_MOVES=(boardRep->BP>>7)&~fileA&(EPWHITE>>8)&rank4&boardRep->emptySquares;

		for (int i=0;i<64;i++){
			if (PAWN_MOVES[i]){
				list+=boardRep->positionMap[i+7]+"x"+boardRep->positionMap[i]+"ep ";
			}
		}

		PAWN_MOVES=(boardRep->BP>>9)&~fileH&(EPWHITE>>8)&rank4&boardRep->emptySquares;
		for (int i=0;i<64;i++){
			if (PAWN_MOVES[i]){
				list+=boardRep->positionMap[i+9]+"x"+boardRep->positionMap[i]+"ep ";
			}
		}

	}

	/****************************************
	ALL OTHER PAWN FUNCTION CODE
	*****************************************/
	PAWN_MOVES=(boardRep->BP>>9)&boardRep->whitePieces&~rank1&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+9]+"x"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->BP>>7)&boardRep->whitePieces&~rank1&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+7]+"x"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->BP>>8)&boardRep->emptySquares&~rank1;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+8]+"-"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->BP>>16)&boardRep->emptySquares&(boardRep->emptySquares>>8)&~rank1&rank5;

	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+16]+"-"+boardRep->positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(boardRep->BP>>7)&boardRep->whitePieces&rank1&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+7]+"x"+boardRep->positionMap[i]+"=q ";
			list+=boardRep->positionMap[i+7]+"x"+boardRep->positionMap[i]+"=r ";
			list+=boardRep->positionMap[i+7]+"x"+boardRep->positionMap[i]+"=n ";
			list+=boardRep->positionMap[i+7]+"x"+boardRep->positionMap[i]+"=b ";
		}
	}

	PAWN_MOVES=(boardRep->BP>>9)&boardRep->whitePieces&rank1&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+9]+"x"+boardRep->positionMap[i]+"=q ";
			list+=boardRep->positionMap[i+9]+"x"+boardRep->positionMap[i]+"=r ";
			list+=boardRep->positionMap[i+9]+"x"+boardRep->positionMap[i]+"=n ";
			list+=boardRep->positionMap[i+9]+"x"+boardRep->positionMap[i]+"=b ";
		}
	}

	PAWN_MOVES=(boardRep->BP>>8)&boardRep->emptySquares&rank1;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=boardRep->positionMap[i+8]+"-"+boardRep->positionMap[i]+"=q ";
			list+=boardRep->positionMap[i+8]+"-"+boardRep->positionMap[i]+"=r ";
			list+=boardRep->positionMap[i+8]+"-"+boardRep->positionMap[i]+"=n ";
			list+=boardRep->positionMap[i+8]+"-"+boardRep->positionMap[i]+"=b ";
		}
	}

	return list;
}

string Moves::possibleWR(){
	string list="";
	vector<int> rook_positions;

	for (int i=0;i<64;i++){
		if (boardRep->WR[i]){
			rook_positions.push_back(i);
		}
	}
	if ((int)rook_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)rook_positions.size();i++){
		bitset<64> rookMoves(HandVMoves(rook_positions[i]));
		bitset<64> ROOK_MOVES = rookMoves & (~boardRep->whitePieces);
		vector<int> rook_destinations;

		for (int j=0;j<64;j++){
			if (ROOK_MOVES[j]){
				rook_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(rook_destinations.size());j++){
			if (boardRep->gameBoard[rook_destinations[j]]=='*'){
				list+=boardRep->positionMap[rook_positions[i]]+"-"+boardRep->positionMap[rook_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[rook_positions[i]]+"x"+boardRep->positionMap[rook_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleBR(){
	string list="";
	vector<int> rook_positions;

	for (int i=0;i<64;i++){
		if (boardRep->BR[i]){
			rook_positions.push_back(i);
		}
	}
	if ((int)rook_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)rook_positions.size();i++){
		bitset<64> rookMoves(HandVMoves(rook_positions[i]));
		bitset<64> ROOK_MOVES = rookMoves & (~boardRep->blackPieces);
		vector<int> rook_destinations;

		for (int j=0;j<64;j++){
			if (ROOK_MOVES[j]){
				rook_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(rook_destinations.size());j++){
			if (boardRep->gameBoard[rook_destinations[j]]=='*'){
				list+=boardRep->positionMap[rook_positions[i]]+"-"+boardRep->positionMap[rook_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[rook_positions[i]]+"x"+boardRep->positionMap[rook_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleWB(){
	string list="";
	vector<int> bishop_positions;


	for (int i=0;i<64;i++){
		if (boardRep->WB[i]){
			bishop_positions.push_back(i);
		}
	}
	if ((int)bishop_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)bishop_positions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishop_positions[i]));
		bitset<64> BISHOP_MOVES = bishopMoves & (~boardRep->whitePieces);
		vector<int> bishop_destinations;

		for (int j=0;j<64;j++){
			if (BISHOP_MOVES[j]){
				bishop_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(bishop_destinations.size());j++){
			if (boardRep->gameBoard[bishop_destinations[j]]=='*'){
				list+=boardRep->positionMap[bishop_positions[i]]+"-"+boardRep->positionMap[bishop_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[bishop_positions[i]]+"x"+boardRep->positionMap[bishop_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleBB(){
	string list="";
	vector<int> bishop_positions;


	for (int i=0;i<64;i++){
		if (boardRep->BB[i]){
			bishop_positions.push_back(i);
		}
	}
	if ((int)bishop_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)bishop_positions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishop_positions[i]));
		bitset<64> BISHOP_MOVES = bishopMoves & (~boardRep->blackPieces);
		vector<int> bishop_destinations;

		for (int j=0;j<64;j++){
			if (BISHOP_MOVES[j]){
				bishop_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(bishop_destinations.size());j++){
			if (boardRep->gameBoard[bishop_destinations[j]]=='*'){
				list+=boardRep->positionMap[bishop_positions[i]]+"-"+boardRep->positionMap[bishop_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[bishop_positions[i]]+"x"+boardRep->positionMap[bishop_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleWQ(){
	string list="";
	vector<int> queen_positions;


	for (int i=0;i<64;i++){
		if (boardRep->WQ[i]){
			queen_positions.push_back(i);
		}
	}
	if ((int)queen_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)queen_positions.size();i++){
		bitset<64> diagnolAttacks(DandAntiMoves(queen_positions[i]));
		bitset<64> lineAttacks(HandVMoves(queen_positions[i]));
		bitset<64> queenMoves=diagnolAttacks|lineAttacks;
		bitset<64> QUEEN_MOVES = queenMoves & (~boardRep->whitePieces);
		vector<int> queen_destinations;

		for (int j=0;j<64;j++){
			if (QUEEN_MOVES[j]){
				queen_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(queen_destinations.size());j++){
			if (boardRep->gameBoard[queen_destinations[j]]=='*'){
				list+=boardRep->positionMap[queen_positions[i]]+"-"+boardRep->positionMap[queen_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[queen_positions[i]]+"x"+boardRep->positionMap[queen_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleBQ(){
	string list="";
	vector<int> queen_positions;

	for (int i=0;i<64;i++){
		if (boardRep->BQ[i]){
			queen_positions.push_back(i);
		}
	}
	if ((int)queen_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)queen_positions.size();i++){
		bitset<64> diagnolAttacks(DandAntiMoves(queen_positions[i]));
		bitset<64> lineAttacks(HandVMoves(queen_positions[i]));
		bitset<64> queenMoves=diagnolAttacks|lineAttacks;
		bitset<64> QUEEN_MOVES = queenMoves & (~boardRep->blackPieces);
		vector<int> queen_destinations;

		for (int j=0;j<64;j++){
			if (QUEEN_MOVES[j]){
				queen_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(queen_destinations.size());j++){
			if (boardRep->gameBoard[queen_destinations[j]]=='*'){
				list+=boardRep->positionMap[queen_positions[i]]+"-"+boardRep->positionMap[queen_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[queen_positions[i]]+"x"+boardRep->positionMap[queen_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleWN(){
	string list="";
	vector<int> knight_positions;
	

	for (int i=0;i<64;i++){
		if (boardRep->WN[i]){
			knight_positions.push_back(i);
		}
	}

	if ((int)knight_positions.size()==0){
		return list;
	}


	for (int i=0;i<(int)knight_positions.size();i++){
		bitset<64> knight_loc;
		knight_loc.set(knight_positions[i]);
		vector<int> knight_destinations;

		bitset<64> spot_1 = (knight_loc & ~fileA & ~fileB) >> 6;
		bitset<64> spot_2 = (knight_loc & ~fileA) >> 15;
		bitset<64> spot_3 = (knight_loc & ~fileH) >> 17;
		bitset<64> spot_4 = (knight_loc & ~fileH & ~fileG) >> 10;

		bitset<64> spot_5 = (knight_loc & ~fileH & ~fileG) << 6;
		bitset<64> spot_6 = (knight_loc & ~fileH) << 15;
		bitset<64> spot_7 = (knight_loc & ~fileA) << 17;
		bitset<64> spot_8 = (knight_loc & ~fileA & ~fileB) << 10;

		bitset<64> KNIGHT_MOVES = (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~boardRep->whitePieces);

		for (int j=0;j<64;j++){
			if (KNIGHT_MOVES[j]){
				knight_destinations.push_back(j);
			}
		}



		for (int j=0;j<(int)(knight_destinations.size());j++){
			if (boardRep->gameBoard[knight_destinations[j]]=='*'){
				list+=boardRep->positionMap[knight_positions[i]]+"-"+boardRep->positionMap[knight_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[knight_positions[i]]+"x"+boardRep->positionMap[knight_destinations[j]]+" ";
			}
		}


	}

	return list;
}

string Moves::possibleBN(){
	string list="";
	vector<int> knight_positions;
	

	for (int i=0;i<64;i++){
		if (boardRep->BN[i]){
			knight_positions.push_back(i);
		}
	}

	if ((int)knight_positions.size()==0){
		return list;
	}


	for (int i=0;i<(int)knight_positions.size();i++){
		bitset<64> knight_loc;
		knight_loc.set(knight_positions[i]);
		vector<int> knight_destinations;

		bitset<64> spot_1 = (knight_loc & ~fileA & ~fileB) >> 6;
		bitset<64> spot_2 = (knight_loc & ~fileA) >> 15;
		bitset<64> spot_3 = (knight_loc & ~fileH) >> 17;
		bitset<64> spot_4 = (knight_loc & ~fileH & ~fileG) >> 10;

		bitset<64> spot_5 = (knight_loc & ~fileH & ~fileG) << 6;
		bitset<64> spot_6 = (knight_loc & ~fileH) << 15;
		bitset<64> spot_7 = (knight_loc & ~fileA) << 17;
		bitset<64> spot_8 = (knight_loc & ~fileA & ~fileB) << 10;

		bitset<64> KNIGHT_MOVES = (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~boardRep->blackPieces);

		for (int j=0;j<64;j++){
			if (KNIGHT_MOVES[j]){
				knight_destinations.push_back(j);
			}
		}



		for (int j=0;j<(int)(knight_destinations.size());j++){
			if (boardRep->gameBoard[knight_destinations[j]]=='*'){
				list+=boardRep->positionMap[knight_positions[i]]+"-"+boardRep->positionMap[knight_destinations[j]]+" ";
			}else{
				list+=boardRep->positionMap[knight_positions[i]]+"x"+boardRep->positionMap[knight_destinations[j]]+" ";
			}
		}


	}

	return list;
}

uint64_t Moves::HandVMoves(int arrayPos){
	/******************************************
	Alright so this disgusting looking code works like this
	Slider bitset marks position of the rook/queen
	Occupied Squares marks position of occupied squares
	horizontal = (o-2s) ^ (o'-2s')'    where the ' marks reverse of bitword
	vertical is same thing except you restrict it to specific file

	function returns unsigned 64 bit integer that can be converted to bitset of H&V moves
	DOES NOT ACCOUNT FOR WHITE OR BLACK, you have to hard code that into the specific pieces
		You can do this just by adding & (~whitePieces) or (~blackPieces)
	*******************************************/
	bitset<64> slider;
	slider.set(arrayPos);
	bitset<64> horizontal=(boardRep->occupiedSquares.to_ulong() - (2*slider.to_ulong()))^(reverseBits(reverseBits(boardRep->occupiedSquares.to_ulong())-(2 * reverseBits(slider.to_ulong()))));
	bitset<64> vertical = ((boardRep->occupiedSquares & files[arrayPos % 8]).to_ulong() - (2 * slider.to_ulong())) ^ (reverseBits(reverseBits((boardRep->occupiedSquares & files[arrayPos % 8]).to_ulong())-(2*reverseBits(slider.to_ulong()))));
	return (uint64_t)((horizontal & ranks[floor(arrayPos/8)])|(vertical & files[arrayPos % 8])).to_ulong();
}

uint64_t Moves::DandAntiMoves(int arrayPos){
	/***********************************************
	Same basic logic as above except just with diagnol masks
	************************************************/
	bitset<64> slider;
	slider.set(arrayPos);
	int diagnolIndex=(floor(arrayPos/8))+(arrayPos%8);
	int adiagnolIndex=(floor(arrayPos/8))+7-(arrayPos%8);
	uint64_t x=diagnols[diagnolIndex];
	uint64_t y=adiagnols[adiagnolIndex];
	bitset<64> diagnol(x);
	bitset<64> adiagnol(y);

	bitset<64> diagnolPossibilities = ((boardRep->occupiedSquares & diagnol).to_ulong() - (2 * slider.to_ulong()))^(reverseBits(reverseBits((boardRep->occupiedSquares&diagnol).to_ulong()) - (2 * reverseBits(slider.to_ulong()))));
	bitset<64> adiagnolPossibilities = ((boardRep->occupiedSquares & adiagnol).to_ulong() - (2 * slider.to_ulong()))^(reverseBits(reverseBits((boardRep->occupiedSquares&adiagnol).to_ulong()) - (2 * reverseBits(slider.to_ulong()))));
	return (uint64_t)((diagnolPossibilities&diagnol) | (adiagnolPossibilities&adiagnol)).to_ulong();

}

vector<string> Moves::possibleWMoves(string history){
	string list;
	string move;
	vector<string> move_list;
	list+=possibleWP(history)+possibleWQ()+possibleWB()+possibleWR()+possibleWN()+possibleWK();

	stringstream ss(list);

	while (getline(ss,move,' ')){
		move_list.push_back(move);
	}

	for(vector<string>::iterator it2 = move_list.begin(); it2 != move_list.end();){
		/**************************
		REMOVES ALL MOVES THAT KILL THE KING
		**************************/
		string startSquare=(*it2).substr(0,2);
		string moveType = (*it2).substr(2,1);
		string destinationSquare=(*it2).substr(3,2);
		if(boardRep->getPiece(destinationSquare)=='k')
	   	{
			it2 = move_list.erase(it2); 
	   	}
	   	else
	   	{
			++it2;
		}
	}


	return move_list;

}

vector<string> Moves::possibleBMoves(string history){
	string list;
	string move;
	vector<string> move_list;
	list+=possibleBP(history)+possibleBQ()+possibleBB()+possibleBR()+possibleBN()+possibleBK();

	stringstream ss(list);

	while (getline(ss,move,' ')){
		move_list.push_back(move);
	}

	for(vector<string>::iterator it2 = move_list.begin(); it2 != move_list.end();){
		/**************************
		REMOVES ALL MOVES THAT KILL THE KING
		**************************/
		string startSquare=(*it2).substr(0,2);
		string moveType = (*it2).substr(2,1);
		string destinationSquare=(*it2).substr(3,2);
		if(boardRep->getPiece(destinationSquare)=='k')
	   	{
			it2 = move_list.erase(it2); 
	   	}
	   	else
	   	{
			++it2;
		}
	}


	return move_list;

}



string Moves::possibleWK(){
	bitset<64> KING_MOVES;
	bitset<64> unsafe(~unsafeForWhite());
	vector<int> king_destinations;
	string list;
	int kingLocation = bitScanForward((uint64_t)boardRep->WK.to_ulong());

	KING_MOVES |= (boardRep->WK << 1) & ~fileH & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK >> 1) & ~fileA & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK >> 8) & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK << 8) & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK << 7) & ~fileA & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK << 9) & ~fileH & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK >> 7) & ~fileH & ~boardRep->whitePieces & unsafe;
	KING_MOVES |= (boardRep->WK >> 9) & ~fileA & ~boardRep->whitePieces & unsafe;

	for (int i=0;i<64;i++){
		if (KING_MOVES[i]) king_destinations.push_back(i);
	}
	for (int j=0;j<(int)(king_destinations.size());j++){
		if (boardRep->gameBoard[king_destinations[j]]=='*'){
			list+=boardRep->positionMap[kingLocation]+"-"+boardRep->positionMap[king_destinations[j]]+" ";
		}else{
			list+=boardRep->positionMap[kingLocation]+"x"+boardRep->positionMap[king_destinations[j]]+" ";
		}
	}
	/********************************************************


	INSERT CODE FOR CASTLING HERE


	*********************************************************/


	return list;
}

string Moves::possibleBK(){
	bitset<64> KING_MOVES;
	bitset<64> unsafe(~unsafeForBlack());
	vector<int> king_destinations;
	string list;
	int kingLocation = bitScanForward((uint64_t)boardRep->BK.to_ulong());

	KING_MOVES |= (boardRep->BK << 1) & ~fileH & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK >> 1) & ~fileA & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK >> 8) & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK << 8) & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK << 7) & ~fileA & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK << 9) & ~fileH & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK >> 7) & ~fileH & ~boardRep->blackPieces & unsafe;
	KING_MOVES |= (boardRep->BK >> 9) & ~fileA & ~boardRep->blackPieces & unsafe;

	for (int i=0;i<64;i++){
		if (KING_MOVES[i]) king_destinations.push_back(i);
	}
	for (int j=0;j<(int)(king_destinations.size());j++){
		if (boardRep->gameBoard[king_destinations[j]]=='*'){
			list+=boardRep->positionMap[kingLocation]+"-"+boardRep->positionMap[king_destinations[j]]+" ";
		}else{
			list+=boardRep->positionMap[kingLocation]+"x"+boardRep->positionMap[king_destinations[j]]+" ";
		}
	}
	/********************************************************


	INSERT CODE FOR CASTLING HERE


	*********************************************************/








	return list;
}

uint64_t Moves::unsafeForBlack(){
	bitset<64> unsafeForBlack;
	bitset<64> KNIGHT_MOVES;
	bitset<64> BISHOP_MOVES;
	bitset<64> ROOK_MOVES;
	bitset<64> QUEEN_MOVES;
	bitset<64> WK_MOVES;
	vector<int> knightPositions;
	vector<int> bishopPositions;
	vector<int> queenPositions;
	vector<int> rookPositions;

	for (int i=0;i<64;i++){
		if (boardRep->WN[i]) knightPositions.push_back(i);
		if (boardRep->WB[i]) bishopPositions.push_back(i);
		if (boardRep->WQ[i]) queenPositions.push_back(i);
		if (boardRep->WR[i]) rookPositions.push_back(i);
	}
	for (int i=0;i<(int)knightPositions.size();i++){
		bitset<64> knight_loc;
		knight_loc.set(knightPositions[i]);
		vector<int> knight_destinations;

		bitset<64> spot_1 = (knight_loc & ~fileA & ~fileB) >> 6;
		bitset<64> spot_2 = (knight_loc & ~fileA) >> 15;
		bitset<64> spot_3 = (knight_loc & ~fileH) >> 17;
		bitset<64> spot_4 = (knight_loc & ~fileH & ~fileG) >> 10;

		bitset<64> spot_5 = (knight_loc & ~fileH & ~fileG) << 6;
		bitset<64> spot_6 = (knight_loc & ~fileH) << 15;
		bitset<64> spot_7 = (knight_loc & ~fileA) << 17;
		bitset<64> spot_8 = (knight_loc & ~fileA & ~fileB) << 10;

		KNIGHT_MOVES |= (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~boardRep->whitePieces);
	}
	for (int i=0;i<(int)bishopPositions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishopPositions[i]));
		BISHOP_MOVES |= bishopMoves & (~boardRep->whitePieces);
	}
	for (int i=0;i<(int)rookPositions.size();i++){
		bitset<64> rookMoves(HandVMoves(rookPositions[i]));
		ROOK_MOVES |= rookMoves & (~boardRep->whitePieces);
	}
	for (int i=0;i<(int)queenPositions.size();i++){
		bitset<64> diagnolAttacks(DandAntiMoves(queenPositions[i]));
		bitset<64> lineAttacks(HandVMoves(queenPositions[i]));
		bitset<64> queenMoves=diagnolAttacks|lineAttacks;
		QUEEN_MOVES |= queenMoves & (~boardRep->whitePieces);
	}

	WK_MOVES |= (boardRep->WK << 1) & ~fileH & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK >> 1) & ~fileA & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK >> 8) & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK << 8) & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK << 7) & ~fileA & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK << 9) & ~fileH & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK >> 7) & ~fileH & ~boardRep->whitePieces;
	WK_MOVES |= (boardRep->WK >> 9) & ~fileA & ~boardRep->whitePieces;

	unsafeForBlack |= (boardRep->WP<<7)&~fileA;
	unsafeForBlack |= (boardRep->WP<<9)&~fileH;
	unsafeForBlack |= KNIGHT_MOVES;
	unsafeForBlack |= BISHOP_MOVES;
	unsafeForBlack |= ROOK_MOVES;
	unsafeForBlack |= QUEEN_MOVES;
	unsafeForBlack |= WK_MOVES;

	return (uint64_t) unsafeForBlack.to_ulong();
}

uint64_t Moves::unsafeForWhite(){
	bitset<64> unsafeForWhite;
	bitset<64> KNIGHT_MOVES;
	bitset<64> BISHOP_MOVES;
	bitset<64> ROOK_MOVES;
	bitset<64> QUEEN_MOVES;
	bitset<64> BK_MOVES;
	vector<int> knightPositions;
	vector<int> bishopPositions;
	vector<int> queenPositions;
	vector<int> rookPositions;

	for (int i=0;i<64;i++){
		if (boardRep->BN[i]) knightPositions.push_back(i);
		if (boardRep->BB[i]) bishopPositions.push_back(i);
		if (boardRep->BQ[i]) queenPositions.push_back(i);
		if (boardRep->BR[i]) rookPositions.push_back(i);
	}
	for (int i=0;i<(int)knightPositions.size();i++){
		bitset<64> knight_loc;
		knight_loc.set(knightPositions[i]);
		vector<int> knight_destinations;

		bitset<64> spot_1 = (knight_loc & ~fileA & ~fileB) >> 6;
		bitset<64> spot_2 = (knight_loc & ~fileA) >> 15;
		bitset<64> spot_3 = (knight_loc & ~fileH) >> 17;
		bitset<64> spot_4 = (knight_loc & ~fileH & ~fileG) >> 10;

		bitset<64> spot_5 = (knight_loc & ~fileH & ~fileG) << 6;
		bitset<64> spot_6 = (knight_loc & ~fileH) << 15;
		bitset<64> spot_7 = (knight_loc & ~fileA) << 17;
		bitset<64> spot_8 = (knight_loc & ~fileA & ~fileB) << 10;

		KNIGHT_MOVES |= (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~boardRep->blackPieces);
	}
	for (int i=0;i<(int)bishopPositions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishopPositions[i]));
		BISHOP_MOVES |= bishopMoves & (~boardRep->blackPieces);
	}
	for (int i=0;i<(int)rookPositions.size();i++){
		bitset<64> rookMoves(HandVMoves(rookPositions[i]));
		ROOK_MOVES |= rookMoves & (~boardRep->blackPieces);
	}
	for (int i=0;i<(int)queenPositions.size();i++){
		bitset<64> diagnolAttacks(DandAntiMoves(queenPositions[i]));
		bitset<64> lineAttacks(HandVMoves(queenPositions[i]));
		bitset<64> queenMoves=diagnolAttacks|lineAttacks;
		QUEEN_MOVES |= queenMoves & (~boardRep->blackPieces);
	}
	BK_MOVES |= (boardRep->BK << 1) & ~fileH & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK >> 1) & ~fileA & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK >> 8) & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK << 8) & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK << 7) & ~fileA & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK << 9) & ~fileH & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK >> 7) & ~fileH & ~boardRep->blackPieces;
	BK_MOVES |= (boardRep->BK >> 9) & ~fileA & ~boardRep->blackPieces;

	unsafeForWhite |= (boardRep->BP>>7)&~fileH;
	unsafeForWhite |= (boardRep->BP>>9)&~fileA;
	unsafeForWhite |= KNIGHT_MOVES;
	unsafeForWhite |= BISHOP_MOVES;
	unsafeForWhite |= ROOK_MOVES;
	unsafeForWhite |= QUEEN_MOVES;
	unsafeForWhite |= BK_MOVES;

	return (uint64_t) unsafeForWhite.to_ulong();
}

void Moves::makeMove(string move){
	string startSquare=move.substr(0,2);
	string moveType = move.substr(2,1);
	string destinationSquare=move.substr(3,2);
	string special;
	if (move.size()>5){
		special=move.substr(5,2);
		cout<<special;
	}

	int startIndex=boardRep->notationMap[startSquare];
	int destinationIndex=boardRep->notationMap[destinationSquare];

	if (moveType=="-"){
		if(boardRep->getPiece(startSquare)=='P')
		{
			boardRep->WP.reset(startIndex);
			if (special=="=Q") boardRep->WQ.set(destinationIndex);
			else if (special=="=R") boardRep->WR.set(destinationIndex);
			else if (special=="=B") boardRep->WB.set(destinationIndex);
			else if (special=="=N") boardRep->WN.set(destinationIndex);
			else boardRep->WP.set(destinationIndex);

		}
		if(boardRep->getPiece(startSquare)=='R')
		{
			boardRep->WR.reset(startIndex);
			boardRep->WR.set(destinationIndex);	
		}
		if(boardRep->getPiece(startSquare)=='N')
		{
			boardRep->WN.reset(startIndex);
			boardRep->WN.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='B')
		{
			boardRep->WB.reset(startIndex);
			boardRep->WB.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='Q')
		{
			boardRep->WQ.reset(startIndex);
			boardRep->WQ.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='K')
		{
			boardRep->WK.reset(startIndex);
			boardRep->WK.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='p')
		{
			boardRep->BP.reset(startIndex);
			if (special=="=q") boardRep->BQ.set(destinationIndex);
			else if (special=="=r") boardRep->BR.set(destinationIndex);
			else if (special=="=b") boardRep->BB.set(destinationIndex);
			else if (special=="=n") boardRep->BN.set(destinationIndex);
			else boardRep->BP.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='r')
		{
			boardRep->BR.reset(startIndex);
			boardRep->BR.set(destinationIndex);	
		}
		if(boardRep->getPiece(startSquare)=='n')
		{
			boardRep->BN.reset(startIndex);
			boardRep->BN.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='b')
		{
			boardRep->BB.reset(startIndex);
			boardRep->BB.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='q')
		{
			boardRep->BQ.reset(startIndex);
			boardRep->BQ.set(destinationIndex);
		}
		if(boardRep->getPiece(startSquare)=='k')
		{
			boardRep->BK.reset(startIndex);
			boardRep->BK.set(destinationIndex);
		}
	}
	if (moveType=="x"){
		if(boardRep->getPiece(startSquare)=='P')
		{
			if (boardRep->getPiece(destinationSquare)=='p'){
				boardRep->WP.reset(startIndex);

				if (special=="ep"){
					boardRep->WP.set(destinationIndex);
					boardRep->BP.reset(destinationIndex-8);
				}else{
					boardRep->WP.set(destinationIndex);
					boardRep->BP.reset(destinationIndex);
				}
				
			}
			if (boardRep->getPiece(destinationSquare)=='r'){
				boardRep->WP.reset(startIndex);
				if (special=="=Q") {boardRep->WQ.set(destinationIndex);boardRep->BR.reset(destinationIndex);}
				else if (special=="=R") {boardRep->WR.set(destinationIndex);boardRep->BR.reset(destinationIndex);}
				else if (special=="=B") {boardRep->WB.set(destinationIndex);boardRep->BR.reset(destinationIndex);}
				else if (special=="=N") {boardRep->WN.set(destinationIndex);boardRep->BR.reset(destinationIndex);}
				else {boardRep->WP.set(destinationIndex);boardRep->BR.reset(destinationIndex);}
			}
			if (boardRep->getPiece(destinationSquare)=='n'){
				boardRep->WP.reset(startIndex);
				if (special=="=Q") {boardRep->WQ.set(destinationIndex);boardRep->BN.reset(destinationIndex);}
				else if (special=="=R") {boardRep->WR.set(destinationIndex);boardRep->BN.reset(destinationIndex);}
				else if (special=="=B") {boardRep->WB.set(destinationIndex);boardRep->BN.reset(destinationIndex);}
				else if (special=="=N") {boardRep->WN.set(destinationIndex);boardRep->BN.reset(destinationIndex);}
				else {boardRep->WP.set(destinationIndex);boardRep->BN.reset(destinationIndex);}
			}
			if (boardRep->getPiece(destinationSquare)=='b'){
				boardRep->WP.reset(startIndex);
				if (special=="=Q") {boardRep->WQ.set(destinationIndex);boardRep->BB.reset(destinationIndex);}
				else if (special=="=R") {boardRep->WR.set(destinationIndex);boardRep->BB.reset(destinationIndex);}
				else if (special=="=B") {boardRep->WB.set(destinationIndex);boardRep->BB.reset(destinationIndex);}
				else if (special=="=N") {boardRep->WN.set(destinationIndex);boardRep->BB.reset(destinationIndex);}
				else {boardRep->WP.set(destinationIndex);boardRep->BB.reset(destinationIndex);}
			}
			if (boardRep->getPiece(destinationSquare)=='q'){
				boardRep->WP.reset(startIndex);
				if (special=="=Q") {boardRep->WQ.set(destinationIndex);boardRep->BQ.reset(destinationIndex);}
				else if (special=="=R") {boardRep->WR.set(destinationIndex);boardRep->BQ.reset(destinationIndex);}
				else if (special=="=B") {boardRep->WB.set(destinationIndex);boardRep->BQ.reset(destinationIndex);}
				else if (special=="=N") {boardRep->WN.set(destinationIndex);boardRep->BQ.reset(destinationIndex);}
				else {boardRep->WP.set(destinationIndex);boardRep->BQ.reset(destinationIndex);}
			}
		}
		if(boardRep->getPiece(startSquare)=='R')
		{
			if (boardRep->getPiece(destinationSquare)=='p'){
				boardRep->WR.reset(startIndex);
				boardRep->WR.set(destinationIndex);
				boardRep->BP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='r'){
				boardRep->WR.reset(startIndex);
				boardRep->WR.set(destinationIndex);
				boardRep->BR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='n'){
				boardRep->WR.reset(startIndex);
				boardRep->WR.set(destinationIndex);
				boardRep->BN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='b'){
				boardRep->WR.reset(startIndex);
				boardRep->WR.set(destinationIndex);
				boardRep->BB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='q'){
				boardRep->WR.reset(startIndex);
				boardRep->WR.set(destinationIndex);
				boardRep->BQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='N')
		{
			if (boardRep->getPiece(destinationSquare)=='p'){
				boardRep->WN.reset(startIndex);
				boardRep->WN.set(destinationIndex);
				boardRep->BP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='r'){
				boardRep->WN.reset(startIndex);
				boardRep->WN.set(destinationIndex);
				boardRep->BR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='n'){
				boardRep->WN.reset(startIndex);
				boardRep->WN.set(destinationIndex);
				boardRep->BN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='b'){
				boardRep->WN.reset(startIndex);
				boardRep->WN.set(destinationIndex);
				boardRep->BB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='q'){
				boardRep->WN.reset(startIndex);
				boardRep->WN.set(destinationIndex);
				boardRep->BQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='B')
		{
			if (boardRep->getPiece(destinationSquare)=='p'){
				boardRep->WB.reset(startIndex);
				boardRep->WB.set(destinationIndex);
				boardRep->BP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='r'){
				boardRep->WB.reset(startIndex);
				boardRep->WB.set(destinationIndex);
				boardRep->BR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='n'){
				boardRep->WB.reset(startIndex);
				boardRep->WB.set(destinationIndex);
				boardRep->BN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='b'){
				boardRep->WB.reset(startIndex);
				boardRep->WB.set(destinationIndex);
				boardRep->BB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='q'){
				boardRep->WB.reset(startIndex);
				boardRep->WB.set(destinationIndex);
				boardRep->BQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='Q')
		{
			if (boardRep->getPiece(destinationSquare)=='p'){
				boardRep->WQ.reset(startIndex);
				boardRep->WQ.set(destinationIndex);
				boardRep->BP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='r'){
				boardRep->WQ.reset(startIndex);
				boardRep->WQ.set(destinationIndex);
				boardRep->BR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='n'){
				boardRep->WQ.reset(startIndex);
				boardRep->WQ.set(destinationIndex);
				boardRep->BN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='b'){
				boardRep->WQ.reset(startIndex);
				boardRep->WQ.set(destinationIndex);
				boardRep->BB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='q'){
				boardRep->WQ.reset(startIndex);
				boardRep->WQ.set(destinationIndex);
				boardRep->BQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='K')
		{
			if (boardRep->getPiece(destinationSquare)=='p'){
				boardRep->WK.reset(startIndex);
				boardRep->WK.set(destinationIndex);
				boardRep->BP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='r'){
				boardRep->WK.reset(startIndex);
				boardRep->WK.set(destinationIndex);
				boardRep->BR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='n'){
				boardRep->WK.reset(startIndex);
				boardRep->WK.set(destinationIndex);
				boardRep->BN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='b'){
				boardRep->WK.reset(startIndex);
				boardRep->WK.set(destinationIndex);
				boardRep->BB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='q'){
				boardRep->WK.reset(startIndex);
				boardRep->WK.set(destinationIndex);
				boardRep->BQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='p')
		{
			if (boardRep->getPiece(destinationSquare)=='P'){
				boardRep->BP.reset(startIndex);

				if (special=="ep"){
					boardRep->BP.set(destinationIndex);
					boardRep->WP.reset(destinationIndex+8);
				}else{
					boardRep->BP.set(destinationIndex);
					boardRep->WP.reset(destinationIndex);
				}
			}
			if (boardRep->getPiece(destinationSquare)=='R'){
				boardRep->BP.reset(startIndex);
				if (special=="=q") {boardRep->BQ.set(destinationIndex);boardRep->WR.reset(destinationIndex);}
				else if (special=="=r") {boardRep->BR.set(destinationIndex);boardRep->WR.reset(destinationIndex);}
				else if (special=="=b") {boardRep->BB.set(destinationIndex);boardRep->WR.reset(destinationIndex);}
				else if (special=="=n") {boardRep->BN.set(destinationIndex);boardRep->WR.reset(destinationIndex);}
				else {boardRep->BP.set(destinationIndex);boardRep->WR.reset(destinationIndex);}
			}
			if (boardRep->getPiece(destinationSquare)=='N'){
				boardRep->BP.reset(startIndex);
				if (special=="=q") {boardRep->BQ.set(destinationIndex);boardRep->WN.reset(destinationIndex);}
				else if (special=="=r") {boardRep->BR.set(destinationIndex);boardRep->WN.reset(destinationIndex);}
				else if (special=="=b") {boardRep->BB.set(destinationIndex);boardRep->WN.reset(destinationIndex);}
				else if (special=="=n") {boardRep->BN.set(destinationIndex);boardRep->WN.reset(destinationIndex);}
				else {boardRep->BP.set(destinationIndex);boardRep->WN.reset(destinationIndex);}
			}
			if (boardRep->getPiece(destinationSquare)=='B'){
				boardRep->BP.reset(startIndex);
				if (special=="=q") {boardRep->BQ.set(destinationIndex);boardRep->WB.reset(destinationIndex);}
				else if (special=="=r") {boardRep->BR.set(destinationIndex);boardRep->WB.reset(destinationIndex);}
				else if (special=="=b") {boardRep->BB.set(destinationIndex);boardRep->WB.reset(destinationIndex);}
				else if (special=="=n") {boardRep->BN.set(destinationIndex);boardRep->WB.reset(destinationIndex);}
				else {boardRep->BP.set(destinationIndex);boardRep->WB.reset(destinationIndex);}
			}
			if (boardRep->getPiece(destinationSquare)=='Q'){
				boardRep->BP.reset(startIndex);
				if (special=="=q") {boardRep->BQ.set(destinationIndex);boardRep->WQ.reset(destinationIndex);}
				else if (special=="=r") {boardRep->BR.set(destinationIndex);boardRep->WQ.reset(destinationIndex);}
				else if (special=="=b") {boardRep->BB.set(destinationIndex);boardRep->WQ.reset(destinationIndex);}
				else if (special=="=n") {boardRep->BN.set(destinationIndex);boardRep->WQ.reset(destinationIndex);}
				else {boardRep->BP.set(destinationIndex);boardRep->WQ.reset(destinationIndex);}
			}
		}
		if(boardRep->getPiece(startSquare)=='r')
		{
			if (boardRep->getPiece(destinationSquare)=='P'){
				boardRep->BR.reset(startIndex);
				boardRep->BR.set(destinationIndex);
				boardRep->WP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='R'){
				boardRep->BR.reset(startIndex);
				boardRep->BR.set(destinationIndex);
				boardRep->WR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='N'){
				boardRep->BR.reset(startIndex);
				boardRep->BR.set(destinationIndex);
				boardRep->WN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='B'){
				boardRep->BR.reset(startIndex);
				boardRep->BR.set(destinationIndex);
				boardRep->WB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='Q'){
				boardRep->BR.reset(startIndex);
				boardRep->BR.set(destinationIndex);
				boardRep->WQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='n')
		{
			if (boardRep->getPiece(destinationSquare)=='P'){
				boardRep->BN.reset(startIndex);
				boardRep->BN.set(destinationIndex);
				boardRep->WP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='R'){
				boardRep->BN.reset(startIndex);
				boardRep->BN.set(destinationIndex);
				boardRep->WR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='N'){
				boardRep->BN.reset(startIndex);
				boardRep->BN.set(destinationIndex);
				boardRep->WN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='B'){
				boardRep->BN.reset(startIndex);
				boardRep->BN.set(destinationIndex);
				boardRep->WB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='Q'){
				boardRep->BN.reset(startIndex);
				boardRep->BN.set(destinationIndex);
				boardRep->WQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='b')
		{
			if (boardRep->getPiece(destinationSquare)=='P'){
				boardRep->BB.reset(startIndex);
				boardRep->BB.set(destinationIndex);
				boardRep->WP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='R'){
				boardRep->BB.reset(startIndex);
				boardRep->BB.set(destinationIndex);
				boardRep->WR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='N'){
				boardRep->BB.reset(startIndex);
				boardRep->BB.set(destinationIndex);
				boardRep->WN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='B'){
				boardRep->BB.reset(startIndex);
				boardRep->BB.set(destinationIndex);
				boardRep->WB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='Q'){
				boardRep->BB.reset(startIndex);
				boardRep->BB.set(destinationIndex);
				boardRep->WQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='q')
		{
			if (boardRep->getPiece(destinationSquare)=='P'){
				boardRep->BQ.reset(startIndex);
				boardRep->BQ.set(destinationIndex);
				boardRep->WP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='R'){
				boardRep->BQ.reset(startIndex);
				boardRep->BQ.set(destinationIndex);
				boardRep->WR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='N'){
				boardRep->BQ.reset(startIndex);
				boardRep->BQ.set(destinationIndex);
				boardRep->WN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='B'){
				boardRep->BQ.reset(startIndex);
				boardRep->BQ.set(destinationIndex);
				boardRep->WB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='Q'){
				boardRep->BQ.reset(startIndex);
				boardRep->BQ.set(destinationIndex);
				boardRep->WQ.reset(destinationIndex);
			}
		}
		if(boardRep->getPiece(startSquare)=='k')
		{
			if (boardRep->getPiece(destinationSquare)=='P'){
				boardRep->BK.reset(startIndex);
				boardRep->BK.set(destinationIndex);
				boardRep->WP.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='R'){
				boardRep->BK.reset(startIndex);
				boardRep->BK.set(destinationIndex);
				boardRep->WR.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='N'){
				boardRep->BK.reset(startIndex);
				boardRep->BK.set(destinationIndex);
				boardRep->WN.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='B'){
				boardRep->BK.reset(startIndex);
				boardRep->BK.set(destinationIndex);
				boardRep->WB.reset(destinationIndex);
			}
			if (boardRep->getPiece(destinationSquare)=='Q'){
				boardRep->BK.reset(startIndex);
				boardRep->BK.set(destinationIndex);
				boardRep->WQ.reset(destinationIndex);
			}
		}
	}

	
	boardRep->updateBoard();
	boardRep->updateGenPositions();
}

bool Moves::WKCheck(){
	bitset<64> unsafe(unsafeForWhite());
	bitset<64> check=boardRep->WK & unsafe;
	if (check.to_ulong()!=0){
		return true;
	}
	return false;
}

bool Moves::BKCheck(){
	bitset<64> unsafe(unsafeForBlack());
	bitset<64> check=boardRep->BK & unsafe;
	if (check.to_ulong()!=0){
		return true;
	}
	return false;
}

int main(){
	Moves moveSet;
	// bitset<64> n(6327421892U);
	// cout<<n<<endl;
	// bitset<64> reverser(reverseBits((uint64_t)n.to_ulong()));			//THIS IS THE SYNTAX FOR REVERSING 64 BIT BITSET
	// cout<<reverser<<endl;
	// bitset<64> rookMoves=moveSet.HandVMoves(45);
	// bitset<64> bishopMoves=moveSet.DandAntiMoves(2);

	// bitset<64> diagnolTemp=moveSet.adiagnols[4];
	// bitset<64> temp;
	

	// cout<<"MSB INDEX: "<<bitScanReverse((uint64_t)boardRep->WK.to_ulong())<<endl;
	// cout<<"LSB INDEX: "<<bitScanForward((uint64_t)boardRep->WP.to_ulong())<<endl;

	// bitset<64> temp(moveSet.possibleBK());

	// 	for (int i=63;i>=0;i--){
	// 	cout<<temp[i];
	// 	if (i%8==0)cout<<endl;
	// }
	vector<string> whiteMoves = moveSet.possibleWMoves("e4-e6");
	for (int i=0;i<(int)whiteMoves.size();i++){
		cout<<whiteMoves[i]<<endl;
	}

	moveSet.boardRep->drawBoard();
	moveSet.makeMove("e2-e4");
	moveSet.boardRep->drawBoard();
	moveSet.makeMove("h7-h5");
	moveSet.boardRep->drawBoard();
	moveSet.makeMove("g1-f3");
	moveSet.boardRep->drawBoard();

	// if (moveSet.WKCheck()){
	// 	cout<<"CHECK!"<<endl;
	// }else{
	// 	cout<<"NO CHECK!"<<endl;
	// }
	// moveSet.makeMove("b1-c3");
	// moveSet.boardRep->drawBoard();

	// bitset<64> temp = ~moveSet.fileA & ~moveSet.fileB;

	// for (int i=63;i>=0;i--){
	// 	cout<<temp[i];
	// 	if (i%8==0)cout<<endl;

	// }

}