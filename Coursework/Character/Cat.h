//
// Created by Alfie on 11/12/2024.
//

#ifndef CAT_H
#define CAT_H
#include "Character.h"
#include "GameObject.h"
#include "Vector.h"

namespace NCL { class KeyboardMouseController; }

namespace NCL::Rendering { class Mesh; }

class Game;
using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

class Cat : public Character {
public:
    Cat(const Game& game, const Vector3& position);

    void Update(float dt) override;

protected:
    Rendering::Mesh* mesh;
    const KeyboardMouseController* controller;
};

#endif //CAT_H
