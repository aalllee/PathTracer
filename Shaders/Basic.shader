#shader vertex
#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 MVP;

void main()
{
	
    gl_Position = MVP*vec4(aPos.x, aPos.y, aPos.z, 1.0);
  
   
};


#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
  FragColor = vec4(1,0,1,1);
};

