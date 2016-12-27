#include "Moves.h"
#include <iostream>
#include <math.h>
#include <cassert>

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

string Moves::possibleMoves(string history){
	string list;
	list+=possibleWP(history)+possibleWQ()+possibleWB()+possibleWR()+possibleWN();

	return list;

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

	cout<<moveSet.possibleMoves("e2-e4")<<endl;
	moveSet.boardRep->drawBoard();

	// bitset<64> temp = ~moveSet.fileA & ~moveSet.fileB;

	// for (int i=63;i>=0;i--){
	// 	cout<<temp[i];
	// 	if (i%8==0)cout<<endl;

	// }

}