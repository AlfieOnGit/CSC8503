//
// Created by Alfie on 11/12/2024.
//

#ifndef FLOOR_H
#define FLOOR_H
#include "GameObject.h"
#include "Vector.h"

namespace NCL::Rendering {
    class Mesh;
}

class Game;
using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

class Cube : public GameObject {
public:
    Cube(const Game& game, const Vector3& position);

    void SetColour(Vector4 colour);

protected:
    Rendering::Mesh* mesh;
};



#endif //FLOOR_H
