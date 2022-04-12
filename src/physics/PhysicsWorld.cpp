#include <algorithm>
#include <graphics/GameObject.h>
#include "PhysicsWorld.h"


PhysicsWorld::PhysicsWorld() {
	gravity = glm::vec3(0.0f, -9.81f, 0.0f);
}

void PhysicsWorld::AddGameObject(std::shared_ptr<GameObject> gameobject) {
	gameobjects.push_back(gameobject);
}

void PhysicsWorld::RemoveGameObject(std::shared_ptr<GameObject> gameobject) {
	gameobjects.erase(std::remove(gameobjects.begin(), gameobjects.end(), gameobject), gameobjects.end());
}


// Calculates dynamic physics for all gameobjects in PhysicsWorld::gameobjects (F = ma):
void PhysicsWorld::Step(float delta_time) {

	// Loop through all gameobjects and secure their rigidbodies and transform components:
	for (std::shared_ptr<GameObject> gameobject : gameobjects) {
		std::shared_ptr<Rigidbody> rigidbody = gameobject->GetRigidbody();
		std::shared_ptr<Transform> transform = gameobject->GetTransform();

		// Apply force:
		rigidbody->force += rigidbody->mass * gravity;

		// Update object's velocity, then position:
		rigidbody->velocity += rigidbody->force / rigidbody->mass * delta_time;
		transform->position += rigidbody->velocity * delta_time;

		// Reset net force each frame:
		rigidbody->force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}