//
// Created by Alfie on 12/12/2024.
//

#include "Kitten.h"

#include "AABBVolume.h"
#include "PhysicsObject.h"
#include "../Game.h"

Kitten::Kitten(const Game &game, const Vector3 &position) {
    float constexpr meshSize = 0.5f;
    float constexpr inverseMass	= 0.25f;

    auto* volume  = new AABBVolume(Vector3(0.125f, 0.25f, 0.25f));

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    mesh = game.GetRenderer()->LoadMesh("Kitten.msh");

    SetRenderObject(new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader()));
    SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    GetPhysicsObject()->InitSphereInertia();
}

void Kitten::Update(float dt) {

}

