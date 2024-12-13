//
// Created by Alfie on 13/12/2024.
//

#ifndef KEY_H
#define KEY_H

#include "Item.h"
#include "../Solid/Cube.h"

class Key : public Item, public Cube {
public:
    Key(short int id, Game& game, Vector3 position);

    void OnCollisionBegin(GameObject *otherObject) override;

protected:
    GameObject* player;
};



#endif //KEY_H
