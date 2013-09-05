#include "camera.h"
#include "creature.h"

using namespace std

void camera::setCamera(creature * model, int zoomout){
	//postion the camera behind the model by zoomout * the models direction vector	
	pos_x = (model->pos_x) - zoomout*model->getDirection("x"); 
	pos_y = (model->pos_y) - zoomout*model->getDirection("y"); 
	pos_z = (model->pos_z) - zoomout*model->getDirection("z"); 

	look_x = (model->pos_x);
	look_y = (model->pos_y);
	look_z = (model->pos_z);
	
	gluLookAt( pos_x, pos_y, pos_z,   look_x, look_y, look_z,   0,1,0);
}
