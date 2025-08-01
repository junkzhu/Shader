#pragma once
#ifndef MESH_H
#define MESH_H


#include <string>
#include <glm/glm.hpp>
#include <vector>

class Vertex {
public:
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec4 joint;
	glm::vec4 weight;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	int num_faces;
};

Mesh* loadMesh(std::string filename);

#endif