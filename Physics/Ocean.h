#pragma once
#ifndef OCEAN_H
#include "PhysicsObject.h"
//The ocean class which doesn't move and is at position 0,0,0
class Ocean : public PhysicsObjects::PhysicsObject {
public:
    Ocean();
};

#endif // !OCEAN_H
