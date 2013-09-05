#ifndef SQUARE_H
#define SQUARE_H
#include "model3DS.h"
#include "imageloader.h"

class square{
	private:
		//int numSquares;
		int xpos,zpos;	//position of square on board
		int value;	//0 = no bomb, 1 - 8 = number of bombs around square, 9 = THIS IS A BOMB
		model3DS* squareModel;
		model3DS* numModel;		
		bool pressed;
		bool playerOn; 
		int *numDirection;
		GLuint loadTexture(Image* image);
		GLuint textureId; 
		GLuint bombTexture;
		void drawRegularBox();
		void fallApart();
		int explodeCount;
		int explosionRotation;	
		bool maybeBomb;			
		//vector<vector<int> > squaresArray;
	public:
		square(int xpoz,int zpos);
		void draw(float dirx, float diry, float dirz);
		void setPlayer();
		bool isPlayer();
		void isntPlayer();
		void setPressed();
		bool isPressed();
		void setValue(int num);
		int getValue();
		void incrValue();
		void setNumModel(int num);
		bool smashbox(int xpos, int zpos);
		void setMaybeBomb();
		void unsetMaybeBomb();

		//void generateBombs(int xcurr,int zcurr);
};
#endif
