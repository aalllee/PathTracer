#pragma once
#include "Camera.h"
#include "OBJmodel.h"
#include "Geometry.h"
#include "VAO.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SSBO.h"
#include "ShaderData.h"
#include "Material.h"
#include <vector>
class World {
public:
	World();
	~World();
	Camera cam;
	Geometry* geo;
	ShaderData shader_data;

	std::vector<OBJmodel> objects;

	std::vector<Material> mats; //?
	int matID = 0;

	Vao* vao;

	//compute storage
	SSBO* worldGeoStorageBuffer;
	SSBO* camDataStorageBuffer;
	SSBO* shaderDataStorageBuffer;
	SSBO* materialDataStorageBuffer;

	Shader* compute;

	GLuint screenTex;
	GLuint HDRI_image_ID;
	GLuint floatIMGID;
	int viewPortWidth, viewPortHeight;
	int maxSamples;

	

	void update();
	void build();
	void rasterize();
	void raytrace();
	void ResizeViewport(int w, int h, GLuint FBO, GLuint RBO, GLuint texture);
	void updateMaterial(int ID, glm::vec4 albedo,float roughness,glm::vec4 specular,glm::vec4 emission, float emPower, int type, float ior);
	void updateShaderData(float rotate_X, float rotate_Y, float HDRIexposure);
	void initWorldSSBO();

	//scenes builds for testing
	void buildBoxScene();
	void buildOpenScene();
	void buildDOFScene();
};

