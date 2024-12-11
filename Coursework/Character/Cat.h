//
// Created by Alfie on 11/12/2024.
//

#ifndef CAT_H
#define CAT_H
#include "GameObject.h"
#include "Vector.h"

namespace NCL::Rendering {
    class Mesh;
}

class Game;
using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

class Cat : public GameObject {
public:
    Cat(Game& game, const Vector3& position);

protected:
    Rendering::Mesh* mesh;
};

#endif //CAT_H
