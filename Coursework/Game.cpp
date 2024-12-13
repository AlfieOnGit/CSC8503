//
// Created by Alfie on 10/12/2024.
//

#include "Game.h"

#include "PhysicsObject.h"
#include "Character/Cat.h"
#include "Character/Goat.h"
#include "Character/Kitten.h"
#include "Item/Key.h"
#include "Screen/ScreenManager.h"
#include "Solid/Base.h"
#include "Solid/Cube.h"
#include "Solid/Door.h"

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

    winScreen = new WinScreen();
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
    float const x = transform.GetScale().x / 4 + transform.GetPosition().x;
    float const z = transform.GetScale().z / 4 + transform.GetPosition().z;
    player = new Cat(*this, Vector3(x, 0, z));
    world->AddGameObject(player);
    LockCameraToObject(player);
    characters.push_back(player);

    auto* kitten = new Kitten(*this, Vector3(x, -15, z - 20));
    world->AddGameObject(kitten);
    characters.push_back(kitten);

    kitten = new Kitten(*this, Vector3(x + 2, -15, z - 12));
    world->AddGameObject(kitten);
    characters.push_back(kitten);

    auto* goat = new Goat(*this, Vector3(x, -15, z - 18));
    world->AddGameObject(goat);
    characters.push_back(goat);

    goat = new Goat(*this, Vector3(x, -15, z - 12));
    world->AddGameObject(goat);
    characters.push_back(goat);

    auto* blueKey = new Key(0, *this, Vector3(x - 15, -15, z));
    blueKey->SetColour(Vector4(0, 0, 1, 1));
    world->AddGameObject(blueKey);

    auto* redKey = new Key(1, *this, Vector3(x + 15, -15, z));
    redKey->SetColour(Vector4(1, 0, 0, 1));
    world->AddGameObject(redKey);

    auto* base = new Base(*this, Vector3(x, -19, z));
    world->AddGameObject(base);

    auto* wall = new Cube(*this, Vector3(x + 5, -19, z - 20));
    wall->SetSize(Vector3(2.5, 5, 7.5));
    world->AddGameObject(wall);

    wall = new Cube(*this, Vector3(x - 5, -19, z - 20));
    wall->SetSize(Vector3(2.5, 5, 7.5));
    world->AddGameObject(wall);

    wall = new Cube(*this, Vector3(x, -19, z - 30));
    wall->SetSize(Vector3(7.5, 5.5, 2.5));
    world->AddGameObject(wall);

    Door* door = new Door(0, *this, Vector3(x, -19, z - 15));
    door->SetSize(Vector3(7.4, 4.5, 2.4));
    door->SetColour(Vector4(0, 0, 1, 1));
    world->AddGameObject(door);
}

void Game::InitWalls() {
    auto* wall = new Cube(*this, Vector3(0, -19, -2));
    wall->SetSize(Vector3(2.5, 3, 7.5));
    wall->GetPhysicsObject()->SetInverseMass(0);
    world->AddGameObject(wall);
}



void Game::OnFirstLoad() {
    Window::GetWindow()->ShowOSPointer(false);
    Window::GetWindow()->LockMouseToWindow(true);
}


void Game::Update(float const dt) {
    if (kittensRescued == 2) ScreenManager::Append(winScreen);

    if (!inSelectionMode) world->GetMainCamera().UpdateCamera(dt);

    for (Character* c : characters) c->Update(dt);

    if (lockedObject != nullptr) {
        Vector3 const objPos = lockedObject->GetTransform().GetPosition();
        Vector3 const camPos = objPos + lockedOffset;

        Matrix4 const temp = Matrix::View(camPos, objPos, Vector3(0,1,0));

        Matrix4 const modelMat = Matrix::Inverse(temp);

        Quaternion const q(modelMat);
        Vector3 const angles = q.ToEuler();

        world->GetMainCamera().SetPosition(camPos);
        world->GetMainCamera().SetPitch(angles.x);
        world->GetMainCamera().SetYaw(angles.y);
    }

    Debug::Print("Kittens rescued: " + std::to_string(kittensRescued), Vector2(60, 10));

    world->UpdateWorld(dt);
    renderer->Update(dt);
    physics->Update(dt);

    renderer->Render();
    Debug::UpdateRenderables(dt);
}

void Game::Reset() {
    GameObjectIterator first, last;
    world->GetObjectIterators(first, last);
    for (auto i = first; i != last; ++i) {
        (*i)->SetActive(true);
        (*i)->GetTransform().SetPosition((*i)->GetStartPos());
        (*i)->GetPhysicsObject()->ClearVelocities();
    }
    player->ClearItems();
    kittensRescued = 0;
}
