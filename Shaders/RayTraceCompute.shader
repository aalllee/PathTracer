#shader compute
#version 450
layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D screen;

layout(rgba32f,binding = 1) uniform image2D HDRenvironment;

layout(rgba32f, binding = 6) uniform image2D HDRI;

//world coordinates unindexed
layout(std430, binding = 3) buffer worldVerts {
	//x y z r g b s t xn yn zn
	float worldVerts[];

} worldVertexBuffer;


layout(std430, binding = 2) buffer bufferData
{
	vec4 camPos;
	vec4 bl;
	vec4 lookat;
	vec4 scrX;
	vec4 scrY;
	float znear;
	int w;
	int h;
	float aspectRatio;
	float apertureRadius;
	float focusDistance;
} camData;

//other test data
layout(std430, binding = 4) buffer shader_data
{

	int samples;
	//environment img settings
	float HDRIrotateX;
	float HDRIrotateY;
	float HDRIexposure;
	int HDRIwidth;
	int HDRIheight;
	//
	bool smoothNormals;
	int rayDepth;

} shaderData;

struct material {
	int ID;
	int type;
	float roughness;
	vec4 albedo;
	vec4 emission;
	vec4 specular;
};

layout(std430, binding = 5) buffer material_data
{
	material m[];

} matData;


//struct def
struct Ray {
	vec3 origin;
	vec3 direction;
};

struct hitRec {
	vec3 hitPoint;
	vec3 normal;
	vec3 albedo;
	int matID;
};



uvec2 imgCoord = gl_GlobalInvocationID.xy;

float kEpsilon = 0.00001;
const float PI = 3.14159265358979323846;
const float InvPi = 0.31830988618379067154;

int getVertMatID(int vertNum) {
	//Vertex.color.x
	int matID = int(worldVertexBuffer.worldVerts[vertNum + 3]);
	return matID;
}

//www.scratchapixel.com/
bool rayTriangleIntersect(const vec3 orig, const vec3 dir, const vec3 v0, const vec3 v1, const vec3 v2, inout float t, inout float u, inout float v) {
	vec3 v0v1 = v1 - v0;
	vec3 v0v2 = v2 - v0;
	vec3 pvec = cross(dir, v0v2);
	float det = dot(v0v1, pvec);

	// ray and triangle are parallel if det is close to 0
	if (abs(det) < kEpsilon) return false;

	float invDet = 1 / det;

	vec3 tvec = orig - v0;
	u = dot(tvec, pvec) * invDet;

	if (u < 0 || u > 1) return false;

	vec3 qvec = cross(tvec, v0v1);
	v = dot(dir, qvec) * invDet;

	if (v < 0 || u + v > 1) return false;

	t = dot(v0v2, qvec) * invDet;

	return true;
}
bool worldHit(Ray r, inout hitRec rec, float tmin ) {

	vec3 v0, v1, v2, n0, n1, n2;
	float nearestHit = 1000.0f;
	float t, u, v;
	bool hit = false;

	//loop throught all triangles
	for (int k = 0; k < worldVertexBuffer.worldVerts.length(); k += 33) {

		v0 = vec3(worldVertexBuffer.worldVerts[k], worldVertexBuffer.worldVerts[k + 1], worldVertexBuffer.worldVerts[k + 2]);
		v1 = vec3(worldVertexBuffer.worldVerts[k + 11], worldVertexBuffer.worldVerts[k + 11 + 1], worldVertexBuffer.worldVerts[k + 11 + 2]);
		v2 = vec3(worldVertexBuffer.worldVerts[k + 2 * 11], worldVertexBuffer.worldVerts[k + 2 * 11 + 1], worldVertexBuffer.worldVerts[k + 2 * 11 + 2]);


		
		
		if (rayTriangleIntersect(r.origin, r.direction, v0, v1, v2, t, u, v)) {
			if (t < nearestHit && t>tmin) {
				nearestHit = t;

				//smooth normal
				
					rec.hitPoint = r.origin + normalize(r.direction) * t;
					rec.normal = vec3(worldVertexBuffer.worldVerts[k + 8], worldVertexBuffer.worldVerts[k + 9], worldVertexBuffer.worldVerts[k + 10]);
					rec.albedo = vec3(worldVertexBuffer.worldVerts[k + 3], worldVertexBuffer.worldVerts[k + 4], worldVertexBuffer.worldVerts[k + 5]);
					rec.matID = getVertMatID(k);

					if (shaderData.smoothNormals) {

						n0 = vec3(worldVertexBuffer.worldVerts[k + 8], worldVertexBuffer.worldVerts[k + 9], worldVertexBuffer.worldVerts[k + 10]);
						n1 = vec3(worldVertexBuffer.worldVerts[k + 8 + 11], worldVertexBuffer.worldVerts[k + 9 + 11], worldVertexBuffer.worldVerts[k + 10 + 11]);
						n2 = vec3(worldVertexBuffer.worldVerts[k + 8 + 2 * 11], worldVertexBuffer.worldVerts[k + 9 + 2 * 11], worldVertexBuffer.worldVerts[k + 10 + 2 * 11]);


						rec.normal = n0 * (1 - u - v) + n1 * u + n2 * v;
					}

				hit = true;
			}
		}
	}

	return hit;
}

