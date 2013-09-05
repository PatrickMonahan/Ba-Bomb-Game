//#include <windows.h>	// for timeGetTime()
//#include <mmsystem.h>	// ditto
#include <iostream>		// I/O
using namespace std;
#include <GL/glut.h>	// GLUT
#include <sys/time.h>
//#include <gl/glu.h>		// for gluPerspective & gluLookAt
#include "model3DS.h" // 3DS model support
#include <math.h>
#include <string.h>
#include "textureTGA.h"
#include "creature.h"
#include "grid.h"
#include "square.h"
#include "imageloader.h"
#include <stdlib.h>
#include <stdarg.h>
#include <sys/time.h>

creature *teddy;
grid* world;
square* mysquare;
# define PI  3.14159265

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keypress(unsigned char key, int x, int y);
void setViewport(int width, int height);

GLuint      textureId;
GLuint	textureId2;
GLuint	texutureId3;
GLuint	menuTexture;
GLuint  creditsTexture;
GLuint castleTexture;
GLuint floorTexture;
bool firstPress = true; //check if this is the first time a square is being pressed
bool inPlay = false;
char* view = "chase";
char* menu = "menu";

int startTime, currentTime, yourTime;

int         tetrahedronAngle=0;
bool		wireframe=false;
int         windowId;

float 	forwardback = 0;
float updown = 0;
float turn = 0;
timeval tim;

float cam_pos_x,cam_pos_y,cam_pos_z;


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);              
	return textureId; //Returns the id of the texture
}




void setCamera(int zoomout,char* view){
		
		float look_x,look_y,look_z;

		//postion the camera behind the model by zoomout * the models direction vector	
		cam_pos_x = (teddy->getPosition("x")) - zoomout*teddy->getDirection("x");// - zoomout*teddy->getDirection("x"); 
		cam_pos_y = 2;
		cam_pos_z = (teddy->getPosition("z")) - zoomout*teddy->getDirection("z");// - zoomout*teddy->getDirection("z"); 

		look_x = (teddy->getPosition("x"));
		look_y = (teddy->getPosition("y"));
		look_z = (teddy->getPosition("z"));

	if(world->isDead() || world->isWon()){
		teddy->exploded();
		cam_pos_x = (teddy->getPosition("x")) + zoomout*teddy->getDirection("x");// - zoomout*teddy->getDirection("x"); 
		cam_pos_y = 2;
		cam_pos_z = (teddy->getPosition("z")) + zoomout*teddy->getDirection("z");// - zoomout*teddy->getDirection("z"); 
		gluLookAt( cam_pos_x , cam_pos_y, cam_pos_z ,   look_x, look_y, look_z,   0,1,0);	
	}else if(strcmp(view,"chase") == 0){	
		gluLookAt( cam_pos_x , cam_pos_y, cam_pos_z ,   look_x, look_y, look_z,   0,1,0);
	}else if(strcmp(view,"top")==0){
		gluLookAt( 50,120,-50 ,   50, 0, -51,   0,1,0);
	}else if(strcmp(view,"middle")==0){
		gluLookAt( 50,20,-50 ,   look_x, look_y, look_z,   0,1,0);
	}else if(strcmp(view,"menu")){
		gluLookAt(0.0, 0.0, 5.0,    0.0, 0.0, -1.0,    0.0, 1.0, 0.0);
	}
		
}


