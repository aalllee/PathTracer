#version 430
layout(local_size_x = 1, local_size_y = 1) in;

layout(binding = 0, rgba8) uniform image2D ColorImg;

//storage block
layout(std430, binding = 2) buffer shader_data
{
	mat4 test;

};


uniform vec4 CompWindow;
uniform uint Width = 256;
uniform uint Height = 256;


//global vars
float MaxDist = 1 / 0.0000000001;
float MinDist = 0.0001;
//---------------


//Ray "class"
struct Ray {
	vec3 origin;
	vec3 direction;

};

struct HitRecord {
	vec3 point;
	vec3 normal;
	float dist;
	bool front_face;
};
//return position along ray at time t
vec3 at(Ray r, float t) { return r.origin + t*r.direction; }

void set_face_normal(inout HitRecord rec, in Ray r, in vec3 out_normal) {
	// set face normal to hit record did we hit front or back
	rec.front_face = dot(r.direction, out_normal) < 0;
	rec.normal = (rec.front_face) ? out_normal : -1 * out_normal;
}

//------------------------------------------------------------


//Camera "class"
struct Camera {
	vec3 position;
	vec3 front;
	vec3 up;
	float width;
	float height;
	vec3 bottomLeft;

};
//---------------------------------------------------------

//Sphere "class"
struct Sphere {
	vec3 center;
	float radius;

};

bool hitSphere(in Sphere s, in Ray r, float dist_min, float dist_max,
	inout HitRecord record) {
	// kureye isin vurdu mu onu test eden fonksiyon
	vec3 origin_to_center = r.origin - s.center;
	float a = dot(r.direction, r.direction);
	float half_b = dot(origin_to_center, r.direction);
	float c = dot(origin_to_center, origin_to_center) - s.radius * s.radius;
	float isHit = half_b * half_b - a * c;
	float margin;
	if (isHit > 0) {
		float root = sqrt(isHit);
		margin = (-1 * half_b - root) / a;
		if (margin < dist_max && margin > dist_min) {
			record.dist = margin;
			record.point = at(r, record.dist);
			vec3 out_normal = (record.point - s.center) / s.radius;
			set_face_normal(record, r, out_normal);
			return true;
		}
		margin = (-1 * half_b + root) / a;
		if (margin < dist_max && margin > dist_min) {
			record.dist = margin;
			record.point = at(r, record.dist);
			vec3 out_normal = (record.point - s.center) / s.radius;
			set_face_normal(record, r, out_normal);
			return true;
		}
	}
	return false;
}


//---------------------------------------------------------------------


/*
Dlight l;
l.intensity = vec3(1, 1, 1);
l.dir = vec3(0, 1, 0);
*/

//scene
//initializing objects

Sphere s1;
Camera cam;



void initWorld() {
	
	s1.center = vec3(0, 0, -2);
	s1.radius = 0.5;


	cam.position = vec3(0, 0, 0);
	cam.bottomLeft = vec3(-0.5, -0.5, -1);
	cam.width = 1;
	cam.height = 1;

}

void transformWorld() {


}



vec4 rayTrace(in Ray r, in Sphere s) {
	vec4 color = vec4(0, 0, 0.4,1);
	HitRecord temp;
	bool hit = hitSphere(s, r, MinDist, MaxDist, temp);
	if (hit) {
		color = vec4(vec3(1,1,1)*dot(temp.normal, vec3(0, 1, 0)),1);
		
	}

	return color;
}

void main() {
	initWorld();

	Ray ray;
	ray.origin = cam.position;

	transformWorld();

	vec4 temp = test * vec4(s1.center,1);
	s1.center.x = temp.x;
	s1.center.y = temp.y;
	s1.center.z = temp.z;

	
	vec3 hor = vec3(1.0, 0.0, 0.0);
	vec3 ver = vec3(0.0, 1.0, 0.0);
	

	float u = float(gl_GlobalInvocationID.x) / (float(256) - 1);
	float v = float(gl_GlobalInvocationID.y) / (float(256) - 1);
	
	ray.origin = cam.position;
	ray.direction = cam.bottomLeft + u * hor + v * ver;

	vec4 color = rayTrace(ray,s1);


	imageStore(ColorImg, ivec2(gl_GlobalInvocationID.xy),color);
}