///----------RNG--------///
uint rndSeed = imgCoord.x * 312 + imgCoord.y * 291 + 2699 * shaderData.samples;
uint GetPCGHash(inout uint  rndSeed)
{
	rndSeed = rndSeed * 747796405u + 2891336453u;
	uint word = ((rndSeed >> ((rndSeed >> 28u) + 4u)) ^ rndSeed) * 277803737u;
	return (word >> 22u) ^ word;
}
float GetRandomFloat01()
{
	return clamp(float(GetPCGHash(rndSeed)) / 4294967296.0,0.0f,1.0f);
}
//-----------------------//


vec3 cosineSampleHemisphere(vec3 n)
{
	float a = GetRandomFloat01();
	
	float b = GetRandomFloat01();
	vec2 u = vec2(a, b);
	float r = sqrt(u.x);
	float theta = 2.0 * PI * u.y;

	vec3  B = normalize(cross(n, vec3(0.0, 1.0, 1.0)));
	vec3  T = cross(B, n);

	return normalize(r * sin(theta) * B + sqrt(1.0 - u.x) * n + r * cos(theta) * T);
}
vec2 sampleUnitDisk() {
	float e0 = GetRandomFloat01();
	float e1 = GetRandomFloat01();

	float r = sqrt(e0);
	float angle = e1 * 2 * PI;

	return vec2(r * cos(angle), r * sin(angle));

}
void createCoordSystem(vec3 n, inout vec3 s, inout vec3 t) {
	//create orthogonal basis at hitpoint using geometry normal as one of the basis vectors
	if (abs(n.x) > abs(n.y)) {
		s = normalize(vec3(n.z,0,-n.x));
	}
	else {
		s = normalize(vec3(0, -n.z, n.y));
	
	}
	t = cross(s,n);

}
vec3 worldToShadingSpace(vec3 v, vec3 n, vec3 s, vec3 t) {
	//n aligned to (0,0,1), s aligned to (1,0,0), t aligned to (0,1,0)
	return vec3(dot(v, s), dot(v, t), dot(v, n));
}
vec3 ShadingToWorldSpace(vec3 v, vec3 n,vec3 s, vec3 t) {
	return vec3(s.x * v.x + t.x * v.y + n.x * v.z,
		s.y * v.x + t.y * v.y + n.y * v.z,
		s.z * v.x + t.z * v.y + n.z * v.z);
}
//www.pbr-book.org/3ed-2018/Reflection_Models#fig:bsdf-basic-interface
//computations for shading space///////////////////////
//theta = angle between normal and w
float CosTheta(const vec3 w) {
	
	return w.z;
}
float Cos2Theta(const vec3 w) { return w.z * w.z; }
float Sin2Theta(vec3 w) {
	return max(0.0f, 1.0f - Cos2Theta(w));
}
float SinTheta(vec3 w) {
	return sqrt(Sin2Theta(w));
}
//phi = angle between x axis and projection of w on xy plane along z
float CosPhi(const vec3 w) {
	float sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1, 1);
}
float SinPhi(const vec3 w) {
	float sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 0 : clamp(w.y / sinTheta, -1, 1);
}
///////////////////////////////////////////////



