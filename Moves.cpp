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


void Moves::generateNotationMap(){

	string notationFileName="notationMap.txt";
	ifstream notationFile (notationFileName.c_str());

	string line;

	while (getline(notationFile, line))
	{
		stringstream ss;
		string AN;
		int arrayPos;

		ss << line;

		ss >> AN;
		ss >> arrayPos;

		notationMap[AN]=arrayPos;
		positionMap[arrayPos]=AN;
	}

	notationFile.close ();
}

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

	generateNotationMap();

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

}

Moves::~Moves(){
}


string Moves::possibleWP(char* board, bitset<64> WP, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	/********************************
	Variable Definition
	********************************/


	string list = "";
	bitset<64> PAWN_MOVES;

	/*******************************
	EN PASSANT CODE
	********************************/
	// PAWN_MOVES=(WP<<1)&(~fileH)&(EP)&(rank5);
	// if (PAWN_MOVES.to_ulong()!=0){
	// 	int index=bitScanForward((uint64_t)EP.to_ulong());
	// 	list+=positionMap[index-1]+"x"+positionMap[index+8]+"ep ";
	// }

	// PAWN_MOVES=(WP>>1)&(~fileA)&(EP)&(rank5);
	// if (PAWN_MOVES.to_ulong()!=0){
	// 	int index=bitScanForward((uint64_t)EP.to_ulong());
	// 	list+=positionMap[index+1]+"x"+positionMap[index+8]+"ep ";
	// }

	/****************************************
	ALL OTHER PAWN FUNCTION CODE
	*****************************************/
	PAWN_MOVES=(WP<<9)&blackPieces&~rank8&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-9]+"x"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(WP<<7)&blackPieces&~rank8&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-7]+"x"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(WP<<8)&emptySquares&~rank8;

	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-8]+"-"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(WP<<16)&emptySquares&(emptySquares<<8)&rank4;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-16]+"-"+positionMap[i]+" ";
		}
	}


	PAWN_MOVES=(WP<<7)&blackPieces&rank8&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-7]+"x"+positionMap[i]+"=Q ";
			list+=positionMap[i-7]+"x"+positionMap[i]+"=R ";
			list+=positionMap[i-7]+"x"+positionMap[i]+"=N ";
			list+=positionMap[i-7]+"x"+positionMap[i]+"=B ";
		}
	}

	PAWN_MOVES=(WP<<9)&blackPieces&rank8&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-9]+"x"+positionMap[i]+"=Q ";
			list+=positionMap[i-9]+"x"+positionMap[i]+"=R ";
			list+=positionMap[i-9]+"x"+positionMap[i]+"=N ";
			list+=positionMap[i-9]+"x"+positionMap[i]+"=B ";
		}
	}

	PAWN_MOVES=(WP<<8)&emptySquares&rank8;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i-8]+"-"+positionMap[i]+"=Q ";
			list+=positionMap[i-8]+"-"+positionMap[i]+"=R ";
			list+=positionMap[i-8]+"-"+positionMap[i]+"=N ";
			list+=positionMap[i-8]+"-"+positionMap[i]+"=B ";
		}
	}

	return list;
}


