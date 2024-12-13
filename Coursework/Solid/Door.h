//
// Created by Alfie on 13/12/2024.
//

#ifndef DOOR_H
#define DOOR_H
#include "Cube.h"


class Cat;

class Door : public Cube {
public:
    Door(short int id, Game& game, Vector3 position);

    void OnCollisionBegin(GameObject *otherObject) override;
protected:
    Cat* player;
    short int id;
};



#endif //DOOR_H
