#pragma once

#include <glm/glm.hpp>
#include <iostream>
namespace Math {
    constexpr float kEpsilon = 1e-8;
	static void test();

    bool rayTriangleIntersect(const glm::vec3& orig,const glm::vec3& dir,const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t, float& u, float& v);
    







};


