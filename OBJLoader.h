#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Vertex.h"

static std::vector<Vertex> loadOBJ(const char* file_name) {

	std::vector<glm::vec3> vertex_position;
	std::vector<glm::vec3> vertex_normal;
	std::vector<glm::vec2> vertex_texcoord;

	//index vector
	std::vector<GLuint> vertex_position_indices;
	std::vector<GLuint> textureCoord_indices;
	std::vector<GLuint> vertexNormal_indices;

	//vertex array
	std::vector<Vertex> vertices;
	std::ifstream in_file(file_name);

	std::stringstream ss;
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLuint temp_int = 0;
	
	if (!in_file.is_open()) {
	
		throw "Error::OBJLOADER::COULD NOT OPEN FILE";
	
	}

	int lineCt = 0;
	while (std::getline(in_file, line)) {
		lineCt++;
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") {}
		else if (prefix == "o") {}
		else if (prefix == "s") {}
		else if (prefix == "use_mtl") {}
		else if (prefix == "v") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_position.push_back(temp_vec3);
		}
		else if (prefix == "vt") {
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoord.push_back(temp_vec2);
		}
		else if (prefix == "vn") {
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normal.push_back(temp_vec3);
		}
		else if(prefix == "f"){
			int counter = 0;
			while (ss >> temp_int) {
				if (counter == 0) {
					vertex_position_indices.push_back(temp_int);
				}
				else if (counter == 1) {
					textureCoord_indices.push_back(temp_int);
				}
				else if (counter == 2) {
					vertexNormal_indices.push_back(temp_int);
				}


				//handle characters
				if (ss.peek() == '/') {
					++counter;
					ss.ignore(1, '/');
				
				}
				else if (ss.peek() == ' ') {
					++counter;
					ss.ignore(1, ' ');
				
				}

				//counter
				if (counter > 2) {
					counter = 0;
				}
			}


		}
		else {
			//ignore
		
		}

		

			//final vertex array
			vertices.resize(vertex_position_indices.size(), Vertex());

			for (size_t i = 0; i < vertices.size(); ++i) {
				vertices[i].position = vertex_position[vertex_position_indices[i]-1];
				vertices[i].texcoord = vertex_texcoord[textureCoord_indices[i]-1];
			    vertices[i].normal = vertex_normal[vertexNormal_indices[i]-1];
				vertices[i].color = glm::vec3(0.5f,0.5f,0.5f);
			
			}

			//std::cout << line << "\n";
			//std::cout << "vert num" << vertices.size() << "\n";
			
	}

	std::cout << line << "\n";
	std::cout << "OBJ loaded" << std::endl;

	return vertices;
}