#include "node.h"

//Constructores
Node::Node():transform(glm::mat4(1.0)){};
Node::Node(glm::mat4 transform) : transform(transform){};
Node::Node(Node *father) : father(father){};
Node::Node(Node *father, glm::mat4 transform) : father(father), transform(transform){};

