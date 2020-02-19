#ifndef NODE_H
#define NODE_H
#include <vector>
#include "PhysicsObject.h"

//Node is a type of physics object that is rendered as a cube
class Node :public PhysicsObjects::PhysicsObject{
public:
    Node();
};
#endif // !NODE_H