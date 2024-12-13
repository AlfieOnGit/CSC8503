//
// Created by Alfie on 12/12/2024.
//

#include "Kitten.h"

#include "AABBVolume.h"
#include "PhysicsObject.h"
#include "../Game.h"

Kitten::Kitten(Game &game, const Vector3 &position) : kittensSaved(game.GetKittensRescued()) {
    float constexpr meshSize = 0.5f;
    float constexpr inverseMass	= 0.25f;
    startPos = position;

    //auto* volume  = new AABBVolume(Vector3(0.125f, 0.25f, 0.25f));
    auto* volume = new SphereVolume(0.2f);

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    mesh = game.GetRenderer()->LoadMesh("Kitten.msh");

    SetRenderObject(new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader()));
    SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    GetPhysicsObject()->InitSphereInertia();

    player = game.GetPlayer();
    behaviour = &Kitten::CheckForPlayer;
    SetBehaviour(&Kitten::CheckForPlayer);
}

void Kitten::CheckForPlayer(float dt) {
    Vector3 const dist = player->GetTransform().GetPosition() - transform.GetPosition();
    if (Vector::Length(dist) < 5) SetBehaviour(&Kitten::FollowPlayer);
}


void Kitten::FollowPlayer(float dt) {
    Vector3 const dist = player->GetTransform().GetPosition() - transform.GetPosition();
    float const len = Vector::Length(dist);
    if (len < 2) return;
    if (len > 5) {
        SetBehaviour(&Kitten::CheckForPlayer);
        return;
    }
    Vector3 const dir = Vector::Normalise(dist);
    float constexpr speed = 1000.0f;
    GetPhysicsObject()->AddForce(dir * speed * dt);
}

void Kitten::OnCollisionBegin(GameObject *otherObject) {
    if (isActive && otherObject->GetName() == "base") {
        kittensSaved++;
        SetActive(false);
    }
}
