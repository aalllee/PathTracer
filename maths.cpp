#include "maths.h"
bool Math::rayTriangleIntersect(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t, float& u, float& v) {


    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 pvec = glm::cross(dir, v0v2);//dir.crossProduct(v0v2);
    float det = glm::dot(v0v1, pvec);//v0v1.dotProduct(pvec);

    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < Math::kEpsilon) return false;

    float invDet = 1 / det;

    glm::vec3 tvec = orig - v0;
    u = glm::dot(tvec, pvec) * invDet;// tvec.dotProduct(pvec) * invDet;
    std::cout << "U " << u << std::endl;
    if (u < 0 || u > 1) return false;

    glm::vec3 qvec = glm::cross(tvec, v0v1);//tvec.crossProduct(v0v1);
    v = glm::dot(dir, qvec) * invDet;//dir.dotProduct(qvec) * invDet;
    std::cout << "V " << v << std::endl;
    if (v < 0 || u + v > 1) return false;

    t = glm::dot(v0v2, qvec) * invDet;//v0v2.dotProduct(qvec) * invDet;
    std::cout << "T " << t << std::endl;

    std::cout << "world point  " << (orig + t * dir).x << " " << (orig + t * dir).y << " " << (orig + t * dir).z << std::endl;
    return true;

}