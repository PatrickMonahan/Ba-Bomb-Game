#include "grid.h"
#include "model3DS.h"
#include <vector>
#include <GL/glut.h>	// GLUT
#include "math.h"
#include "square.h"
using std::vector;

using namespace std;



grid::grid(){
	gridsize = 10;
	squares.resize(gridsize);		//set the array 1 dimension to size of gridsize
	boardMade = false;	
	deadsies = false;
	//make vector to store square - gridsize*gridsize
	for(int i = 0; i < gridsize; i++){
		squares[i].resize(gridsize);
	}
	numBombs = 5;
	numBoxes = 100 - numBombs;

	for(int i = 0; i < gridsize; i++){
		for(int j = 0; j < gridsize; j++){
			squares[i][j] = new square(i*10,j*10);		
		}
	}
	won = false;

}

void grid::draw(float camx,float camy, float camz){
	int i,j;
	
//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	for(i = 0; i < gridsize; i++){
		for(j = 0; j < gridsize; j++){
						
			if(squares[i][j]->isPressed()){
				//	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				///	double r = (float)rand()/(float)RAND_MAX;
				//	double g = (float)rand()/(float)RAND_MAX;						
				//	glColor3f(0,0,1);					
					squares[i][j]->draw(camx,camy,camz);	
				//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	
				//	squares[i][j]->isntPlayer();	
				//	glColor3f(1,0,0);		
			}else{
					squares[i][j]->draw(camx,camy,camz);	
			}

			
		}
	}
}



void grid::generateGame(){
	int nbombs = numBombs;
	int **squareVals;	

	//allocate memory for squareVals
	squareVals = new int*[gridsize];
	for(int k = 0; k < gridsize; k++){
		squareVals[k] = new int(gridsize);	
	}

	for(int i = 0; i < gridsize; i++){
		for(int j = 0; j < gridsize; j++){
		squares[i][j]->setValue(0);
		}
	}
		
//	cout<<"Generating game board...bombs = "<<numBombs<<endl;	
	while(nbombs>0){
		//randomly generate the bomb positions on the map
		//double r = (float)rand()/(float)RAND_MAX;
		int bposx = rand()%10;
		int bposz = rand()%10;
		//cout<<"bomb pos1: "<<bposx<<" , "<<bposz<<endl;
		if(!(bposx == playerx && bposz == playerz)){
			squares[bposx][bposz]->setValue(9);
			nbombs--;	
		}
	}

	//check for bombs and add number of bombs into each corresponding square! ya know what you mean..minesweeper!
	for(int i = 0; i < gridsize; i++){
		for(int j = 0; j < gridsize; j++){
			if(squares[i][j]->getValue() == 9){
				//bomb found found, increment corresponding squares unless they're also a b-b-b-BOMB!
				if((i-1)>=0){
					squares[i-1][j]->incrValue();
					if((j-1)>=0){
						squares[i-1][j-1]->incrValue();						
					}
					if((j+1)<10){
						squares[i-1][j+1]->incrValue();
					}
				}

				if((i+1)<10){
					squares[i+1][j]->incrValue();					
					if((j-1)>=0){
						squares[i+1][j-1]->incrValue();
					}
					if((j+1)<10){
						squares[i+1][j+1]->incrValue();				
					}
				}

				if((j-1)>=0){
					squares[i][j-1]->incrValue();
				}
				if((j+1)<10){
					squares[i][j+1]->incrValue();
				}						
			}				
			
		}	
	}
};


bool grid::isWon(){
	return won;
}

void grid::zeroPressed(int i, int j){
	if((i-1)>=0){
		squares[i-1][j]->setPressed();
		if(squares[i-1][j]->getValue() == 0);
			zeroPressed(i-1,j);		
		
		if((j-1)>=0){
			squares[i-1][j-1]->setPressed();						
			if(squares[i-1][j-1]->getValue() == 0);
				zeroPressed(i-1,j-1);		
		}
		if((j+1)<10){
			squares[i-1][j+1]->setPressed();
			if(squares[i-1][j+1]->getValue() == 0);
			zeroPressed(i-1,j+1);		
		}
	}

	if((i+1)<10){
		squares[i+1][j]->setPressed();
		if(squares[i+1][j]->getValue() == 0);
			zeroPressed(i+1,j);					
		if((j-1)>=0){
			squares[i+1][j-1]->setPressed();
			if(squares[i+1][j-1]->getValue() == 0);
			zeroPressed(i+1,j-1);		
		}
		if((j+1)<10){
			squares[i+1][j+1]->setPressed();				
			if(squares[i+1][j+1]->getValue() == 0);
			zeroPressed(i+1,j+1);		
		}
	}

	if((j-1)>=0){
		squares[i][j-1]->setPressed();
		if(squares[i][j-1]->getValue() == 0);
			zeroPressed(i,j-1);	
	}
	if((j+1)<10){
		squares[i][j+1]->setPressed();
		if(squares[i][j+1]->getValue() == 0);
			zeroPressed(i,j+1);	
	}
}


/*void grid::playerSquare(int xpos,int zpos){
	//divide xpos and zpos by ten and round down to find i and j in vector of squares.
	playerx = (xpos/10);
	playerz = -(zpos/10);
//	cout<<" square i: "<<playerx<<" square j: "<<playerz<<endl;
	squares[playerx][playerz]->smashbox(xpos,ypos);	
}
*/
void grid::setMaybeBomb(int xpos, int zpos){
	squares[xpos/10][-zpos/10]->setMaybeBomb();
}


void grid::smashBox(int xpos,int zpos){
//	cout<<"calling smash box.."<<endl;	
	playerx = (xpos/10);
	playerz = -(zpos/10);	
	
	if(boardMade == false){
		

			//cout<<"calling smash box.."<<endl;	
		if(squares[playerx][playerz]->smashbox((xpos),(-zpos))){
			numBoxes--;			
			cout<<"generating games, boardmade = true"<<endl;				
			generateGame();
			boardMade = true;					
			for(int i = 0; i < gridsize; i++){
				for(int j = 0; j < gridsize; j++){
					cout<<squares[i][j]->getValue()<<"  ";
				}
				cout<<endl;	
			}		
		};
		//cout<<boardMade<<endl;	
		
	
	}else{
		
		//generating bombs and numbers on board
		//smash the box if the player is on the box
			
		if(squares[playerx][playerz]->smashbox(xpos,-zpos)){
			numBoxes--;			
			cout<<"smashing box.."<<endl;	
		}	
	}		
	//if a zero is found set all the squares around it to be pressed
	if(squares[playerx][playerz]->getValue() == 0){
		//zeroPressed(playerx,playerz);		
	}
	if(squares[playerx][playerz]->getValue() == 9){
		squares[playerx][playerz]->smashbox(xpos,-zpos);
		deadsies = true;
	}else if(numBombs == numBoxes){
		won = true;	
	}
	cout<<"numBoxes: "<<numBoxes<<"  num Bombs"<<numBombs<<endl;
};

bool grid::isDead(){
	return deadsies;
	//return false;
};
