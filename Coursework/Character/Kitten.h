//
// Created by Alfie on 12/12/2024.
//

#ifndef KITTEN_H
#define KITTEN_H
#include "Character.h"
#include "Mesh.h"


class Game;

class Kitten : public Character {
public:
    Kitten(const Game& game, const Vector3& position);

    void Update(float dt) override;

protected:
    NCL::Rendering::Mesh* mesh;
};



#endif //KITTEN_H
