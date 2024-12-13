//
// Created by Alfie on 11/12/2024.
//

#include "Cube.h"

#include "AABBVolume.h"
#include "PhysicsObject.h"
#include "../Game.h"

Cube::Cube(const Game& game, const Vector3 &position) {
    auto const floorSize = Vector3(200, 2, 200);
    auto* volume = new AABBVolume(floorSize);
    SetBoundingVolume(volume);
    SetBoundingVolume(volume);
    GetTransform().SetScale(floorSize * 2.0f).SetPosition(position);
    startPos = position;

    mesh = game.GetRenderer()->LoadMesh("cube.msh");

    SetRenderObject(new RenderObject(&GetTransform(), mesh, game.GetBasicTex(), game.GetShader()));
    SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(0);
    GetPhysicsObject()->InitCubeInertia();
}

void Cube::SetColour(Vector4 const colour) {
    GetRenderObject()->SetColour(colour);
}

