//
// Created by Alfie on 10/12/2024.
//

#include "Game.h"

#include "PhysicsObject.h"
#include "RenderObject.h"

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
    useGravity = false;
    inSelectionMode = false;
    world->GetMainCamera().SetController(controller);

    controller.MapAxis(0, "Sidestep");
    controller.MapAxis(1, "UpDown");
    controller.MapAxis(2, "Forward");
    controller.MapAxis(3, "XLook");
    controller.MapAxis(4, "YLook");

    InitRenderer();
    InitCamera();
    InitWorld();
}

Game::~Game() {
    delete world;
    delete physics;
    delete renderer;
}

void Game::InitRenderer() {
    cubeMesh	= renderer->LoadMesh("cube.msh");
    sphereMesh	= renderer->LoadMesh("sphere.msh");
    catMesh		= renderer->LoadMesh("ORIGAMI_Chat.msh");
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
    lockedObject = nullptr;
}

void Game::InitWorld() {
    world->ClearAndErase();
    physics->Clear();

    //GameObject* floor = AddFloorToWorld(Vector3(0, -20, 0));
    Transform& transform = AddFloorToWorld(Vector3(0, -20, 0))->GetTransform();
    float x = transform.GetScale().x / 2 + transform.GetPosition().x;
    float z = transform.GetScale().z / 2 + transform.GetPosition().z;
    LockCameraToObject(AddCatToWorld(Vector3(x, 0, z)));
}

GameObject *Game::AddCatToWorld(const Vector3 &position) const {
    float constexpr meshSize		= 1.0f;
    float constexpr inverseMass	= 0.5f;

    auto* character = new GameObject();
    auto* volume  = new SphereVolume(1.0f);

    character->SetBoundingVolume(volume);

    character->GetTransform()
        .SetScale(Vector3(meshSize, meshSize, meshSize))
        .SetPosition(position);

    character->SetRenderObject(new RenderObject(&character->GetTransform(), catMesh, nullptr, basicShader));
    character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

    character->GetPhysicsObject()->SetInverseMass(inverseMass);
    character->GetPhysicsObject()->InitSphereInertia();

    world->AddGameObject(character);

    return character;
}


GameObject* Game::AddFloorToWorld(const Vector3& position) const {
    auto* floor = new GameObject();

    auto const floorSize = Vector3(200, 2, 200);
    auto* volume = new AABBVolume(floorSize);
    floor->SetBoundingVolume(volume);
    floor->SetBoundingVolume(volume);
    floor->GetTransform()
        .SetScale(floorSize * 2.0f)
        .SetPosition(position);

    floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
    floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

    floor->GetPhysicsObject()->SetInverseMass(0);
    floor->GetPhysicsObject()->InitCubeInertia();

    world->AddGameObject(floor);

    return floor;
}


void Game::Update(float const dt) {
    if (!inSelectionMode) world->GetMainCamera().UpdateCamera(dt);

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
    //if (Window::GetKeyboard()->Key)
}


