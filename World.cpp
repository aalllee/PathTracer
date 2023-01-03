#include "World.h"



World::World() {
	
	//build();
	geo = nullptr;
	vao = nullptr;
	worldGeoStorageBuffer = nullptr;
	shader_data = {};
	mats = {};
	objects = {};

}

World::~World() {
	delete geo;
	delete vao;
	delete worldGeoStorageBuffer;
}

void World::initWorldSSBO() {
	worldGeoStorageBuffer = new SSBO();
	worldGeoStorageBuffer->update(geo->model->getSizeOf(), geo->model->data_ptr, 3);

	camDataStorageBuffer = new SSBO();
	cam.prepCamData();
	camDataStorageBuffer->update(cam.camData_getSizeOf(), &(cam.data),2);

	shaderDataStorageBuffer = new SSBO();
	shaderDataStorageBuffer->update(sizeof(ShaderData), &(shader_data), 4);

	materialDataStorageBuffer = new SSBO();
	materialDataStorageBuffer->update(sizeof(Material) * mats.size(), mats.data(),5);
}

void World::build() {
	//initial data
	viewPortWidth = 600;
	viewPortHeight = 300;

	cam = Camera(viewPortWidth, viewPortHeight);

	//VAO bind
	vao = new Vao();
	vao->Bind();

	
	//build a scene

	//buildBoxScene();
	//buildDOFScene();

	mats.push_back(Material(glm::vec4(1.0, 1.0, 1.0, 1.0), 0.0f, matID, 0)); //0
	OBJmodel epsilon("models/epsilon.obj");
	epsilon.applyMaterial(0);
	geo = new Geometry(&epsilon, "Shaders/Basic.shader");
	
	//init render variables
	shader_data.samples = 0;
	shader_data.HDRIrotateX = 0.0f;
	shader_data.HDRIrotateY = 0.0f;
	shader_data.HDRIexposure = 1.0f;
	shader_data.smoothNormals = true;
	shader_data.rayDepth = 3;
	maxSamples = 256;
	shader_data.epsilon = 0.0001f;

	//prepare SSBOs for compute shader
	compute = new Shader("Shaders/RayTraceCompute.shader");
	initWorldSSBO();
}

void World::rasterize() {
	
	glViewport(0, 0, viewPortWidth, viewPortHeight);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	geo->shader->Activate();
	geo->shader->setUniforms("MVP", cam.getMvp(viewPortWidth, viewPortHeight));
	vao->LinkVBO(geo->vertexBufferObject, 0, 3, sizeof(Vertex));
	glDrawArrays(GL_TRIANGLES, 0, geo->model->getVertNum());
}

void World::update() {


}

void World::raytrace() {

	
	if (shader_data.samples == maxSamples) {
		return;
	}
	cam.prepCamData();
	camDataStorageBuffer->update(cam.camData_getSizeOf(), &(cam.data), 2);

	shader_data.samples++;
	shaderDataStorageBuffer->update(sizeof(ShaderData), &(shader_data), 4);

	materialDataStorageBuffer->update(sizeof(Material) * mats.size(), mats.data(), 5);
	
	glBindTexture(GL_TEXTURE_2D, screenTex);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, viewPortWidth, viewPortHeight);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	
	compute->Activate();
	
	glDispatchCompute(ceil(viewPortWidth / 8), ceil(viewPortHeight / 4), 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS); //check 

	
}


void World::updateMaterial(int ID, glm::vec4 albedo, float roughness, glm::vec4 specular, glm::vec4 emission, float emPower, int type,float ior) {
	mats[ID].albedo = albedo;
	mats[ID].roughness = roughness;
	mats[ID].specular = specular;
	mats[ID].emission = emission;
	mats[ID].emissivePower = emPower;
	mats[ID].type = type;
	mats[ID].ior = ior;
	shader_data.samples = 0;
	
}

void World::updateShaderData(float rotate_X, float rotate_Y, float HDRIexposure) {
	shader_data.HDRIrotateX = rotate_X;
	shader_data.HDRIrotateY = rotate_Y;
	shader_data.HDRIexposure = HDRIexposure;
	
	shader_data.samples = 0;
	shaderDataStorageBuffer->update(sizeof(ShaderData), &(shader_data), 4);

}

void World::ResizeViewport(int w, int h, GLuint FBO, GLuint RBO, GLuint texture) {
	this->viewPortWidth = w;
	this->viewPortHeight = h;

	cam.camResize(w, h);
	cam.updateViewPlaneCorner();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, viewPortWidth, viewPortHeight);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	glViewport(0, 0, w, h);
}

