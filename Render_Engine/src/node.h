#ifndef NODE_H_
#define NODE_H_

#include <glm/glm.hpp>

class Node
{
	Node *father;

public:
	Node(){};
	Node(Node *father) : father(father){};
	Node* getFather() { return father; }
	void setFather(Node *father) { this->father = father; }
};

#endif