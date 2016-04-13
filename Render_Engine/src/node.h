#ifndef NODE_H_
#define NODE_H_

#include <glm/glm.hpp>

class Node
{
	Node *father;
	glm::mat4 transform;

public:
	Node();
	Node(glm::mat4 transform);
	Node(Node *father);
	Node(Node *father, glm::mat4 transform);

	~Node(){};
	//Setters
	inline void setTransform(const glm::mat4 &tf);
	void setFather(Node *father) { this->father = father; }

	//Getters
	glm::mat4 getTransform() { return transform; }
	Node* getFather() { return father; }
};

#endif