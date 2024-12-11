#include "Window.h"

#include "Debug.h"

#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

#include "GameServer.h"
#include "GameClient.h"

#include "NavigationGrid.h"
#include "NavigationMesh.h"

#include "TutorialGame.h"
#include "NetworkedGame.h"

#include "PushdownMachine.h"

#include "PushdownState.h"

#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"

using namespace NCL;
using namespace CSC8503;

#include <chrono>
#include <thread>
#include <sstream>

vector<Vector3> testNodes;

class PauseScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::U)) return PushdownResult::Pop;
		return PushdownResult::NoChange;
	}

	void OnAwake() override {
		std::cout << "Press U to unpause game!\n";
	}
};

class GameScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		pauseReminder -= dt;
		if (pauseReminder < 0) {
			std::cout << "Coins mined: " << coinsMined << '\n';
			std::cout << "Press P to pause game, or F1 to return to the main menu!\n";
			pauseReminder += 1.0f;
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::P)) {
			*newState = new PauseScreen();
			return PushdownResult::Push;
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::F1)) {
			std::cout << "Returning to main menu!\n";
			return PushdownResult::Pop;
		}
		if (rand() % 7 == 0) coinsMined++;
		return PushdownResult::NoChange;
	}

	void OnAwake() override {
		std::cout << "Preparing to mine coins!\n";
	}

protected:
	int coinsMined = 0;
	float pauseReminder = 1;
};

class IntroScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState **newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::SPACE)) {
			*newState = new GameScreen();
			return PushdownResult::Push;
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
			return PushdownResult::Pop;
		}

		return PushdownResult::NoChange;
	}

	void OnAwake() override {
		std::cout << "Welcome to a really awesome game!\n";
		std::cout << "Press SPACE to begin or ESCAPE to quit!\n";
	}
};

class TestPacketReceiver : public PacketReceiver {
public:
	TestPacketReceiver(std::string name) { this->name = name; }

	void ReceivePacket(int type, GamePacket *payload, int source) override {
		if (type != String_Message) return;

		auto* realPacket = static_cast<StringPacket*>(payload);
		const std::string msg = realPacket->GetStringFromData();
		std::cout << name << " received message: " << msg << '\n';
	}

protected:
	std::string name;
};

void TestNetworking() {
	NetworkBase::Initialise();

	TestPacketReceiver serverReceiver("Server1"), clientReceiver("Client1");
	int port = NetworkBase::GetDefaultPort();

	auto* server = new GameServer(port, 1);
	server->RegisterPacketHandler(String_Message, &serverReceiver);
	auto* client = new GameClient();
	client->RegisterPacketHandler(String_Message, &clientReceiver);

	bool canConnect = client->Connect(127, 0, 0, 1, port);

	for (int i = 0; i < 100; ++i) {
		StringPacket serverMsg("Server says hello! " + std::to_string(i));
		server->SendGlobalPacket(serverMsg);
		StringPacket clientMsg("Client says hello! " + std::to_string(i));
		client->SendPacket(clientMsg);

		server->UpdateServer();
		client->UpdateClient();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	NetworkBase::Destroy();
}

void TestPushdownAutomata(Window* w) {
	PushdownMachine machine(new IntroScreen);
	while (w->UpdateWindow()) {
		float dt = w->GetTimer().GetTimeDeltaSeconds();
		if (!machine.Update(dt)) return;
	}
}

void TestPathfinding() {
	NavigationGrid grid("TestGrid1.txt");

	NavigationPath outPath;

	Vector3 startPos(80, 0, 10), endPos(80, 0, 80);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos)) testNodes.push_back(pos);
}

