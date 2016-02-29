#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

class Camera{
	glm::mat4 view;	
	glm::mat4 proj;	
public:
	Camera();
	~Camera();
	void setPos(const glm::vec4 &pos);
	glm::vec4 getPos();
	inline glm::mat4 getView(){ return view; }
	inline glm::mat4 getProj(){ return proj; }
	



};
#endif