void World::buildBoxScene() {
	//create material

	//base diffuse (Lambertian)
	mats.push_back(Material(glm::vec4(1.0, 1.0, 1.0, 1.0), 0.0f, matID, 0)); //0

	//leftWall diffuse (Lambertian)
	mats.push_back(Material(glm::vec4(0.0, 1.0, 0.0, 1.0), 0.0f, matID, 0)); //1

	//rightWall diffuse (Lambertian)
	mats.push_back(Material(glm::vec4(0.0, 0.0, 1.0, 1.0), 0.0f, matID, 0)); //2

	//Oren_Nayar
	mats.push_back(Material(glm::vec4(0.0, 0.1, 0.8, 1.0), 0.0f, matID, 2)); //3

	//Metallic GGX
	mats.push_back(Material(glm::vec4(0.0, 0.0, 0.0, 1.0), 0.1f, matID, 1, glm::vec4(0.0f, 0.0f, 0.0, 1.0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); //4

	//diffuse emissive (light)
	mats.push_back(Material(glm::vec4(0.0, 0.0, 0.0, 1.0), 0.0f, matID, 0, glm::vec4(1.5f, 1.5f, 1.5f, 1.0f))); //5



	//load data and populate VBOs
	OBJmodel base("models/box/base.obj");
	base.applyMaterial(0);

	OBJmodel rightWall("models/box/rightWall.obj");
	rightWall.applyMaterial(1);

	OBJmodel leftWall("models/box/leftWall.obj");
	leftWall.applyMaterial(2);

	OBJmodel leftCube("models/box/leftCube.obj");
	leftCube.applyMaterial(3);

	OBJmodel rightCube("models/box/rightCube.obj");
	rightCube.applyMaterial(4);

	OBJmodel light("models/box/light.obj");
	light.applyMaterial(5);

	base.merge(light).merge(leftCube).merge(rightCube).merge(leftWall).merge(rightWall);

	geo = new Geometry(&base, "Shaders/Basic.shader");
}


void World::buildDOFScene() {
	//create material

	//base diffuse (Lambertian)
	mats.push_back(Material(glm::vec4(1.0, 1.0, 1.0, 1.0), 0.0f, matID, 0)); //0

	//diffuse (Lambertian)
	mats.push_back(Material(glm::vec4(0.0, 1.0, 0.0, 1.0), 0.0f, matID, 0)); //1

	//diffuse (Lambertian)
	mats.push_back(Material(glm::vec4(0.0, 0.0, 1.0, 1.0), 0.0f, matID, 0)); //2

	//diffuse
	mats.push_back(Material(glm::vec4(0.0, 0.1, 0.8, 1.0), 0.0f, matID, 0)); //3

	//Metallic GGX
	mats.push_back(Material(glm::vec4(0.0, 0.0, 0.0, 1.0), 0.1f, matID, 1, glm::vec4(0.0f, 0.0f, 0.0, 1.0), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))); //4

	//diffuse emissive
	mats.push_back(Material(glm::vec4(0.0, 0.0, 0.0, 1.0), 0.0f, matID, 0, glm::vec4(1.5f, 1.5f, 1.5f, 1.0f))); //5

	//refractive
	mats.push_back(Material(glm::vec4(0.0, 0.0, 0.0, 1.0), 0.0f, matID, 3)); //6

	//diffuse
	mats.push_back(Material(glm::vec4(1.0, 1.0, 1.0, 1.0), 0.0f, matID, 0)); //7

	//load data and populate VBOs
	OBJmodel plane("models/dof_scene/Plane.obj");
	plane.applyMaterial(0);

	OBJmodel c0("models/dof_scene/Cube0.obj");
	c0.applyMaterial(1);

	OBJmodel c1("models/dof_scene/Cube1.obj");
	c1.applyMaterial(2);

	OBJmodel c2("models/dof_scene/Cube2.obj");
	c2.applyMaterial(3);

	OBJmodel c3("models/dof_scene/Cube3.obj");
	c3.applyMaterial(4);

	OBJmodel c4("models/dof_scene/Cube4.obj");
	c4.applyMaterial(5);

	OBJmodel c5("models/dof_scene/Cube5.obj");
	c5.applyMaterial(6);

	plane.merge(c0).merge(c1).merge(c2).merge(c3).merge(c4).merge(c5);

	geo = new Geometry(&plane, "Shaders/Basic.shader");
}