#ifndef ADD_H
#define ADD_H
#include "model3DS.h"
#include "imageloader.h"

class creature{
	private:
		//int[] direction;
		//int[] bearpos;
		int rotation;
		float pos_x,pos_y,pos_z;
		float dir_x,dir_y,dir_z;
		float bob; //value to translate to give "bobing effect"
		float bobmult;
		model3DS *body, *bomb,*wick,*leftFoot,*rightFoot, *winder, *leftEye, *rightEye;
		bool explode;
		bool goingUp;
		float explodeIncrement;
		GLuint loadTexture(Image* image);
		GLuint textureId; 
		Image* image; 
		int winderAngle;
		float walkingAngle;
		bool walkfb;
		
	public:
		creature(float x,float y,float z);		
		void draw();	
		void rotate(char* leftright);
		void move(char* forwardback);	
		void calculateDirection();
		float getPosition(char* coord);
		float getDirection(char* coord);
		void exploded();
		void walk();
		//int[]	getpos(int x){}
		//int[]	calculateDirection(){}
		//void	rotateCreature();
		//void 	moveBear(String movement)
};


#endif
