#include "group.h"

//Constructores
Group::Group() { this->transform = glm::mat4(1.0); }
Group::Group(glm::mat4 transform) { this->transform = transform; }

//Destructor
Group::~Group() {}

//
void Group::addLeaf(Node *leaf)
{
	leaves.push_back(leaf);
}

Node* Group::getLeaf(unsigned int index)
{
	return leaves[index];
}

const int Group::getSize() 
{
	return leaves.size();
}