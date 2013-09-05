#ifndef GRID_H
#define GRID_H
#include "square.h"
#include <vector>
using std::vector;

class grid{
	private:
		//int numSquares;
		int gridsize;
		int playerx,playerz;
		vector<vector<square*> > squares;
		bool boardMade;
		bool deadsies;
		int numBoxes;
		int numBombs;
		bool won;
	public:
		grid();
		void draw(float x,float y, float z);	
		void playerSquare(int xpos, int zpos);
		void generateGame();
		void pressSquare();
		void smashBox(int xpos, int zpos);
		void zeroPressed(int playerx,int playerz);
		bool isDead();
		void setMaybeBomb(int xpos, int zpos);
		bool isWon();
};
#endif
