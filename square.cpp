#include "square.h"
#include <math.h>
#include "imageloader.h"

#define PI 3.14159265
using namespace std;
square::square(int x, int z){
	xpos = x + 4;
	zpos = z + 4;
	pressed = false;
	value = 0;
	explodeCount = 0;
	explosionRotation = 0;
	playerOn = false;
	squareModel = new model3DS("./world/box.3ds",1.2);
	cout<<"explodeCount: "<<explodeCount<<endl;
	Image* image = loadBMP("textures/crate.bmp");
	textureId = loadTexture(image);
	delete image;	
	Image* image2 = loadBMP("textures/bombImage.bmp");
	bombTexture = loadTexture(image2);
	delete image2;
	maybeBomb =  false;
	//teddyModel = new model3DS("./teddy/teddy.3ds", .08);
};

void square::setNumModel(int num){
	cout<<"loading in texture for: "<<num<<endl;	
	switch (num)
   	{
		case 1 : squareModel = new model3DS("./world/num1.3ds",1); break;
		case 2 : squareModel = new model3DS("./world/num2.3ds",1); break;	
		case 3 : squareModel = new model3DS("./world/num3.3ds",1); break;
		case 4 : squareModel = new model3DS("./world/num4.3ds",1); break;
		case 5 : squareModel = new model3DS("./world/num5.3ds",1); break;
		case 6 : squareModel = new model3DS("./world/num6.3ds",1); break;
		case 7 : squareModel = new model3DS("./world/num7.3ds",1); break;
		case 8 : squareModel = new model3DS("./world/num8.3ds",1); break;
		case 9 : squareModel = new model3DS("./world/bomb.3ds",3); break;
	}
};



void square::drawRegularBox(){
	if(maybeBomb){	
		glBindTexture(GL_TEXTURE_2D, bombTexture);
	}else{
		glBindTexture(GL_TEXTURE_2D, textureId);	
	}			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			
						//back wall
			glBegin(GL_QUADS);	
				glNormal3f(0.0, 2.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, -2.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, -2.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 0.0f, -2.0f);
	
			glEnd();

			//left wall

			glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(0.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0.0f, 0.0f, -2.0f);
	
			glEnd();

			//right wall	
			glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(2.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(2.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 0.0f, -2.0f);
	
			glEnd();

	
			//front wall
			glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 0.0f, 0.0f);
	
			glEnd();

			//top wall
			glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, -2.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, 0.0f);
	
			glEnd();
	
}


void square::fallApart(){

		if(explodeCount < 250){	
			explodeCount++;		
			explosionRotation+= 1;
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			
			
		glRotatef(explosionRotation,1,0,0);			//back wall
		glPushMatrix();			
			glRotatef(explosionRotation,1,0,0);	
			glBegin(GL_QUADS);
				glNormal3f(0.0, 2.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, -2.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, -2.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 0.0f, -2.0f);
	
			glEnd();
		glPopMatrix();
			//left wall

		glRotatef(explosionRotation,0,0,1);	
		glPushMatrix();		
		glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(0.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0.0f, 0.0f, -2.0f);
	
			glEnd();
		glPopMatrix();

			//right wall	
			glRotatef(explosionRotation,0,0,1);
			glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(2.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(2.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 0.0f, -2.0f);
	
			glEnd();

	
			//front wall
			glRotatef(explosionRotation,1,0,0);
			glBegin(GL_QUADS);	
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 0.0f, 0.0f);
	
			glEnd();

			//top wall
			glBegin(GL_QUADS);	
				glTranslatef(0,0,0);
				glNormal3f(0.0, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(0.0f, 2.0f, -2.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, -2.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(2.0f, 2.0f, 0.0f);
	
			glEnd();
	

	}
	
}


void square::draw(float dirx, float diry, float dirz){
	//calculate the direction of the number
	

	float dvectorx =  xpos*10 - dirx;
	float dvectory = -diry;
	float dvectorz = -(zpos*10 - dirz);	
	
	//normalise the desired direction vector;
	float dvect_length = sqrt(dvectorx*dvectorx + dvectory*dvectory + dvectorz*dvectorz);
	dvectorx = dvectorx/dvect_length;
	dvectory = dvectory/dvect_length;
	dvectorz = dvectorz/dvect_length;	
	
	float lengthXZ = sqrt(dvectorx*dvectorx + dvectorz*dvectorz);
			
	float angleY = (acos(dvectorz/lengthXZ))*180/PI;//angle to rotate about the x-axis
//	cout<<xpos<<"  "<<zpos<<"   angleY: "<<angleY<<endl;

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	glPushMatrix();

		glTranslatef(xpos,-1,-zpos);
		glRotatef(90,0,1,0);		
		if(!pressed){	
			drawRegularBox();	
		}else if(value == 9){
			fallApart();			
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(1,0,0);				
			squareModel->draw();
			
		
		}else if(value > 0){
			
			fallApart();
			glEnable(GL_COLOR_MATERIAL);
			glTranslatef(0,1,0);		
			glColor3f(1,0,0);				
			squareModel->draw();
		}else{
			fallApart();
		}
	glPopMatrix();
};





void square::setPressed(){
	if(pressed == false)	
	setNumModel(value);
//	cout<<"square value: "<<value<<endl;	
	pressed = true;
	cout<<"explodeCount: "<<explodeCount<<endl;
};

bool square::isPressed(){
	return pressed;
};

void square::setValue(int num){
	value = num;	
};

void square::setMaybeBomb(){
		
	maybeBomb = !maybeBomb;
	
}
 


int square::getValue(){
	return value;
};

void square::incrValue(){
	if(value < 9){
		value++;	
	}
};

bool square::smashbox(int x,int z){

	if(pressed == false && x > xpos - 3 && x < xpos + 3 && z > zpos - 3 && z < zpos +3){
		cout<<"Smashed box with value "<<value<<endl;
		setPressed();
		return true;
	}else{
		return false;	
	}
};

//Makes the image into a texture, and returns the id of the texture
GLuint square::loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);              
	return textureId; //Returns the id of the texture

};
