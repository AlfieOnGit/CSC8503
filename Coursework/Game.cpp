//
// Created by Alfie on 10/12/2024.
//

#include "Game.h"

#include "Character/Cat.h"
#include "Character/Kitten.h"
#include "Solid/Cube.h"

Game::Game() : controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
    world = new GameWorld();
    physics = new PhysicsSystem(*world);

#ifdef USEVULKAN
    renderer	= new GameTechVulkanRenderer(*world);
    renderer->Init();
    renderer->InitStructures();
#else
    renderer = new GameTechRenderer(*world);
#endif

    forceMagnitude = 10.0f;
    inSelectionMode = false;
    world->GetMainCamera().SetController(controller);

    controller.MapAxis(0, "Sidestep");
    controller.MapAxis(1, "UpDown");
    controller.MapAxis(2, "Forward");
    controller.MapAxis(3, "XLook");
    controller.MapAxis(4, "YLook");

    characters = vector<Character*>();

    InitRenderer();
    InitCamera();
    InitWorld();

    physics->UseGravity(true);
}


Game::~Game() {
    delete world;
    delete physics;
    delete renderer;
}


void Game::InitRenderer() {
    sphereMesh	= renderer->LoadMesh("sphere.msh");
    kittenMesh	= renderer->LoadMesh("Kitten.msh");

    enemyMesh	= renderer->LoadMesh("Keeper.msh");
    bonusMesh	= renderer->LoadMesh("19463_Kitten_Head_v1.msh");
    capsuleMesh = renderer->LoadMesh("capsule.msh");

    basicTex	= renderer->LoadTexture("checkerboard.png");
    basicShader = renderer->LoadShader("scene.vert", "scene.frag");
}


void Game::InitCamera() {
    world->GetMainCamera().SetNearPlane(0.1f);
    world->GetMainCamera().SetFarPlane(500.0f);
    world->GetMainCamera().SetPitch(-15.0f);
    world->GetMainCamera().SetYaw(315.0f);
    world->GetMainCamera().SetPosition(Vector3(-60, 40, 60));
}


void Game::InitWorld() {
    world->ClearAndErase();
    physics->Clear();

    auto* floor = new Cube(*this, Vector3(0, -20, 0));
    world->AddGameObject(floor);

    Transform const transform = floor->GetTransform();
    std::cout << "Scale.x = " << transform.GetScale().x << '\n';
    float const x = transform.GetScale().x / 4 + transform.GetPosition().x;
    float const z = transform.GetScale().z / 4 + transform.GetPosition().z;
    std::cout << "X = " << x << ", Z = " << z << '\n';
    player = new Cat(*this, Vector3(x, 0, z));
    world->AddGameObject(player);
    LockCameraToObject(player);
    characters.push_back(player);

    auto* kitten = new Kitten(*this, Vector3(x, 0, z));
    world->AddGameObject(kitten);
    characters.push_back(kitten);
}


void Game::Update(float const dt) {
    if (!inSelectionMode) world->GetMainCamera().UpdateCamera(dt);

    for (Character* c : characters) c->Update(dt);

    if (lockedObject != nullptr) {
        Vector3 const objPos = lockedObject->GetTransform().GetPosition();
        Vector3 const camPos = objPos + lockedOffset;

        Matrix4 const temp = Matrix::View(camPos, objPos, Vector3(0,1,0));

        Matrix4 const modelMat = Matrix::Inverse(temp);

        Quaternion const q(modelMat);
        Vector3 const angles = q.ToEuler(); //nearly there now!

        world->GetMainCamera().SetPosition(camPos);
        world->GetMainCamera().SetPitch(angles.x);
        world->GetMainCamera().SetYaw(angles.y);
    }

    world->UpdateWorld(dt);
    renderer->Update(dt);
    physics->Update(dt);

    renderer->Render();
    Debug::UpdateRenderables(dt);
}

void Game::UpdateKeys() {
}
