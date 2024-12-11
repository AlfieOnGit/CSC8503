//
// Created by Alfie on 11/12/2024.
//

#include "Cat.h"

#include "../Game.h"
#include "PhysicsObject.h"
#include "SphereVolume.h"

Cat::Cat(Game& game, const Vector3& position) {
    float constexpr meshSize = 1.0f;
    float constexpr inverseMass	= 0.5f;

    auto* character = new GameObject();
    auto* volume  = new SphereVolume(1.0f);

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    mesh = game.getRenderer()->LoadMesh("ORIGAMI_Chat.msh");

    SetRenderObject(new RenderObject(&character->GetTransform(), mesh, nullptr, game.getShader()));
    SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    GetPhysicsObject()->InitSphereInertia();
}
