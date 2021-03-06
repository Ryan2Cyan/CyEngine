#pragma once
#include "GpContext.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Sampler.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "GameObject.h"

namespace Gp {

	std::shared_ptr<GpContext> CreateContext();
	GLFWwindow* CreateWindow(glm::vec2 resolution);
}