string Moves::possibleBP(char* board, bitset<64> BP, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	/********************************
	Variable Definition
	********************************/
	

	string list = "";
	bitset<64> PAWN_MOVES;

	/*******************************
	EN PASSANT CODE
	********************************/
	// PAWN_MOVES=(BP<<1)&(~fileH)&(EP)&(rank4);
	// if (PAWN_MOVES.to_ulong()!=0){
	// 	int index=bitScanForward((uint64_t)EP.to_ulong());
	// 	list+=positionMap[index-1]+"x"+positionMap[index-8]+"ep ";
	// }

	// PAWN_MOVES=(BP>>1)&(~fileA)&(EP)&(rank4);
	// if (PAWN_MOVES.to_ulong()!=0){
	// 	int index=bitScanForward((uint64_t)EP.to_ulong());
	// 	list+=positionMap[index+1]+"x"+positionMap[index-8]+"ep ";
	// }

	/****************************************
	ALL OTHER PAWN FUNCTION CODE
	*****************************************/
	PAWN_MOVES=(BP>>9)&whitePieces&~rank1&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+9]+"x"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(BP>>7)&whitePieces&~rank1&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+7]+"x"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(BP>>8)&emptySquares&~rank1;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+8]+"-"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(BP>>16)&emptySquares&(emptySquares>>8)&~rank1&rank5;

	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+16]+"-"+positionMap[i]+" ";
		}
	}

	PAWN_MOVES=(BP>>7)&whitePieces&rank1&~fileA;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+7]+"x"+positionMap[i]+"=q ";
			list+=positionMap[i+7]+"x"+positionMap[i]+"=r ";
			list+=positionMap[i+7]+"x"+positionMap[i]+"=n ";
			list+=positionMap[i+7]+"x"+positionMap[i]+"=b ";
		}
	}

	PAWN_MOVES=(BP>>9)&whitePieces&rank1&~fileH;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+9]+"x"+positionMap[i]+"=q ";
			list+=positionMap[i+9]+"x"+positionMap[i]+"=r ";
			list+=positionMap[i+9]+"x"+positionMap[i]+"=n ";
			list+=positionMap[i+9]+"x"+positionMap[i]+"=b ";
		}
	}

	PAWN_MOVES=(BP>>8)&emptySquares&rank1;
	for (int i=0;i<64;i++){
		if (PAWN_MOVES[i]){
			list+=positionMap[i+8]+"-"+positionMap[i]+"=q ";
			list+=positionMap[i+8]+"-"+positionMap[i]+"=r ";
			list+=positionMap[i+8]+"-"+positionMap[i]+"=n ";
			list+=positionMap[i+8]+"-"+positionMap[i]+"=b ";
		}
	}

	return list;
}

string Moves::possibleWR(bitset<64> WR, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> rook_positions;

	for (int i=0;i<64;i++){
		if (WR[i]){
			rook_positions.push_back(i);
		}
	}
	if ((int)rook_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)rook_positions.size();i++){
		bitset<64> rookMoves(HandVMoves(rook_positions[i]));
		bitset<64> ROOK_MOVES = rookMoves & (~whitePieces);
		vector<int> rook_destinations;

		for (int j=0;j<64;j++){
			if (ROOK_MOVES[j]){
				rook_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(rook_destinations.size());j++){
			if (board[rook_destinations[j]]=='*'){
				list+=positionMap[rook_positions[i]]+"-"+positionMap[rook_destinations[j]]+" ";
			}else{
				list+=positionMap[rook_positions[i]]+"x"+positionMap[rook_destinations[j]]+" ";
			}
		}

	}
	return list;
}

void Moves::updateGenPositions(){

	whitePieces = (WP|WR|WN|WB|WK|WQ);
	blackPieces = (BP|BR|BN|BB|BK|BQ);
	occupiedSquares = (whitePieces | blackPieces);
	emptySquares = (~occupiedSquares);
}

