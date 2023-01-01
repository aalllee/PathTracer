#pragma once
//test shader data for compute shader
#include <glm/glm.hpp>
struct ShaderData {
	int samples;
	float HDRIrotateX;
	float HDRIrotateY;
	float HDRIexposure;
	int HDRIwidth;
	int HDRIheight;
	bool smoothNormals;
	int rayDepth;
};

