#ifndef SEARCH_H_
#define SEARCH_H

class Search{
	public:
		Search();
		~Search();
		double negaMax(char* board);
		void makeMove(char& board, string move);
		void undoMove(char& board, string lastMove, char pieceTaken, bool white);
		double staticEvaluate(char* board);
		void makeBestMove(char* board, bool white, int plyDepth);



		
	private:
		Moves* moveSet;








	protected:
};



#endif