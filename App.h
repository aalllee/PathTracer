#pragma once
#include "World.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define IM_VEC4_CLASS_EXTRA \
        constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
        operator glm::vec4() const { return glm::vec4(x,y,z,w); }

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include  <cstdlib>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "Shader.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Vertex.h"
#include "OBJLoader.h"
#include "OBJmodel.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


class App {
public:
	App();

	~App();

	void Run();
	void Update();
	void StartUp();

	void updateUI();
	void renderUI();
	void updateWorld();
	void displayWorld();
	
private:
	World w;
	GLFWwindow* window;
	
	//mouse 
	float lastX, lastY;
	bool firstMouse = true;
	
	//img/viewport
	GLuint texture;//
	GLuint FBO;//
	GLuint RBO;//
	float* HDRI;//
	float* data;//load image data stbimage
	bool render = false;

	//ui
	int matID = 0;
	ImVec4 clear_color;
	bool show_demo_window;
	bool show_another_window;
};