string Moves::possibleBR(bitset<64> BR, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> rook_positions;

	for (int i=0;i<64;i++){
		if (BR[i]){
			rook_positions.push_back(i);
		}
	}
	if ((int)rook_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)rook_positions.size();i++){
		bitset<64> rookMoves(HandVMoves(rook_positions[i]));
		bitset<64> ROOK_MOVES = rookMoves & (~blackPieces);
		vector<int> rook_destinations;

		for (int j=0;j<64;j++){
			if (ROOK_MOVES[j]){
				rook_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(rook_destinations.size());j++){
			if (board[rook_destinations[j]]=='*'){
				list+=positionMap[rook_positions[i]]+"-"+positionMap[rook_destinations[j]]+" ";
			}else{
				list+=positionMap[rook_positions[i]]+"x"+positionMap[rook_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleWB(bitset<64> WB, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> bishop_positions;


	for (int i=0;i<64;i++){
		if (WB[i]){
			bishop_positions.push_back(i);
		}
	}
	if ((int)bishop_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)bishop_positions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishop_positions[i]));
		bitset<64> BISHOP_MOVES = bishopMoves & (~whitePieces);
		vector<int> bishop_destinations;

		for (int j=0;j<64;j++){
			if (BISHOP_MOVES[j]){
				bishop_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(bishop_destinations.size());j++){
			if (board[bishop_destinations[j]]=='*'){
				list+=positionMap[bishop_positions[i]]+"-"+positionMap[bishop_destinations[j]]+" ";
			}else{
				list+=positionMap[bishop_positions[i]]+"x"+positionMap[bishop_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleBB(bitset<64> BB, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> bishop_positions;


	for (int i=0;i<64;i++){
		if (BB[i]){
			bishop_positions.push_back(i);
		}
	}
	if ((int)bishop_positions.size()==0){
		return list;
	}
	for (int i=0;i<(int)bishop_positions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishop_positions[i]));
		bitset<64> BISHOP_MOVES = bishopMoves & (~blackPieces);
		vector<int> bishop_destinations;

		for (int j=0;j<64;j++){
			if (BISHOP_MOVES[j]){
				bishop_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(bishop_destinations.size());j++){
			if (board[bishop_destinations[j]]=='*'){
				list+=positionMap[bishop_positions[i]]+"-"+positionMap[bishop_destinations[j]]+" ";
			}else{
				list+=positionMap[bishop_positions[i]]+"x"+positionMap[bishop_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleWQ(bitset<64> WQ, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> queen_positions;


	for (int i=0;i<64;i++){
		if (WQ[i]){
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
		bitset<64> QUEEN_MOVES = queenMoves & (~whitePieces);
		vector<int> queen_destinations;

		for (int j=0;j<64;j++){
			if (QUEEN_MOVES[j]){
				queen_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(queen_destinations.size());j++){
			if (board[queen_destinations[j]]=='*'){
				list+=positionMap[queen_positions[i]]+"-"+positionMap[queen_destinations[j]]+" ";
			}else{
				list+=positionMap[queen_positions[i]]+"x"+positionMap[queen_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleBQ(bitset<64> BQ, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> queen_positions;

	for (int i=0;i<64;i++){
		if (BQ[i]){
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
		bitset<64> QUEEN_MOVES = queenMoves & (~blackPieces);
		vector<int> queen_destinations;

		for (int j=0;j<64;j++){
			if (QUEEN_MOVES[j]){
				queen_destinations.push_back(j);
			}
		}

		for (int j=0;j<(int)(queen_destinations.size());j++){
			if (board[queen_destinations[j]]=='*'){
				list+=positionMap[queen_positions[i]]+"-"+positionMap[queen_destinations[j]]+" ";
			}else{
				list+=positionMap[queen_positions[i]]+"x"+positionMap[queen_destinations[j]]+" ";
			}
		}

	}
	return list;
}

string Moves::possibleWN(bitset<64> WN, char* board, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> knight_positions;
	

	for (int i=0;i<64;i++){
		if (WN[i]){
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

		bitset<64> KNIGHT_MOVES = (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~whitePieces);

		for (int j=0;j<64;j++){
			if (KNIGHT_MOVES[j]){
				knight_destinations.push_back(j);
			}
		}



		for (int j=0;j<(int)(knight_destinations.size());j++){
			if (board[knight_destinations[j]]=='*'){
				list+=positionMap[knight_positions[i]]+"-"+positionMap[knight_destinations[j]]+" ";
			}else{
				list+=positionMap[knight_positions[i]]+"x"+positionMap[knight_destinations[j]]+" ";
			}
		}


	}

	return list;
}

string Moves::possibleBN(bitset<64> BN, char* board,bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	string list="";
	vector<int> knight_positions;
	

	for (int i=0;i<64;i++){
		if (BN[i]){
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

		bitset<64> KNIGHT_MOVES = (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~blackPieces);

		for (int j=0;j<64;j++){
			if (KNIGHT_MOVES[j]){
				knight_destinations.push_back(j);
			}
		}



		for (int j=0;j<(int)(knight_destinations.size());j++){
			if (board[knight_destinations[j]]=='*'){
				list+=positionMap[knight_positions[i]]+"-"+positionMap[knight_destinations[j]]+" ";
			}else{
				list+=positionMap[knight_positions[i]]+"x"+positionMap[knight_destinations[j]]+" ";
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
	bitset<64> horizontal=(occupiedSquares.to_ulong() - (2*slider.to_ulong()))^(reverseBits(reverseBits(occupiedSquares.to_ulong())-(2 * reverseBits(slider.to_ulong()))));
	bitset<64> vertical = ((occupiedSquares & files[arrayPos % 8]).to_ulong() - (2 * slider.to_ulong())) ^ (reverseBits(reverseBits((occupiedSquares & files[arrayPos % 8]).to_ulong())-(2*reverseBits(slider.to_ulong()))));
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

	bitset<64> diagnolPossibilities = ((occupiedSquares & diagnol).to_ulong() - (2 * slider.to_ulong()))^(reverseBits(reverseBits((occupiedSquares&diagnol).to_ulong()) - (2 * reverseBits(slider.to_ulong()))));
	bitset<64> adiagnolPossibilities = ((occupiedSquares & adiagnol).to_ulong() - (2 * slider.to_ulong()))^(reverseBits(reverseBits((occupiedSquares&adiagnol).to_ulong()) - (2 * reverseBits(slider.to_ulong()))));
	return (uint64_t)((diagnolPossibilities&diagnol) | (adiagnolPossibilities&adiagnol)).to_ulong();

}

// string Moves::possibleCW(bool CWK, bool CWQ){
// 	string list = "";
// 	if (CWK){
// 		list+="O-Ow ";
// 	}
// 	if (CWQ){
// 		list+="O-O-Ow ";
// 	}
// 	return list;

// }
// string Moves::possibleCB(bool CBK, bool CBQ){
// 	string list = "";
// 	if (CWK){
// 		list+="O-Ob ";
// 	}
// 	if (CWQ){
// 		list+="O-O-Ob ";
// 	}
// 	return list;

// }

vector<string> Moves::possibleWMoves(char* board){
	string list;
	string move;
	vector<string> move_list;

	for (int i=63;i>=0;i--){
		if (board[i]=='P') WP.set(i);
		if (board[i]=='R') WR.set(i);
		if (board[i]=='N') WN.set(i);
		if (board[i]=='B') WB.set(i);
		if (board[i]=='Q') WQ.set(i);
		if (board[i]=='K') WK.set(i);
		if (board[i]=='p') BP.set(i);
		if (board[i]=='r') BR.set(i);
		if (board[i]=='n') BN.set(i);
		if (board[i]=='b') BB.set(i);
		if (board[i]=='q') BQ.set(i);
		if (board[i]=='k') BK.set(i);
	}

	updateGenPositions();









	list+=possibleWP(board, WP,whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleWQ(WQ, board, whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleWB(WB, board, whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleWR(WR, board, whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleWN(WN, board, whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleWK(WK, board, whitePieces,blackPieces,occupiedSquares,emptySquares);

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
		if(board[notationMap[destinationSquare]]=='k')
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

vector<string> Moves::possibleBMoves(char* board){
	string list;
	string move;
	vector<string> move_list;

	for (int i=0;i<64;i++){
		if (board[i]=='P') WP.set(i);
		if (board[i]=='R') WR.set(i);
		if (board[i]=='N') WN.set(i);
		if (board[i]=='B') WB.set(i);
		if (board[i]=='Q') WQ.set(i);
		if (board[i]=='K') WK.set(i);
		if (board[i]=='p') BP.set(i);
		if (board[i]=='r') BR.set(i);
		if (board[i]=='n') BN.set(i);
		if (board[i]=='b') BB.set(i);
		if (board[i]=='q') BQ.set(i);
		if (board[i]=='k') BK.set(i);
	}

	updateGenPositions();



	list+=possibleBP(board,BP,whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleBQ(BQ,board,whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleBB(BB,board,whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleBR(BR,board,whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleBN(BN,board,whitePieces,blackPieces,occupiedSquares,emptySquares)+
		possibleBK(BK,board,whitePieces,blackPieces,occupiedSquares,emptySquares);

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
		if(board[notationMap[destinationSquare]]=='k')
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



string Moves::possibleWK(bitset<64> WK, char* board, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	bitset<64> KING_MOVES;
	bitset<64> unsafe(~unsafeForWhite());
	vector<int> king_destinations;
	string list;
	int kingLocation = bitScanForward((uint64_t)WK.to_ulong());

	KING_MOVES |= (WK << 1) & ~fileH & ~whitePieces & unsafe;
	KING_MOVES |= (WK >> 1) & ~fileA & ~whitePieces & unsafe;
	KING_MOVES |= (WK >> 8) & ~whitePieces & unsafe;
	KING_MOVES |= (WK << 8) & ~whitePieces & unsafe;
	KING_MOVES |= (WK << 7) & ~fileA & ~whitePieces & unsafe;
	KING_MOVES |= (WK << 9) & ~fileH & ~whitePieces & unsafe;
	KING_MOVES |= (WK >> 7) & ~fileH & ~whitePieces & unsafe;
	KING_MOVES |= (WK >> 9) & ~fileA & ~whitePieces & unsafe;

	for (int i=0;i<64;i++){
		if (KING_MOVES[i]) king_destinations.push_back(i);
	}
	for (int j=0;j<(int)(king_destinations.size());j++){
		if (board[king_destinations[j]]=='*'){
			list+=positionMap[kingLocation]+"-"+positionMap[king_destinations[j]]+" ";
		}else{
			list+=positionMap[kingLocation]+"x"+positionMap[king_destinations[j]]+" ";
		}
	}
	/********************************************************


	INSERT CODE FOR CASTLING HERE


	*********************************************************/


	return list;
}

string Moves::possibleBK(bitset<64> BK, char* board, bitset<64> whitePieces, bitset<64> blackPieces, bitset<64> occupiedSquares, bitset<64> emptySquares){
	bitset<64> KING_MOVES;
	bitset<64> unsafe(~unsafeForBlack());
	vector<int> king_destinations;
	string list;
	int kingLocation = bitScanForward((uint64_t)BK.to_ulong());

	KING_MOVES |= (BK << 1) & ~fileH & ~blackPieces & unsafe;
	KING_MOVES |= (BK >> 1) & ~fileA & ~blackPieces & unsafe;
	KING_MOVES |= (BK >> 8) & ~blackPieces & unsafe;
	KING_MOVES |= (BK << 8) & ~blackPieces & unsafe;
	KING_MOVES |= (BK << 7) & ~fileA & ~blackPieces & unsafe;
	KING_MOVES |= (BK << 9) & ~fileH & ~blackPieces & unsafe;
	KING_MOVES |= (BK >> 7) & ~fileH & ~blackPieces & unsafe;
	KING_MOVES |= (BK >> 9) & ~fileA & ~blackPieces & unsafe;

	for (int i=0;i<64;i++){
		if (KING_MOVES[i]) king_destinations.push_back(i);
	}
	for (int j=0;j<(int)(king_destinations.size());j++){
		if (board[king_destinations[j]]=='*'){
			list+=positionMap[kingLocation]+"-"+positionMap[king_destinations[j]]+" ";
		}else{
			list+=positionMap[kingLocation]+"x"+positionMap[king_destinations[j]]+" ";
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
		if (WN[i]) knightPositions.push_back(i);
		if (WB[i]) bishopPositions.push_back(i);
		if (WQ[i]) queenPositions.push_back(i);
		if (WR[i]) rookPositions.push_back(i);
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

		KNIGHT_MOVES |= (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~whitePieces);
	}
	for (int i=0;i<(int)bishopPositions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishopPositions[i]));
		BISHOP_MOVES |= bishopMoves & (~whitePieces);
	}
	for (int i=0;i<(int)rookPositions.size();i++){
		bitset<64> rookMoves(HandVMoves(rookPositions[i]));
		ROOK_MOVES |= rookMoves & (~whitePieces);
	}
	for (int i=0;i<(int)queenPositions.size();i++){
		bitset<64> diagnolAttacks(DandAntiMoves(queenPositions[i]));
		bitset<64> lineAttacks(HandVMoves(queenPositions[i]));
		bitset<64> queenMoves=diagnolAttacks|lineAttacks;
		QUEEN_MOVES |= queenMoves & (~whitePieces);
	}

	WK_MOVES |= (WK << 1) & ~fileH & ~whitePieces;
	WK_MOVES |= (WK >> 1) & ~fileA & ~whitePieces;
	WK_MOVES |= (WK >> 8) & ~whitePieces;
	WK_MOVES |= (WK << 8) & ~whitePieces;
	WK_MOVES |= (WK << 7) & ~fileA & ~whitePieces;
	WK_MOVES |= (WK << 9) & ~fileH & ~whitePieces;
	WK_MOVES |= (WK >> 7) & ~fileH & ~whitePieces;
	WK_MOVES |= (WK >> 9) & ~fileA & ~whitePieces;

	unsafeForBlack |= (WP<<7)&~fileA;
	unsafeForBlack |= (WP<<9)&~fileH;
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
		if (BN[i]) knightPositions.push_back(i);
		if (BB[i]) bishopPositions.push_back(i);
		if (BQ[i]) queenPositions.push_back(i);
		if (BR[i]) rookPositions.push_back(i);
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

		KNIGHT_MOVES |= (spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |spot_7 | spot_8)&(~blackPieces);
	}
	for (int i=0;i<(int)bishopPositions.size();i++){
		bitset<64> bishopMoves(DandAntiMoves(bishopPositions[i]));
		BISHOP_MOVES |= bishopMoves & (~blackPieces);
	}
	for (int i=0;i<(int)rookPositions.size();i++){
		bitset<64> rookMoves(HandVMoves(rookPositions[i]));
		ROOK_MOVES |= rookMoves & (~blackPieces);
	}
	for (int i=0;i<(int)queenPositions.size();i++){
		bitset<64> diagnolAttacks(DandAntiMoves(queenPositions[i]));
		bitset<64> lineAttacks(HandVMoves(queenPositions[i]));
		bitset<64> queenMoves=diagnolAttacks|lineAttacks;
		QUEEN_MOVES |= queenMoves & (~blackPieces);
	}
	BK_MOVES |= (BK << 1) & ~fileH & ~blackPieces;
	BK_MOVES |= (BK >> 1) & ~fileA & ~blackPieces;
	BK_MOVES |= (BK >> 8) & ~blackPieces;
	BK_MOVES |= (BK << 8) & ~blackPieces;
	BK_MOVES |= (BK << 7) & ~fileA & ~blackPieces;
	BK_MOVES |= (BK << 9) & ~fileH & ~blackPieces;
	BK_MOVES |= (BK >> 7) & ~fileH & ~blackPieces;
	BK_MOVES |= (BK >> 9) & ~fileA & ~blackPieces;

	unsafeForWhite |= (BP>>7)&~fileH;
	unsafeForWhite |= (BP>>9)&~fileA;
	unsafeForWhite |= KNIGHT_MOVES;
	unsafeForWhite |= BISHOP_MOVES;
	unsafeForWhite |= ROOK_MOVES;
	unsafeForWhite |= QUEEN_MOVES;
	unsafeForWhite |= BK_MOVES;

	return (uint64_t) unsafeForWhite.to_ulong();
}

bool Moves::WKCheck(){
	bitset<64> unsafe(unsafeForWhite());
	bitset<64> check=WK & unsafe;
	if (check.to_ulong()!=0){
		return true;
	}
	return false;
}

bool Moves::BKCheck(){
	bitset<64> unsafe(unsafeForBlack());
	bitset<64> check=BK & unsafe;
	if (check.to_ulong()!=0){
		return true;
	}
	return false;
}


// int main(){
// 	Moves moveSet;
// 	bitset<64> n(6327421892U);

// 	// cout<<n<<endl;
// 	// bitset<64> reverser(reverseBits((uint64_t)n.to_ulong()));			//THIS IS THE SYNTAX FOR REVERSING 64 BIT BITSET
// 	// cout<<reverser<<endl;
// 	// bitset<64> rookMoves=moveSet.HandVMoves(45);
// 	// bitset<64> bishopMoves=moveSet.DandAntiMoves(2);

// 	// bitset<64> diagnolTemp=moveSet.adiagnols[4];
// 	// bitset<64> temp;
	

// 	// cout<<"MSB INDEX: "<<bitScanReverse((uint64_t)WK.to_ulong())<<endl;
// 	// cout<<"LSB INDEX: "<<bitScanForward((uint64_t)WP.to_ulong())<<endl;

// 	// bitset<64> temp(moveSet.possibleBK());

// 	// 	for (int i=63;i>=0;i--){
// 	// 	cout<<temp[i];
// 	// 	if (i%8==0)cout<<endl;
// 	// }
// 	char gameboard[64];
// 	for (int i=0;i<64;i++){
// 		gameboard[i]='*';
// 	}
	
	
// 	gameboard[0]='R';
// 	gameboard[1]='N';
// 	gameboard[2]='B';
// 	gameboard[3]='Q';
// 	gameboard[4]='K';
// 	gameboard[5]='B';
// 	gameboard[6]='N';
// 	gameboard[7]='R';
// 	for (int i=8;i<16;i++){
// 		gameboard[i]='P';
// 	}


// 	for (int i=48;i<56;i++){
// 		gameboard[i]='p';
// 	}
// 	gameboard[56]='r';
// 	gameboard[57]='n';
// 	gameboard[58]='b';
// 	gameboard[59]='q';
// 	gameboard[60]='k';
// 	gameboard[61]='b';
// 	gameboard[62]='n';
// 	gameboard[63]='r';
// 	// gameboard[18]='N';
// 	// gameboard[21]='P';
// 	// gameboard[24]='P';
// 	// gameboard[25]='K';
// 	// gameboard[27]='Q';
// 	// gameboard[29]='p';
// 	// gameboard[34]='P';
// 	// gameboard[35]='P';
// 	// gameboard[40]='b';
// 	// gameboard[41]='p';
// 	// gameboard[44]='R';
// 	// gameboard[50]='p';
// 	// gameboard[57]='k';
// 	// gameboard[59]='r';
// 	// gameboard[62]='q';

// 	cout<<endl;
// 	for (int i=63;i>=0;i--){
// 		cout<<gameboard[i];
// 		if (i%8==0)cout<<endl;
// 	}
// 	cout<<endl;

// 	// for (int i=0;i<64;i++){
// 	// 	if (i%8==0)cout<<endl;

// 	// 	cout<<gameboard[i];
// 	// }
// 	// cout<<endl;

// 	bitset<64> EP;
// 	bitset<64> WP;
// 	bitset<64> BP;


// 	vector<string> whiteMoves = moveSet.possibleBMoves(EP, gameboard);
// 	for (int i=0;i<(int)whiteMoves.size();i++){
// 		cout<<whiteMoves[i]<<endl;
// 	}


// 	// string list = moveSet.possibleBPEP(EP,BP);
// 	// cout<<list<<endl;

// 	// moveSet.boardRep->drawBoard();
// 	// moveSet.makeMove("g1-h3");
// 	// moveSet.boardRep->drawBoard();
// 	// moveSet.makeMove("h7-h5");
// 	// moveSet.boardRep->drawBoard();
// 	// moveSet.makeMove("g1-f3");
// 	// moveSet.boardRep->drawBoard();

// 	// if (moveSet.WKCheck()){
// 	// 	cout<<"CHECK!"<<endl;
// 	// }else{
// 	// 	cout<<"NO CHECK!"<<endl;
// 	// }
// 	// moveSet.makeMove("b1-c3");
// 	// moveSet.boardRep->drawBoard();

// 	// bitset<64> temp = ~moveSet.fileA & ~moveSet.fileB;

// 	// for (int i=63;i>=0;i--){
// 	// 	cout<<temp[i];
// 	// 	if (i%8==0)cout<<endl;

// 	// }

// }