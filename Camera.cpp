#include "Camera.h"


Camera::Camera() {
	position = { 0,0,8 };
	lookAt = { 0,0,0 };
	up = { 0,1,0 };
}

Camera::Camera(int w,int h) : w(w), h(h) {
	position = {0,0,8};
	lookAt = { 0,0,0 };
	up = { 0,1,0 };
	aspectRatio = float(w) / h;
	apertureRadius = 0.001f;
	focusDistance = 5.0f;
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up) :
 position(pos), lookAt(lookAt), up(up){}


glm::mat4& Camera::getMvp(int w, int h) {
	this->w = w;
	this->h = h;
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(1, -1, 1));
	glm::mat4 viewMat = glm::lookAt(position, lookAt, up);
	glm::mat4 Projection = glm::perspective(glm::radians(this->FOV), (float)w / (float)h, 0.1f, 100.0f);
	MVP = Projection * viewMat * model;
	return MVP;
}

void Camera::updateRotation() {

	glm::mat4 camRotMat = glm::mat4(1.0f);
	viewPlaneX = glm::normalize(glm::cross(lookAt - position, up));
	viewPlaneY = glm::normalize(glm::cross(viewPlaneX, lookAt - position));
	camRotMat = glm::rotate(camRotMat, panX * 0.01f, glm::vec3(0, 1, 0));
	camRotMat = glm::rotate(camRotMat, -panY * 0.01f, viewPlaneX);
	position = camRotMat * glm::vec4(position, 1);

}

void Camera::updateTranslation() {
	glm::vec3 zoomDir = glm::normalize(lookAt - position); //wheel scroll dir
	viewPlaneX = glm::normalize(glm::cross(lookAt - position, up));
	viewPlaneY = glm::normalize(glm::cross(viewPlaneX, lookAt - position));
	position = (float)zoomInOut * (zoomDir * 0.4f) + position;
	position = ((float)panX * viewPlaneX * 0.03f) + ((float)panY * viewPlaneY * 0.03f) + position;
	lookAt = ((float)panX * viewPlaneX * 0.03f) + ((float)panY * viewPlaneY * 0.03f) + lookAt;
}

void Camera::updateGeneral() {
	//panx pany zoomInOut FOV viewPlaneDist viewX viewY corner
}

void Camera::updateViewPlaneCorner() {
	viewPlaneDistance = 0.5f / tan((FOV / 2) * 3.1415926535 / 180);
	ViewPlaneBottomLeft = float(viewPlaneDistance) * glm::normalize(lookAt - position) + float(-0.5) * viewPlaneX + float(-0.5) * viewPlaneY;
	ViewPlaneBottomLeft = float(viewPlaneDistance) * glm::normalize(lookAt - position) + float(-aspectRatio/2) * viewPlaneX + float(-0.5) * viewPlaneY;

}

unsigned int Camera::camData_getSizeOf() {
	return sizeof(camData);
}

void Camera::camResize(int w,int h) {
	this->w = w;
	this->h = h;
	aspectRatio = float(w) / h;

}

void Camera::prepCamData() {
	data.camera_position = glm::vec4(position,1);
	data.cam_lookat = glm::vec4(lookAt,1);
	data.ViewPlaneBottomLeft = glm::vec4(ViewPlaneBottomLeft,1);
	data.scrX = glm::vec4(viewPlaneX,1);
	data.scrY = glm::vec4(viewPlaneY,1);
	data.viewPlaneDistance = viewPlaneDistance;
	data.w = this->w;
	data.h = this->h;
	data.aspectRatio = aspectRatio;
	data.apertureRadius = apertureRadius;
	data.focusDistance = focusDistance;
}