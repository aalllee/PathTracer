
#include "Cube.h"

Cube::Cube(glm::vec3 c, float r) : center(c), radius(r) {
	vertexData = new float[108]{
		//front face
		   //t1
		   c.x + r, c.y + r, c.z + r,
		   c.x + r, c.y - r, c.z + r,
		   c.x - r, c.y - r, c.z + r,

		   //t2
		   c.x + r, c.y + r, c.z + r,
		   c.x - r, c.y - r, c.z + r,
		   c.x - r, c.y + r, c.z + r,

		   //left face
			 //t1
			 c.x - r, c.y + r, c.z + r,
			 c.x - r, c.y - r, c.z + r,
			 c.x - r, c.y + r, c.z - r,

			 //t2
			 c.x - r, c.y + r, c.z - r,
			 c.x - r, c.y - r, c.z + r,
			 c.x - r, c.y - r, c.z - r,

			 //right face
				//t1
				c.x + r, c.y + r, c.z + r,
				c.x + r, c.y - r, c.z - r,
				c.x + r, c.y - r, c.z + r,

				//t2
				c.x + r, c.y + r, c.z + r,
				c.x + r, c.y + r, c.z - r,
				c.x + r, c.y - r, c.z - r,

				//back face
						   //t1
						   c.x + r, c.y + r, c.z - r,
						   c.x - r, c.y + r, c.z - r,
						   c.x - r, c.y - r, c.z - r,


						   //t2
						   c.x + r, c.y + r, c.z - r,
						   c.x - r, c.y - r, c.z - r,
						   c.x + r, c.y - r, c.z - r,

						   //bottom face
												//t1
												c.x + r, c.y - r, c.z + r,
												c.x + r, c.y - r, c.z - r,
												c.x - r, c.y - r, c.z - r,

												//t2
												c.x + r, c.y - r, c.z + r,
												c.x - r, c.y - r, c.z - r,
												c.x - r, c.y - r, c.z + r,

												//top face
														//t1
														c.x + r, c.y + r, c.z + r,
														c.x - r, c.y + r, c.z + r,
														c.x - r, c.y + r, c.z - r,

														//t2
														c.x + r, c.y + r, c.z + r,
														c.x - r, c.y + r, c.z - r,
														c.x + r, c.y + r, c.z - r,
	};;
	//vertexData = setUpCubeVerts(cen, r);

}

Cube::~Cube() {
	delete [] vertexData;

}

float* Cube::setUpCubeVerts(glm::vec3 c, double r) {

	float vertices[108] = {
		//front face
		   //t1
		   c.x+r, c.y+r, c.z+r,
		   c.x + r, c.y - r, c.z + r,
		   c.x - r, c.y - r, c.z + r,

		   //t2
		   c.x + r, c.y + r, c.z + r,
		   c.x - r, c.y - r, c.z + r,
		   c.x - r, c.y + r, c.z + r,

		 //left face
		   //t1
		   c.x - r, c.y + r, c.z + r,
		   c.x - r, c.y - r, c.z + r,
		   c.x - r, c.y + r, c.z - r,

		   //t2
		   c.x - r, c.y + r, c.z - r,
		   c.x - r, c.y - r, c.z + r,
		   c.x - r, c.y - r, c.z - r,

		 //right face
			//t1
			c.x + r, c.y + r, c.z + r,
			c.x + r, c.y - r, c.z - r,
			c.x + r, c.y - r, c.z + r,

			//t2
			c.x + r, c.y + r, c.z + r,
			c.x + r, c.y + r, c.z - r,
			c.x + r, c.y - r, c.z - r,

		//back face
				   //t1
				   c.x + r, c.y + r, c.z - r,
				   c.x - r, c.y + r, c.z - r,
				   c.x - r, c.y - r, c.z - r,
				  

				   //t2
				   c.x + r, c.y + r, c.z - r,
				   c.x - r, c.y - r, c.z - r,
				   c.x + r, c.y - r, c.z - r,

		 //bottom face
							  //t1
							  c.x + r, c.y - r, c.z + r,
							  c.x + r, c.y - r, c.z - r,
							  c.x - r, c.y - r, c.z - r,

							  //t2
							  c.x + r, c.y - r, c.z + r,
							  c.x - r, c.y - r, c.z - r,
							  c.x - r, c.y - r, c.z + r,

		//top face
				//t1
				c.x + r, c.y + r, c.z + r,
				c.x - r, c.y + r, c.z + r,
				c.x - r, c.y + r, c.z - r,

				//t2
				c.x + r, c.y + r, c.z + r,
				c.x - r, c.y + r, c.z - r,
				c.x + r, c.y + r, c.z - r,
	};

	return vertices;
}

bool Cube::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
	
	
	float t, u, v;
	auto hit = false;
	auto closest_so_far = t_max;

	for (int i = 0;i<108; i+=9) {
		

			glm::vec3 v0(this->vertexData[i], this->vertexData[i+1], this->vertexData[i+2]);
			glm::vec3 v1(this->vertexData[i+3], this->vertexData[i+4], this->vertexData[i+5]);
			glm::vec3 v2(this->vertexData[i+6], this->vertexData[i+7], this->vertexData[i+8]);

			if (Math::rayTriangleIntersect(r.getOrigin(), r.getDirection(), v0, v1, v2, t, u, v)) {

				hit = true;
				if (t < closest_so_far)
				{ 
					closest_so_far = t; 

					rec.t = t;
					rec.p = r.pointAt(t);
					
					rec.normal = glm::normalize(glm::cross(v1-v0,v2-v0));
					//rec.mat_ptr = mat_ptr;
				}
				
				
				
		}
	}


	return hit;
}