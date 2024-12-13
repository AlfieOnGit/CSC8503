//
// Created by Alfie on 11/12/2024.
//

#include "Cat.h"

#include "../Game.h"
#include "../Screen/DeathScreen.h"
#include "PhysicsObject.h"
#include "../Item/Item.h"
#include "../Screen/ScreenManager.h"

Cat::Cat(const Game& game, const Vector3& position) {
    float constexpr meshSize = 1.0f;
    float constexpr inverseMass	= 0.5f;
    startPos = position;

    auto* volume  = new AABBVolume(Vector3(0.25f, 0.5f, 0.5f));

    SetBoundingVolume(volume);

    GetTransform().SetScale(Vector3(meshSize, meshSize, meshSize)).SetPosition(position);

    mesh = game.GetRenderer()->LoadMesh("ORIGAMI_Chat.msh");

    SetRenderObject(new RenderObject(&GetTransform(), mesh, nullptr, game.GetShader()));
    SetPhysicsObject(new PhysicsObject(&GetTransform(), GetBoundingVolume()));

    GetPhysicsObject()->SetInverseMass(inverseMass);
    GetPhysicsObject()->InitSphereInertia();
    //GetPhysicsObject()->SetLinearDampening(Vector3(0.9, 1, 0.9));

    controller = game.GetController();
    deathScreen = new DeathScreen();
}


Cat::~Cat() {
    delete deathScreen;
}


void Cat::Update(float const dt) {
    auto pos = transform.GetPosition();
    float constexpr speed = 3;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) pos.z -= speed * dt;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) pos.z += speed * dt;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) pos.x -= speed * dt;
    if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) pos.x += speed * dt;
    transform.SetPosition(pos);

    std::string inventoryText = "Inventory:";
    if (HasItem(0)) inventoryText.append(" Blue Key,");
    if (HasItem(1)) inventoryText.append(" Red Key,");
    if (items.empty()) inventoryText.append(" EMPTY! ");
    inventoryText.pop_back();
    Debug::Print(inventoryText, NCL::Vector2(5, 15));

    // auto pos = Vector3();// - GetPhysicsObject()->GetForce();
    // float constexpr speed = 1000.0f;
    // if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) pos.z -= 1;
    // if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) pos.z += 1;
    // if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) pos.x -= 1;
    // if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) pos.x += 1;
    // Vector3 const dir = Vector::Normalise(pos);
    //
    // GetPhysicsObject()->AddForce(dir * speed * dt);
}


void Cat::Kill() {
    ScreenManager::Append(deathScreen);
}

bool Cat::HasItem(short int id) const {
    for (Item* i : items) if (i->GetId() == id) return true;
    return false;
}


