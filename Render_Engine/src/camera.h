#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glut.h>

#define PI 3.1416f

class Camera{
	glm::mat4 view;	
	glm::mat4 proj;

	// Vectores
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f); //lookAt
	glm::vec3 rght = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Ángulos
	float yaw;
	float pitch;

public:
	Camera(){};
	~Camera(){};

	inline void setProj(glm::mat4 proj) { this->proj = proj; }
	inline glm::mat4 getProj(){ return proj; }
	inline void setPos(glm::vec3 pos) { this->pos = pos; }
	inline glm::vec3 getPos(){ return pos; }
	inline void setdir(glm::vec3 pos) { this->dir = dir; }
	inline glm::vec3 getDir(){ return dir; }
	inline void setUp(glm::vec3 up) { this->up = up; }
	inline glm::vec3 getUp(){ return up; }
	inline void setAll(glm::vec3 pos, glm::vec3 dir, glm::vec3 up) { this->pos = pos; this->dir = dir; this->up = up; }
	inline void setAll(float px, float py, float pz, float dx, float dy, float dz, float ux, float uy, float uz) { 
		this->pos = glm::vec3(px, py, pz); this->dir = glm::vec3(dx,dy,dz); this->up = glm::vec3(ux, uy, uz);
	}

	
	//Desde render
	inline glm::mat4 getView(){ return view; }

	
	//////////////
	// Rotación //
	//////////////

	//Desde passiveMotionFunc

	void rotate(int x, int y, int w, int h) 
	{
		float k = PI*0.001;

		int xoff = x - (GLfloat)w / 2.0f;
		int yoff = y - (GLfloat)h / 2.0f;

		if (yaw > PI * 2) yaw = 0;
		if (yaw < 0) yaw = 2 * PI;

		yaw -= xoff*k;
		pitch += yoff*k;

		pitch = glm::clamp(pitch, -PI/5.0f, PI/5.0f);

		if (xoff != 0 || yoff != 0)
			glutWarpPointer((float)w / 2.0f, (float)h / 2.0f);
	}	
	
	////////////////
	// Movimiento //
	////////////////

	//Desde keyOps
	
	void forward(float spd) // spd > 0 = avanzar || spd < 0 = retr.
	{
		pos += dir*spd;
	}

	void strife(float spd) // spd > 0 = dcha || spd < 0 = izda
	{
		pos += rght*spd;
	}

	void fly(float spd) // spd > 0 = up || spd < 0 = down
	{
		pos += up*spd;
	}
	

	///////////////////
	// Actualización //
	///////////////////

	//Desde idle

	void updateView()
	{
		glLoadIdentity();
		
		dir = glm::vec3(cos(pitch)*sin(yaw), sin(pitch), cos(pitch)*cos(yaw));
		glm::normalize(dir);
		
		rght = glm::cross(dir, up);
		glm::normalize(rght);

		view = glm::lookAt(pos, pos+dir, up); //(pos, pos+dir, up)
	}


};
#endif