#include "Mesh.h"
#include "Texture.h"
#include "RenderTexture.h"

// Init mesh via vao and texture:
Mesh::Mesh(std::shared_ptr<VertexArray> vao, std::shared_ptr<Texture> tex) {

	is_wf = false;
	// Assign values:
	this->vao = vao;
	this->tex = tex;
	diff_light = glm::vec3(1.0, 1.0, 1.0);
	specular_color = glm::vec3(1.0, 1.0, 1.0);
	glow_intensity = 0.0f;
	specular_intensity = 100.0f;
}

// Init mesh via wavefront .obj file:
Mesh::Mesh(std::string filepath) {

	// Load model from specified filepath:
	is_wf = true;
	const char* p = filepath.c_str();
	if (WfModelLoad(p, &wf_model) != 0) {
		throw std::exception("Could not load model");
	}
	diff_light = glm::vec3(1.0, 1.0, 1.0);
	specular_color = glm::vec3(1.0, 1.0, 1.0);
	glow_intensity = 0.0f;
	specular_intensity = 100.0f;
}


const WfModel Mesh::GetWfModel() {
	return wf_model;
}

const GLuint Mesh::GetTexId() {

	return tex->GetId();
}

void Mesh::SetDiffuse(glm::vec3 arg) {
	diff_light = arg;
}

void Mesh::SetSpecular(glm::vec3 arg) {
	specular_color = arg;
}

void Mesh::SetGlow(float arg) {
	glow_intensity = arg;
}