void printw()
{
    // Temporary disable lighting
    glDisable( GL_LIGHTING );
    
	//setting the time to print to the screen
	//while(timeGetTime()-lastTickCount<16);

	timeval tim;
	gettimeofday(&tim,NULL); 
	currentTime = tim.tv_sec;; //+(tim.tv_usec/1000000000);;//->get milliseconds
	int gameTime = currentTime - startTime;
	
	glEnable(GL_COLOR_MATERIAL);
   	glMatrixMode( GL_PROJECTION );
    	glPushMatrix();
	const char *message, *timeMessage;        
	glLoadIdentity();
        glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );
	glColor3f( 1.0f, 1.0f, 1.0f );
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glLoadIdentity();
	int index;
        if(world->isDead() == false && world->isWon() == false){
		yourTime = gameTime;	
	    glTranslatef( 60.0f, 70.0f, 0.0f );
            glScalef( 0.1f, 0.1f, 0.1f );
		
		string s;		
		stringstream out;
		out<<gameTime;
		//cout<<"gametime: "<<gameTime<<endl;
		
		
		message = (out.str()+'\0').c_str();				

		index = 0;
		
        	while( *( message + index++ ) != '\0' )
	                glutStrokeCharacter( GLUT_STROKE_ROMAN, *( message + index -1 ));

	}else if(world->isDead()){
		
		glTranslatef( -98.0f, 60.0f, 0.0f );
            	glScalef( 0.1f, 0.1f, 0.1f );
		message = "B-B-B-BOMBS AWAY...LOSER!! ";            
	
		index = 0;
		
         	while( *( message + index++ ) != '\0' )
                	glutStrokeCharacter( GLUT_STROKE_ROMAN, *( message + index -1 ));
		
		
	}else{
		glTranslatef( -30.0f, 60.0f, 0.0f );
            	glScalef( 0.1f, 0.1f, 0.1f );
		message = "WINNER!!!!!!";            
	
		index = 0;
		
         	while( *( message + index++ ) != '\0' )
                	glutStrokeCharacter( GLUT_STROKE_ROMAN, *( message + index -1 ));
	
	
		string s;		
		stringstream out;
		out<<yourTime;
		//cout<<"gametime: "<<gameTime<<endl;
		
		
		timeMessage = (out.str()+'\0').c_str();				

		index = 0;
		glTranslatef(-980,-440,0);
		
        	while( *( timeMessage + index++ ) != '\0' )
	                glutStrokeCharacter( GLUT_STROKE_ROMAN, *( timeMessage + index -1 ));

		message = " seconds YOU ROCK!";            
	
		index = 0;
		
         	while( *( message + index++ ) != '\0' )
                	glutStrokeCharacter( GLUT_STROKE_ROMAN, *( message + index -1 ));


	}	
            
        glPopMatrix();

		glMatrixMode( GL_PROJECTION );
    glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

    // Reenable lighting
    //glEnable( GL_LIGHTING );
}


