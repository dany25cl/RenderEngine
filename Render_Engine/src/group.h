#ifndef GROUP_H_
#define GROUP_H_

#include <glm/glm.hpp>
#include "node.h"
#include <vector>
using namespace std;

class Group: public Node
{
	vector<Node*> leaves;
public:
	Group();
	~Group();

	//Hojas
	void addLeaf(Node *leaf);
	Node* getLeaf(unsigned int index);
	const int getSize();

};
#endif