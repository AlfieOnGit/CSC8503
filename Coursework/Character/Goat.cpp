//
// Created by Alfie on 13/12/2024.
//

#include "Goat.h"

#include "AABBVolume.h"
#include "PhysicsObject.h"
#include "../Game.h"

Goat::Goat(const Game &game, const Vector3 &position) {
    float constexpr meshSize = 0.5f;
    float constexpr inverseMass	= 0.25f;
    startPos = position;

    auto* volume  = new AABBVolume(Vector3(0.25f, 0.5f, 0.5f));

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    mesh = game.GetRenderer()->LoadMesh("Goat.msh");

    SetRenderObject(new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader()));
    renderObject->GetTransform()->SetOrientation(Quaternion(0.0f, 180, 0, 1));
    renderObject->SetColour(Vector4(1, 0, 0, 1));
    SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    GetPhysicsObject()->InitSphereInertia();

    player = game.GetPlayer();
    SetBehaviour(&Goat::CheckForPlayer);
}

void Goat::CheckForPlayer(float dt) {
    Vector3 const dist = player->GetTransform().GetPosition() - transform.GetPosition();
    if (Vector::Length(dist) < 10) SetBehaviour(&Goat::ChasePlayer);
}

void Goat::ChasePlayer(float dt) {
    Vector3 dist = player->GetTransform().GetPosition() - transform.GetPosition();
    float const len = Vector::Length(dist);
    if (len > 10) {
        SetBehaviour(&Goat::CheckForPlayer);
        return;
    }
    Vector3 const dir = Vector::Normalise(dist);
    float constexpr speed = 1500.0f;
    GetPhysicsObject()->AddForce(dir * speed * dt);
}

void Goat::OnCollisionBegin(GameObject *otherObject) {
    if (otherObject == player) player->Kill();
}

