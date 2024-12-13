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

    auto* volume  = new NCL::AABBVolume(Vector3(0.25f, 0.25f, 0.5f));
    //SetColour(Vector4(0, 0, 1, 1));

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    //mesh = game.GetRenderer()->LoadMesh("cube.msh");
    auto* ro = new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader());
    ro->SetColour(Vector4(0, 0, 1, 1));

    SetRenderObject(ro);
    //SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    //GetPhysicsObject()->InitSphereInertia();

    player = game.GetPlayer();
}

void Key::OnCollisionBegin(GameObject *otherObject) {
    if (otherObject == player && isActive) {
        this->SetActive(false);
        std::cout << "Key picked up!\n";
    }
}
