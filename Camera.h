#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
//camData structure for compute shader
struct camData {
	glm::vec4 camera_position;
	glm::vec4 ViewPlaneBottomLeft;
	glm::vec4 cam_lookat;
	glm::vec4 scrX;
	glm::vec4 scrY;
	float viewPlaneDistance;
	int w;
	int h;
	float aspectRatio;
	float apertureRadius;
	float focusDistance;
};


class Camera {
public:
	Camera();
	Camera(int w, int h);
	Camera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up);
	void updateGeneral();
	void updateRotation();
	void updateTranslation();
	glm::mat4& getMvp(int w, int h);
	void updateViewPlaneCorner();
	void camResize(int w, int h);
	unsigned int camData_getSizeOf();
	void prepCamData();
	
	float viewPlaneDistance;
	float FOV;
	float panX, panY;
	int zoomInOut;
	int w, h;
	float aspectRatio;

	float apertureRadius;
	float focusDistance;

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;

	glm::vec3 viewPlaneX;
	glm::vec3 viewPlaneY;

	//data for raytracer
	glm::vec3 ViewPlaneBottomLeft;
	glm::mat4 MVP; 
	camData data;




};

