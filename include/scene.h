#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "model.h"
#include "shader.h"
#include "camera.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

class Scene {
public:
	std::vector<Model*> models;
	std::vector<Material*> materials;
	std::vector<glm::mat4> transforms;
	std::string environment;

	Shader shader;
	Shader geometry_shader;
	Shader skybox_shader;
	Shader shadow_shader;
	Shader SAT_shader;
	Shader shading_shader;
	Shader post_shader;
	Shader taa_shader;
	Shader final_shader;

	unsigned int e_avg;
	unsigned int e_lut;

	unsigned int skybox_texture;
	unsigned int skybox_vao;
	unsigned int skybox_vbo;

	unsigned int prefilter_map;
	unsigned int brdf_lut;
	unsigned int ibl_fbo;
	unsigned int ibl_rbo;

	unsigned int shadow_map;
	unsigned int shadow_fbo;
	unsigned int shadow_rbo;

	unsigned int SAT_fbo;
	unsigned int SAT_rbo;
	unsigned int SAT_target;

	unsigned int geometry_fbo;
	unsigned int geometry_rbo;
	unsigned int g_position, g_normal, g_basecolor, g_rmo, g_emission, g_depth, g_velocity;

	unsigned int shading_fbo;
	unsigned int shading_rbo;
	unsigned int color_buffer;
	unsigned int final_color;

	unsigned int post_fbo;
	unsigned int post_rbo;
	unsigned int cur_frame;
	unsigned int pre_frame;

	unsigned int taa_fbo;
	unsigned int taa_rbo;

	unsigned int quad_vao;
	unsigned int quad_vbo;



	Scene(std::string filename);
	void readLight(FILE* file);
	Material* readMaterial(FILE* file);
	glm::mat4 readTransform(FILE* file);
	Model* readModel(FILE* file);

	void configSkybox();
	void configKullaConty();
	void configIBL();
	void configShadowMap();
	void configDeferred();

	void drawSkybox(Camera camera);
	void drawShadowMap(glm::mat4 light_view, glm::mat4 light_projection);
	void drawSceneForward(Camera camera);
	void drawSceneDeferred(Camera camera);
};


#endif