void DisplayPathfinding() {
	for (int i = 1; i < testNodes.size(); ++i) {
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

void TestStateMachine() {
	auto* testMachine = new StateMachine();
	int data = 0;

	auto* stateA = new State([&](float dt)->void {
		std::cout << "I'm in state A!\n";
		data++;
	});

	auto* stateB = new State([&](float dt)->void {
		std::cout << "I'm in state B!\n";
		data--;
	});

	auto* transitionAB = new StateTransition(stateA, stateB, [&](void)->bool { return data > 10; });
	auto* transitionBA = new StateTransition(stateB, stateA, [&](void)->bool { return data < 0; });

	testMachine->AddState(stateA);
	testMachine->AddState(stateB);
	testMachine->AddTransition(transitionAB);
	testMachine->AddTransition(transitionBA);

	for (int i = 0; i < 100; ++i) {
		testMachine->Update(1.0f);
	}
}

void TestBehaviourTree() {
	float behaviourTimer;
	float distanceToTarget;
	BehaviourAction* findKey = new BehaviourAction("Find Key", [&](float dt, BehaviourState state)->BehaviourState {
		switch (state) {
			case Initialise:
				std::cout << "Looking for a key!\n";
				behaviourTimer = rand() % 100;
				state = Ongoing;
				return Ongoing;
			case Ongoing:
				behaviourTimer -= dt;
				if (behaviourTimer <= 0.0f) {
					std::cout << "Found a key!\n";
					return Success;
				}
			default:
				return state;
		}
	});

	BehaviourAction* goToRoom = new BehaviourAction("Go To Room", [&](float dt, BehaviourState state)->BehaviourState {
		switch (state) {
			case Initialise:
				std::cout << "Going to the loot room!\n";
				state = Ongoing;
				return Ongoing;
			case Ongoing:
				distanceToTarget -= dt;
				if (distanceToTarget <= 0.0f) {
					std::cout << "Reached room!\n";
					return Success;
				}
			default:
				return state;
		}
	});

	BehaviourAction* openDoor = new BehaviourAction("Open Door", [&](float dt, BehaviourState state)->BehaviourState {
		switch (state) {
			case Initialise:
				std::cout << "Opening door!\n";
				return Success;
			default:
				return state;
		}
	});

	BehaviourAction* lookForTreasure = new BehaviourAction("Look For Treasure", [&](float dt, BehaviourState state)->BehaviourState {
		switch (state) {
			case Initialise:
				std::cout << "Looking for treasure!\n";
				return Ongoing;
			case Ongoing: {
				bool found = rand() % 2;
				if (found) {
					std::cout << "I found some treasure!\n";
					return Success;
				}
				std::cout << "No treasure in here...\n";
				return Failure;
			}
			default:
				return state;
		}
	});

	BehaviourAction* lookForItems = new BehaviourAction("Look For Items", [&](float dt, BehaviourState state)->BehaviourState {
		switch (state) {
			case Initialise:
				std::cout << "Looking for items!\n";
				return Ongoing;
			case Ongoing: {
				bool found = rand() % 2;
				if (found) {
					std::cout << "I found some items!\n";
					return Success;
				}
				std::cout << "No treasure in here...\n";
				return Failure;
			}
			default:
				return state;
		}
	});

	auto* sequence = new BehaviourSequence("Room Sequence");
	sequence->AddChild(findKey);
	sequence->AddChild(goToRoom);
	sequence->AddChild(openDoor);

	auto* selection = new BehaviourSelector("Loot Selection");
	selection->AddChild(lookForTreasure);
	selection->AddChild(lookForItems);

	auto* rootSequence = new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(selection);

	for (int i = 0; i < 5; ++i) {
		rootSequence->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = rand() % 250;
		BehaviourState state = Ongoing;
		std::cout << "We're going on an adventure!\n";
		while (state == Ongoing) state = rootSequence->Execute(1.0f);
		if (state == Success) std::cout << "What a successful adventure!\n";
		else if (state == Failure) std::cout << "What a waste of time!\n";
	}

	std::cout << "All done!\n";
}

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead. 

This time, we've added some extra functionality to the window class - we can
hide or show the 

*/
int main() {
	//TestStateMachine();
	//TestBehaviourTree();
	//TestNetworking();
	//return 0;

	WindowInitialisation initInfo;
	initInfo.width		= 1280;
	initInfo.height		= 720;
	initInfo.windowTitle = "CSC8503 Game technology!";

	Window*w = Window::CreateGameWindow(initInfo);

	if (!w->HasInitialised()) {
		return -1;
	}

	//TestPushdownAutomata(w);

	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();
	w->GetTimer().GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	TestPathfinding();

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyCodes::ESCAPE)) {
		float dt = w->GetTimer().GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyCodes::T)) {
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g->UpdateGame(dt);
		DisplayPathfinding();
	}
	Window::DestroyGameWindow();
}