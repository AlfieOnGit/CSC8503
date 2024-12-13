//
// Created by Alfie on 12/12/2024.
//

#ifndef KITTEN_H
#define KITTEN_H
#include "Cat.h"
#include "Character.h"
#include "Mesh.h"


class Game;

class Kitten : public Character {
public:
    Kitten(const Game& game, const Vector3& position);

    void Update(float const dt) override {
        (this->*behaviour)(dt);
    }

protected:
    Rendering::Mesh* mesh;
    Cat* player;

    void SetBehaviour(void(Kitten::*behaviour)(float)) { this->behaviour = behaviour; }
    void CheckForPlayer(float dt);
    void FollowPlayer(float dt);

    void(Kitten::*behaviour)(float);
};



#endif //KITTEN_H
