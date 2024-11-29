#include "PositionConstraint.h"
//#include "../../Common/Vector3.h"
#include "GameObject.h"
#include "PhysicsObject.h"
//#include "Debug.h"



using namespace NCL;
using namespace Maths;
using namespace CSC8503;

PositionConstraint::PositionConstraint(GameObject* a, GameObject* b, float d)
{
	objectA		= a;
	objectB		= b;
	distance	= d;
}

PositionConstraint::~PositionConstraint()
{

}

//a simple constraint that stops objects from being more than <distance> away
//from each other...this would be all we need to simulate a rope, or a ragdoll
void PositionConstraint::UpdateConstraint(float dt)	{
	const Vector3 relativePos = objectA->GetTransform().GetPosition() - objectB->GetTransform().GetPosition();
	const float currentDistance = Vector::Length(relativePos);
	const float offset = distance - currentDistance;

	if (abs(offset) == 0.0f) return;

	const Vector3 offsetDir = Vector::Normalise(relativePos);

	PhysicsObject* physA = objectA->GetPhysicsObject();
	PhysicsObject* physB = objectB->GetPhysicsObject();

	const Vector3 relativeVelocity = physA->GetLinearVelocity() - physB->GetLinearVelocity();
	const float constraintMass = physA->GetInverseMass() + physB->GetInverseMass();
	if (constraintMass <= 0.0f) return;

	const float velocityDot = Vector::Dot(relativeVelocity, offsetDir);
	constexpr float biasFactor = 0.01f;
	const float bias = -(biasFactor / dt) * offset;
	const float lambda = -(velocityDot + bias) / constraintMass;

	const Vector3 aImpulse = offsetDir * lambda;
	const Vector3 bImpulse = -offsetDir * lambda;

	physA->ApplyLinearImpulse(aImpulse);
	physB->ApplyLinearImpulse(bImpulse);
}
