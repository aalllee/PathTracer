#shader vertex
#version 450 core

layout(location = 0) in vec3 aPos;
uniform mat4 MVP;

void main()
{

    gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);


};


#shader fragment
#version 330 core

out vec4 FragColor;
vec4 color = vec4(1.);
vec2 pitch = vec2(50., 50.);


void main() {
    if (mod(gl_FragCoord.x, pitch[0]) < 1. ||
        mod(gl_FragCoord.y, pitch[1]) < 1.) {
        gl_FragColor = color;
    }
    else {
        gl_FragColor = vec4(0.);
    }

  gl_FragColor = vec4(0.2, 0.2, 0.2, 1);
}