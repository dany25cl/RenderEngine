#ifndef GROUP_H_
#define GROUP_H_

#include <glm/glm.hpp>
#include <vector>
using namespace std;

class Group 
{
	glm::mat4 transform;
	vector<Group*> leaves;
public:
	Group();
	~Group();

	void setTransform(const glm::mat4 &tf);
	glm::mat4 getTransform() { return transform; }

	void addLeaf(Group *leaf);

	Group* getLeaf(unsigned int index);
};
#endif