void drawWorld(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	world->draw(cam_pos_x,cam_pos_y,cam_pos_z);

//	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//draw left plane
//	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	
	//Bottom
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glColor3f(1.0f, 1.0f, 1.0f);
	
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//back wall
	glBegin(GL_QUADS);	
		glNormal3f(0.0, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, -2.0f, -100.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 50.0f, -100.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(100.0f, 50.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(100.0f, -2.0f, -100.0f);
	
	glEnd();

	glBindTexture(GL_TEXTURE_2D, castleTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		
	//front wall
	glBegin(GL_QUADS);	
		glNormal3f(0.0, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, -10.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 50.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(100.0f, 50.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(100.0f, -10.0f, 0.0f);
	
	glEnd();
	
	

	glBindTexture(GL_TEXTURE_2D, textureId2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glDisable(GL_TEXTURE_2D);


	glBegin(GL_QUADS);	
		glNormal3f(0.0, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, -2.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 50.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.0f, 50.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.0f, -2.0f, -100.0f);
	
	glEnd();

	glBegin(GL_QUADS);	
		glNormal3f(0.0, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(100.0f, -2.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(100.0f, 50.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(100.0f, 50.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(100.0f, -2.0f, -100.0f);
	
	glEnd();


	//ground
	//right wall

	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);	
		glNormal3f(0.0, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, -2.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, -2.0f, -100.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(100.0f, -2.0f, -100.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(100.0f, -2.0f, 0.0f);

	glEnd();	


//	glutSolidCube(1);
	glDisable(GL_TEXTURE_2D);

}

void drawMenu(){
	
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_TEXTURE_2D);
	
	if(strcmp(menu,"menu")==0){
		glBindTexture(GL_TEXTURE_2D,menuTexture);
	}else{
		glBindTexture(GL_TEXTURE_2D,creditsTexture);			
	}	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);	
		glNormal3f(0.0, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-5.0f, -5.0f, -10.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-5.0f, 4.6f, -10.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(5.0f, 4.6f, -10.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(5.0f, -5.0f, -10.0f);
	
	glEnd();
}

void mouse(int button, int state, int x, int y){
		
	if(strcmp(menu,"menu")==0){
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
			if(x >149 && x < 491 && y > 259 && y < 307){
				inPlay = true;
				gettimeofday(&tim,NULL); 
				startTime = tim.tv_sec; 	
			}else if(x > 280 && x < 380 && y > 405 && y < 477){
				exitScene();		
			}else if(x > 257 && x < 401 && y > 345 && y < 385){
				menu = "credits";
			}	
		}	
	}
	if(strcmp(menu,"credits")==0){
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
			if(x >64 && x < 379 && y > 417 && y < 477){
				 menu = "menu";	
			}	
		}	
	}
}

void renderScene(){
        
	
		// Reset Modelview matrix      	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	
		//set ambient light
		GLfloat ambientColor[] = {0.2f,0.2f,0.2f,1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
		glEnable(GL_LIGHTING);
	if(inPlay == true){
		//Draw the world and the game	
	
		setCamera(14,view);
		
	    	//set the square the player's on!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		world->smashBox(teddy->getPosition("x"),teddy->getPosition("z"));		//set which square the players on!	

	

		drawWorld();

	//	glColor3f(1.0,0.0,0.0);
		teddy->draw();
		printw();
	}else{
		//draw the game menu
		setCamera(0,"menu");	
		drawMenu();	
	}
	//displayText();	

    // Swap double buffer for flicker-free animation
    glutSwapBuffers();
        
}

void updateScene(){
	
    glutPostRedisplay();

}

void keypress(unsigned char key, int x, int y){
	
switch (key)
   	{
		case 27 : exitScene();
      		case 'a' : if(world->isDead() == false && !(world->isWon())){teddy->rotate("left");};break;
      		case 'd' : if(world->isDead() == false && !(world->isWon())){teddy->rotate("right");};break;
      		case 'w' : if(world->isDead() == false && !(world->isWon())){teddy->move("forward");};break;
      		case 's' : if(world->isDead() == false && !(world->isWon())){teddy->move("back");};break;
		case ' ' : if(!(world->isDead())&& !(world->isWon())){world->setMaybeBomb(teddy->getPosition("x"),teddy->getPosition("z"));};break;      		
		case 'u' : view = "chase";break;
		case 'i' : view = "top" ;break;
      		case 'o' : view = "middle" ;break;
      		case 'r' : inPlay = false; teddy = new creature(50,1.5,-50);world = new grid();break;
		
   	}	//

}

void setupScene(){

	std::cout<<"Initializing scene..."<<std::endl;
    	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
//	glEnable(GL_NORMALISE);
	glShadeModel(GL_SMOOTH);
	//Load the teddy model
	teddy = new creature(50,1.5,-50);
	world = new grid();	
	
	  // Generate GL texture ID & load texture
		
	Image* image = loadBMP("textures/mario2.bmp");
	textureId = loadTexture(image);
	Image* image2 = loadBMP("textures/grass.bmp");
	textureId2 = loadTexture(image2);
	Image* image3 = loadBMP("textures/menu.bmp");	
	menuTexture = loadTexture(image3);	
	Image* image4 = loadBMP("textures/credits.bmp");	
	creditsTexture = loadTexture(image4);	
	Image* image5 = loadBMP("textures/castle.bmp");	
	castleTexture = loadTexture(image5);	
	Image* image6 = loadBMP("textures/pavement.bmp");	
	floorTexture = loadTexture(image6);	
	delete image6;	
	delete image5;	
	delete image4;	
	delete image2;
	delete image;
	delete image3;  

}

void exitScene(){

    std::cout<<"Exiting scene..."<<std::endl;

    // Close window
    glutDestroyWindow(windowId);

    // Free any allocated memory

    // Exit program
    exit(0);
}

void setViewport(int width, int height) {

    // Work out window ratio, avoid divide-by-zero
    if(height==0)height=1;
	float ratio = float(width)/float(height);

	// Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Fill screen with viewport
	glViewport(0, 0, width, height);

	// Set a 45 degree perspective
	gluPerspective(45, ratio, .1, 2000);

}

int main(int argc, char *argv[]){
        
    // Initialise OpenGL
    glutInit(&argc, argv); 
	
	//+(tim.tv_usec/1000000000);;//->get milliseconds


    // Set window position, size & create window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(640,480);
	windowId = glutCreateWindow("Lab 4: Loading a textured model");
    
    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keypress);
    if(inPlay == false){    
	glutMouseFunc(mouse);
    }   
	 // Setup OpenGL state & scene resources (models, textures etc)
	setupScene();

    // Show window & start update loop
    glutMainLoop();    

    return 0;
    
}
