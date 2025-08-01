#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>

#include "mesh.h"

class Material {
public:
	glm::vec4 basecolor_factor;
	float metalness_factor;
	float roughness_factor;
	std::string basecolor_map;
	std::string metalness_map;
	std::string roughness_map;
	std::string normal_map;
	std::string occlusion_map;
	std::string emission_map;
	bool double_sided;
	bool enable_blend;
	float alpha_cutoff;
};

class Model {
public:
	Mesh* mesh;
	Material* material;
	glm::mat4 transform;

	unsigned int VAO;
	unsigned int VBO;

	unsigned int basecolor_map;
	unsigned int metalness_map;
	unsigned int roughness_map;
	unsigned int normal_map;
	unsigned int occlusion_map;
	unsigned int emission_map;

	Model(Mesh* mesh, Material* material, glm::mat4 transform);
	void configBuffer();
	void configTexture();
	void draw();
};

#endif