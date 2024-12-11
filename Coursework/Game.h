//
// Created by Alfie on 10/12/2024.
//

#ifndef GAME_H
#define GAME_H
#include "GameWorld.h"
#include "KeyboardMouseController.h"
#include "PhysicsSystem.h"
#include "RenderObject.h"
#include "Character/Cat.h"
#include "GameTechRenderer/GameTechRenderer.h"
#include "Screen/Screen.h"

namespace NCL::CSC8503 {
    class StateGameObject;
}

using namespace NCL;


class Game final : public Screen {
public:
    Game();
    ~Game();

    void Update(float dt) override;

    [[nodiscard]] GameWorld* GetWorld() const { return world; }
    [[nodiscard]] GameTechRenderer* GetRenderer() const { return renderer; }
    [[nodiscard]] Shader* GetShader() const { return basicShader; }
    [[nodiscard]] Texture* GetBasicTex() const { return basicTex; }

private:
    void InitRenderer();
    void InitCamera();
    void InitWorld();

    void UpdateKeys();
    void LockCameraToObject(GameObject* o) { lockedObject = o; }

    GameWorld* world;
    PhysicsSystem* physics;

#ifdef USEVULKAN
    GameTechVulkanRenderer*	renderer;
#else
    GameTechRenderer* renderer;
#endif

    KeyboardMouseController controller;

    bool useGravity = true;
    bool inSelectionMode;

    float forceMagnitude;

    GameObject* selectionObject = nullptr;

    Mesh* capsuleMesh = nullptr;
    Mesh* sphereMesh = nullptr;

    Texture* basicTex = nullptr;
    Shader* basicShader = nullptr;

    Cat* player = nullptr;

    Mesh* kittenMesh = nullptr;
    Mesh* enemyMesh	= nullptr;
    Mesh* bonusMesh	= nullptr;

    GameObject* lockedObject = nullptr;
    Vector3 lockedOffset = Vector3(0, 14, 20);

    GameObject* objClosest = nullptr;
};



#endif //GAME_H
