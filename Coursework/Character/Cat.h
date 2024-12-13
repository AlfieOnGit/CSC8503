//
// Created by Alfie on 11/12/2024.
//

#ifndef CAT_H
#define CAT_H
#include "Character.h"
#include "GameObject.h"
#include "Vector.h"
#include "../Screen/DeathScreen.h"

namespace NCL { class KeyboardMouseController; }

namespace NCL::Rendering { class Mesh; }

class Game;
using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

class Cat : public Character {
public:
    Cat(const Game& game, const Vector3& position);
    ~Cat();

    void Update(float dt) override;

    void Kill();

protected:
    Rendering::Mesh* mesh;
    const KeyboardMouseController* controller;
    DeathScreen* deathScreen;
};

#endif //CAT_H
