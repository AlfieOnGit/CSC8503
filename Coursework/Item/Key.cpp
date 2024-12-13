//
// Created by Alfie on 13/12/2024.
//

#include "Key.h"
#include "PhysicsObject.h"
#include "../Game.h"

#include "AABBVolume.h"

Key::Key(short int id, Game& game, Vector3 position) : Item(id), Cube(game, position) {
    float constexpr meshSize = 1.0f;
    float constexpr inverseMass	= 0.5f;

    name = "key";

    auto* volume  = new NCL::AABBVolume(Vector3(0.25f, 0.25f, 0.5f));

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    SetRenderObject(new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader()));

    GetPhysicsObject()->SetInverseMass(inverseMass);

    player = game.GetPlayer();
}

void Key::OnCollisionBegin(GameObject *otherObject) {
    if (otherObject == player && isActive) {
        this->SetActive(false);
        std::cout << "Key picked up!\n";
        player->AddItem(this);
    }
}
