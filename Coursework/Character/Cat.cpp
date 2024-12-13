//
// Created by Alfie on 11/12/2024.
//

#include "Cat.h"

#include "../Game.h"
#include "PhysicsObject.h"

Cat::Cat(const Game& game, const Vector3& position) {
    float constexpr meshSize = 1.0f;
    float constexpr inverseMass	= 0.5f;

    auto* volume  = new AABBVolume(Vector3(0.25f, 0.5f, 0.5f));

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    mesh = game.GetRenderer()->LoadMesh("ORIGAMI_Chat.msh");

    SetRenderObject(new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader()));
    SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    GetPhysicsObject()->InitSphereInertia();

    controller = game.GetController();
}

void Cat::Update(float const dt) {
    auto pos = transform.GetPosition();
    float constexpr speed = 3;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) pos.z -= speed * dt;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) pos.z += speed * dt;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) pos.x -= speed * dt;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) pos.x += speed * dt;
    transform.SetPosition(pos);
}

void Cat::Kill() {
    Debug::Print("You died!", Vector2(5, 85));
}

