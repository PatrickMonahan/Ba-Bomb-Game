#include "creature.h"
#include <string.h>
#include "textureTGA.h"
#include "model3DS.h"
#include "math.h"
#include <GL/glut.h>	// GLUT
#include "imageloader.h"

#define PI 3.14159265

using namespace std;


//constructor for model
creature::creature(float x, float y, float z){
	bomb = new model3DS("./world/mrb2.3ds", 1);
	wick = new model3DS("./world/wick4.3ds", 1);
	leftFoot = new model3DS("./world/foot.3ds", 4);
	rightFoot = new model3DS("./world/foot.3ds", 4);
	winder = new model3DS("./world/winder.3ds", 0.75);
	leftEye = new model3DS("./world/eye.3ds", 0.4);
	rightEye = new model3DS("./world/eye.3ds", 0.4);
	pos_x = x;
	pos_y = y;
	pos_z = z;
	rotation = 0;
	dir_x = 0.0;
	dir_y = 0.0;
	dir_z = -1.0;
	bob = 0;
	bobmult = 1;
	explode = false;
	goingUp = true;
	explodeIncrement = 0;
	Image* image = loadBMP("textures/face.bmp");
	textureId = loadTexture(image);
	winderAngle = 0;
	walkingAngle = 0.0;
	walkfb = true;
}

//translate and rotate object into correct position, then draw
void creature::draw(){
	winderAngle += 1;
	
	//increment values to use for explode tranlates;	
	if(explode == true && goingUp == true){
		explodeIncrement += 0.014;
		if(explodeIncrement > 6){
			goingUp = false;
		}
	}else if(explode == true && goingUp == false){
		if(explodeIncrement > 0){		
			explodeIncrement -= 0.017;
		}
	}

	glTranslatef(pos_x,pos_y,pos_z);	
	glRotatef(rotation,0,1,0);	

	glPushMatrix();
	glColor3f(1,0,0);	

	//set animation rotations and translations for when an 
	glTranslatef(0,explodeIncrement - 2,0);
	glRotatef(explodeIncrement*25,1,0,0);
	glRotatef(explodeIncrement*35,0,0,1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);		
			glEnable(GL_COLOR_MATERIAL);
						
			glColor3f(0,0,0);	
			bomb->draw();			//back wall
			
					
			glPushMatrix();
				glColor3f(1,1,1);
//				glColor3f(0.7,0.7,0.7);				
				glTranslatef(0,1,0);
				wick->draw();
			glPopMatrix();

			glPushMatrix();
				glColor3f(1,1,0);
				glRotatef(walkingAngle,1,0,0);				
				glTranslatef(-0.5,-1.1,-0.2);
				glRotatef(180,0,1,0);				
				leftFoot->draw();
			glPopMatrix();

			glPushMatrix();
				glColor3f(1,1,0);
				glRotatef(-walkingAngle,1,0,0);
				glTranslatef(0.5,-1.1,-0.2);
				glRotatef(180,0,1,0);				
				rightFoot->draw();
			glPopMatrix();

			glPushMatrix();
				glColor3f(1,1,0);
				glTranslatef(0,0.3,1.1);
				glRotatef(75,1,0,0);
				glRotatef(winderAngle,0,1,0);
				winder->draw();
			glPopMatrix();

			glPushMatrix();
				glColor3f(1,1,1);
				glTranslatef(0.4,0.2,-0.9);
				glRotatef(10,1,0,0);				
				glRotatef(90,0,1,0);
				leftEye->draw();
			glPopMatrix();

			glPushMatrix();
				glColor3f(1,1,1);
				glTranslatef(-0.4,0.2,-0.9);
				glRotatef(10,1,0,0);				
				glRotatef(90,0,1,0);
				leftEye->draw();
			glPopMatrix();
		/*	glBegin(GL_QUADS);	
				glNormal3f(0.0, 2.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-1.0f, -1.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-1.0f, 1.0f, -2.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(1.0f, 1.0f, -2.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(1.0f, -1.0f, -2.0f);
	
			glEnd();*/


//	glNormal3f(0,1,0);	
//	head->draw();
	
/*
		glScalef(2.0,2.0,2.0);		
		glTranslatef(0,-0.75,0);		
		glutSolidCube(1);
*/
	glPopMatrix();
	



}

void creature::walk(){
	//walk the creatures using walkingAngle to rotate the legs
	if(walkfb == true){
		walkingAngle+=14;
		if(walkingAngle > 38){
			walkfb = false;	
		}	
	}else{
		walkingAngle-= 14;
		if(walkingAngle < -38){
			walkfb = true;
		}
	}	
}

//rotate model and calculate direction vector
void creature::rotate(char* leftright){
	if(strcmp(leftright,"left")==0){
		if(rotation == 350){
			rotation = 0;
		}else{
			rotation += 10;	
		}	
	}else{
		if(rotation == 0){
			rotation = 350;		
		}else{
			rotation -= 10;
		}	
	}
	//calculate the new direction vector
	calculateDirection();
	walk();
}

//move model in the direction of the direction matrix
void creature::move(char* forwardback){
	float tempx;
	float tempz;	
	if(strcmp(forwardback,"forward")==0){
		tempx = pos_x + 2*dir_x;				
		tempz = pos_z + 2*dir_z;		
	}else{
		tempx = pos_x - dir_x;
		tempz = pos_z - dir_z;	
	}
	if(tempx > 0 && tempx < 100 && tempz < 0 && tempz > -100){
		pos_x = tempx;
		pos_z = tempz;
	}
	walk();
	
}


//calcuate the direction vector of the model
void creature::calculateDirection(){
	if(rotation == 180){
		dir_z = 1;
		dir_x = 0;
	}else if(rotation == 90){
		dir_z = 0;
		dir_x = -1;
	}else if(rotation == 270){
		dir_z = 0;
		dir_x = 1;
	}else{	
	//new coords == transform (0,0,-1) by the rotation angle
	dir_x = (-1)*sin((rotation*PI)/180);
	dir_z = (-1)*cos((rotation*PI)/180);
	}
//	cout<<"Dir_x: "<<dir_x<<endl;
//	cout<<"Dir_z: "<<dir_z<<endl;
}

float creature::getPosition(char* coord){
	if(strcmp(coord,"x")==0){
		return pos_x;
	}else if(strcmp(coord,"z")==0){
		return pos_z;
	}else{
		return pos_y;
	}	
}

float creature::getDirection(char* coord){
	if(strcmp(coord,"x")==0){
		return dir_x;
	}else if(strcmp(coord,"z")==0){
		return dir_z;
	}else{
		return dir_y;
	}	
};

void creature::exploded(){
	explode = true;
}


//Makes the image into a texture, and returns the id of the texture
GLuint creature::loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);              
	return textureId; //Returns the id of the texture

};
