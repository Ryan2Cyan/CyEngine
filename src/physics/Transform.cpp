#include "Transform.h"

Transform::Transform() {

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
}