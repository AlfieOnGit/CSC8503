//
// Created by Alfie on 13/12/2024.
//

#include "Base.h"
#include "PhysicsObject.h"

Base::Base(Game& game, Vector3 pos) : Cube(game, pos) {
    name = "base";
    GetPhysicsObject()->SetInverseMass(0);
    SetSize(Vector3(5, 1.1, 5));
    SetColour(Vector4(0, 1, 0, 1));
}
