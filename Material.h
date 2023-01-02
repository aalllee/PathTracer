#pragma once
#include <glm/glm.hpp>
struct Material {
	Material(glm::vec4 albedo,float roughness, int& matId, int type, glm::vec4 em = glm::vec4(0.0f), glm::vec4 specular = glm::vec4(0.0f)): albedo(albedo),roughness(roughness), ID(matId),type(type), emission(em), specular(specular) {
	
		matId++;
		emissivePower = 1.0f;
		ior = 1.3f;
	}
	int ID;
	int type; // 0 : Lambertian Diffuse | 1 : Metallic (ggx) | 2 : Diffuse (Oren-Nayar) | 3 : Refractive (specular)
	float roughness;
	float emissivePower;
	float ior;
	alignas(16)glm::vec4 albedo;
	alignas(16)glm::vec4 emission;
	alignas(16)glm::vec4 specular;
	
};