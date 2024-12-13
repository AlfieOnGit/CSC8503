//
// Created by Alfie on 13/12/2024.
//

#ifndef GOAT_H
#define GOAT_H
#include "Cat.h"
#include "Character.h"
#include "Vector.h"


class Game;

class Goat : public Character {
public:
    Goat(const Game& game, const Vector3& position);

    void Update(float const dt) override {
        (this->*behaviour)(dt);
    }

protected:
    NCL::Rendering::Mesh* mesh;
    Cat* player;

    void SetBehaviour(void(Goat::*behaviour)(float)) { this->behaviour = behaviour; }
    void CheckForPlayer(float dt);
    void ChasePlayer(float dt);

    void(Goat::*behaviour)(float);
};



#endif //GOAT_H
