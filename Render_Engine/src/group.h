#ifndef GROUP_H_
#define GROUP_H_

#include <glm/glm.hpp>
#include "node.h"
#include <vector>
using namespace std;

class Group: public Node
{
	glm::mat4 transform;
	vector<Node*> leaves;
public:
	Group();
	Group(glm::mat4 transform);
	~Group();

	void setTransform(const glm::mat4 &tf);
	glm::mat4 getTransform() { return transform; }

	void addLeaf(Node *leaf);
	Node* getLeaf(unsigned int index);

	const int getSize();
};
#endif