vec3 uniformSampleHemisphere(vec3 hitNormal) {
	vec3 Nt, Nb;
	createCoordSystem(hitNormal, Nt, Nb);


	float rand1 = GetRandomFloat01();
	float rand2 = GetRandomFloat01();
	float sinTheta = sqrt(1 - rand1 * rand1);
	float phi = 2 * PI * rand2;
	float x = sinTheta * cos(phi);
	float z = sinTheta * sin(phi);
	//local sample
	//return vec3(x, rand1, z);
	
	return vec3(x * Nb.x + rand1 * hitNormal.x + z * Nt.x,
						  x * Nb.y+ rand1 * hitNormal.y + z * Nt.y,
						  x * Nb.z+ rand1 * hitNormal.z + z * Nt.z);

}
vec3 reflect(vec3 n, vec3 incoming) {
	if (dot(n, incoming) < 0) {
		n = -n;
	}
	return 2 * dot(n, incoming) * n - incoming;
}

////www.scratchapixel.com/
vec3 refracted(vec3 N, vec3 I, float ior) {
	
	//incident direction wi, surface normal n in the same hemisphere as wi, and eta, the ratio of indices of refraction
	float cosi = clamp(dot(I, N), -1, 1);
	float etai = 1; 
	float etat = ior;
	float eta = etai / etat;
	vec3 n = N;
	
	if (cosi < 0) { cosi = -cosi; }
	else {
		eta = 1 / eta;
		n = -N;
	}
	
	float k = 1 - eta * eta * (1 - cosi * cosi);
	if (k < 0) {
		return reflect(n, I);
	}
	return eta * I + (eta * cosi - sqrt(k)) * n;
	
}
void fresnel2(vec3 I,vec3 N, float ior, inout float kr)
{
	float cosi = clamp(dot(I, N), -1, 1);
	float etai = 1;
	float etat = ior;
	float eta = etai / etat;
	if (cosi > 0) { eta=1/eta; }
	// Compute sint using Snell's law
	float sint = etai / etat * sqrt(max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrt(max(0.f, 1 - sint * sint));
		cosi = abs(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
}

vec3 memberwiseMult(vec3 a, vec3 b) {
	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

//brdfs
//wi and wo both normalized and outward facing
vec3 OrenNayar_f(float sigma, vec3 albedo, vec3 wi, vec3 wo) {
	float sinThetaI = SinTheta(wi);
	float sinThetaO = SinTheta(wo);
	float sigma2 = sigma * sigma; //sigma = standard deviation of microfacet normal angle (in radians)

	float A = 1 - ((sigma2*0.5f) /(sigma2 + 0.33f));

	float B = (0.45f * sigma2) / (sigma2 + 0.009);

	float maxCos = 0;
	if (sinThetaI > 0.00001f && sinThetaO > 0.00001f) {
		float sinPhiI = SinPhi(wi), cosPhiI = CosPhi(wi);
		float sinPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
		float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
		maxCos = max(0.0f, dCos);
	}
	float sinAlpha, tanBeta;
	if (abs(CosTheta(wi)) > abs(CosTheta(wo))) {
		sinAlpha = sinThetaO;
		tanBeta = sinThetaI / abs(CosTheta(wi));
	}
	else {
		sinAlpha = sinThetaI;
		tanBeta = sinThetaO / abs(CosTheta(wo));
	}

	return albedo * InvPi *(A + B * maxCos * sinAlpha * tanBeta);
}
//D(wh) fn for GGX, not used yet since it cancels out with importance sampling pdf
float GGX_Distribution(float alpha, vec3 wh) {
	float alpha2 = alpha * alpha;
	float denominator = PI * pow(((CosTheta(wh) * CosTheta(wh)) * (alpha2 - 1) + 1), 2);

	return alpha2 / denominator;
}
//fresnel approximation
vec3 SchlickFresnel(vec3 spec, float radians) {
	return spec + (vec3(1.0f) - spec) * pow(1.0f - radians, 5.0f);

}
float SmithGGXMaskingShadowing(vec3 wi, vec3 wo, float a2)
{
	float cosi =CosTheta(wi);
	float coso = CosTheta(wo);

	float denomA = coso * sqrt(a2 + (1.0f - a2) * cosi * cosi);
	float denomB = cosi * sqrt(a2 + (1.0f - a2) * coso * coso);

	return 2.0f * coso * cosi / (denomA + denomB);
}

//sample the direction of half vector and calculate the sum term of montecarlo integration
vec3 sampleGGX_D(float roughness, vec3 wo, inout vec3 specular,inout vec3 hitPass, vec3 n, vec3 s, vec3 t) {
	float a2 = roughness * roughness;
	float uVar = GetRandomFloat01();
	float phi = GetRandomFloat01() * 2 * PI;
	float theta = acos(sqrt((1 - uVar) / (uVar * (a2 - 1) + 1))); //sampling the angle of microfacet normal to macro normal
	//create half vector in the local shading space 
	vec3 wh = vec3(sin(theta) * cos(phi), (sin(theta) * sin(phi)), cos(theta) );
	vec3 wi = reflect(wh, wo);

	if (CosTheta(wi) > 0.0f && dot(wi, wh) > 0.0f) {
		float G = SmithGGXMaskingShadowing(wi, wo, a2);
		vec3 F = SchlickFresnel(specular, dot(wi, wh));
		hitPass = F * G * abs(dot(wo, wh)) / (CosTheta(wo) * CosTheta(wh));  //brdf * cos(theta) * 1/(pdf(wh) part of monte carlo integration
		return wi;
	}
	else {
		hitPass = vec3(0.0f);
		return wi;
	}
}

vec3 sampleEnvironment(vec3 direction, int w, int h) {
	//sphere to equirectangular
	vec3 dir = normalize(direction);

	//rodrigues's rotation formula
	dir = dir* cos(shaderData.HDRIrotateY) + cross(camData.scrX.xyz, dir) * sin(shaderData.HDRIrotateY) + camData.scrX.xyz * dot(camData.scrX.xyz, dir) * (1 - cos(shaderData.HDRIrotateY));


	float theta = acos(dir.y);
	float phi = acos((dir.x / sin(theta)));


	if (theta > PI) {
		theta -= PI;
	}


	if (dir.z < 0) {
		phi = 2 * PI - phi;
	}

	phi += shaderData.HDRIrotateX;

	if (phi > 2 * PI) {
		phi -= 2 * PI;

	}

	double slopeX = 1.0 * (w - 0) / (2 * PI - 0);
	int outputX = int(0 + slopeX * (phi - 0));

	double slopeY = 1.0 * (h - 0) / (PI - 0);
	int outputY = int((0 + slopeY * (theta - 0)));

	ivec2 imgPixels = ivec2(outputX, outputY);

	vec3 envCol = shaderData.HDRIexposure * imageLoad(HDRI, imgPixels).xyz;

	return envCol;
}


void main() {
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	
	float pixel_offset_u = GetRandomFloat01();
	float pixel_offset_v = GetRandomFloat01();
	
	float i = (float(pixel_coords.x + pixel_offset_u) / (float(camData.w - 1))) * camData.aspectRatio;
	float j = float(pixel_coords.y + pixel_offset_v) / (float(camData.h - 1) );

	//init cam data
	vec3 hor = camData.scrX.xyz;
	vec3 ver = camData.scrY.xyz;
	vec3 origin = camData.camPos.xyz;
	vec3 dir = normalize((camData.bl.xyz) + (i)*hor + (j)*ver);
	vec3 camUp = vec3(0, 1, 0);
	
	//init path trace data
	vec3 throughput = vec3(1, 1, 1);
	vec3 col = vec3(0, 0, 0);
	hitRec rec;
	vec3 reflDir;
	vec3 hitPass;
	int depth = 0;
	Ray r = Ray(origin, dir);

	//depth of field ray
	vec3 pointOfConvergence = r.origin + camData.focusDistance * r.direction;
	vec2 OriginOffset = sampleUnitDisk()*camData.apertureRadius;
	origin = origin + (camData.scrX.xyz * OriginOffset.x + camData.scrY.xyz * OriginOffset.y);
	dir = normalize(pointOfConvergence - origin);

	r = Ray(origin, dir);

	while (depth < shaderData.rayDepth) {

		if (worldHit(r, rec, 0.0001f)) {

			//emission at hitpoint contribution 
			col += memberwiseMult(matData.m[rec.matID].emission.rgb, throughput.rgb);

			//Material type handling

			//Lambertian diffuse
			if (matData.m[rec.matID].type == 0) {
				if (dot(rec.normal, r.direction) > 0) {
					rec.normal *= -1;
				}
				reflDir = cosineSampleHemisphere(rec.normal);
				float pdf = dot(rec.normal, reflDir) * InvPi;

				if (pdf > 0) {
					//	float invPdf = PI / dot(rec.normal, reflDir);
					//	vec3 hitPass = (matData.m[rec.matID].albedo.rgb * InvPi) * dot(rec.normal, reflDir) * invPdf; 

					//montecarlo sample cancels out to albedo, full equation commented above
					hitPass = matData.m[rec.matID].albedo.rgb; 
					throughput = memberwiseMult(hitPass, throughput);


				}
				else {
					break;
				}
				
			}

			//metalic ggx
			if (matData.m[rec.matID].type == 1) {
				vec3 n = rec.normal;
				vec3 s = vec3(0.0f);
				vec3 t = vec3(0.0f);
				createCoordSystem(n, s, t);
				vec3 specular = matData.m[rec.matID].specular.rgb;
				vec3 wo = worldToShadingSpace(normalize(-r.direction), n, s, t);
				vec3 wi = sampleGGX_D(matData.m[rec.matID].roughness, wo, specular, hitPass, n, s, t);
				throughput = memberwiseMult(hitPass, throughput);
				reflDir = ShadingToWorldSpace(wi, n, s, t);
			}

			//Oren-Nayar diffuse
			if (matData.m[rec.matID].type == 2) {
				if (dot(rec.normal, r.direction) > 0) {
					rec.normal *= -1;
				}

				reflDir = cosineSampleHemisphere(rec.normal);
				float pdf = dot(rec.normal, reflDir) / PI;

				if (pdf > 0) {
					//create on hit orthogonal basis in world space
					vec3 n = rec.normal;
					vec3 s = vec3(0.0f);
					vec3 t = vec3(0.0f);
					createCoordSystem(n, s, t);

					//transform orthogonal basis to shading space////////////////////////////
					vec3 wi = worldToShadingSpace(reflDir, n, t, s);
					vec3 wo = worldToShadingSpace(normalize(-r.direction), n, t, s);

					//calc Oren-Nayar brdf
					float sigma = matData.m[rec.matID].roughness; //in radians
					vec3 albedo = matData.m[rec.matID].albedo.rgb;
					hitPass = OrenNayar_f(sigma, albedo, wi, wo) *dot(rec.normal, reflDir) * 1.0f / pdf;

					throughput = memberwiseMult(hitPass, throughput);
				}
				else {
					break;
				}

			}

			//refractive
			if (matData.m[rec.matID].type == 3) {
				float ior = 1.5;
				float kr = 0;
				fresnel2(normalize(r.direction), rec.normal, ior, kr);
				float chance = GetRandomFloat01();

				if (chance > kr) {
					reflDir = refracted(rec.normal, normalize(r.direction), ior);

				}
				else {
					reflDir = reflect(rec.normal, r.direction);
				}

			}
			
		}
		else {
			//if hit nothing get environment light contribution
			vec3 envEmission = sampleEnvironment(r.direction, shaderData.HDRIwidth-1, shaderData.HDRIheight-1);
			col += memberwiseMult(envEmission, throughput.rgb);
			break;
		}

		reflDir = normalize(reflDir);
		r = Ray(rec.hitPoint, reflDir);
		depth++;
	}
	
	vec3 lastFrameColor = imageLoad(screen, pixel_coords).xyz;
	
	//progressive rendering
	col = (lastFrameColor * (shaderData.samples) + col) / (shaderData.samples + 1);
	imageStore(screen, pixel_coords, vec4(col,1));

}
