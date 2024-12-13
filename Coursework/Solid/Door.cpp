//
// Created by Alfie on 13/12/2024.
//

#include "Door.h"

#include "../Game.h"
#include "../Item/Key.h"

Door::Door(short int id, Game &game, Vector3 position) : Cube(game, position) {
    this->id = id;
    this->player = game.GetPlayer();
}

void Door::OnCollisionBegin(GameObject *otherObject) {
    if (otherObject == player && isActive && player->HasItem(id)) {
        SetActive(false);
    }
}

