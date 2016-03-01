#include "group.h"

//Tratar hojas
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