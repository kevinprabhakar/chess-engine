#include "ChessBoard.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iterator>

ChessBoard::ChessBoard(){

	setBoard();

	generateNotationMap();
	updateGenPositions();

	


}

void ChessBoard::setBoard(){
	for (int i=8;i<16;i++){
		WP.set(i);
	}
	WR.set(0);
	WN.set(1);
	WB.set(2);
	WK.set(3);
	WQ.set(4);
	WB.set(5);
	WN.set(6);
	WR.set(7);




	for (int i=48;i<56;i++){
		BP.set(i);
	}
	BR.set(56);
	BN.set(57);
	BB.set(58);
	BK.set(59);
	BQ.set(60);
	BB.set(61);
	BN.set(62);
	BR.set(63);






}

void ChessBoard::generateNotationMap(){

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
	
	
	


ChessBoard::~ChessBoard(){

}

void ChessBoard::drawBoard(){
	for (int i=0;i<64;i++){
		gameBoard[i]='*';
	}
	for (int i=63;i>=0;i--){
		if (WP[i]) gameBoard[i]='P';
		if (WR[i]) gameBoard[i]='R';
		if (WN[i]) gameBoard[i]='N';
		if (WB[i]) gameBoard[i]='B';
		if (WK[i]) gameBoard[i]='K';
		if (WQ[i]) gameBoard[i]='Q';
		if (BP[i]) gameBoard[i]='p';
		if (BR[i]) gameBoard[i]='r';
		if (BN[i]) gameBoard[i]='n';
		if (BB[i]) gameBoard[i]='b';
		if (BK[i]) gameBoard[i]='k';
		if (BQ[i]) gameBoard[i]='q';
	}
	for (int i=63;i>=0;i--){
		cout<<gameBoard[i];
		if (i%8==0)cout<<endl;

	}
}

void ChessBoard::updateGenPositions(){

	whitePieces = (WP|WR|WN|WB|WK|WQ);
	blackPieces = (BP|BR|BN|BB|BK|BQ);
	occupiedSquares = (whitePieces | blackPieces);
	emptySquares = (~occupiedSquares);
}


