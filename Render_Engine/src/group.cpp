#include "group.h"


Group::Group() {}

Group::~Group() {}

void Group::addLeaf(Group *leaf)
{
	leaves.push_back(leaf);
}

Group* Group::getLeaf(unsigned int index)
{
	return leaves[0];
}