#pragma once
#include "HitableObject.h"
#include "Ray.h"
#include <glm/glm.hpp>

#include "maths.h"


class Cube : public HitableObject {
    public:
     //   Cube() {}

        Cube(glm::vec3 c,float r);
        ~Cube();

        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        float* setUpCubeVerts(glm::vec3 cen, double r);
    public:
        glm::vec3 center;
        double radius;
        float* vertexData;
     //   shared_ptr<material> mat_ptr;
};