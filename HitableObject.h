#pragma once
#include "Ray.h"
#include <glm/glm.hpp>

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
  //  shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

};


class HitableObject {
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;


};